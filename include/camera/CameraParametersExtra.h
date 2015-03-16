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

#define CAMERA_PARAMETERS_EXTRA_C \
    const char CameraParameters::PIXEL_FORMAT_YUV420SP_NV21[] = "yuv420sp"; \
    const char CameraParameters::EFFECT_CARTOONIZE[] = "cartoonize"; \
    const char CameraParameters::EFFECT_POINT_RED_YELLOW[] = "point-red-yellow"; \
    const char CameraParameters::EFFECT_POINT_GREEN[] = "point-green"; \
    const char CameraParameters::EFFECT_POINT_BLUE[] = "point-blue"; \
    const char CameraParameters::EFFECT_VINTAGE_COLD[] = "vintage-cold"; \
    const char CameraParameters::EFFECT_VINTAGE_WARM[] = "vintage-warm"; \
    const char CameraParameters::EFFECT_WASHED[] = "washed";

#define CAMERA_PARAMETERS_EXTRA_H \
    static const char PIXEL_FORMAT_YUV420SP_NV21[]; \
    static const char EFFECT_CARTOONIZE[]; \
    static const char EFFECT_POINT_RED_YELLOW[]; \
    static const char EFFECT_POINT_GREEN[]; \
    static const char EFFECT_POINT_BLUE[]; \
    static const char EFFECT_VINTAGE_COLD[]; \
    static const char EFFECT_VINTAGE_WARM[]; \
    static const char EFFECT_WASHED[];
