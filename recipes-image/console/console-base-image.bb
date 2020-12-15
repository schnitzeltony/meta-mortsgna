LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

IMAGE_INSTALL += " \
    packagegroup-boot \
    packagegroup-base \
    ${ROOTFS_PKGMANAGE} \
    update-alternatives-opkg \
    coreutils \
    bash \
    bash-completion \
    systemd-bash-completion \
    systemd-analyze \
    cpufrequtils \
    findutils \
    htop \
    util-linux-lsblk \
    nano \
    rsync \
    wget \
"

IMAGE_DEV_MANAGER   = "udev"
IMAGE_INIT_MANAGER  = "systemd"
IMAGE_INITSCRIPTS   = " "
IMAGE_LOGIN_MANAGER = "busybox shadow"

inherit image

export IMAGE_BASENAME = "console-base-image"

# we have journal
BAD_RECOMMENDATIONS += "busybox-syslog"

# This is the mother af all or images. Add more love to root's console
python () {
    d.appendVar('ROOTFS_POSTPROCESS_COMMAND', 'root_defaults;')
}

root_defaults () {
    cp ${IMAGE_ROOTFS}/${sysconfdir}/skel/.bashrc ${IMAGE_ROOTFS}/${ROOT_HOME}
    cp ${IMAGE_ROOTFS}/${sysconfdir}/skel/.profile ${IMAGE_ROOTFS}/${ROOT_HOME}
}
