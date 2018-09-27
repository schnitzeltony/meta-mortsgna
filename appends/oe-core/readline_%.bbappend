FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://inputrc"

# Get more out of our console
do_install_append() {
	cp ${WORKDIR}/inputrc ${D}${sysconfdir}/inputrc
}
