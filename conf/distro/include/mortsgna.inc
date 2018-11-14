# Preserve original DISTRO value
USERDISTRO := "${DISTRO}"
DISTRO = "mortsgna"
DISTRO_NAME = "Mortsgna"

BUILDNAME = "Mortsgna ${DISTRO_VERSION}"

TARGET_VENDOR = "-mortsgna"
SDK_VENDOR = "-mortsgnasdk"

# Yocto work around for missing ONLINE_PACKAGE_MANAGEMENT support
ONLINE_PACKAGE_MANAGEMENT = "full"

# Can be "glibc" or "musl"
TCLIBC ?= "glibc"

PREFERRED_PROVIDER_virtual/gettext ??= "gettext"
PREFERRED_PROVIDER_linux-libc-headers-nativesdk = "linux-libc-headers-nativesdk"

require conf/distro/include/mortsgna-${TCLIBC}.inc

# thumb2 only
ARM_INSTRUCTION_SET ??= "thumb"
ARM_INSTRUCTION_SET_armv4 ?= "arm"
ARM_INSTRUCTION_SET_armv5 ?= "arm"
ARM_INSTRUCTION_SET_armv6 ?= "arm"

# set MORTSGNA_ARMV7_TUNE="1" in your local.conf to build armv7athf
require conf/distro/include/mortsgna-armv7-tune.inc
DEFAULTTUNE_mortsgna := "${@armv7_tune_handler(d)}"


require conf/distro/include/mortsgna-inherits.inc

# Generate locales on the buildsystem instead of on the target. Speeds up first boot, set to "1" to enable
ENABLE_BINARY_LOCALE_GENERATION ?= "1"

# build UTF8 locales only
LOCALE_UTF8_ONLY = "1"

# qemu doesn't like this fp
ENABLE_BINARY_LOCALE_GENERATION_ep9312 = "0"

# qemu has taken a dislike to armeb as well
ENABLE_BINARY_LOCALE_GENERATION_armeb = "0"

# Name the generated images in a sane way
IMAGE_NAME = "${DISTRO_NAME}-${IMAGE_BASENAME}-${TCLIBC}-${IMAGE_PKGTYPE}-${DISTRO_VERSION}-${MACHINE}"
DEPLOY_DIR_IMAGE = "${DEPLOY_DIR}/images/${MACHINE}"

# Prefer ext4 images where possible
SDIMG_ROOTFS_TYPE = "ext4"

require conf/distro/include/mortsgna-distro-features.inc

# image specific volatile logfile handlding with image feature 'volatile-log'
require conf/distro/include/mortsgna-image-logfile-volatile.inc

# Yocto tweaks
require conf/distro/include/mortsgna-core-tweaks.inc
