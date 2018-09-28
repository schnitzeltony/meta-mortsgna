require ../console/console-base-image-nm.bb

export IMAGE_BASENAME = "xfce4-base-image"

XFCE_DM ?= "lxdm"

IMAGE_INSTALL += " \
    packagegroup-core-x11-xserver \
    ${XFCE_DM} \
    \
    packagegroup-gnome-xserver-base \
    packagegroup-xfce-base \
    packagegroup-image-base \
    \
    network-manager-applet \
    xfce4-default-config \
"


# reenable graphical target
IMAGE_FEATURES += "x11-base"
