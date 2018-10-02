#! /bin/bash

# machine-ti-old-omap.inc
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script contains settings and callbacks fir raspberrypi boards

# default settings
DEFAULT_MACHINE_FAMILY=''
DEFAULT_FIND_ROOTFS='-name *.rootfs.tar.bz2 -type l'
DEFAULT_KERNEL_IMAGE_TYPE='Image'
DEFAULT_FIND_KERNEL="-name ${DEFAULT_KERNEL_IMAGE_TYPE}-abiversion-* -type l"

# callback for card-write
RootCardWriteCallback() {
    tmpdir=`mktemp -d`

    # mount boot partition
    EvalExAuto "mount ${DevicePath}1 $tmpdir" "\nMount boot partition ${DevicePath}1 to $tmpdir..."
    # clean boot partition
    EvalExAuto "rm -rf $tmpdir/*" "\nCleanup boot patition..."
    # MLO
    if [ -e ${DeployFileDir}/MLO ] ; then
        EvalExAuto "cp ${DeployFileDir}/MLO $tmpdir/MLO" "\nCopy MLO (u-boot SPL) boot loader..."
    fi
    # u-boot
    EvalExAuto "cp ${DeployFileDir}/u-boot.img $tmpdir/u-boot.img" "\nCopy u-boot loader..."
    # uEnv.txt
    if [ -e ${DeployFileDir}/uEnv.txt ] ; then
        EvalExAuto "cp ${DeployFileDir}/uEnv.txt $tmpdir" "\nCopy uEnv.txt..."
    fi
    # unmount boot partition
    EvalExAuto "sleep 1 && umount ${DevicePath}1" "\nUnmount boot partition..."

    # rootfs
    time(
        # mount rootfs
        EvalExAuto "mount ${DevicePath}2 $tmpdir" "\nMount rootfs ${DevicePath}2 to $tmpdir..."
        # clean rootfs
        EvalExAuto "rm -rf $tmpdir/*" "\nCleanup rootfs..."
        olddir=`pwd`
        cd $tmpdir
        EvalExAuto "tar xvjf $DeployedFile" "\nExtract rootfs..."
        cd "$olddir"
        # unmount rootfs
        EvalExAuto "sleep 1 && umount ${DevicePath}2" "\nUnmount rootfs..."
    )
    rm -rf $tmpdir
}

# callback for card-kernel-write
RootCardKernelWriteCallback() {
    ErrorOut "Card kernel write for old TI is not yet implemented"
}

CheckPrerequisite "time"
CheckPrerequisite "gunzip"
CheckPrerequisite "dd"
CheckPrerequisite "parted"
CheckPrerequisite "resize2fs"
CheckPrerequisite "depmod"

