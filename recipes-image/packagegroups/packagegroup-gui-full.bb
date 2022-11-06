SUMMARY = "All packages required for a full image"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# qwt-qt5-examples package gets dynamically renamed
PACKAGE_ARCH = "${TUNE_PKGARCH}"

inherit packagegroup

MACHINE_SPECIFICS_FULL = ""
MACHINE_SPECIFICS_FULL_mx6 = "gstreamer1.0-plugins-imx"

RDEPENDS:${PN} = " \
    source-han-sans-cn-fonts \
    source-han-sans-kr-fonts \
    source-han-sans-jp-fonts \
    source-han-sans-tw-fonts \
    ttf-noto-emoji-color \
    ttf-noto-emoji-regular \
    \
    gimp \
    fontforge \
    menulibre \
    \
    gparted \
    gnome-disk-utility \
    \
    mpd \
    mpv \
    vlc \
    parole gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-omx gstreamer1.0-libav \
    gstreamer1.0-plugins-base-meta gstreamer1.0-plugins-good-meta \
    pulseaudio-misc \
    \
    qwt-qt5-examples \
    gnuplot gnuplot-x11 octave \
    \
    ${MACHINE_SPECIFICS_FULL} \
    \
    epiphany ${@bb.utils.contains_any('BBFILE_COLLECTIONS', 'browser-layer chromium-browser-layer', 'chromium-x11', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-retro', 'meta-retro-world', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'office-layer', 'libreoffice inkscape scribus abiword-meta', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'games-layer', 'pingus supertuxkart supertux2 freeciv wesnoth-all astromenace chromium-bsu maelstrom scummvm pinball pinball-table-gnu pinball-table-hurd', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-musicians', 'packagegroup-gui-musicians', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'sdr-layer', 'packagegroup-sdr-python-extended packagegroup-sdr-gnuradio-base packagegroup-sdr-gnuradio-extended packagegroup-sdr-rtlsdr packagegroup-sdr-uhd', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-qt5-extra', 'gottcode-world kde-apps-world qmlarkdown quaternion spectral kdreports', '', d)} \
"

#    firefox 
#    gursormaker
#    kodi 

