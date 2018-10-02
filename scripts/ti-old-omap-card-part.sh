#! /bin/bash

# card-part.sh
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script prepares partitions on SDCards. It wraps
# http://omappedia.org/wiki/Minimal-FS_SD_Configuration by dialog based GUI.

# includes
. `dirname $0`/include/common-helpers.inc
. `dirname $0`/../tools.inc


# overrride default
SelectInOut() {
    # DevicePath for target card
    SelectCardDevice

    RootParams="$DevicePath"
}

# implement here - not im machine-ti-old-omap.inc
RootCardWriteCallback() {
	# kill u-boot environment
    dd if=/dev/zero of=$DevicePath bs=1024 count=1024

    # Create the FAT partition of 64MB and make it bootable
    parted -s $DevicePath mklabel msdos
    parted -s $DevicePath mkpart primary fat32 63s 64MB
    parted -s $DevicePath toggle 1 boot

    # Create the rootfs partition until end of device
    parted -s $DevicePath -- mkpart primary ext4 64MB -0

    # write partitions
    mkfs.vfat -F 32 -n "boot" -I ${DevicePath}1
    mke2fs -F -j -t ext4 -L "rootfs" ${DevicePath}2
}


CheckPrerequisite "parted"
CheckPrerequisite "dd"
CheckPrerequisite "mkfs.vfat"
CheckPrerequisite "mkfs.vfat"

StartCardWrite
