#! /bin/bash

# ti-old-omap-card-part.sh
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script prepares partitions on SDCards. It wraps
# http://omappedia.org/wiki/Minimal-FS_SD_Configuration by dialog based GUI.

# includes
. `dirname $0`/include/common-helpers.inc
. `dirname $0`/include/card-helpers.inc


# overrride default SelectInOut - we have nothing to deploy here
SelectInOut() {
    # DevicePath for target card
    SelectCardDevice
}

# implement here - not im machine-ti-old-omap.inc
RootCardWriteCallback() {
    # evt. write partition table
    CheckPartitionTable "$DevicePath"

    # kill u-boot environment
    EvalExAuto "dd if=/dev/zero of=$DevicePath bs=1024 count=1024" "\nKill u-boot environment..."

    # Create the FAT partition of 64MB and make it bootable
    EvalExAuto "parted -s $DevicePath mklabel msdos && parted -s $DevicePath mkpart primary fat32 63s 64MB && parted -s $DevicePath toggle 1 boot" "\nCreate boot partition..."

    # Create the rootfs partition until end of device
    EvalExAuto "parted -s $DevicePath -- mkpart primary ext4 64MB -0" "\nCreate rootfs partition..."

    # create filesystems
    EvalExAuto "mkfs.vfat -F 32 -n "boot" -I ${DevicePath}1" "\nCreate boot filesystem..."
    EvalExAuto "mke2fs -F -j -t ext4 -L "rootfs" ${DevicePath}2" "\nCreate rootfs filesystem..."
}

CheckPrerequisite "parted"
CheckPrerequisite "dd"
CheckPrerequisite "mkfs.vfat"
CheckPrerequisite "mke2fs"

StartCardWrite
