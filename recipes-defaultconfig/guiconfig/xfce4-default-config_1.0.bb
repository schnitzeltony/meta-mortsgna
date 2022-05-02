SUMMARY = "XFCE default user configuration"
LICENSE = "MIT & CC0-1.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"
PR = "r4"

# License sunrise: https://www.pexels.com/photo/sunrise-under-cloudy-sky-illustration-67832/

inherit allarch

SRC_URI = " \
    file://.config \
    file://sonnenuntergang_08.jpg \
    file://sunrise-sky-blue-sunlight-67832.jpeg \
"

do_install() {
    # default user configration -> /etc/skel
    install -d ${D}${sysconfdir}/skel
    cp -r ${WORKDIR}/.config ${D}${sysconfdir}/skel/

    # background
    install -d ${D}${datadir}/backgrounds
    install -m 0644 ${WORKDIR}/sonnenuntergang_08.jpg ${D}${datadir}/backgrounds/ 
    install -m 0644 ${WORKDIR}/sunrise-sky-blue-sunlight-67832.jpeg ${D}${datadir}/backgrounds/ 
}

FILES:${PN} += "${datadir}/backgrounds"

RDEPENDS:${PN} += " \
    adwaita-icon-theme-cursors \
    adwaita-icon-theme-symbolic \
    adwaita-icon-theme-symbolic-hires \
    \
    gnome-theme-adwaita-dark \
    xfce4-whiskermenu-plugin \
"
