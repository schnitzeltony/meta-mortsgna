#! /bin/bash

# machine-bbone.inc
# (c) Copyright 2019 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script contains settings and callbacks for TI beagle boards

# default settings
DEFAULT_MACHINE_FAMILY='*bone*'
DEFAULT_FIND_ROOTFS='-name *.wic -o -name *.wic.gz -type l'
DEFAULT_KERNEL_IMAGE_TYPE='zImage'

# callback for card-write
RootCardWriteCallback() {
    # rootfs write/resize to card fit
    time(
        # evt. write partition table
        CheckPartitionTable "$DevicePath"
        # write
        StartMessage="\nWrite $DeployedFile to $DevicePath..."
        if echo $DeployedFile | grep -q '.wic.gz'; then
            EvalExAuto "gunzip -c $DeployedFile | dd of=$DevicePath oflag=nocache bs=1024K" "$StartMessage"
        elif echo $DeployedFile | grep -q '.wic.xz'; then
            EvalExAuto "tar -x -f $DeployedFile --to-stdout | dd of=$DevicePath oflag=nocache bs=1024K" "$StartMessage"
        else
            EvalExAuto "dd of=$DevicePath oflag=nocache if=$DeployedFile bs=1024K" "$StartMessage"
        fi
        # resize I
        EvalExAuto "parted -s $DevicePath -- resizepart 2 -0" "\nResize I ${DevicePath}2..."
        # resize II
        EvalExAuto "resize2fs ${DevicePath}2" "\nResize II ${DevicePath}2..."
     )
}

# callback for card-kernel-write (WIP)
RootCardKernelWriteCallback() {
    tmpdir=`mktemp -d`

    # mount boot partition
    EvalExAuto "mount ${DevicePath}1 $tmpdir" "\nMount boot partition ${DevicePath}1 to $tmpdir..."
    # initial kernel
    EvalExAuto "rm -f $tmpdir/${KernelImageType}*" "\nRemove old kernels..."
    EvalExAuto "cp $DeployedFile $tmpdir/$KernelImageType" "\nCopy new kernel $KernelImageType..."
    # devicetrees
    EvalExAuto "rm -f $tmpdir/*.dtb" "\nRemove old devicetrees..."
    echo
    for dtb in `find ${DeployFileDir} -name "${KernelImageType}*.dtb" -type l`; do
        dtbname=`basename $dtb | sed 's:'${KernelImageType}'-::'`
        EvalExAuto "cp $dtb $tmpdir/${dtbname}" "Copy $dtb -> $tmpdir/${dtbname}..."
    done
    # unmount boot partition
    EvalExAuto "sleep 1 && umount ${DevicePath}1" "\nUnmount boot partition..."

    # mount rootfs
    EvalExAuto "mount ${DevicePath}2 $tmpdir" "\nMount rootfs ${DevicePath}2 to $tmpdir..."
    # rootfs/boot kernel
    EvalExAuto "rm -f $tmpdir/boot/${KernelImageType}*" "\nRemove old kernels..."
    EvalExAuto "cp $DeployedFile $tmpdir/boot/$KernelWithAbiName" "\nCopy new kernel to /boot/$KernelWithAbiName..."
    EvalExAuto "ln -sf $KernelWithAbiName $tmpdir/boot/$KernelImageType" "\nLink kernel to /boot/$KernelImageType -> $KernelWithAbiName..."
    # kernel modules
    CopyKernelModules
    RegisterKernelModules
    # unmount rootfs
    EvalExAuto "sleep 1 && umount ${DevicePath}2" "\nUnmount rootfs..."

    rm -rf $tmpdir
}

CheckPrerequisite "time"
CheckPrerequisite "gunzip"
CheckPrerequisite "tar"
CheckPrerequisite "dd"
CheckPrerequisite "parted"
CheckPrerequisite "resize2fs"
