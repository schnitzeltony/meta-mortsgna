SUMMARY = "All packages required for a dev image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

RDEPENDS_${PN} = " \
    qt5-creator \
    hexedit \
    glade3 \
    umbrello \
    lokalize \
    geany-plugins \
    gtk-demo gtk+3-demo \
    \
    unzip xz tar bzip2 \
    make cmake \
    automake autoconf libtool m4 \
    gcc-symlinks g++-symlinks cpp-symlinks \
    dtc \
    pkgconfig binutils-symlinks elfutils coreutils gnu-config \
    gdb gdbserver \
    git git-bash-completion subversion \
    python-shell python-subprocess \
    python perl php vala lua php json-glib \
    texinfo gettext \
    patch quilt ncurses dialog \
    man man-pages \
    gawk \
    chrpath \
    \
    qtbase-tools \
    qtbase-plugins \
    qtbase-examples \
    qtbase-doc \
    qtdeclarative-tools \
    qtdeclarative-qmlplugins \
    qtscript \
    qtgraphicaleffects-qmlplugins \
    qtquickcontrols-qmlplugins \
    qtsvg-plugins \
    \
    qtbase-dbg \
    qt3d-dbg \
    qtconnectivity-dbg \
    qtdeclarative-dbg \
    qtenginio-dbg \
    qtimageformats-dbg \
    qtlocation-dbg \
    qtmultimedia-dbg \
    qtsensors-dbg \
    qtsvg-dbg \
    qtsystems-dbg \
    qttools-dbg \
    qtwebsockets-dbg \
    qtwebchannel-dbg \
    qtquickcontrols-dbg \
    \
    wireshark \
    nfs-utils-client \
    lmsensors-sensord lmsensors-sensorsdetect \
"

#    qemu
#    valgrind

