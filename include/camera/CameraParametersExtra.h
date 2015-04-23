/*
 * Copyright (C) 2014 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "camera/CameraParametersExtraDurationTimer.h"

#define CAMERA_PARAMETERS_EXTRA_C \
    const char CameraParameters::PIXEL_FORMAT_YUV420SP_NV21[] = "yuv420sp"; \
    const char CameraParameters::EFFECT_WASHED[] = "washed"; \
    const char CameraParameters::EFFECT_COLD[] = "cold"; \
    const char CameraParameters::EFFECT_WARM[] = "warm"; \
    const char CameraParameters::EFFECT_POINT_YELLOW[] = "point-yellow"; \
    const char CameraParameters::EFFECT_POINT_RED[] = "point-red"; \
    const char CameraParameters::EFFECT_POINT_BLUE[] = "point-blue"; \
    const char CameraParameters::EFFECT_ANTIQUE[] = "antique"; \
    const char CameraParameters::KEY_CITYID[] = "contextualtag-cityid"; \
    const char CameraParameters::KEY_ISO[] = "iso"; \
    const char CameraParameters::KEY_METERING[] = "metering"; \
    const char CameraParameters::KEY_SUPPORTED_METERING_MODE[] = "metering-mode-values"; \
    const char CameraParameters::KEY_WEATHER[] = "weather"; \
    const char CameraParameters::KEY_DYNAMIC_RANGE_CONTROL[] = "dynamic-range-control"; \
    const char CameraParameters::KEY_SUPPORTED_DYNAMIC_RANGE_CONTROL[] = "dynamic-range-control-values"; \
    const char CameraParameters::KEY_PHASE_AF[] = "phase-af"; \
    const char CameraParameters::KEY_SUPPORTED_PHASE_AF[] = "phase-af-values"; \
    const char CameraParameters::KEY_RT_HDR[] = "rt-hdr"; \
    const char CameraParameters::KEY_SUPPORTED_RT_HDR[] = "rt-hdr-values"; \
    const char CameraParameters::METERING_CENTER[] = "center"; \
    const char CameraParameters::METERING_MATRIX[] = "matrix"; \
    const char CameraParameters::METERING_SPOT[] = "spot"; \
    const char CameraParameters::ISO_NIGHT[] = "night"; \
    const char CameraParameters::ISO_SPORT[] = "sport"; \
    const char CameraParameters::ISO_6400[] = "ISO6400"; \
    const char CameraParameters::ISO_3200[] = "ISO3200"; \
    const char CameraParameters::ISO_1600[] = "ISO1600"; \
    const char CameraParameters::ISO_800[] = "ISO800"; \
    const char CameraParameters::ISO_400[] = "ISO400"; \
    const char CameraParameters::ISO_200[] = "ISO200"; \
    const char CameraParameters::ISO_100[] = "ISO100"; \
    const char CameraParameters::ISO_80[] = "ISO80"; \
    const char CameraParameters::ISO_50[] = "ISO50"; \
    const char CameraParameters::ISO_AUTO[] = "auto"; \
    CAMERA_PARAMETERS_EXTRA_C_DURATION_TIMER \
    \
    /* LAST_LINE OF CAMERA_PARAMETERS_EXTRA_C, every line before this one *MUST* have
     * a backslash \ at the end of the line or else everything will break.
     */

#define CAMERA_PARAMETERS_EXTRA_H \
    static const char PIXEL_FORMAT_YUV420SP_NV21[]; \
    static const char EFFECT_WASHED[]; \
    static const char EFFECT_COLD[]; \
    static const char EFFECT_WARM[]; \
    static const char EFFECT_POINT_YELLOW[]; \
    static const char EFFECT_POINT_RED[]; \
    static const char EFFECT_POINT_BLUE[]; \
    static const char EFFECT_ANTIQUE[]; \
    static const char KEY_ISO[]; \
    static const char KEY_CITYID[]; \
    static const char KEY_METERING[]; \
    static const char KEY_SUPPORTED_METERING_MODE[]; \
    static const char KEY_WEATHER[]; \
    static const char KEY_DYNAMIC_RANGE_CONTROL[]; \
    static const char KEY_SUPPORTED_DYNAMIC_RANGE_CONTROL[]; \
    static const char KEY_PHASE_AF[]; \
    static const char KEY_SUPPORTED_PHASE_AF[]; \
    static const char KEY_RT_HDR[]; \
    static const char KEY_SUPPORTED_RT_HDR[]; \
    static const char METERING_CENTER[]; \
    static const char METERING_MATRIX[]; \
    static const char METERING_SPOT[]; \
    static const char ISO_NIGHT[]; \
    static const char ISO_SPORT[]; \
    static const char ISO_6400[]; \
    static const char ISO_3200[]; \
    static const char ISO_1600[]; \
    static const char ISO_800[]; \
    static const char ISO_400[]; \
    static const char ISO_200[]; \
    static const char ISO_100[]; \
    static const char ISO_80[]; \
    static const char ISO_50[]; \
    static const char ISO_AUTO[]; \
    \
    /* LAST_LINE OF CAMERA_PARAMETERS_EXTRA_H, every line before this one *MUST* have
     * a backslash \ at the end of the line or else everything will break.
     */
