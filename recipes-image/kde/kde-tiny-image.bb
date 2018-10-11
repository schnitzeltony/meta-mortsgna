require kde-base-image.bb

export IMAGE_BASENAME = "kde-tiny-image"

IMAGE_INSTALL += " \
    packagegroup-gui-tiny \
    packagegroup-kde-apps-tiny \
"
