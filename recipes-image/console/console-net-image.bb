require console-base-image.bb

IMAGE_INSTALL += " \
    avahi-autoipd \
    networkmanager \
    openssh-sftp-server \
    openssh-sshd \
    openssh-ssh \
    openssh-scp \
    iputils \
"

export IMAGE_BASENAME = "console-net-image"
