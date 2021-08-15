do_install:append() {
    # Since we
    # * do not have angstroms's DISTRO_TYPE
    # * do not want to set all debug-tweaks
    # * want passwordless login
    # * don't want to allow root login
    # -> we have to align installation slightly
    install -m 644 ${WORKDIR}/lxdm-pam-debug ${D}${sysconfdir}/pam.d/lxdm
}

