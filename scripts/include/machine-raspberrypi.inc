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
        EvalExAuto "dd of=$DevicePath if=$DeployedFile bs=1024K" "\nWrite $DeployedFile to $DevicePath..."
        # sync
        EvalExAuto "sync" "\nSync..."
        # resize
        EvalExAuto "parted -s $DevicePath -- resizepart 2 -0 && resize2fs ${DevicePath}2" "\nResize ${DevicePath}2..."
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
    for dtb in `find ${DeployFileDir} -name "bcm27*.dtb"`; do
        EvalExAuto "cp -f $dtb $tmpdir/" "\nCopy $dtb -> $tmpdir..."
    done
    # devicetree overlays
    EvalExAuto "rm -f $tmpdir/overlays/*.dtbo" "\nRemove old devicetree-overlays..."
    for dtbo in `find ${DeployFileDir} -name "*.dtbo"`; do
        bname=`basename $dtbo`
        if ! echo "${bname}" | grep -q 'Image-'; then
            EvalExAuto "cp -f ${dtbo} $tmpdir/overlays/" "\nCopy $dtbo -> $tmpdir/overlays/..."
        fi
    done
    # TODO bootfiles?
    # unmount boot partition
    EvalExAuto "sleep 1 && umount ${DevicePath}1" "\nUnmount boot partition..."

    # mount rootfs partition
    EvalExAuto "mount ${DevicePath}2 $tmpdir" "\nMount rootfs partition ${DevicePath}2 to $tmpdir..."
    # rootfs/boot kernel
    EvalExAuto "rm -f $tmpdir/boot/${KernelImageType}*" "\nRemove old kernels..."
    KernelFileName=`basename $DeployedFile`
    EvalExAuto "cp $DeployedFile $tmpdir/boot/" "\nCopy new kernel to /boot/$KernelFileName..."
    EvalExAuto "ln -sf $KernelFileName $tmpdir/boot/$KernelImageType" "\nLink kernel to /boot/$KernelImageType -> $KernelFileName..."
    # rootfs/lib/modules
    kernel_abi_ver=`echo $KernelWithAbiName | sed 's:'${KernelImageType}'::g'`
    for modules in `find ${DeployFileDir} -name "modules-${Machine}.tgz"`; do
        EvalExAuto "tar xvzf ${modules} -C $tmpdir/" "\nUnpack kernel modules..."
    done
    # run depmod (stolen from dempodwrapper)
    sys_map=`realpath ${DeployFileDir}/../../../pkgdata/${Machine}/kernel-depmod/System.map-$kernel_abi_ver`
    EvalExAuto "depmod -a -b $tmpdir -F $sys_map $kernel_abi_ver" "\nRun depmod on modules..."
    # unmount rootfs
    EvalExAuto "sleep 1 && umount ${DevicePath}2" "\nUnmount rootfs..."

    rm -rf $tmpdir
}


CheckPrerequisite "time"
CheckPrerequisite "gunzip"
CheckPrerequisite "dd"
CheckPrerequisite "parted"
CheckPrerequisite "resize2fs"
CheckPrerequisite "depmod"
