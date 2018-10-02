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
DEFAULT_FIND_KERNEL="-name ${DEFAULT_KERNEL_IMAGE_TYPE}-abiversion-* -type l"

# callback for card-write
RootCardWriteCallback() {
    # rootfs write/resize to card fit
    time(
        # write
        EvalEx "dd of=$DevicePath if=$DeployedFile bs=1024K" "\nWrite $DeployedFile to $DevicePath..." "Write done." "Write failed!"
        # sync
        EvalEx "sync" "\nSync..." "Sync done." "Sync failed!"
        # resize
        EvalEx "parted -s $DevicePath -- resizepart 2 -0 && resize2fs ${DevicePath}2" "\nResize ${DevicePath}2..." "Reszize done." "Resize failed!"
    )
}

# callback for card-kernel-write
RootCardKernelWriteCallback() {
    tmpdir=`mktemp -d`

    # mount boot partition
    EvalEx "mount ${DevicePath}1 $tmpdir" "\nMount boot partition ${DevicePath}1 to $tmpdir..." "Mount done" "Mount failed"
    # initial kernel (REVISIT for aarch64)
    EvalEx "cp -f $DeployedFile $tmpdir/kernel7.img" "\nCopy new kernel kernel7.img..." "Copy done" "Copy failed"
    # devicetrees
    EvalEx "rm -f $tmpdir/*.dtb" "\nRemove old devicetrees..." "Remove done" "Remove failed"
    for dtb in `find ${DeployFileDir} -name "bcm27*.dtb"`; do
        EvalEx "cp -f $dtb $tmpdir/" "\nCopy $dtb -> $tmpdir..." "Copy done" "Copy failed"
    done
    # devicetree overlays
    EvalEx "rm -f $tmpdir/overlays/*.dtbo" "\nRemove old devicetree-overlays..." "Remove done" "Remove failed"
    for dtbo in `find ${DeployFileDir} -name "*.dtbo"`; do
        bname=`basename $dtbo`
        if ! echo "${bname}" | grep -q 'Image-'; then
            EvalEx "cp -f ${dtbo} $tmpdir/overlays/" "\nCopy $dtbo -> $tmpdir/overlays/..." "Copy done" "Copy failed"
        fi
    done
    # TODO bootfiles?
    # unmount boot partition
    EvalEx "sleep 1 && umount ${DevicePath}1" "\nUnmount boot partition..." "Unmount done" "Unmount failed"

    # mount rootfs partition
    EvalEx "mount ${DevicePath}2 $tmpdir" "\nMount rootfs partition ${DevicePath}2 to $tmpdir..." "Mount done" "Mount failed"
    # rootfs/boot kernel
    EvalEx "rm -f $tmpdir/boot/${KernelImageType}*" "\nRemove old kernels..." "Remove done" "Remove failed"
    KernelFileName=`basename $DeployedFile`
    EvalEx "cp $DeployedFile $tmpdir/boot/" "\nCopy new kernel to /boot/$KernelFileName..." "Copy done" "Copy failed"
    EvalEx "ln -sf $KernelFileName $tmpdir/boot/$KernelImageType" "\nLink kernel to /boot/$KernelImageType -> $KernelFileName..." "Copy done" "Copy failed"
    # rootfs/lib/modules
    kernel_abi_ver=`echo $KernelWithAbiName | sed 's:'${KernelImageType}'::g'`
    for modules in `find ${DeployFileDir} -name "modules-${Machine}.tgz"`; do
        EvalEx "tar xvzf ${modules} -C $tmpdir/" "\nUnpack kernel modules..." "Unpack done" "Unpack failed"
    done
    # run depmod (stolen from dempodwrapper)
    sys_map=`realpath ${DeployFileDir}/../../../pkgdata/${Machine}/kernel-depmod/System.map-$kernel_abi_ver`
    EvalEx "depmod -a -b $tmpdir -F $sys_map $kernel_abi_ver" "\nRun depmod on modules..." "Run done" "Run failed"
    # unmount rootfs
    EvalEx "sleep 1 && umount ${DevicePath}2" "\nUnmount rootfs..." "Unmount done" "Unmount failed"

    rm -rf $tmpdir
}


CheckPrerequisite "time"
CheckPrerequisite "gunzip"
CheckPrerequisite "dd"
CheckPrerequisite "parted"
CheckPrerequisite "resize2fs"
CheckPrerequisite "depmod"

