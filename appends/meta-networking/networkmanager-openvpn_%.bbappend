PACKAGECONFIG_append = " gnome"
DEPENDS_append = " gtk+3 networkmanager network-manager-applet"

do_configure_append() {
    # network-manager-openvpn.metainfo.xml is created in source folder but
    # compile expects it in build folder. As long as nobody comes up with a
    # better solution just support build:
    if [ -e ${S}/appdata/network-manager-openvpn.metainfo.xml ]; then
        mkdir -p ${B}/appdata
        cp -f ${S}/appdata/network-manager-openvpn.metainfo.xml ${B}/appdata/
    fi
}
