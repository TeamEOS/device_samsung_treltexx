$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_eu_supl.mk)

DEVICE_PACKAGE_OVERLAYS += device/samsung/treltexx/overlay

LOCAL_PATH := device/samsung/treltexx

###########################################################
### FLAT DEVICE TREE
###########################################################

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/dtb.img:dt.img

###########################################################
### RAMDISK
###########################################################

PRODUCT_PACKAGES += \
    fstab.universal5433 \
    init.carrier.rc \
    init.recovery.universal5433.rc \
    init.samsung.rc \
    init.universal5433.rc \
    init.universal5433.usb.rc \
    init.universal5433.wifi.rc \
    ueventd.universal5433.rc

###########################################################
### BCMDHD DRIVER
###########################################################

PRODUCT_PACKAGES += \
    init.sec.boot.sh

###########################################################
### PERMISSONS
###########################################################

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.audio.low_latency.xml:system/etc/permissions/android.hardware.audio.low_latency.xml \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.consumerir.xml:system/etc/permissions/android.hardware.consumerir.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.nfc.hce.xml:system/etc/permissions/android.hardware.nfc.hce.xml \
    frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml \
    frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.barometer.xml:system/etc/permissions/android.hardware.sensor.barometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.sensor.stepcounter.xml:system/etc/permissions/android.hardware.sensor.stepcounter.xml \
    frameworks/native/data/etc/android.hardware.sensor.stepdetector.xml:system/etc/permissions/android.hardware.sensor.stepdetector.xml \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    frameworks/native/data/etc/android.software.sip.xml:system/etc/permissions/android.software.sip.xml \
    frameworks/native/data/etc/com.android.nfc_extras.xml:system/etc/permissions/com.android.nfc_extras.xml \
    frameworks/native/data/etc/com.nxp.mifare.xml:system/etc/permissions/com.nxp.mifare.xml \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml

###########################################################
### GRAPHICS
###########################################################

PRODUCT_AAPT_CONFIG := normal hdpi xhdpi xxhdpi 560dpi xxxhdpi
PRODUCT_AAPT_PREF_CONFIG := 560dpi

# OpenGLES:
# 196608 is decimal for 0x30000 to report major/minor versions as 3/0
# 196609 is decimal for 0x30001 to report major/minor versions as 3/1
# Set to 3.0 (even though the blobs support 3.1) to maintain compatibility
# with third party applications that do not support 3.1
PRODUCT_PROPERTY_OVERRIDES += \
    ro.opengles.version=196608 \
    ro.sf.lcd_density=560 \
    ro.bq.gpu_to_cpu_unsupported=1

PRODUCT_PACKAGES += \
    gralloc.exynos5 \
    hwcomposer.exynos5 \
    memtrack.exynos5

PRODUCT_PACKAGES += \
    libion \
    libcec

###########################################################
### RADIO
###########################################################

# cpboot-daemon for modem
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/ril/sbin/cbd:root/sbin/cbd

PRODUCT_PACKAGES += \
    libsecril-client \
    libsecril-client-sap

PRODUCT_PROPERTY_OVERRIDES += \
    rild.libargs=-d /dev/ttyS0 \
    rild.libpath=/system/lib/libsec-ril.so \
    ro.ril.hsxpa=1 \
    ro.ril.gprsclass=10 \
    ro.telephony.ril_class=SlteRIL \
    ro.use_data_netmgrd=false \
    persist.data.netmgrd.qos.enable=false \
    ro.data.large_tcp_window_size=true \
    telephony.lteOnCdmaDevice=0 \
    persist.radio.add_power_save=1 \
    persist.radio.apm_sim_not_pwdn=1

###########################################################
### WIFI
###########################################################

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/wifi/p2p_supplicant_overlay.conf:system/etc/wifi/p2p_supplicant_overlay.conf \
    $(LOCAL_PATH)/configs/wifi/wpa_supplicant_overlay.conf:system/etc/wifi/wpa_supplicant_overlay.conf

