SUMMARY = "All packages required for a base image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

MACHINE_SPECIFICS_TINY = ""

RDEPENDS_${PN} = " \
    tzdata \
    plymouth plymouth-set-default-theme plymouth-initrd \
    xdg-user-dirs \
    \
    evince \
    geany \
    \
    ntp ntp-utils \
    tzdata \
    dnsmasq \
    bind \
    devilspie2 \
    \
    bash-completion \
    dconf-editor \
    \
    ${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'glmark2 sdl2box sdl2renderex', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'opengl x11', 'eglinfo-x11', '', d)} \
    \
    ${MACHINE_SPECIFICS_TINY} \
"
