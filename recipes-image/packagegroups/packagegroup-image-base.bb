SUMMARY = "All packages required for a base image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

RDEPENDS_${PN} = " \
    packagegroup-gnome-fonts \
    ttf-dejavu-sans ttf-dejavu-sans-mono ttf-dejavu-common \
    source-han-sans-cn-fonts \
    source-han-sans-kr-fonts \
    source-han-sans-jp-fonts \
    source-han-sans-tw-fonts \
    \
    operator-user \
    \
    gedit gtksourceview-classic-light \
    xarchiver \
    shared-mime-info \
    xterm \
"
