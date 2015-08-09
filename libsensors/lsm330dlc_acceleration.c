/*
 * Copyright (C) 2013 Paul Kocialkowski <contact@paulk.fr>
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

struct lsm330dlc_acceleration_data {
	struct smdk4x12_sensors_handlers *orientation_sensor;

	char path_delay[PATH_MAX];

	sensors_vec_t acceleration;
};

int lsm330dlc_acceleration_init(struct smdk4x12_sensors_handlers *handlers,
	struct smdk4x12_sensors_device *device)
{
	struct lsm330dlc_acceleration_data *data = NULL;
	char path[PATH_MAX] = { 0 };
	int input_fd = -1;
	int rc;
	int i;

	ALOGD("%s(%p, %p)", __func__, handlers, device);

	if (handlers == NULL)
		return -EINVAL;

	data = (struct lsm330dlc_acceleration_data *) calloc(1, sizeof(struct lsm330dlc_acceleration_data));

	for (i = 0; i < device->handlers_count; i++) {
		if (device->handlers[i] == NULL)
			continue;

		if (device->handlers[i]->handle == SENSOR_TYPE_ORIENTATION)
			data->orientation_sensor = device->handlers[i];
	}

	input_fd = input_open("accelerometer_sensor");
	if (input_fd < 0) {
		ALOGE("%s: Unable to open input", __func__);
		goto error;
	}

	rc = sysfs_path_prefix("accelerometer_sensor", (char *) &path);
	if (rc < 0 || path[0] == '\0') {
		ALOGE("%s: Unable to open sysfs", __func__);
		goto error;
	}

	snprintf(data->path_delay, PATH_MAX, "%s/acc_poll_delay", path);

	handlers->poll_fd = input_fd;
	handlers->data = (void *) data;

	return 0;

error:
	if (data != NULL)
		free(data);

	if (input_fd >= 0)
		close(input_fd);

	handlers->poll_fd = -1;
	handlers->data = NULL;

	return -1;
}

int lsm330dlc_acceleration_deinit(struct smdk4x12_sensors_handlers *handlers)
{
	ALOGD("%s(%p)", __func__, handlers);

	if (handlers == NULL)
		return -EINVAL;

	if (handlers->poll_fd >= 0)
		close(handlers->poll_fd);
	handlers->poll_fd = -1;

	if (handlers->data != NULL)
		free(handlers->data);
	handlers->data = NULL;

	return 0;
}

int lsm330dlc_acceleration_activate(struct smdk4x12_sensors_handlers *handlers)
{
	struct lsm330dlc_acceleration_data *data;
	int rc;

	ALOGD("%s(%p)", __func__, handlers);

	if (handlers == NULL || handlers->data == NULL)
		return -EINVAL;

	data = (struct lsm330dlc_acceleration_data *) handlers->data;

	rc = ssp_sensor_enable(ACCELEROMETER_SENSOR);
	if (rc < 0) {
		ALOGE("%s: Unable to enable ssp sensor", __func__);
		return -1;
	}

	handlers->activated = 1;

	return 0;
}

int lsm330dlc_acceleration_deactivate(struct smdk4x12_sensors_handlers *handlers)
{
	struct lsm330dlc_acceleration_data *data;
	int rc;

	ALOGD("%s(%p)", __func__, handlers);

	if (handlers == NULL || handlers->data == NULL)
		return -EINVAL;

	data = (struct lsm330dlc_acceleration_data *) handlers->data;

	rc = ssp_sensor_disable(ACCELEROMETER_SENSOR);
	if (rc < 0) {
		ALOGE("%s: Unable to disable ssp sensor", __func__);
		return -1;
	}

	handlers->activated = 0;

	return 0;
}

int lsm330dlc_acceleration_set_delay(struct smdk4x12_sensors_handlers *handlers, long int delay)
{
	struct lsm330dlc_acceleration_data *data;
	int rc;

	ALOGD("%s(%p, %ld)", __func__, handlers, delay);

	if (handlers == NULL || handlers->data == NULL)
		return -EINVAL;

	data = (struct lsm330dlc_acceleration_data *) handlers->data;

	rc = sysfs_value_write(data->path_delay, (int) delay);
	if (rc < 0) {
		ALOGE("%s: Unable to write sysfs value", __func__);
		return -1;
	}

	return 0;
}

float lsm330dlc_acceleration_convert(int value)
{
	return (float) value * (GRAVITY_EARTH / 1024.0f);
}

int lsm330dlc_acceleration_get_data(struct smdk4x12_sensors_handlers *handlers,
	struct sensors_event_t *event)
{
	struct lsm330dlc_acceleration_data *data;
	struct input_event input_event;
	int input_fd;
	int rc;

//	ALOGD("%s(%p, %p)", __func__, handlers, event);

	if (handlers == NULL || handlers->data == NULL || event == NULL)
		return -EINVAL;

	data = (struct lsm330dlc_acceleration_data *) handlers->data;

	input_fd = handlers->poll_fd;
	if (input_fd < 0)
		return -1;

	memset(event, 0, sizeof(struct sensors_event_t));
	event->version = sizeof(struct sensors_event_t);
	event->sensor = handlers->handle;
	event->type = handlers->handle;

	event->acceleration.x = data->acceleration.x;
	event->acceleration.y = data->acceleration.y;
	event->acceleration.z = data->acceleration.z;

	event->magnetic.status = SENSOR_STATUS_ACCURACY_MEDIUM;

	do {
		rc = read(input_fd, &input_event, sizeof(input_event));
		if (rc < (int) sizeof(input_event))
			break;

		if (input_event.type == EV_REL) {
			switch (input_event.code) {
				case REL_X:
					event->acceleration.x = lsm330dlc_acceleration_convert(input_event.value);
					break;
				case REL_Y:
					event->acceleration.y = lsm330dlc_acceleration_convert(input_event.value);
					break;
				case REL_Z:
					event->acceleration.z = lsm330dlc_acceleration_convert(input_event.value);
					break;
				default:
					continue;
			}
		} else if (input_event.type == EV_SYN) {
			if (input_event.code == SYN_REPORT)
				event->timestamp = input_timestamp(&input_event);
		}
	} while (input_event.type != EV_SYN);

	data->acceleration.x = event->acceleration.x;
	data->acceleration.y = event->acceleration.y;
	data->acceleration.z = event->acceleration.z;

	if (data->orientation_sensor != NULL)
		orientation_fill(data->orientation_sensor, &event->acceleration, NULL);

	return 0;
}

struct smdk4x12_sensors_handlers lsm330dlc_acceleration = {
	.name = "LSM330DLC Acceleration",
	.handle = SENSOR_TYPE_ACCELEROMETER,
	.init = lsm330dlc_acceleration_init,
	.deinit = lsm330dlc_acceleration_deinit,
	.activate = lsm330dlc_acceleration_activate,
	.deactivate = lsm330dlc_acceleration_deactivate,
	.set_delay = lsm330dlc_acceleration_set_delay,
	.get_data = lsm330dlc_acceleration_get_data,
	.activated = 0,
	.needed = 0,
	.poll_fd = -1,
	.data = NULL,
};
