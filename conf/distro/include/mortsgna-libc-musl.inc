# musl:

# The things musl can provide.  We default to wanting musl to provide them.
# libc settings
DISTRO_FEATURES += "${@['', ' nls'][d.getVar('ENABLE_BINARY_LOCALE_GENERATION', 1) == '1']}"

USE_NLS_glib-2.0 = "yes"
USE_NLS_glib-2.0-native = "yes"
USE_NLS_gcc-cross = "no"

TARGET_LINK_HASH_STYLE = "${@['-Wl,--hash-style=gnu',''][d.getVar('TARGET_ARCH',1) in ['mips', 'mipsel', 'mips64', 'mips64el', 'avr32']]}"
