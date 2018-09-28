require ../console/console-base-image-nm.bb

export IMAGE_BASENAME = "lxqt-base-image"

LXQT_DM ?= "lxdm"

IMAGE_INSTALL += " \
    packagegroup-gnome-xserver-base \
    packagegroup-core-x11-xserver \
    ${LXQT_DM} \
    \
    packagegroup-lxqt-base \
    \
    packagegroup-image-base \
    lxqt-default-config \
"


# reenable graphical target
IMAGE_FEATURES += "x11-base"
