FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

BASEFILESISSUEINSTALL = "do_install_angstromissue"

do_install_angstromissue () {
    echo ${MACHINE} > ${D}${sysconfdir}/hostname

    install -m 644 ${WORKDIR}/issue*  ${D}${sysconfdir}
        if [ -n "${DISTRO_NAME}" ]; then
        echo -n "${DISTRO_NAME} " >> ${D}${sysconfdir}/issue
        echo -n "${DISTRO_NAME} " >> ${D}${sysconfdir}/issue.net
        if [ -n "${DISTRO_VERSION}" ]; then
            echo -n "${DISTRO_VERSION} " >> ${D}${sysconfdir}/issue
            echo -e "${DISTRO_VERSION} \n" >> ${D}${sysconfdir}/issue.net
        fi
        echo "- Kernel \r" >> ${D}${sysconfdir}/issue
        echo >> ${D}${sysconfdir}/issue
    fi
}
