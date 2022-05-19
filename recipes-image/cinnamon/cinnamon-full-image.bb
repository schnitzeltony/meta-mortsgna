require cinnamon-tiny-image.bb

export IMAGE_BASENAME = "cinnamon-full-image"

IMAGE_INSTALL += " \
    packagegroup-gui-full \
"

