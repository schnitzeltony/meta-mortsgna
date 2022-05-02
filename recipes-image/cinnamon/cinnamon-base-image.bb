require recipes-image/console/console-net-image.bb
require recipes-image/include/x11-base.inc

export IMAGE_BASENAME = "cinnamon-base-image"

CINNAMON_DM ?= "lxdm"

IMAGE_INSTALL += " \
    ${X11_BASE_INSTALL} \
    ${CINNAMON_DM} \
    \
    cinnamon-base \
    cinnamon-default-config \
    packagegroup-gui-base \
    \
    xfce4-terminal \
"

# reenable graphical target
IMAGE_FEATURES += "x11-base"
