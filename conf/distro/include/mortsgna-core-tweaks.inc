# This include is to import changes from OE that are hard to overlay into core

# Default hash policy for distro
BB_SIGNATURE_HANDLER ?= "OEBasicHash"

# Empty commercial license, we want to build a superset for the default config
COMMERCIAL_LICENSE = ""
LICENSE_FLAGS_WHITELIST += "commercial"

# Unbreak multimachine builds
LICENSE_DIRECTORY = "${DEPLOY_DIR}/licenses/${MACHINE_ARCH}"

# Shared state fixups
MACHINEOVERRIDES[vardepsexclude] += "SOC_FAMILY"

#
# udev, devfsd, busybox-mdev (from busybox) or none
#
IMAGE_DEV_MANAGER ?= "udev"
#
# sysvinit, upstart, systemd
#
IMAGE_INIT_MANAGER ?= "systemd"
IMAGE_INITSCRIPTS ?= "systemd-compat-units"
#
# busybox, getty
#
IMAGE_LOGIN_MANAGER ?= "busybox"

IMAGE_KEEPROOTFS ?= ""
IMAGE_KEEPROOTFS[doc] = "Set to non-empty to keep ${IMAGE_ROOTFS} around after image creation."

IMAGE_BOOT ?= "${IMAGE_INITSCRIPTS} \
               ${IMAGE_DEV_MANAGER} \
               ${IMAGE_INIT_MANAGER} \
               ${IMAGE_LOGIN_MANAGER}"

IMAGE_INSTALL += "${IMAGE_BOOT}"

# some default locales
IMAGE_LINGUAS ?= "de-de fr-fr en-gb en-us es-es it-it"

# there are important missing I bet
IMAGE_LINGUAS_FULL ?= " \
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
