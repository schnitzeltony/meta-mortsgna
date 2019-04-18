DESCRIPTION = "Set default locale"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit allarch

SRC_URI = "file://locale.conf"

do_configure[noexec] = '1'

do_compile[noexec] = '1'

do_install() {
    install -d ${D}${sysconfdir}
    install -m 644 ${WORKDIR}/locale.conf ${D}${sysconfdir}/
}
