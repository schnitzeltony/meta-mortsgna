SUMMARY = "All packages required for a dev image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

RDEPENDS_${PN} = " \
    qt5-creator \
    qtquickcontrols2-examples qtwayland-examples \
    hexedit \
    glade3 \
    umbrello \
    lokalize \
    geany-plugins \
    gtk-demo gtk+3-demo \
    \
    weston weston-xwayland  \
    \
    unzip xz tar bzip2 \
    make cmake \
    automake autoconf libtool m4 \
    gcc-symlinks g++-symlinks cpp-symlinks \
    dtc \
    pkgconfig binutils-symlinks elfutils gnu-config \
    gdb gdbserver \
    git git-bash-completion subversion \
    python python-shell python-subprocess \
    python3 \
    perl php vala lua php json-glib \
    texinfo gettext \
    patch quilt ncurses dialog \
    man man-pages \
    gawk \
    chrpath \
    \
    wireshark \
    nfs-utils-client \
    lmsensors-sensord lmsensors-sensorsdetect \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'browser-layer', 'chromium-x11', '', d)} \
"

#    qemu
#    valgrind

