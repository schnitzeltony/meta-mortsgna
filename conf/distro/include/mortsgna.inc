# Preserve original DISTRO value
USERDISTRO := "${DISTRO}"
DISTRO = "mortsgna"
DISTRO_NAME = "Mortsgna"

BUILDNAME = "Mortsgna ${DISTRO_VERSION}"

TARGET_VENDOR = "-mortsgna"
SDK_VENDOR = "-mortsgnasdk"

# Yocto work around for missing ONLINE_PACKAGE_MANAGEMENT support
ONLINE_PACKAGE_MANAGEMENT = "full"

# Put links to sources in deploy/sources to make it easier for people to be GPL compliant
#INHERIT += "src_distribute_local"
SRC_DIST_LOCAL ?= "symlink"

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

# use debian style naming
INHERIT += "debian"

# activate config checker
INHERIT += "sanity" 

# activate the blacklister
INHERIT += "blacklist"

# make devshell available as task
INHERIT += "devshell" 

# run QA tests on builds and packages and log them
INHERIT += "insane"
QA_LOG = "1"

INHERIT += "buildhistory"
BUILDHISTORY_COMMIT = "1"

# run QA tests on recipes
#INHERIT += "recipe_sanity"

# have a shared state a.k.a package-staging2
INHERIT += "sstate"

# Generate locales on the buildsystem instead of on the target. Speeds up first boot, set to "1" to enable
ENABLE_BINARY_LOCALE_GENERATION ?= "1"

# build UTF8 locales only
LOCALE_UTF8_ONLY = "1"

# qemu doesn't like this fp
ENABLE_BINARY_LOCALE_GENERATION_ep9312 = "0"

# qemu has taken a dislike to armeb as well
ENABLE_BINARY_LOCALE_GENERATION_armeb = "0"

# drop buildinfo to /etc/build
INHERIT += "image-buildinfo"

# Prelink images
INHERIT += "image-prelink"

# Name the generated images in a sane way
IMAGE_NAME = "${DISTRO_NAME}-${IMAGE_BASENAME}-${TCLIBC}-${IMAGE_PKGTYPE}-${DISTRO_VERSION}-${MACHINE}"
DEPLOY_DIR_IMAGE = "${DEPLOY_DIR}/images/${MACHINE}"

# Prefer ext4 images where possible
SDIMG_ROOTFS_TYPE = "ext4"

# We want images supporting the following features (for packagegroup(-core)-base)
DISTRO_FEATURES = "alsa argp bluetooth ext2 ipv4 ipv6 irda largefile nfs pam pci pcmcia ppp smbfs systemd usbgadget usbhost vfat wifi xattr zeroconf gobject-introspection-data"

# Disable sysvinit for recipes with systemd support
DISTRO_FEATURES_BACKFILL_CONSIDERED += "sysvinit"
VIRTUAL-RUNTIME_init_manager = "systemd"
# Uncomment to completely disable support for sysv scripts:
#PACKAGECONFIG_pn-systemd = "xz"

# USE-flag like features
DISTRO_FEATURES += "tk"
DISTRO_FEATURES += "x11"
DISTRO_FEATURES += "3g"
DISTRO_FEATURES += "pulseaudio"
DISTRO_FEATURES += "ldconfig"

# OpenGL support
DISTRO_FEATURES += "opengl"

# wayland...
DISTRO_WAYLAND = "wayland"

# No wayland for vivante blobs
DISTRO_WAYLAND_mx6 = ""
DISTRO_WAYLAND_mx7 = ""
DISTRO_WAYLAND_mx8 = ""
DISTRO_WAYLAND_use-mainline-bsp = "wayland"

DISTRO_FEATURES += "${DISTRO_WAYLAND}"

# Inherit the default LIBC features superset from OE-core
DISTRO_FEATURES += "${DISTRO_FEATURES_LIBC}"

# image specific volatile logfile handlding with image feature 'volatile-log'
require conf/distro/include/mortsgna-image-logfile-volatile.inc

# Yocto tweaks
require conf/distro/include/mortsgna-core-tweaks.inc
