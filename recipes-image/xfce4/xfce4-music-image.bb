require xfce4-tiny-image.bb

export IMAGE_BASENAME = "xfce4-music-image"

XFCE_DM = "sddm"

# Saving to SDCard creates delays/underruns
MORTSGNA_IMAGE_VOLATILE_LOG_DIR = "1"

IMAGE_INSTALL += " \
    packagegroup-gui-music \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'office-layer', 'libreoffice', '', d)} \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'browser-layer', 'chromium-x11', '', d)} \
"
