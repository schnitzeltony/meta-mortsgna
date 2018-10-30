require recipes-image/console/console-net-image.bb

export IMAGE_BASENAME = "weston-base-image"

IMAGE_INSTALL += " \
    ${DISTRO_GUI_USER} \
    packagegroup-gui-base \
    \
    sddm \
    \
    weston weston-xwayland \
    \
    mesa-demos \
    ${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'glmark2', '', d)} \
"

# reenable graphical target
IMAGE_FEATURES += "x11-base"
