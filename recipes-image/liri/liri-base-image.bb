require recipes-image/console/console-net-image.bb
require recipes-image/include/x11-base.inc

export IMAGE_BASENAME = "liri-base-image"

IMAGE_INSTALL += " \
    ${X11_BASE_INSTALL} \
    sddm \
    \
    liri-world \
"

# reenable graphical target
IMAGE_FEATURES += "x11-base"
