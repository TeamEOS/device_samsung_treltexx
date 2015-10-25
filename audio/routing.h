/*
 * Copyright (C) 2013 The CyanogenMod Project
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

#ifndef _ROUTING_H_
#define _ROUTING_H_

enum {
    OUT_DEVICE_SPEAKER,
    OUT_DEVICE_EARPIECE,
    OUT_DEVICE_HEADSET,
    OUT_DEVICE_HEADPHONES,
    OUT_DEVICE_BT_SCO,
    OUT_DEVICE_BT_SCO_HEADSET_OUT,
    OUT_DEVICE_BT_SCO_CARKIT,
    OUT_DEVICE_SPEAKER_AND_HEADSET,
    OUT_DEVICE_SPEAKER_AND_EARPIECE,
    OUT_DEVICE_TAB_SIZE,           /* number of rows in route_configs[][] */
    OUT_DEVICE_NONE,
    OUT_DEVICE_CNT
};

enum {
    IN_SOURCE_MIC,
    IN_SOURCE_CAMCORDER,
    IN_SOURCE_VOICE_RECOGNITION,
    IN_SOURCE_VOICE_COMMUNICATION,
    IN_SOURCE_VOICE_CALL,
    IN_SOURCE_VOICE_CALL_WB,
    IN_SOURCE_TAB_SIZE,            /* number of lines in route_configs[][] */
    IN_SOURCE_NONE,
    IN_SOURCE_CNT
};

struct route_config {
    const char * const output_route;
    const char * const output_device;
    const char * const input_route;
    const char * const input_device;
};

const struct route_config voice_speaker = {
    "voice-speaker",
    "device-speaker",
    "voice-speaker-two-mic",
    "device-two-mic"
};

const struct route_config voice_speaker_wb = {
    "voice-speaker-wb",
    "device-speaker",
    "voice-speaker-two-mic-wb",
    "device-two-mic"
};

const struct route_config voice_earpiece = {
    "voice-earpiece",
    "device-earpiece",
    "voice-earpiece-two-mic",
    "device-two-mic"
};

const struct route_config voice_earpiece_wb = {
    "voice-earpiece-wb",
    "device-earpiece",
    "voice-earpiece-two-mic-wb",
    "device-two-mic"
};

const struct route_config voice_headphones = {
    "voice-headphones",
    "device-headphones",
    "voice-headphones-two-mic",
    "device-two-mic"
};

const struct route_config voice_headphones_wb = {
    "voice-headphones-wb",
    "device-headphones",
    "voice-headphones-two-mic-wb",
    "device-two-mic"
};

const struct route_config voice_headset = {
    "voice-headphones",
    "device-headphones",
    "voice-headset-mic",
    "device-headset-mic"
};

const struct route_config voice_headset_wb = {
    "voice-headphones-wb",
    "device-headphones",
    "voice-headset-mic-wb",
    "device-headset-mic"
};

const struct route_config voice_bt_sco = {
    "voice-bt-sco",
    "device-sco",
    "voice-bt-sco-headset-mic",
    "device-sco-headset-mic"
};

const struct route_config voice_bt_sco_wb = {
    "voice-bt-sco-wb",
    "device-sco",
    "voice-bt-sco-headset-mic-wb",
    "device-sco-headset-mic"
};

const struct route_config voice_bt_sco_headset_out = {
    "voice-bt-sco-headset-out",
    "device-sco-headset-out",
    "voice-bt-sco-headset-mic",
    "device-sco-headset-mic"
};

const struct route_config voice_bt_sco_headset_out_wb = {
    "voice-bt-sco-headset-out-wb",
    "device-sco-headset-out",
    "voice-bt-sco-headset-mic-wb",
    "device-sco-headset-mic"
};

const struct route_config media_speaker = {
    "media-speaker",
    "device-speaker",
    "media-builtin-mic",
    "device-builtin-mic"
};

const struct route_config media_earpiece = {
    "media-earpiece",
    "device-earpiece",
    "media-builtin-mic",
    "device-builtin-mic"
};

