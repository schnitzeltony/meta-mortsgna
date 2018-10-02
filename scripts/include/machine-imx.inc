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
            ExecEx "gunzip -c $DeployedFile | dd of=$DevicePath bs=1024K" "$StartMessage" "Write done." "Write failed!"
        else
            ExecEx "dd of=$DevicePath if=$DeployedFile bs=1024K" "$StartMessage" "Write done." "Write failed!"
        fi
        # sync
        ExecEx "sync" "\nSync..." "Sync done." "Sync failed!"
        # resize
        ExecEx "parted -s $DevicePath -- resizepart 2 -0 && resize2fs ${DevicePath}2" "\nResize ${DevicePath}2..." "Reszize done." "Resize failed!"
     )
}

# callback for card-kernel-write
RootCardKernelWriteCallback() {
    tmpdir=`mktemp -d`

    # mount boot partition
    ExecEx "mount ${DevicePath}1 $tmpdir" "\nMount boot partition ${DevicePath}1 to $tmpdir..." "Mount done" "Mount failed"
    # initial kernel
    ExecEx "rm -f $tmpdir/${KernelImageType}*" "\nRemove old kernels..." "Remove done" "Remove failed"
    ExecEx "cp $DeployedFile $tmpdir/$KernelImageType" "\nCopy new kernel $KernelImageType..." "Copy done" "Copy failed"
    # devicetrees
    ExecEx "rm -f $tmpdir/*.dtb" "\nRemove old devicetrees..." "Remove done" "Remove failed"
    for dtb in `find ${DeployFileDir} -name "${KernelImageType}*.dtb" -type l`; do
        dtbname=`basename $dtb | sed 's:'${KernelImageType}'-::'`
        ExecEx "cp $dtb $tmpdir/${dtbname}" "\nCopy $dtb -> $tmpdir/${dtbname}..." "Copy done" "Copy failed"
    done
    # unmount boot partition
    ExecEx "sleep 1 && umount ${DevicePath}1" "\nUnmount boot partition..." "Unmount done" "Unmount failed"

    # mount rootfs
    ExecEx "mount ${DevicePath}2 $tmpdir" "\nMount rootfs ${DevicePath}2 to $tmpdir..." "Mount done" "Mount failed"
    # rootfs/boot kernel
    ExecEx "rm -f $tmpdir/boot/${KernelImageType}*" "\nRemove old kernels..." "Remove done" "Remove failed"
    KernelWithAbiName=`basename $DeployedFile | sed -e 's:-abiversion-::'`
    ExecEx "cp $DeployedFile $tmpdir/boot/$KernelImageType" "\nCopy new kernel to /boot/$KernelImageType..." "Copy done" "Copy failed"
    ExecEx "ln -sf $KernelWithAbiName $tmpdir/boot/$KernelImageType" "\nLink kernel to /boot/$KernelImageType -> $KernelWithAbiName..." "Copy done" "Copy failed"
    # rootfs/lib/modules
    kernel_abi_ver=`echo $KernelWithAbiName | sed 's:'${KernelImageType}'::g'`
    for modules in `find ${DeployFileDir} -name "modules-${Machine}.tgz"`; do
        ExecEx "tar xvzf ${modules} -C $tmpdir/" "\nUnpack kernel modules..." "Unpack done" "Unpack failed"
    done
    # run depmod (stolen from dempodwrapper)
    sys_map=`realpath ${DeployFileDir}/../../../pkgdata/${Machine}/kernel-depmod/System.map-$kernel_abi_ver`
    ExecEx "depmod -a -b $tmpdir -F $sys_map $kernel_abi_ver" "\nRun depmod on modules..." "Run done" "Run failed"
    # unmount boot partition
    ExecEx "sleep 1 && umount ${DevicePath}2" "\nUnmount rootfs..." "Unmount done" "Unmount failed"

    rm -rf $tmpdir
}


CheckPrerequisite "time"
CheckPrerequisite "gunzip"
CheckPrerequisite "dd"
CheckPrerequisite "parted"
CheckPrerequisite "resize2fs"
CheckPrerequisite "depmod"

