SUMMARY = "All packages required for a dev image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

RDEPENDS:${PN} = " \
    qt5-creator qtwebengine-dev \
    qtquickcontrols2-examples qtwayland-examples \
    hexedit \
    glade \
    umbrello \
    lokalize \
    geany-plugins \
    gtk+3-demo \
    \
    weston weston-xwayland  \
    \
    unzip xz tar bzip2 \
    make cmake extra-cmake-modules meson \
    automake autoconf libtool m4 \
    gcc-symlinks g++-symlinks cpp-symlinks libgomp-dev \
    dtc \
    pkgconfig binutils-symlinks elfutils gnu-config util-linux \
    gdb gdbserver \
    valgrind \
    git git-bash-completion git-perltools subversion \
    python3-core \
    perl php vala lua php json-glib \
    texinfo gettext \
    patch quilt ncurses dialog kdialog \
    man man-pages \
    gawk \
    chrpath \
    nodejs nodejs-npm nodejs-systemtap \
    \
    ruby ruby-dev \
    \
    libeigen-dev \
    \
    wireshark \
    nfs-utils-client \
    lmsensors-sensord lmsensors-sensorsdetect \
    \
    pulseview qtiohelper minicom i2c-tools \
    python3-pyserial python3-smbus python3-smbus2 python3-spidev \
    \
    ${@bb.utils.contains_any('BBFILE_COLLECTIONS', 'browser-layer chromium-browser-layer', 'chromium-x11', '', d)} \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-microcontroller', 'meta-microcontroller-world', '', d)} \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'rubygems', 'rubygems-jekyll', '', d)} \
"

RDEPENDS:${PN}:append:rpi = "rpi-gpio"

#    qemu
#    

