#! /bin/bash

# machine-ti-old-omap.inc
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script contains settings and callbacks fir raspberrypi boards

# default settings
DEFAULT_MACHINE_FAMILY=''
DEFAULT_FIND_ROOTFS='-name *.tar.bz2 -type l'
DEFAULT_KERNEL_IMAGE_TYPE='zImage'

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
    tmpdir=`mktemp -d`

    # skip boot partition - it contains bootloaders and their config files only

    # mount rootfs
    EvalExAuto "mount ${DevicePath}2 $tmpdir" "\nMount rootfs ${DevicePath}2 to $tmpdir..."
    # rootfs/boot kernel
    EvalExAuto "rm -f $tmpdir/boot/${KernelImageType}*" "\nRemove old kernels..."
    KernelWithAbiName=`basename $DeployedFile | sed -e 's:-abiversion::'`
    EvalExAuto "cp $DeployedFile $tmpdir/boot/$KernelWithAbiName" "\nCopy new kernel to /boot/$KernelWithAbiName..."
    EvalExAuto "ln -sf $KernelWithAbiName $tmpdir/boot/$KernelImageType" "\nLink kernel to /boot/$KernelImageType -> $KernelWithAbiName..."
    # devicetrees
    EvalExAuto "rm -f $tmpdir/boot/*.dtb" "\nRemove old devicetrees..."
    echo
    for dtb in `find ${DeployFileDir} -name '*.dtb' -type l`; do
        if ! echo "${dtb}" | grep -q $KernelImageType; then
            EvalExAuto "cp -f $dtb $tmpdir/boot/" "Copy $dtb -> $tmpdir/boot/..."
        fi
    done
    # devicetree links
    for dtb in `find ${tmpdir}/boot -name '*.dtb'`; do
        dtbname=`basename $dtb`
        EvalExAuto "ln -sf $dtbname $tmpdir/boot/devicetree-$KernelImageType-$dtbname" "Create link $tmpdir/boot/devicetree-$KernelImageType-$dtbname -> $dtbname..."
    done
    # rootfs/lib/modules
    kernel_abi_ver=`echo $KernelWithAbiName | sed 's:'${KernelImageType}-'::g'`
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

