require ../console/console-base-image-nm.bb

export IMAGE_BASENAME = "kde-base-image"

IMAGE_INSTALL += " \
    packagegroup-core-x11-xserver \
    packagegroup-image-base \
    \
    sddm \
    \
    kf5-world \
    plasma-world \
"

# reenable graphical target
IMAGE_FEATURES += "x11-base"
