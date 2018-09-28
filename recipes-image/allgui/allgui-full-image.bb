require allgui-tiny-image.bb

export IMAGE_BASENAME = "allgui-full-image"

IMAGE_INSTALL += " \
    packagegroup-image-full \
    kde-world \
    \
    gnome-bluetooth \
    onboard \
    \
    faenza-icon-theme \
    openzone \
    \
    orage \
    xfce4-orageclock-plugin \
"
