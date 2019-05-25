require allgui-base-image.bb

export IMAGE_BASENAME = "allgui-tiny-image"

IMAGE_INSTALL += " \
    packagegroup-gui-tiny \
    qpdfview \
    \
    packagegroup-xfce-extended \
    packagegroup-xfce-multimedia \
    rodent-icon-theme \
    adwaita-qt \
    \
    packagegroup-kde-apps-tiny \
    \
    packagegroup-gnome-apps \
    \
    weston-examples \
"
