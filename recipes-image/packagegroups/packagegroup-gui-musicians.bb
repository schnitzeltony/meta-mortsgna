SUMMARY = "All packages required for musician image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# fluidsynth-bin gets dynamically renamed
PACKAGE_ARCH = "${TUNE_PKGARCH}"

inherit packagegroup

RDEPENDS:${PN} = " \
    musicians-world \
    mixxx \
    qmmp \
    alsa-utils alsa-utils-scripts \
    jack-utils \
    pulseaudio-module-alsa-card \
    pulseaudio-module-jack-sink \
    pulseaudio-module-jack-source \
    pulseaudio-module-jackdbus-detect \
    sox \
    a2jmidid \
    fluidsynth-bin \
    xfce4-mixer \
"
