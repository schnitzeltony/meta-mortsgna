SUMMARY = "All packages required for a base image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

RDEPENDS:${PN} = " \
    ttf-dejavu-sans ttf-dejavu-sans-mono ttf-dejavu-common \
    ttf-liberation-sans ttf-liberation-mono ttf-liberation-serif \
    \
    fontconfig fontconfig-utils font-util \
    \
    gedit gtksourceview-classic-light \
    xarchiver \
    shared-mime-info \
    xterm \
"
