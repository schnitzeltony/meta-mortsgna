SUMMARY = "All packages required for a base image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

RDEPENDS_${PN} = " \
    tzdata \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-initramfs', 'plymouth plymouth-set-default-theme plymouth-initrd', '', d)} \
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
    dconf-editor \
    \
    glmark2 mesa-demos eglinfo-x11 \
"
