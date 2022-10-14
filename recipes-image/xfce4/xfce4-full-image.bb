require xfce4-tiny-image.bb

XFCE_DM = "sddm sddm-morona-autologin"

export IMAGE_BASENAME = "xfce4-full-image"

IMAGE_INSTALL += " \
    packagegroup-gui-full \
    \
    faenza-icon-theme \
    openzone \
    \
    orage \
    xfce4-orageclock-plugin \
"
