require recipes-image/console/console-net-image.bb
require recipes-image/include/x11-base.inc

export IMAGE_BASENAME = "gnome-base-image"

# TBD: move to gdm
GNOME_DM ?= "sddm"

IMAGE_INSTALL += " \
    ${X11_BASE_INSTALL} \
    ${GNOME_DM} \
    \
    packagegroup-gnome-desktop \
    packagegroup-gui-base \
    \
    network-manager-applet \
"

# reenable graphical target
IMAGE_FEATURES += "x11-base"
