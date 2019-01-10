require console-base-image.bb

NETWORKPACKS ?= " \
    ${TASK_BASIC_SSHDAEMON} \
    avahi-autoipd \
    iputils \
    networkmanager \
"

IMAGE_INSTALL += " \
    ${NETWORKPACKS} \
"

export IMAGE_BASENAME = "console-net-image"