PRODUCT_PROPERTY_OVERRIDES += \
    wifi.interface=wlan0

PRODUCT_PACKAGES += \
    libnetcmdiface \
    macloader \
    hostapd \
    libwpa_client \
    wpa_supplicant

# hardware/broadcom/wlan/bcmdhd/config/Android.mk
PRODUCT_PACKAGES += \
    dhcpcd.conf

# external/wpa_supplicant_8/wpa_supplicant/wpa_supplicant_conf.mk
PRODUCT_PACKAGES += \
    wpa_supplicant.conf

###########################################################
### NFC
###########################################################

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/nfc/libnfc-sec-hal.conf:system/etc/libnfc-sec-hal.conf \
    $(LOCAL_PATH)/configs/nfc/libnfc-sec.conf:system/etc/libnfc-brcm.conf \
    $(LOCAL_PATH)/configs/nfc/nfcee_access.xml:system/etc/nfcee_access.xml

PRODUCT_PACKAGES += \
    com.android.nfc_extras \
    libnfc_nci_jni \
    libnfc-nci \
    NfcNci \
    Tag

PRODUCT_PROPERTY_OVERRIDES += \
    ro.nfc.sec_hal=true

###########################################################
### BLUETOOTH
###########################################################
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/bluetooth/bt_vendor.conf:system/etc/bluetooth/bt_vendor.conf

###########################################################
### AUDIO
###########################################################

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/audio/audio_policy.conf:system/etc/audio_policy.conf \
    $(LOCAL_PATH)/configs/audio/mixer_paths.xml:system/etc/mixer_paths.xml

PRODUCT_PACKAGES += \
    audio.a2dp.default \
    audio.usb.default \
    audio.r_submix.default \
    audio.primary.universal5433

PRODUCT_PROPERTY_OVERRIDES += \
    af.fast_track_multiplier=1 \
    audio_hal.force_wideband=true

###########################################################
### OMX/MEDIA
###########################################################

PRODUCT_COPY_FILES += \
    frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/etc/media_codecs_google_audio.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_telephony.xml:system/etc/media_codecs_google_telephony.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_video.xml:system/etc/media_codecs_google_video.xml  \
    $(LOCAL_PATH)/configs/media/media_codecs.xml:system/etc/media_codecs.xml \
    $(LOCAL_PATH)/configs/media/media_profiles.xml:system/etc/media_profiles.xml

# Audio codecs
PRODUCT_PACKAGES += \
    libOMX.Exynos.AAC.Decoder \
    libOMX.Exynos.FLAC.Decoder \
    libOMX.Exynos.MP3.Decoder \
    libOMX.Exynos.WMA.Encoder

# Seiren hardware audio decoder
PRODUCT_PACKAGES += \
    libseirenhw

# Stagefright and device specific modules
PRODUCT_PACKAGES += \
    libstagefrighthw \
    libExynosOMX_Core

# Video codecs
PRODUCT_PACKAGES += \
    libOMX.Exynos.AVC.Decoder \
    libOMX.Exynos.HEVC.Decoder \
    libOMX.Exynos.MPEG4.Decoder \
    libOMX.Exynos.MPEG4.Encoder \
    libOMX.Exynos.VP8.Decoder \
    libOMX.Exynos.WMV.Decoder

# H.264 encoder is broken
#PRODUCT_PACKAGES += \
#    libOMX.Exynos.AVC.Encoder


# Some Exynos HW codecs require AwesomePlayer
PRODUCT_PROPERTY_OVERRIDES += \
	persist.sys.media.use-awesome=true

###########################################################
### POWER
###########################################################

PRODUCT_PACKAGES += \
    power.universal5433

###########################################################
### LIGHTS
###########################################################

PRODUCT_PACKAGES += \
    lights.universal5433

