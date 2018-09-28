require kde-tiny-image.bb

export IMAGE_BASENAME = "kde-full-image"

IMAGE_INSTALL += " \
    kde-world \
    packagegroup-image-full \
" 
