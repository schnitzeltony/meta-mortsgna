require xfce4-full-image.bb

export IMAGE_BASENAME = "xfce4-games-image"

IMAGE_INSTALL += " \
    packagegroup-image-games \
"
