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
DISTRO_FEATURES += "polkit"

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

