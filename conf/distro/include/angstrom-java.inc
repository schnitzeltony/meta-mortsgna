# Set PREFFERED and VERSION stuff for working java runtime

PREFERRED_PROVIDER_java2-vm = "${DEFAULTJRE}"
PREFERRED_PROVIDER_java2-runtime = "${DEFAULTJRE}"
PREFERRED_PROVIDER_openjdk-7-jre = "${DEFAULTJRE}"
# Possible provider: cacao-initial-native and jamvm-initial-native
PREFERRED_PROVIDER_virtual/java-initial-native = "cacao-initial-native"

DEFAULTJRE ?= "openjdk-7-jre"
