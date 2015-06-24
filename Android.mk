#
# Copyright (C) 2014 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

ifneq ($(filter treltexx, $(TARGET_DEVICE)),)

$(shell mkdir -p out/target/product/treltexx/obj/KERNEL_OBJ/drivers/net/wireless/bcmdhd4358)
$(shell mkdir -p out/target/product/treltexx/obj/KERNEL_OBJ/drivers/sensorhub/brcm/factory)
$(shell mkdir -p out/target/product/treltexx/obj/KERNEL_OBJ/drivers/media/platform/exynos/fimc-is/sensor)
$(shell mkdir -p out/target/product/treltexx/obj/KERNEL_OBJ/security/tima_uevent)

include $(call all-subdir-makefiles,$(LOCAL_PATH))

endif