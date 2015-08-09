/*
 * Copyright (C) 2014 Paul Kocialkowski <contact@paulk.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>

#include <hardware/sensors.h>
#include <hardware/hardware.h>

#define LOG_TAG "smdk4x12_sensors"
#include <utils/Log.h>

#include "smdk4x12_sensors.h"
#include "ssp.h"

#define AKM8963_CONFIG_PATH			"/data/misc/akmd_set.txt"

struct akm8963_data {
	struct smdk4x12_sensors_handlers *orientation_sensor;

	sensors_vec_t magnetic;
	short magnetic_data[4][3];
	int magnetic_data_count;
	int magnetic_data_index;

	short magnetic_extrema[2][3];
	unsigned char asa[3];
	int ho[3];

	long int delay;
	int device_fd;
	int uinput_fd;

	pthread_t thread;
	pthread_mutex_t mutex;
	int thread_continue;
};

// This AKM8963 implementation is based on intuitive understanding of how the
// AKM8963 data is translated to SI units.
//
// The raw data is a two-byte short value that is the 16 bit ADC read value.
// This value has to be corrected using the ASA Sensivity Adujstment value
// as such: v_adj = v * (((ASA - 128) * 0.5) / 128 + 1)
// LSB values can then be converted to uT units, with 0.15uT/LSB:
// m = 0.15 * v * (((ASA - 128) * 0.5) / 128 + 1)
//
// Moreover, we calculate and apply a software offset (HO) in order to have the
// maximum final values for each axis at ~45uT and minimum at ~-45uT.
// In order to store HO as an integer, the applied offset is 0.06 * HO:
// m = 0.15 * v * (((ASA - 128) * 0.5) / 128 + 1) - 0.06 * HO


int akm8963_magnetic_extrema(struct akm8963_data *data, int index)
{
	if (data == NULL || index < 0 || index >= 3)
		return -EINVAL;

	// Calculate the extrema from HO (software offset)
	data->magnetic_extrema[0][index] = (short) ((-45.0f + 0.06 * data->ho[index]) / (0.15f * (((data->asa[index] - 128) * 0.5f) / 128 + 1)));
	data->magnetic_extrema[1][index] = (short) ((45.0f + 0.06 * data->ho[index]) / (0.15f * (((data->asa[index] - 128) * 0.5f) / 128 + 1)));

	return 0;
}

int akm8963_config_read(struct akm8963_data *data)
{
	char buffer[256] = { 0 };
	int config_fd = -1;
	int offset = 0;
	int length;
	int count;
	int value;
	char *p;
	int rc;

	if (data == NULL)
		return -EINVAL;

	config_fd = open(AKM8963_CONFIG_PATH, O_RDONLY);
	if (config_fd < 0) {
		ALOGE("%s: Unable to open akm8963 config %d %s", __func__, errno, strerror(errno));
		goto error;
	}

	rc = 0;

	do {
		lseek(config_fd, offset, SEEK_SET);

		length = read(config_fd, buffer, sizeof(buffer));
		if (length <= 0)
			break;

		p = strchr((const char *) &buffer, '\n');
		if (p != NULL) {
			offset += (int) p - (int) buffer + 1;
			*p = '\0';
		} else if ((size_t) length < sizeof(buffer)) {
			buffer[length] = '\0';
		}

		count = sscanf((char const *) &buffer, "HSUC_HO_FORM0.x = %d", &value);
		if (count == 1) {
			data->ho[0] = value;
			rc |= akm8963_magnetic_extrema(data, 0);
		}

		count = sscanf((char const *) &buffer, "HSUC_HO_FORM0.y = %d", &value);
		if (count == 1) {
			data->ho[1] = value;
			rc |= akm8963_magnetic_extrema(data, 1);
		}

		count = sscanf((char const *) &buffer, "HSUC_HO_FORM0.z = %d", &value);
		if (count == 1) {
			data->ho[2] = value;
			rc |= akm8963_magnetic_extrema(data, 2);
		}
	} while (p != NULL && length > 0);

	goto complete;

error:
	rc = -1;

complete:
	if (config_fd >= 0)
		close(config_fd);

	return rc;
}

int akm8963_config_write(struct akm8963_data *data)
{
	char buffer[256] = { 0 };
	int config_fd = -1;
	int length;
	int value;
	int rc;

	if (data == NULL)
		return -EINVAL;

	config_fd = open(AKM8963_CONFIG_PATH, O_WRONLY | O_TRUNC | O_CREAT, 0664);
	if (config_fd < 0) {
		ALOGE("%s: Unable to open akm8963 config", __func__);
		goto error;
	}

	value = (int) data->ho[0];
	length = snprintf((char *) &buffer, sizeof(buffer), "HSUC_HO_FORM0.x = %d\n", value);

	rc = write(config_fd, buffer, length);
	if (rc < length) {
		ALOGE("%s: Unable to write akm8963 config", __func__);
		goto error;
	}

	value = (int) data->ho[1];
	length = snprintf((char *) &buffer, sizeof(buffer), "HSUC_HO_FORM0.y = %d\n", value);

	rc = write(config_fd, buffer, length);
	if (rc < length) {
		ALOGE("%s: Unable to write akm8963 config", __func__);
		goto error;
	}

	value = (int) data->ho[2];
	length = snprintf((char *) &buffer, sizeof(buffer), "HSUC_HO_FORM0.z = %d\n", value);

	rc = write(config_fd, buffer, length);
	if (rc < length) {
		ALOGE("%s: Unable to write akm8963 config", __func__);
		goto error;
	}

	rc = 0;
	goto complete;

error:
	rc = -1;

complete:
	if (config_fd >= 0)
		close(config_fd);

	return rc;
}

int akm8963_ho_calibration(struct akm8963_data *data,
	short *magnetic_data, size_t magnetic_data_size)
{
	float ho[2];
	int gain_index;
	int i;

	if (data == NULL || magnetic_data == NULL || magnetic_data_size < 3)
		return -EINVAL;

	// Update the extrema from the current raw magnetic data
	for (i = 0; i < 3; i++) {
		if (magnetic_data[i] < data->magnetic_extrema[0][i] || data->magnetic_extrema[0][i] == 0)
			data->magnetic_extrema[0][i] = magnetic_data[i];
		if (magnetic_data[i] > data->magnetic_extrema[1][i] || data->magnetic_extrema[1][i] == 0)
			data->magnetic_extrema[1][i] = magnetic_data[i];
	}

	// Calculate HO (software offset)
	if (data->magnetic_data_count % 10 == 0) {
		for (i = 0; i < 3; i++) {
			// Calculate offset for minimum to be at -45uT
			ho[0] = (0.15f * (((data->asa[i] - 128) * 0.5f) / 128 + 1) * data->magnetic_extrema[0][i] + 45.0f) / 0.06f;
			// Calculate offset for maximum to be at +45uT
			ho[1] = (0.15f * (((data->asa[i] - 128) * 0.5f) / 128 + 1) * data->magnetic_extrema[1][i] - 45.0f) / 0.06f;
			// Average offset to make everyone (mostly) happy
			data->ho[i] = (int) (ho[0] + ho[1]) / 2.0f;
		}
	}

	return 0;
}

int akm8963_magnetic_axis(struct akm8963_data *data, int index, float *axis)
{
	float value;
	int count;
	int i;

	if (data == NULL || axis == NULL || index < 0 || index >= 3)
		return -EINVAL;

	count = data->magnetic_data_count >= 4 ? 4 : data->magnetic_data_count;
	value = 0;

	// Average the last 4 (or less) raw magnetic values
	for (i = 0; i < count; i++)
		value += (float) data->magnetic_data[i][index];
	value /= count;

	// Adjust sensitivity using ASA value
	value *= (((data->asa[index] - 128) * 0.5f) / 128 + 1);

	// Magnetic field value in uT from corrected value and HO offset
	*axis = 0.15f * value - 0.06f * data->ho[index];

	return 0;
}

int akm8963_magnetic(struct akm8963_data *data)
{
	int rc;

	if (data == NULL)
		return -EINVAL;

	rc = 0;
	rc |= akm8963_magnetic_axis(data, 0, &data->magnetic.x);
	rc |= akm8963_magnetic_axis(data, 1, &data->magnetic.y);
	rc |= akm8963_magnetic_axis(data, 2, &data->magnetic.z);

	return rc;
}

void *akm8963_thread(void *thread_data)
{
	struct smdk4x12_sensors_handlers *handlers = NULL;
	struct akm8963_data *data = NULL;
	struct input_event event;
	struct timeval time;
	unsigned char i2c_data[8] = { 0 };
	short magnetic_data[3] = { 0 };
	int index;
	long int before, after;
	int diff;
	int device_fd;
	int uinput_fd;
	int rc;

	if (thread_data == NULL)
		return NULL;

	handlers = (struct smdk4x12_sensors_handlers *) thread_data;
	if (handlers->data == NULL)
		return NULL;

	data = (struct akm8963_data *) handlers->data;

	device_fd = data->device_fd;
	if (device_fd < 0)
		return NULL;

	uinput_fd = data->uinput_fd;
	if (uinput_fd < 0)
		return NULL;

	while (data->thread_continue) {
		pthread_mutex_lock(&data->mutex);
		if (!data->thread_continue)
			break;

		while (handlers->activated) {
			gettimeofday(&time, NULL);
			before = timestamp(&time);

			memset(&i2c_data, 0, sizeof(i2c_data));
			rc = ioctl(device_fd, ECS_IOCTL_GET_MAGDATA, &i2c_data);
			if (rc < 0) {
				ALOGE("%s: Unable to get akm8963 data", __func__);
				goto next;
			}

			if (!(i2c_data[0] & 0x01)) {
				ALOGE("%s: akm8963 data is not ready", __func__);
				goto next;
			}

			magnetic_data[0] = (short) ((i2c_data[2] << 8) | (i2c_data[1] & 0xff));
			magnetic_data[1] = (short) ((i2c_data[4] << 8) | (i2c_data[3] & 0xff));
			magnetic_data[2] = (short) ((i2c_data[6] << 8) | (i2c_data[5] & 0xff));

			index = data->magnetic_data_index;

			data->magnetic_data[index][0] = magnetic_data[0];
			data->magnetic_data[index][1] = magnetic_data[1];
			data->magnetic_data[index][2] = magnetic_data[2];

			data->magnetic_data_index = (index + 1) % 4;
			data->magnetic_data_count++;

			rc = akm8963_ho_calibration(data, (short *) &magnetic_data, sizeof(magnetic_data));
			if (rc < 0) {
				ALOGE("%s: Unable to calibrate akm8963 HO", __func__);
				goto next;
			}

			rc = akm8963_magnetic(data);
			if (rc < 0) {
				ALOGE("%s: Unable to get akm8963 magnetic", __func__);
				goto next;
			}

			input_event_set(&event, EV_REL, REL_X, (int) (data->magnetic.x * 1000));
			write(uinput_fd, &event, sizeof(event));
			input_event_set(&event, EV_REL, REL_Y, (int) (data->magnetic.y * 1000));
			write(uinput_fd, &event, sizeof(event));
			input_event_set(&event, EV_REL, REL_Z, (int) (data->magnetic.z * 1000));
			write(uinput_fd, &event, sizeof(event));
			input_event_set(&event, EV_SYN, 0, 0);
			write(uinput_fd, &event, sizeof(event));

next:
			gettimeofday(&time, NULL);
			after = timestamp(&time);

			diff = (int) (data->delay - (after - before)) / 1000;
			if (diff <= 0)
				continue;

			usleep(diff);
		}
	}
	return NULL;
}

int akm8963_init(struct smdk4x12_sensors_handlers *handlers,
	struct smdk4x12_sensors_device *device)
{
	struct akm8963_data *data = NULL;
	pthread_attr_t thread_attr;
	int device_fd = -1;
	int uinput_fd = -1;
	int input_fd = -1;
	int rc;
	int i;

	ALOGD("%s(%p, %p)", __func__, handlers, device);

	if (handlers == NULL || device == NULL)
		return -EINVAL;

	data = (struct akm8963_data *) calloc(1, sizeof(struct akm8963_data));

	for (i = 0; i < device->handlers_count; i++) {
		if (device->handlers[i] == NULL)
			continue;

		if (device->handlers[i]->handle == SENSOR_TYPE_ORIENTATION)
			data->orientation_sensor = device->handlers[i];
	}

	device_fd = open("/dev/akm8963", O_RDONLY);
	if (device_fd < 0) {
		ALOGE("%s: Unable to open device", __func__);
		goto error;
	}

	rc = ioctl(device_fd, ECS_IOCTL_GET_FUSEROMDATA, &data->asa);
	if (rc < 0) {
		ALOGE("%s: Unable to get akm8963 FUSE ROM data", __func__);
		goto error;
	}

	ALOGD("AKM8963 ASA (Sensitivity Adjustment) values are: (%d, %d, %d)",
		data->asa[0], data->asa[1], data->asa[2]);

	uinput_fd = uinput_rel_create("magnetic");
	if (uinput_fd < 0) {
		ALOGD("%s: Unable to create uinput", __func__);
		goto error;
	}

	input_fd = input_open("magnetic");
	if (input_fd < 0) {
		ALOGE("%s: Unable to open magnetic input", __func__);
		goto error;
	}

	data->thread_continue = 1;

	pthread_mutex_init(&data->mutex, NULL);
	pthread_mutex_lock(&data->mutex);

	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

	rc = pthread_create(&data->thread, &thread_attr, akm8963_thread, (void *) handlers);
	if (rc < 0) {
		ALOGE("%s: Unable to create akm8963 thread", __func__);
		pthread_mutex_destroy(&data->mutex);
		goto error;
	}

	data->device_fd = device_fd;
	data->uinput_fd = uinput_fd;
	handlers->poll_fd = input_fd;
	handlers->data = (void *) data;

	return 0;

error:
	if (data != NULL)
		free(data);

	if (uinput_fd >= 0)
		close(uinput_fd);

	if (input_fd >= 0)
		close(input_fd);

	if (device_fd >= 0)
		close(device_fd);

	handlers->poll_fd = -1;
	handlers->data = NULL;

	return -1;
}

int akm8963_deinit(struct smdk4x12_sensors_handlers *handlers)
{
	struct akm8963_data *data = NULL;
	int rc;

	ALOGD("%s(%p)", __func__, handlers);

	if (handlers == NULL || handlers->data == NULL)
		return -EINVAL;

	data = (struct akm8963_data *) handlers->data;

	handlers->activated = 0;
	data->thread_continue = 0;
	pthread_mutex_unlock(&data->mutex);

	pthread_mutex_destroy(&data->mutex);

	if (data->uinput_fd >= 0) {
		uinput_destroy(data->uinput_fd);
		close(data->uinput_fd);
	}
	data->uinput_fd = -1;

	if (handlers->poll_fd >= 0)
		close(handlers->poll_fd);
	handlers->poll_fd = -1;

	if (data->device_fd >= 0)
		close(data->device_fd);
	data->device_fd = -1;

	free(handlers->data);
	handlers->data = NULL;

	return 0;
}

int akm8963_activate(struct smdk4x12_sensors_handlers *handlers)
{
	struct akm8963_data *data;
	int rc;

	ALOGD("%s(%p)", __func__, handlers);

	if (handlers == NULL || handlers->data == NULL)
		return -EINVAL;

	data = (struct akm8963_data *) handlers->data;

	rc = akm8963_config_read(data);
	if (rc < 0) {
		ALOGE("%s: Unable to read akm8963 config", __func__);
	}

	rc = ssp_sensor_enable(GEOMAGNETIC_SENSOR);
	if (rc < 0) {
		ALOGE("%s: Unable to enable ssp sensor", __func__);
		return -1;
	}

	handlers->activated = 1;
	pthread_mutex_unlock(&data->mutex);

	return 0;
}

int akm8963_deactivate(struct smdk4x12_sensors_handlers *handlers)
{
	struct akm8963_data *data;
	int empty;
	int rc;
	int i;

	ALOGD("%s(%p)", __func__, handlers);

	if (handlers == NULL || handlers->data == NULL)
		return -EINVAL;

	data = (struct akm8963_data *) handlers->data;

	empty = 1;

	for (i = 0; i < (ssize_t) (sizeof(data->magnetic_extrema) / (sizeof(short) * 2)); i++) {
		if (data->magnetic_extrema[0][i] != 0 || data->magnetic_extrema[1][i] != 0) {
			empty = 0;
			break;
		}
	}

	if (!empty) {
		rc = akm8963_config_write(data);
		if (rc < 0)
			ALOGE("%s: Unable to write akm8963 config", __func__);
	}

	rc = ssp_sensor_disable(GEOMAGNETIC_SENSOR);
	if (rc < 0) {
		ALOGE("%s: Unable to disable ssp sensor", __func__);
		return -1;
	}

	handlers->activated = 0;

	return 0;
}

int akm8963_set_delay(struct smdk4x12_sensors_handlers *handlers, long int delay)
{
	struct akm8963_data *data;
	char path_delay[PATH_MAX] = "/sys/class/sensors/ssp_sensor/mag_poll_delay";
	int rc;

	ALOGD("%s(%p, %ld)", __func__, handlers, delay);

	if (handlers == NULL || handlers->data == NULL)
		return -EINVAL;

	data = (struct akm8963_data *) handlers->data;

	rc = sysfs_value_write(path_delay, (int) delay);
	if (rc < 0) {
		ALOGE("%s: Unable to write sysfs value", __func__);
		return -1;
	}

	data->delay = delay;

	return 0;
}

float akm8963_convert(int value)
{
	return (float) value / 1000.0f;
}

int akm8963_get_data(struct smdk4x12_sensors_handlers *handlers,
	struct sensors_event_t *event)
{
	struct akm8963_data *data;
	struct input_event input_event;
	int input_fd;
	int rc;

//	ALOGD("%s(%p, %p)", __func__, handlers, event);

	if (handlers == NULL || handlers->data == NULL || event == NULL)
		return -EINVAL;

	data = (struct akm8963_data *) handlers->data;

	input_fd = handlers->poll_fd;
	if (input_fd < 0)
		return -1;

	memset(event, 0, sizeof(struct sensors_event_t));
	event->version = sizeof(struct sensors_event_t);
	event->sensor = handlers->handle;
	event->type = handlers->handle;

	event->magnetic.status = SENSOR_STATUS_ACCURACY_MEDIUM;

	do {
		rc = read(input_fd, &input_event, sizeof(input_event));
		if (rc < (int) sizeof(input_event))
			break;

		if (input_event.type == EV_REL) {
			switch (input_event.code) {
				case REL_X:
					event->magnetic.x = akm8963_convert(input_event.value);
					break;
				case REL_Y:
					event->magnetic.y = akm8963_convert(input_event.value);
					break;
				case REL_Z:
					event->magnetic.z = akm8963_convert(input_event.value);
					break;
				default:
					continue;
			}
		} else if (input_event.type == EV_SYN) {
			if (input_event.code == SYN_REPORT)
				event->timestamp = input_timestamp(&input_event);
		}
	} while (input_event.type != EV_SYN);

	if (data->orientation_sensor != NULL)
		orientation_fill(data->orientation_sensor, NULL, &event->magnetic);

	return 0;
}

struct smdk4x12_sensors_handlers akm8963 = {
	.name = "AKM8963",
	.handle = SENSOR_TYPE_MAGNETIC_FIELD,
	.init = akm8963_init,
	.deinit = akm8963_deinit,
	.activate = akm8963_activate,
	.deactivate = akm8963_deactivate,
	.set_delay = akm8963_set_delay,
	.get_data = akm8963_get_data,
	.activated = 0,
	.needed = 0,
	.poll_fd = -1,
	.data = NULL,
};
