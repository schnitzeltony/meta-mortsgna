# This include is to import changes from OE that are hard to overlay into core

# Default hash policy for distro
BB_SIGNATURE_HANDLER ?= "OEBasicHash"

# Empty commercial license, we want to build a superset for the default config
LICENSE_FLAGS_WHITELIST += "commercial"

# Unbreak multimachine builds
LICENSE_DIRECTORY = "${DEPLOY_DIR}/licenses/${MACHINE_ARCH}"

# Shared state fixups
MACHINEOVERRIDES[vardepsexclude] += "SOC_FAMILY"


