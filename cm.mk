# Release name
PRODUCT_RELEASE_NAME := treltexx

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/samsung/treltexx/device_treltexx.mk)

## Device identifier. This must come after all inclusions
PRODUCT_NAME := treltexx
PRODUCT_DEVICE := treltexx
PRODUCT_BRAND := samsung
PRODUCT_MODEL := SM-N910C
PRODUCT_MANUFACTURER := samsung
