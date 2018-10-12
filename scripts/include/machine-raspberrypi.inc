#! /bin/bash

# machine-raspberrypi.inc
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script contains settings and callbacks fir raspberrypi boards

# default settings
DEFAULT_MACHINE_FAMILY='raspberrypi*'
DEFAULT_FIND_ROOTFS='-name *.rpi-sdimg -type l'
DEFAULT_KERNEL_IMAGE_TYPE='Image'

# callback for card-write
RootCardWriteCallback() {
    # rootfs write/resize to card fit
    time(
        # evt. write partition table
        CheckPartitionTable "$DevicePath"
        # write
        EvalExAuto "dd of=$DevicePath oflag=nocache if=$DeployedFile bs=1024K" "\nWrite $DeployedFile to $DevicePath..."
        # resize I
        EvalExAuto "parted -s $DevicePath -- resizepart 2 -0" "\nResize I ${DevicePath}2..."
        # resize II
        EvalExAuto "resize2fs ${DevicePath}2" "\nResize II ${DevicePath}2..."
    )
}

# callback for card-kernel-write
RootCardKernelWriteCallback() {
    tmpdir=`mktemp -d`

    # mount boot partition
    EvalExAuto "mount ${DevicePath}1 $tmpdir" "\nMount boot partition ${DevicePath}1 to $tmpdir..."
    # initial kernel (REVISIT for aarch64)
    EvalExAuto "cp -f $DeployedFile $tmpdir/kernel7.img" "\nCopy new kernel kernel7.img..."
    # devicetrees
    EvalExAuto "rm -f $tmpdir/*.dtb" "\nRemove old devicetrees..."
    echo
    for dtb in `find ${DeployFileDir} -name "bcm27*.dtb"`; do
        EvalExAuto "cp -f $dtb $tmpdir/" "Copy $dtb -> $tmpdir..."
    done
    # devicetree overlays
    EvalExAuto "rm -f $tmpdir/overlays/*.dtbo" "\nRemove old devicetree-overlays..."
    echo
    for dtbo in `find ${DeployFileDir} -name "*.dtbo"`; do
        bname=`basename $dtbo`
        if ! echo "${bname}" | grep -q 'Image-'; then
            EvalExAuto "cp -f ${dtbo} $tmpdir/overlays/" "Copy $dtbo -> $tmpdir/overlays/..."
        fi
    done
    # TODO bootfiles?
    # unmount boot partition
    EvalExAuto "sleep 1 && umount ${DevicePath}1" "\nUnmount boot partition..."

    # mount rootfs partition
    EvalExAuto "mount ${DevicePath}2 $tmpdir" "\nMount rootfs partition ${DevicePath}2 to $tmpdir..."
    # Note: /boot is empty by default!!
    # kernel modules
    CopyKernelModules
    RegisterKernelModules
    # unmount rootfs
    EvalExAuto "sleep 1 && umount ${DevicePath}2" "\nUnmount rootfs..."

    rm -rf $tmpdir
}

CheckPrerequisite "time"
CheckPrerequisite "dd"
CheckPrerequisite "parted"
CheckPrerequisite "resize2fs"
