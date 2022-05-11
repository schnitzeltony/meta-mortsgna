SUMMARY = "All packages required for a dev image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# json-glib gets dynamically renamed
PACKAGE_ARCH = "${TUNE_PKGARCH}"

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
    ccache gcc-symlinks g++-symlinks cpp-symlinks libgomp-dev \
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
    googletest-dev \
    protobuf protobuf-compiler protobuf-dev \
    flatbuffers flatbuffers-compiler flatbuffers-dev flatbuffers-staticdev flatbuffers-python3 \
    modemmanager-dev modemmanager-qt-dev \
    networkmanager-dev networkmanager-qt-dev libnma-dev \
    qtvirtualkeyboard-dev qwt-qt5-dev \
    syntax-highlighting-dev \
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
    epiphany ${@bb.utils.contains_any('BBFILE_COLLECTIONS', 'browser-layer chromium-browser-layer', 'chromium-x11', '', d)} \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-microcontroller', 'meta-microcontroller-world', '', d)} \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'rubygems', 'rubygems-jekyll', '', d)} \
    \
    fftw-dev fftwf-wisdom fftw-wisdom fftwl-wisdom \
    libxerces-c libxerces-c-dev \
    libxml2-utils \
"

RDEPENDS:${PN}:append:rpi = "rpi-gpio"

#    qemu
#    

