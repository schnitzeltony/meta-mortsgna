require ../console/console-base-image-nm.bb

export IMAGE_BASENAME = "liri-base-image"

# reenable graphical target
IMAGE_FEATURES += "x11-base"

IMAGE_INSTALL += " \
    liri-world \
    sddm \
    hicolor-icon-theme \
    strace \
    \
    systemd-analyze \
    bash-completion \
    \
    qtbase-tools \
    qtbase-plugins \
    qtdeclarative-tools \
    qtdeclarative-qmlplugins \
    qtscript \
    qtgraphicaleffects-qmlplugins \
    qtquickcontrols-qmlplugins \
    qtsvg-plugins \
    \
    qtwayland-plugins \
    qtwayland-examples qtwayland-tools \
    \
    operator-user \
"


#    weston-init
#    upower udisks
#    shared-mime-info
