require lxqt-full-image.bb

export IMAGE_BASENAME = "lxqt-games-image"

IMAGE_INSTALL += " \
    packagegroup-image-games \
"
