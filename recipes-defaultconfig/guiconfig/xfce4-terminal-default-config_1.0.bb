SUMMARY = "xfce4-terminal default user configuration"
LICENSE = "MIT & CC0-1.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit allarch

SRC_URI = " \
    file://.config \
"

do_install() {
    # default user configration -> /etc/skel
    install -d ${D}${sysconfdir}/skel
    cp -r ${WORKDIR}/.config ${D}${sysconfdir}/skel/
}
