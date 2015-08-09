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
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml

###########################################################
### GRAPHICS
###########################################################

# This device is xhdpi.  However the platform doesn't
# currently contain all of the bitmaps at xhdpi density so
# we do this little trick to fall back to the hdpi version
# if the xhdpi doesn't exist.
PRODUCT_AAPT_CONFIG := normal hdpi xhdpi
PRODUCT_AAPT_PREF_CONFIG := xhdpi

PRODUCT_PROPERTY_OVERRIDES += \
    ro.opengles.version=196608 \
    ro.sf.lcd_density=560

PRODUCT_PACKAGES += \
    gralloc.exynos5

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
    af.fast_track_multiplier=1

###########################################################
### OMX/MEDIA
###########################################################

PRODUCT_COPY_FILES += \
    frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/etc/media_codecs_google_audio.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_telephony.xml:system/etc/media_codecs_google_telephony.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_video.xml:system/etc/media_codecs_google_video.xml  \
    $(LOCAL_PATH)/configs/media/media_codecs.xml:system/etc/media_codecs.xml \
    $(LOCAL_PATH)/configs/media/media_profiles.xml:system/etc/media_profiles.xml

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

PRODUCT_PACKAGES += \
    camera.universal5433

PRODUCT_PROPERTY_OVERRIDES += \
    camera2.portability.force_api=1

###########################################################
### TOUCHSCREEN
###########################################################

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/idc/Synaptics_HID_TouchPad.idc:/system/usr/idc/Synaptics_HID_TouchPad.idc \
    $(LOCAL_PATH)/configs/keylayout/gpio_keys_8.kl:/system/usr/keylayout/gpio_keys_8.kl \
    $(LOCAL_PATH)/configs/keylayout/sec_touchkey.kl:/system/usr/keylayout/sec_touchkey.kl

###########################################################
### STYLUS
###########################################################

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/idc/ft5x06_ts.idc:/system/usr/idc/ft5x06_ts.idc

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
    

$(call inherit-product-if-exists, frameworks/native/build/phone-xxhdpi-2048-dalvik-heap.mk)
$(call inherit-product-if-exists, frameworks/native/build/phone-xxhdpi-2048-hwui-memory.mk)
$(call inherit-product-if-exists, build/target/product/full.mk)

# call the proprietary setup
$(call inherit-product-if-exists, vendor/samsung/treltexx/treltexx-vendor.mk)

# call Samsung LSI board support package
$(call inherit-product, hardware/samsung_slsi/exynos5-insignal/exynos5.mk)
$(call inherit-product, hardware/samsung_slsi/exynos5433/exynos5433.mk)

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0
PRODUCT_NAME := full_treltexx
PRODUCT_DEVICE := treltexx
