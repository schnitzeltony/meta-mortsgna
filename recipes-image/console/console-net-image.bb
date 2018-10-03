require console-base-image.bb

IMAGE_INSTALL += " \
    packagegroup-base-wifi \
    packagegroup-base-nfs \
    packagegroup-base-ppp \
    packagegroup-base-zeroconf \
    avahi-autoipd \
    networkmanager \
    openssh-sftp-server \
"

export IMAGE_BASENAME = "console-net-image"
