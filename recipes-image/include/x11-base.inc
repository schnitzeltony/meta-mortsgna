inherit user-with-full-skel

X11_BASE_INSTALL += " \
    ${DISTRO_GUI_USER} \
    packagegroup-core-x11-xserver \
    \
    iso-codes \
    mime-support \
    xauth \
    xdg-utils \
    xhost \
    xinetd \
    xinit \
    xrandr \
    xrdb \
    xset \
    xvinfo \
"
