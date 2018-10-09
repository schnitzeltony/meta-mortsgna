SMMARY = "Add udisk/udev rule to hide boot partition from udev"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PACKAGE_ARCH = "${MACHINE_ARCH}"

# BOOTPARTSPEC is an array of comma separated entries Each entry can have
# 1. A label e.g 'boot' or machine name
# 2. A partition type e.g. 'vfat'
#
# Feel free to add more...
BOOTPARTSPEC = "boot,vfat BOOT,vfat"
BOOTPARTSPEC_raspberrypi = "raspberrypi,vfat"

do_install () {
	install -d ${D}${base_libdir}/udev/rules.d

    # generate udev rule
    rulefile="${D}${base_libdir}/udev/rules.d/80-udisks-hide-bootpart.rules"

    # create header
    echo '###############################################################################' >> "$rulefile"
    echo '# Partitions hidden for udisks/udisks2' >> "$rulefile"
    echo '###############################################################################' >> "$rulefile"
    echo >> "$rulefile"

    # create entries
    for spec in ${BOOTPARTSPEC}; do
        # extract label/header
        label=`echo $spec | awk -F',' '{print $1}'`
        ptype=`echo $spec | awk -F',' '{print $2}'`

        if [ -n "$ptype" -o -n "$label" ]; then
            # header line
            echo -n "# Hide partition" >> "$rulefile"
            if [ -n "$ptype" ]; then
                echo -n " type=\"$ptype\"" >> "$rulefile"
            fi
            if [ -n "$label" ]; then
                echo -n " label=\"$label\"" >> "$rulefile"
            fi
            echo >> "$rulefile"

            # condition line
            if [ -n "$ptype" ]; then
                echo -n "ENV{ID_FS_TYPE}==\"$ptype\", " >> "$rulefile"
            fi
            if [ -n "$label" ]; then
                echo -n "ENV{ID_FS_LABEL}==\"$label\", " >> "$rulefile"
            fi
            echo "\\" >> "$rulefile"

            # hide/ignore line
            echo "  ENV{UDISKS_PRESENTATION_HIDE}=\"1\", ENV{UDISKS_IGNORE}=\"1\"" >> "$rulefile"
            echo >> "$rulefile"
        fi
    done
}

FILES_${PN} = "${base_libdir}/udev/rules.d"
