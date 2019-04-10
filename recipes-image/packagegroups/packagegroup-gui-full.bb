SUMMARY = "All packages required for a full image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

MACHINE_SPECIFICS_FULL = ""
MACHINE_SPECIFICS_FULL_mx6 = "gstreamer1.0-plugins-imx"

RDEPENDS_${PN} = " \
    packagegroup-gui-music \
    \
    source-han-sans-cn-fonts \
    source-han-sans-kr-fonts \
    source-han-sans-jp-fonts \
    source-han-sans-tw-fonts \
    \
    tzdata-misc tzdata-africa \
    tzdata-americas tzdata-antarctica tzdata-arctic tzdata-asia \
    tzdata-atlantic tzdata-australia tzdata-europe tzdata-pacific \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'office-layer', 'libreoffice', '', d)} \
    abiword-meta \
    gimp \
    fontforge \
    menulibre \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'browser-layer', 'chromium-x11', '', d)} \
    \
    gparted \
    gnome-system-monitor \
    \
    mpd \
    parole gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-omx gstreamer1.0-libav \
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
    gottcode-world \
    \
    qwt-qt5-examples \
    gnuplot gnuplot-x11 \
    qskinny \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-retro', 'mame dosbox z80 vice stella opentyrian d1x-rebirth', '', d)} \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'games-layer', 'pingus supertux2 freeciv wesnoth-all', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'sdr-layer', 'packagegroup-sdr-python-extended packagegroup-sdr-gnuradio-base packagegroup-sdr-gnuradio-extended packagegroup-sdr-rtlsdr packagegroup-sdr-uhd', '', d)} \
"

#    firefox 
#    gursormaker
#    kodi 
#    mpv 