###########################################################
### GPS
###########################################################

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/gps/gps.conf:system/etc/gps.conf \
    $(LOCAL_PATH)/configs/gps/gps.xml:system/etc/gps.xml

###########################################################
### CAMERA
###########################################################

PRODUCT_PROPERTY_OVERRIDES += \
    camera2.portability.force_api=1

###########################################################
### TOUCHSCREEN
###########################################################

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/idc/Synaptics_HID_TouchPad.idc:/system/usr/idc/Synaptics_HID_TouchPad.idc \
    $(LOCAL_PATH)/configs/idc/Synaptics_RMI4_TouchPad_Sensor.idc:/system/usr/idc/Synaptics_RMI4_TouchPad_Sensor.idc \
    $(LOCAL_PATH)/configs/keylayout/gpio_keys_8.kl:/system/usr/keylayout/gpio_keys_8.kl \
    $(LOCAL_PATH)/configs/keylayout/sec_touchkey.kl:/system/usr/keylayout/sec_touchkey.kl

###########################################################
### STYLUS
###########################################################

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/idc/ft5x06_ts.idc:/system/usr/idc/ft5x06_ts.idc \
    $(LOCAL_PATH)/configs/idc/sec_e-pen.idc:/system/usr/idc/sec_e-pen.idc

###########################################################
### CHARGER
###########################################################

PRODUCT_PACKAGES += \
    charger_res_images \
    charger

###########################################################
### MTP
###########################################################

PRODUCT_PACKAGES += \
    com.android.future.usb.accessory

# Enable USB OTG support
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.isUsbOtgEnabled=true

# Default to MTP protocol
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.usb.config=mtp

###########################################################
### FILESYSTEM MANAGEMENT
###########################################################

PRODUCT_PACKAGES += \
    make_ext4fs \
    e2fsck \
    setup_fs

###########################################################
### MOBICORE
###########################################################

PRODUCT_PACKAGES += \
    libMcClient \
    libMcRegistry \
    libgdmcprov \
    mcDriverDaemon

###########################################################
### PACKAGES
###########################################################

PRODUCT_PACKAGES += \
    SamsungDoze \
    SamsungServiceMode \
    Torch

###########################################################
### RANDOM NUMBER GENERATOR
###########################################################

PRODUCT_PACKAGES += \
    exyrngd

###########################################################
### DEFAULT PROPS
###########################################################

ADDITIONAL_DEFAULT_PROPERTIES += \
    ro.debug_level=0x4948 \
    ro.secure=0

###########################################################
### DALVIK/ART
###########################################################

PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.heapstartsize=8m \
    dalvik.vm.heapgrowthlimit=256m \
    dalvik.vm.heapsize=512m \
    dalvik.vm.heaptargetutilization=0.75 \
    dalvik.vm.heapminfree=2m \
    dalvik.vm.heapmaxfree=8m

###########################################################
### HWUI
###########################################################

PRODUCT_PROPERTY_OVERRIDES += \
    ro.hwui.texture_cache_size=88 \
    ro.hwui.layer_cache_size=58 \
    ro.hwui.path_cache_size=32 \
    ro.hwui.shape_cache_size=4 \
    ro.hwui.gradient_cache_size=2 \
    ro.hwui.drop_shadow_cache_size=8 \
    ro.hwui.r_buffer_cache_size=8 \
    ro.hwui.text_small_cache_width=2048 \
    ro.hwui.text_small_cache_height=2048 \
    ro.hwui.text_large_cache_width=4096 \
    ro.hwui.text_large_cache_height=4096

$(call inherit-product-if-exists, build/target/product/full.mk)

# call the proprietary setup
$(call inherit-product-if-exists, vendor/samsung/treltexx/treltexx-vendor.mk)

# call Samsung LSI board support package
$(call inherit-product, hardware/samsung_slsi/exynos5433/exynos5433.mk)

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0
PRODUCT_NAME := full_treltexx
PRODUCT_DEVICE := treltexx
