require allgui-tiny-image.bb

export IMAGE_BASENAME = "allgui-full-image"

IMAGE_INSTALL += " \
    packagegroup-gui-full \
    kde-world \
    \
    onboard \
    \
    faenza-icon-theme \
    openzone \
    \
    orage \
    xfce4-orageclock-plugin \
"

IMAGE_LINGUAS = "${IMAGE_LINGUAS_FULL}"
