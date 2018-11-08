require xfce4-tiny-image.bb

export IMAGE_BASENAME = "xfce4-music-image"

XFCE_DM = "sddm sddm-morona-autologin"

# Saving to SDCard creates delays/underruns
IMAGE_FEATURES += "volatile-log"

IMAGE_INSTALL += " \
    packagegroup-gui-music \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'office-layer', 'libreoffice', '', d)} \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'browser-layer', 'chromium-x11', '', d)} \
"
