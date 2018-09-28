require allgui-base-image.bb

export IMAGE_BASENAME = "allgui-tiny-image"

IMAGE_INSTALL += " \
    packagegroup-image-tiny \
    qpdfview \
    \
    packagegroup-xfce-extended \
    packagegroup-xfce-multimedia \
    rodent-icon-theme \
    gnome-theme-adwaita-dark \
    adwaita-qt \
    \
    packagegroup-kde-apps-tiny \
    \
    weston-examples \
"
