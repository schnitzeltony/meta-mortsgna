PACKAGECONFIG ?= " \
    sndfile \
    jack \
    ${@bb.utils.contains('DISTRO_FEATURES', 'pulseaudio', 'pulseaudio', '', d)} \
    portaudio \
"
