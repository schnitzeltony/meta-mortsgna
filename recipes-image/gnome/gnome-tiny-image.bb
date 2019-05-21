require gnome-base-image.bb

export IMAGE_BASENAME = "gnome-tiny-image"

IMAGE_INSTALL += " \
    packagegroup-gnome-apps \
    \
    packagegroup-gui-tiny \
"

