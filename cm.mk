# Release name
PRODUCT_RELEASE_NAME := trelte

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/samsung/trelte/device_trelte.mk)

## Device identifier. This must come after all inclusions
PRODUCT_NAME := cm_trelte
PRODUCT_DEVICE := trelte
PRODUCT_BRAND := samsung
PRODUCT_MODEL := SM-N910C
PRODUCT_MANUFACTURER := samsung
