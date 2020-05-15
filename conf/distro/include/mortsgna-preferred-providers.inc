# common
PREFERRED_PROVIDER_hotplug                       = "systemd"
PREFERRED_PROVIDER_virtual/gettext               ??= "gettext"
PREFERRED_PROVIDER_linux-libc-headers-nativesdk  = "linux-libc-headers-nativesdk"

# Prefer libjpeg-turbo
PREFERRED_PROVIDER_jpeg = "libjpeg-turbo"
PREFERRED_PROVIDER_jpeg-native = "libjpeg-turbo-native"

# Prefer gummiboot over grub
EFI_PROVIDER                                     = "gummiboot"

# X11
PREFERRED_PROVIDER_virtual/libx11 = "libx11"
PREFERRED_PROVIDER_virtual/xserver-xf86 = "xserver-xorg"
PREFERRED_PROVIDER_virtual/xserver = "xserver-xorg"

# Select xserver-xorg and common drivers as default
XSERVER ?= " \
    xserver-xorg \
    xf86-input-libinput \
    xf86-video-modesetting \
"

# Follow meta-java README:
# Possible provider: cacao-initial-native and jamvm-initial-native
PREFERRED_PROVIDER_virtual/java-initial-native = "cacao-initial-native"

# Possible provider: cacao-native and jamvm-native
PREFERRED_PROVIDER_virtual/java-native = "jamvm-native"

# Optional since there is only one provider for now
PREFERRED_PROVIDER_virtual/javac-native = "ecj-bootstrap-native"

