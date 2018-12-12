require recipes-image/console/console-net-image.bb
require recipes-image/include/x11-base.inc

export IMAGE_BASENAME = "allgui-base-image"

# currently: plasma / xfce / lxqt / weston
IMAGE_INSTALL += " \
    ${X11_BASE_INSTALL} \
    packagegroup-gui-base \
    \
    sddm sddm-morona-autologin \
    \
    kf5-world \
    plasma-world \
    \
    packagegroup-xfce-base \
    network-manager-applet \
    xfce4-default-config \
    \
    packagegroup-lxqt-base \
    lxqt-default-config \
    \
    liri-world \
    \
    lumina \
    \
    weston weston-xwayland \
"

# reenable graphical target
IMAGE_FEATURES += "x11-base"