const struct route_config media_headphones = {
    "media-headphones",
    "device-headphones",
    "media-builtin-mic",
    "device-builtin-mic"
};

const struct route_config media_headset = {
    "media-headphones",
    "device-headphones",
    "media-headset-mic",
    "device-headset-mic"
};

const struct route_config media_bt_sco = {
    "media-bt-sco",
    "device-sco",
    "media-bt-sco-headset-mic",
    "device-sco-headset-mic"
};

const struct route_config media_bt_sco_headset_out = {
    "media-bt-sco",
    "device-sco-headset-out",
    "media-bt-sco-headset-mic",
    "device-sco-headset-mic"
};

const struct route_config camcorder_speaker = {
    "media-speaker",
    "device-speaker",
    "media-two-mic",
    "device-two-mic"
};

const struct route_config camcorder_headphones = {
    "media-headphones",
    "none",
    "media-second-mic",
    "none"
};

const struct route_config voice_rec_speaker = {
    "voice-rec-speaker",
    "device-speaker",
    "voice-rec-two-mic",
    "device-builtin-mic"
};

const struct route_config voice_rec_headphones = {
    "voice-rec-headphones",
    "device-headphones",
    "voice-rec-two-mic",
    "device-builtin-mic"
};

const struct route_config voice_rec_headset = {
    "voice-rec-headphones",
    "device-headphones",
    "voice-rec-headset-mic",
    "device-headset-mic"
};

const struct route_config communication_speaker = {
    "communication-speaker",
    "device-speaker",
    "communication-speaker-two-mic",
    "device-two-mic"
};

const struct route_config communication_earpiece = {
    "communication-earpiece",
    "device-earpiece",
    "communication-two-mic",
    "device-two-mic"
};

const struct route_config communication_headphones = {
    "communication-headphones",
    "device-headphones",
    "communication-two-mic",
    "device-two-mic"
};

const struct route_config communication_headset = {
    "communication-headphones",
    "device-headphones",
    "communication-headset-mic",
    "device-headset-mic"
};

const struct route_config speaker_and_headphones = {
    "speaker-and-headphones",
    "device-speaker",
    "scenario-two-mic-speaker-default",
    "device-two-mic"
};

const struct route_config bt_sco_carkit = {
    "bt-sco-carkit",
    "device-sco-carkit",
    "media-bt-sco-headset-mic",
    "device-sco-headset-mic"
};

const struct route_config none = {
    "none",
    "none",
    "none",
    "none"
};

