require xfce4-tiny-image.bb

export IMAGE_BASENAME = "xfce4-full-image"

IMAGE_INSTALL += " \
    packagegroup-image-full \
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
