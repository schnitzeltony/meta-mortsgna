FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

BASEFILESISSUEINSTALL = "do_install_angstromissue"

# In previous versions of base-files, /run was a softlink to /var/run and the
# directory was located in /var/volatlie/run.  Also, /var/lock was a softlink
# to /var/volatile/lock which is where the real directory was located.  Now,
# /run and /run/lock are the real directories.  If we are upgrading, we may
# need to remove the symbolic links first before we create the directories.
# Otherwise the directory creation will fail and we will have circular symbolic
# links.
# 
pkg_preinst_${PN} () {
    #!/bin/sh -e
    if [ x"$D" = "x" ]; then
        rm -rf /var/lock

        if [ -h "/run" ]; then
            # Remove the symbolic link
            rm -f /run
        fi
    fi     
}

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
