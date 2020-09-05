require xfce4-music-image.bb

export IMAGE_BASENAME = "xfce4-music-dev-image"

XFCE_DM = "sddm sddm-morona-autologin"

IMAGE_INSTALL += " \
    packagegroup-gui-dev \
    \
    exo-csource \
    xfce4-dev-tools \
    libxfce4ui-glade \
    \
    packagegroup-gui-musicians \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'office-layer', 'libreoffice', '', d)} \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'browser-layer', 'chromium-x11', '', d)} \
    \
    jack-dev \
    lv2-dev \
    libvorbis-dev \
    aubio-dev \
    libmad-dev \
    libsamplerate0-dev \
    rubberband-dev \
    liblo-dev \
    lilv-dev \
    suil-dev \
    serd-dev \
    sord-dev \
"
