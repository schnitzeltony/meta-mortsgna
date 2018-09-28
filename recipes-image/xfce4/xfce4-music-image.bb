require xfce4-tiny-image.bb

export IMAGE_BASENAME = "xfce4-music-image"

XFCE_DM = "sddm"

IMAGE_INSTALL += " \
    packagegroup-image-music \
    libreoffice \
    chromium-x11 \
"
