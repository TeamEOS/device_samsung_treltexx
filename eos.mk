# Release name
PRODUCT_RELEASE_NAME := treltexx

# Inherit some common EOS stuff.
$(call inherit-product, vendor/eos/config/common_full_phone.mk)

# Inherit nfc enhanced configuration
#$(call inherit-product, vendor/eos/config/nfc_enhanced.mk)

# Inherit device configuration
$(call inherit-product, device/samsung/treltexx/device.mk)

## Device identifier. This must come after all inclusions
PRODUCT_NAME := eos_treltexx
PRODUCT_DEVICE := treltexx
PRODUCT_BRAND := samsung
PRODUCT_MODEL := SM-N910C
PRODUCT_MANUFACTURER := samsung

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRODUCT_NAME=treltexx