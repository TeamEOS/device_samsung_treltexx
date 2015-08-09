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
#include <errno.h>

#include <hardware/sensors.h>
#include <hardware/hardware.h>

#define LOG_TAG "smdk4x12_sensors"
#include <utils/Log.h>

#include "smdk4x12_sensors.h"
#include "ssp.h"

int ssp_sensor_enable(int sensor_type)
{
	char path_enable[PATH_MAX] = "/sys/class/sensors/ssp_sensor/enable";
	int value;
	int rc;

	ALOGD("%s(%d)", __func__, sensor_type);

	if (sensor_type < 0 || sensor_type >= SENSOR_FACTORY_MAX)
		return -EINVAL;

	value = sysfs_value_read(path_enable);
	if (value < 0)
		value = 0;

	value |= (1 << sensor_type);

	rc = sysfs_value_write(path_enable, value);
	if (rc < 0)
		return -1;

	return 0;
}

int ssp_sensor_disable(int sensor_type)
{
	char path_enable[PATH_MAX] = "/sys/class/sensors/ssp_sensor/enable";
	int value;
	int rc;

	ALOGD("%s(%d)", __func__, sensor_type);

	if (sensor_type < 0 || sensor_type >= SENSOR_FACTORY_MAX)
		return -EINVAL;

	value = sysfs_value_read(path_enable);
	if (value < 0)
		value = 0;

	value &= ~(1 << sensor_type);

	rc = sysfs_value_write(path_enable, value);
	if (rc < 0)
		return -1;

	return 0;
}
