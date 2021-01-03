require allgui-full-image.bb

export IMAGE_BASENAME = "allgui-dev-image"

IMAGE_INSTALL += " \
    packagegroup-gui-dev \
    \
    xfce4-dev-tools \
    libxfce4ui-glade \
"
