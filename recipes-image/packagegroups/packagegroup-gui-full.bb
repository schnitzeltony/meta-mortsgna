SUMMARY = "All packages required for a full image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit packagegroup

MACHINE_SPECIFICS_FULL = ""
MACHINE_SPECIFICS_FULL_mx6 = "gstreamer1.0-plugins-imx"

RDEPENDS_${PN} = " \
    source-han-sans-cn-fonts \
    source-han-sans-kr-fonts \
    source-han-sans-jp-fonts \
    source-han-sans-tw-fonts \
    ttf-noto-emoji-color \
    ttf-noto-emoji-regular \
    \
    tzdata-misc tzdata-africa \
    tzdata-americas tzdata-antarctica tzdata-arctic tzdata-asia \
    tzdata-atlantic tzdata-australia tzdata-europe tzdata-pacific \
    \
    gimp \
    fontforge \
    menulibre \
    \
    gparted \
    \
    mpd \
    vlc \
    parole gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-omx gstreamer1.0-libav \
    gstreamer1.0-plugins-base-meta gstreamer1.0-plugins-good-meta \
    pulseaudio-misc \
    \
    qwt-qt5-examples \
    gnuplot gnuplot-x11 \
    \
    ${MACHINE_SPECIFICS_FULL} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'browser-layer', 'chromium-x11', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-retro', 'mame dosbox cass80 z80 vice stella opentyrian d1x-rebirth', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'office-layer', 'libreoffice abiword-meta', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'games-layer', 'pingus supertuxkart supertux2 freeciv wesnoth-all astromenace chromium-bsu maelstrom', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-musicians', 'packagegroup-gui-musicians', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'sdr-layer', 'packagegroup-sdr-python-extended packagegroup-sdr-gnuradio-base packagegroup-sdr-gnuradio-extended packagegroup-sdr-rtlsdr packagegroup-sdr-uhd', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-qt5-extra', 'gottcode-world kde-apps-world qmlarkdown', '', d)} \
"

#    firefox 
#    gursormaker
#    kodi 
#    mpv 

