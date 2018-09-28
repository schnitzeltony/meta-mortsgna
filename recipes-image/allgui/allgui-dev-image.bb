require allgui-full-image.bb

export IMAGE_BASENAME = "allgui-dev-image"

IMAGE_INSTALL += " \
    packagegroup-image-dev \
    \
    exo-csource \
    xfce4-dev-tools \
    libxfce4ui-glade \
"
