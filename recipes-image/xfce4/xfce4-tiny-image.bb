require xfce4-base-image.bb

export IMAGE_BASENAME = "xfce4-tiny-image"

IMAGE_INSTALL += " \
    packagegroup-xfce-extended \
    packagegroup-xfce-multimedia \
    rodent-icon-theme \
    gnome-theme-adwaita-dark \
    \
    packagegroup-image-tiny \
"

