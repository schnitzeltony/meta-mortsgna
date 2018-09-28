LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

IMAGE_FEATURES += "empty-root-password allow-empty-password"

# to be removed again later?
IMAGE_FEATURES += "package-management"

IMAGE_INSTALL += " \
    packagegroup-boot \
    packagegroup-basic \
    ${ROOTFS_PKGMANAGE} \
    update-alternatives-opkg \
    systemd-analyze \
    cpufrequtils \
    fixmac \
    glibc-utils \
    evtest \
    strace \
    nano \
"

IMAGE_DEV_MANAGER   = "udev"
IMAGE_INIT_MANAGER  = "systemd"
IMAGE_INITSCRIPTS   = " "
IMAGE_LOGIN_MANAGER = "busybox shadow"

inherit image

export IMAGE_BASENAME = "console-base-image-no-nm"

# there are important missing I bet
IMAGE_LINGUAS ?= " \
    af-za \
    ar-sa \
    da-dk \
    de-de \
    el-gr \
    en-gb \
    en-us \
    es-es \
    fi-fi \
    fr-fr \
    he-il \
    hi-in \
    hu-hu \
    it-it \
    ja-jp \
    ko-kr \
    nn-no \
    ku-tr \
    pl-pl \
    pt-pt \
    ru-ru \
    sv-se \
    vi-vn \
    zh-cn \
"
# broken?
#    tr-tr 

# we have journal
BAD_RECOMMENDATIONS += "busybox-syslog"
