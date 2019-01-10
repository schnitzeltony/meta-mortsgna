require console-base-image.bb

NETWORKMAN ?= "networkmanager"

IMAGE_INSTALL += " \
    avahi-autoipd \
    openssh-sshd \
    openssh-ssh \
    openssh-scp \
    iputils \
    ${NETWORKMAN} \
"

export IMAGE_BASENAME = "console-net-image"
