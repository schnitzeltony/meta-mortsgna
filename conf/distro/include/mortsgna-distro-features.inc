# We want images supporting the following features (for packagegroup(-core)-base)

DISTRO_FEATURES = " \
    alsa \
    argp \
    bluetooth \
    ext2 \
    ipv4 \
    ipv6 \
    irda \
    largefile \
    nfs \
    pam \
    pci \
    pcmcia \
    ppp \
    smbfs \
    systemd \
    usbgadget \
    usbhost \
    vfat \
    wifi \
    xattr \
    zeroconf \
    gobject-introspection-data \
"

# Disable sysvinit for recipes with systemd support
DISTRO_FEATURES_BACKFILL_CONSIDERED += "sysvinit"
VIRTUAL-RUNTIME_init_manager = "systemd"
# Uncomment to completely disable support for sysv scripts:
#PACKAGECONFIG:pn-systemd = "xz"

# USE-flag like features
DISTRO_FEATURES += "tk"
DISTRO_FEATURES += "x11"
DISTRO_FEATURES += "pulseaudio"
DISTRO_FEATURES += "ldconfig"
DISTRO_FEATURES += "polkit"
DISTRO_FEATURES += "helpfiles"

# OpenGL support
DISTRO_FEATURES += "opengl"

# wayland...
DISTRO_WAYLAND = "wayland"
# No wayland for vivante blobs
DISTRO_WAYLAND:mx6 = ""
DISTRO_WAYLAND:mx7 = ""
DISTRO_WAYLAND:mx8 = ""
DISTRO_WAYLAND:use-mainline-bsp = "wayland"
DISTRO_FEATURES += "${DISTRO_WAYLAND}"

# vulkan
DISTRO_VULKAN = ""
DISTRO_VULKAN:raspberrypi4 = "vulkan"
DISTRO_VULKAN:raspberrypi4-64 = "vulkan"
DISTRO_FEATURES += "${DISTRO_VULKAN}"

# Uncomment for tests on gold-trouble only
#DISTRO_FEATURES += 'ld-is-gold'

