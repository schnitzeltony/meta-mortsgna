SUMMARY = "All packages required for a music image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

RDEPENDS_${PN} = " \
    meta-qt5-extra-music-world \
    alsa-utils alsa-utils-scripts \
    jack-utils \
    pulseaudio-module-alsa-card \
    sox \
    a2jmidid \
    fluidsynth-bin \
"
