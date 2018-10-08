require recipes-image/console/console-net-image.bb
require recipes-image/include/x11-base.inc

export IMAGE_BASENAME = "lxqt-base-image"

LXQT_DM ?= "lxdm"

IMAGE_INSTALL += " \
    ${X11_BASE_INSTALL} \
    ${LXQT_DM} \
    \
    packagegroup-lxqt-base \
    \
    packagegroup-image-base \
    lxqt-default-config \
"

# reenable graphical target
IMAGE_FEATURES += "x11-base"
