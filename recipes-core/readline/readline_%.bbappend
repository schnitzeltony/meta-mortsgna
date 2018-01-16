FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://inputrc"

do_install_append() {
	cp ${WORKDIR}/inputrc ${D}${sysconfdir}/inputrc
}
