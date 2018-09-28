SUMMARY = "All packages required for a full image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

MACHINE_SPECIFICS_FULL = ""
MACHINE_SPECIFICS_FULL_mx6 = "gstreamer1.0-plugins-imx"

RDEPENDS_${PN} = " \
    packagegroup-image-music \
    \
    tzdata-misc tzdata-africa \
    tzdata-americas tzdata-antarctica tzdata-arctic tzdata-asia \
    tzdata-atlantic tzdata-australia tzdata-europe tzdata-pacific \
    \
    libreoffice \
    abiword-meta \
    gimp \
    fontforge \
    menulibre \
    \
    chromium-x11 \
    \
    gparted \
    gnome-system-monitor \
    \
    mpd \
    parole \
    dragon \
    gstreamer1.0-plugins-base-meta gstreamer1.0-plugins-good-meta \
    pulseaudio-misc \
    ${MACHINE_SPECIFICS_FULL} \
    \
    ark \
    filelight \
    konsole \
    spectacle \
    \
    kde-apps-world \
    \
    gottet \
    hexalate \
    peg-e \
    simsu \
    \
    qwt-qt5-examples \
    gnuplot gnuplot-x11 \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-retro', 'mame dosbox z80 vice stella opentyrian d1x-rebirth', '', d)} \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'games-layer', 'pingus supertux2 freeciv wesnoth-all', '', d)} \
    \
"

#    firefox 
#    gursormaker
#    kodi 
#    mpv 

