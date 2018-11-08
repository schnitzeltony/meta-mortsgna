DESCRIPTION = "Add SDDM autologing for user 'morona'"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://sddm.conf"

inherit allarch

do_configure[noexec] = '1'
do_compile[noexec] = '1'

do_install() {
    install -d ${D}/${sysconfdir}/sddm.conf.d
    install -m 644 ${WORKDIR}/sddm.conf ${D}/${sysconfdir}/sddm.conf.d/90-morona-autologin.conf
}
