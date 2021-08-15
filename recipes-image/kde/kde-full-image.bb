require kde-tiny-image.bb

export IMAGE_BASENAME = "kde-full-image"

IMAGE_INSTALL += " \
    kde-world \
    packagegroup-gui-full \
" 

IMAGE_LINGUAS:libc-glibc = "${IMAGE_LINGUAS_FULL}"
