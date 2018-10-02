require console-base-image.bb

# by default it connects by dhcp and does useful initializations - e.g for wifi
IMAGE_INSTALL += " \
    networkmanager \
"

export IMAGE_BASENAME = "console-base-image-nm"
