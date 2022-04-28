require cinnamon-base-image.bb

export IMAGE_BASENAME = "cinnamon-tiny-image"

IMAGE_INSTALL += " \
    packagegroup-gui-tiny \
"

