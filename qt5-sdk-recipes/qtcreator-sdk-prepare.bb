SUMMARY = "Prepare environment for cross building/debugging with QtCreator"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit qmake5_base instant-paths

DEPENDS = "qtbase"

deltask do_prepare_recipe_sysroot

do_install[depends] = "qtbase:do_copy_to_cross_sysroot"
do_install[nostamp] = "1"

do_install() {
    # inject qt.conf into native sysroot
    qtconf=`basename ${OE_QMAKE_QTCONF_PATH}`
    sourcepath=${INSTANT_CROSS_PATH}${OE_QMAKE_PATH_HOST_BINS}
    targetpath=${INSTANT_NATIVE_PATH}${OE_QMAKE_PATH_HOST_BINS}
    # be sure we remove old link (if any...)
    rm -f $targetpath/$qtconf
    cp $sourcepath/$qtconf $targetpath/
}

ALLOW_EMPTY_${PN} = "1"

RDEPENDS_${PN} = " \
    qtbase \
    gdbserver \
    openssh-sshd \
    openssh-ssh \
    openssh-scp \
"
