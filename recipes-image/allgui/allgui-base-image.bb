require ../console/console-base-image-nm.bb

export IMAGE_BASENAME = "allgui-base-image"

# currently: plasma / xfce / lxqt
# not included: gnome / efl
IMAGE_INSTALL += " \
    packagegroup-core-x11-xserver \
    packagegroup-image-base \
    \
    sddm \
    \
    kf5-world \
    plasma-world \
    \
    packagegroup-xfce-base \
    \
    packagegroup-lxqt-base \
    lxqt-default-config \
    \
    lumina \
    \
    weston weston-xwayland \
    \
    mesa-demos \
"

# reenable graphical target
IMAGE_FEATURES += "x11-base"
