require console-base-image.bb

NETWORKPACKS ?= " \
    ${TASK_BASIC_SSHDAEMON} \
    avahi-autoipd \
    iputils \
    networkmanager \
    networkmanager-openvpn \
    networkmanager-bash-completion \
"

IMAGE_INSTALL += " \
    ${NETWORKPACKS} \
"

export IMAGE_BASENAME = "console-net-image"
