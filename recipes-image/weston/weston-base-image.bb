require recipes-image/console/console-net-image.bb

export IMAGE_BASENAME = "weston-base-image"

IMAGE_INSTALL += " \
    ${DISTRO_GUI_USER} \
    \
    sddm \
    \
    weston \
"

# reenable graphical target
IMAGE_FEATURES += "x11-base"
