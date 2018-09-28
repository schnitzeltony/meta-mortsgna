require recipes-image/xfce4/xfce4-full-image.bb

export IMAGE_BASENAME = "xfce4-dev-image"

IMAGE_INSTALL += " \
    packagegroup-image-dev \
    \
    exo-csource \
    xfce4-dev-tools \
    libxfce4ui-glade \
"
