require recipes-image/console/console-net-image.bb
require recipes-image/include/x11-base.inc

export IMAGE_BASENAME = "kde-base-image"

IMAGE_INSTALL += " \
    ${X11_BASE_INSTALL} \
    packagegroup-gui-base \
    \
    sddm \
    \
    kf5-world \
    plasma-world \
    \
    konsole \
    dolphin \
"

# reenable graphical target
IMAGE_FEATURES += "x11-base"
