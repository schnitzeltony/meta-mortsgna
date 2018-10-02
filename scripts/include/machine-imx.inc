#! /bin/bash

# machine-imx.inc
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script contains settings and callbacks for NXP imx boards

# default settings
DEFAULT_MACHINE_FAMILY='*imx*'
DEFAULT_FIND_ROOTFS='-name *.sdcard -o -name *.wic.gz -type l'
DEFAULT_KERNEL_IMAGE_TYPE='uImage'
DEFAULT_FIND_KERNEL="-name ${DEFAULT_KERNEL_IMAGE_TYPE}-abiversion-* -type l"

# callback for card-write
RootCardWriteCallback() {
    # rootfs write/resize to card fit
    time(
        # write
        StartMessage="\nWrite $DeployedFile to $DevicePath..."
        if echo $DeployedFile | grep -q '.wic.gz'; then
            EvalExAuto "gunzip -c $DeployedFile | dd of=$DevicePath bs=1024K" "$StartMessage"
        else
            EvalExAuto "dd of=$DevicePath if=$DeployedFile bs=1024K" "$StartMessage"
        fi
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
    # initial kernel
    EvalExAuto "rm -f $tmpdir/${KernelImageType}*" "\nRemove old kernels..."
    EvalExAuto "cp $DeployedFile $tmpdir/$KernelImageType" "\nCopy new kernel $KernelImageType..."
    # devicetrees
    EvalExAuto "rm -f $tmpdir/*.dtb" "\nRemove old devicetrees..."
    for dtb in `find ${DeployFileDir} -name "${KernelImageType}*.dtb" -type l`; do
        dtbname=`basename $dtb | sed 's:'${KernelImageType}'-::'`
        EvalExAuto "cp $dtb $tmpdir/${dtbname}" "\nCopy $dtb -> $tmpdir/${dtbname}..."
    done
    # unmount boot partition
    EvalExAuto "sleep 1 && umount ${DevicePath}1" "\nUnmount boot partition..."

    # mount rootfs
    EvalExAuto "mount ${DevicePath}2 $tmpdir" "\nMount rootfs ${DevicePath}2 to $tmpdir..."
    # rootfs/boot kernel
    EvalExAuto "rm -f $tmpdir/boot/${KernelImageType}*" "\nRemove old kernels..."
    KernelWithAbiName=`basename $DeployedFile | sed -e 's:-abiversion-::'`
    EvalExAuto "cp $DeployedFile $tmpdir/boot/$KernelWithAbiName" "\nCopy new kernel to /boot/$KernelWithAbiName..."
    EvalExAuto "ln -sf $KernelWithAbiName $tmpdir/boot/$KernelImageType" "\nLink kernel to /boot/$KernelImageType -> $KernelWithAbiName..."
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
