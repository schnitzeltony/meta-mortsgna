require recipes-image/console/console-net-image.bb

export IMAGE_BASENAME = "weston-base-image"

inherit user-with-full-skel

IMAGE_INSTALL += " \
    ${DISTRO_GUI_USER} \
    \
    sddm \
    \
    weston \
    weston-examples \
"

# reenable graphical target
IMAGE_FEATURES += "x11-base"
