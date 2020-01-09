require xfce4-base-image.bb

export IMAGE_BASENAME = "xfce4-tiny-image"

IMAGE_INSTALL += " \
    packagegroup-xfce-extended \
    packagegroup-xfce-multimedia \
    rodent-icon-theme \
    \
    packagegroup-gui-tiny \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-musicians', 'xfce4-mixer', '', d)} \
"

