# musl:

# The things musl can provide.  We default to wanting musl to provide them.
# libc settings
DISTRO_FEATURES += "${@['', ' nls'][d.getVar('ENABLE_BINARY_LOCALE_GENERATION') == '1']}"

DISTRO_FEATURES:remove = "ldconfig"

USE_NLS_glib-2.0 = "yes"
USE_NLS_glib-2.0-native = "yes"
USE_NLS_gcc-cross = "no"
