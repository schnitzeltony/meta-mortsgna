require xfce4-music-image.bb

export IMAGE_BASENAME = "xfce4-music-dev-image"

XFCE_DM = "sddm sddm-morona-autologin"

IMAGE_INSTALL += " \
    packagegroup-gui-dev \
    \
    xfce4-dev-tools \
    libxfce4ui-glade \
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
