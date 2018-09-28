require liri-base-image.bb

export IMAGE_BASENAME = "liri-tiny-image"

IMAGE_INSTALL += " \
    packagegroup-image-tiny \
    qpdfview \
"

IMAGE_INSTALL_remove = "evince"
