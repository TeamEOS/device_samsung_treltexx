# Copyright (C) 2013-2014 Paul Kocialkowski <contact@paulk.fr>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	smdk4x12_sensors.c \
	input.c \
	orientation.c \
	ssp.c \
	akm8963.c \
	cm36651_proximity.c \
	cm36651_light.c \
	lsm330dlc_acceleration.c \
	lsm330dlc_gyroscope.c \
	bmp180.c

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)

LOCAL_SHARED_LIBRARIES := libutils libcutils liblog libhardware
LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := sensors.smdk4x12
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
