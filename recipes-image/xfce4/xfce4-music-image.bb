require xfce4-tiny-image.bb

export IMAGE_BASENAME = "xfce4-music-image"

XFCE_DM = "sddm sddm-morona-autologin"

IMAGE_INSTALL += " \
    packagegroup-gui-musicians \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'office-layer', 'libreoffice', '', d)} \
    ${@bb.utils.contains_any('BBFILE_COLLECTIONS', 'browser-layer chromium-browser-layer', 'chromium-x11', '', d)} \
"
