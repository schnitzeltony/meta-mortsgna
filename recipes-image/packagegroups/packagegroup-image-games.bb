SUMMARY = "All packages required for a games image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

# Those listed here are either ugly or nonworking
RDEPENDS_${PN} = " \
    atanks \
    scummvm \
    etr \
    gnome-chess \
    gnome-robots \
    supertuxkart \
    \
    gnome-games \
    mog \
    openastromenace \
"