const struct route_config * const route_configs[IN_SOURCE_TAB_SIZE]
                                               [OUT_DEVICE_TAB_SIZE] = {
    {   /* IN_SOURCE_MIC */
        &media_speaker,             /* OUT_DEVICE_SPEAKER */
        &media_earpiece,            /* OUT_DEVICE_EARPIECE */
        &media_headset,             /* OUT_DEVICE_HEADSET */
        &media_headphones,          /* OUT_DEVICE_HEADPHONES */
        &media_bt_sco,              /* OUT_DEVICE_BT_SCO */
        &media_bt_sco_headset_out,  /* OUT_DEVICE_BT_SCO_HEADSET_OUT */
        &bt_sco_carkit,             /* OUT_DEVICE_BT_SCO_CARKIT */
        &speaker_and_headphones,    /* OUT_DEVICE_SPEAKER_AND_HEADSET */
        &media_speaker              /* OUT_DEVICE_SPEAKER_AND_EARPIECE */
    },
    {   /* IN_SOURCE_CAMCORDER */
        &camcorder_speaker,         /* OUT_DEVICE_SPEAKER */
        &none,                      /* OUT_DEVICE_EARPIECE */
        &camcorder_headphones,      /* OUT_DEVICE_HEADSET */
        &camcorder_headphones,      /* OUT_DEVICE_HEADPHONES */
        &media_bt_sco,              /* OUT_DEVICE_BT_SCO */
        &media_bt_sco_headset_out,  /* OUT_DEVICE_BT_SCO_HEADSET_OUT */
        &bt_sco_carkit,             /* OUT_DEVICE_BT_SCO_CARKIT */
        &speaker_and_headphones,    /* OUT_DEVICE_SPEAKER_AND_HEADSET */
        &camcorder_speaker          /* OUT_DEVICE_SPEAKER_AND_EARPIECE */
    },
    {   /* IN_SOURCE_VOICE_RECOGNITION */
        &voice_rec_speaker,         /* OUT_DEVICE_SPEAKER */
        &none,                      /* OUT_DEVICE_EARPIECE */
        &voice_rec_headset,         /* OUT_DEVICE_HEADSET */
        &voice_rec_headphones,      /* OUT_DEVICE_HEADPHONES */
        &media_bt_sco,              /* OUT_DEVICE_BT_SCO */
        &media_bt_sco_headset_out,  /* OUT_DEVICE_BT_SCO_HEADSET_OUT */
        &bt_sco_carkit,             /* OUT_DEVICE_BT_SCO_CARKIT */
        &speaker_and_headphones,    /* OUT_DEVICE_SPEAKER_AND_HEADSET */
        &voice_rec_speaker          /* OUT_DEVICE_SPEAKER_AND_EARPIECE */
    },
    {   /* IN_SOURCE_VOICE_COMMUNICATION */
        &communication_speaker,     /* OUT_DEVICE_SPEAKER */
        &communication_earpiece,    /* OUT_DEVICE_EARPIECE */
        &communication_headset,     /* OUT_DEVICE_HEADSET */
        &communication_headphones,  /* OUT_DEVICE_HEADPHONES */
        &media_bt_sco,              /* OUT_DEVICE_BT_SCO */
        &media_bt_sco_headset_out,  /* OUT_DEVICE_BT_SCO_HEADSET_OUT */
        &bt_sco_carkit,             /* OUT_DEVICE_BT_SCO_CARKIT */
        &speaker_and_headphones,    /* OUT_DEVICE_SPEAKER_AND_HEADSET */
        &communication_earpiece     /* OUT_DEVICE_SPEAKER_AND_EARPIECE */
    },
    {   /* IN_SOURCE_VOICE_CALL */
        &voice_speaker,             /* OUT_DEVICE_SPEAKER */
        &voice_earpiece,            /* OUT_DEVICE_EARPIECE */
        &voice_headset,             /* OUT_DEVICE_HEADSET */
        &voice_headphones,          /* OUT_DEVICE_HEADPHONES */
        &voice_bt_sco,              /* OUT_DEVICE_BT_SCO */
        &voice_bt_sco_headset_out,  /* OUT_DEVICE_BT_SCO_HEADSET_OUT */
        &bt_sco_carkit,             /* OUT_DEVICE_BT_SCO_CARKIT */
        &voice_headphones,          /* OUT_DEVICE_SPEAKER_AND_HEADSET */
        &voice_earpiece             /* OUT_DEVICE_SPEAKER_AND_EARPIECE */
    },
    {   /* IN_SOURCE_VOICE_CALL_WB */
        &voice_speaker_wb,          /* OUT_DEVICE_SPEAKER */
        &voice_earpiece_wb,         /* OUT_DEVICE_EARPIECE */
        &voice_headset_wb,          /* OUT_DEVICE_HEADSET */
        &voice_headphones_wb,       /* OUT_DEVICE_HEADPHONES */
        &voice_bt_sco_wb,           /* OUT_DEVICE_BT_SCO */
        &voice_bt_sco_headset_out_wb, /* OUT_DEVICE_BT_SCO_HEADSET_OUT */
        &bt_sco_carkit,             /* OUT_DEVICE_BT_SCO_CARKIT */
        &voice_headphones_wb,       /* OUT_DEVICE_SPEAKER_AND_HEADSET */
        &voice_earpiece_wb          /* OUT_DEVICE_SPEAKER_AND_EARPIECE */
    },
};

#endif
