require recipes-image/xfce4/xfce4-tiny-image.bb

XFCE_DM = "sddm sddm-morona-autologin"

export IMAGE_BASENAME = "xfce4-dev-image"

IMAGE_INSTALL += " \
    packagegroup-gui-dev \
    \
    exo-csource \
    xfce4-dev-tools \
    libxfce4ui-glade \
"
