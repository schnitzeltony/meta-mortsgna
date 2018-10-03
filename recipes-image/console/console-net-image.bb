require console-base-image.bb

IMAGE_INSTALL += " \
    avahi-autoipd \
    networkmanager \
    openssh-sftp-server \
"

export IMAGE_BASENAME = "console-net-image"
