require allgui-tiny-image.bb

export IMAGE_BASENAME = "allgui-full-image"

IMAGE_INSTALL += " \
    packagegroup-gui-full \
    kde-world \
    \
    onboard \
    \
    faenza-icon-theme \
    openzone \
    \
    orage \
    xfce4-orageclock-plugin \
    \
    pkgconfig binutils-symlinks elfutils gnu-config \
    cmake meson \
    gdbserver \
    git git-bash-completion \
    qt5-creator \
    qwt-qt5 \
    \
    nodejs \
"

IMAGE_LINGUAS_libc-glibc = "${IMAGE_LINGUAS_FULL}"
