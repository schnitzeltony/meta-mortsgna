require console-base-image-no-nm.bb

IMAGE_INSTALL += " \
    networkmanager \
"

export IMAGE_BASENAME = "console-base-image-nm"
