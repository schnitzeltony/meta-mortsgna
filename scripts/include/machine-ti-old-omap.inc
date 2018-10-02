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

    # bootloaders and environment
    echo "Writing bootloaders and environment to boot partition"
    mount ${DevicePath}1 $tmpdir || exit 1
    rm -rf $tmpdir/*
    if [ -e ${DeployFileDir}/MLO ] ; then
        cp ${DeployFileDir}/MLO $tmpdir/MLO
    fi
    cp ${DeployFileDir}/u-boot.img $tmpdir/u-boot.img
    if [ -e ${DeployFileDir}/uEnv.txt ] ; then
        cp ${DeployFileDir}/uEnv.txt $tmpdir
    fi
    sleep 1
    umount ${DevicePath}1 || exit 1

    # rootfs
    time(
        mount ${DevicePath}2 $tmpdir || exit 1
        rm -rf $tmpdir/*
        cd $tmpdir
        tar xvjf $DeployedFile
        cd ..
        umount ${DevicePath}2 || exit 1
    )
    rm -rf $tmpdir
}

# callback for card-kernel-write
RootCardKernelWriteCallback() {
    # TODO
    tmpdir=`mktemp -d`

    # initial kernel / devicetrees
    mount ${DevicePath}1 $tmpdir || exit 1
    echo "Write initial kernel and devicetrees to boot partition..."
    # kernel (REVISIT for aarch64)
    cp -f $DeployedFile $tmpdir/kernel7.img
    # devicetrees
    rm -f $tmpdir/*.dtb
    for dtb in `find ${DeployFileDir} -name "bcm27*.dtb"`; do
        cp -f $dtb $tmpdir/
    done
    echo "${style_green}Write done.${style_normal}"

    # devicetree overlays
    echo "Write devicetree-overlays to boot partition..."
    rm -f $tmpdir/overlays/*.dtbo
    for dtbo in `find ${DeployFileDir} -name "*.dtbo"`; do
        bname=`basename $dtbo`
        if ! echo "${bname}" | grep -q 'Image-'; then
            cp -f ${dtbo} $tmpdir/overlays/
        fi
    done
    echo "${style_green}Write done.${style_normal}"

    # TODO bootfiles?
    echo "Unmount boot partition..."
    sleep 1
    umount ${DevicePath}1 || exit 1
    echo "${style_green}Unmount done.${style_normal}"


    # rootfs/boot
    mount ${DevicePath}2 $tmpdir || exit 1
    echo "Write kernel to rootfs..."
    rm -f $tmpdir/boot/${KernelImageType}*
    KernelFileName=`basename $DeployedFile`
    cp $DeployedFile $tmpdir/boot/
    ln -sf $KernelFileName $tmpdir/boot/$KernelImageType
    echo "${style_green}Write done.${style_normal}"

    # rootfs/lib/modules
    echo "Write modules to rootfs..."
    kernel_abi_ver=`echo $KernelFileName | sed 's:'${KernelImageType}'-::g'`
    rm -rf $tmpdir/lib/modules/$kernel_abi_ver
    for modules in `find ${DeployFileDir} -name "modules-${Machine}.tgz"`; do
        tar xvzf ${modules} -C $tmpdir/
    done
    echo "${style_green}Write done.${style_normal}"

    # run depmod (stolen from dempodwrapper)
    sys_map=`realpath ${OeTmpDir}/pkgdata/${Machine}/kernel-depmod/System.map-$kernel_abi_ver`
    echo "Run depmod on modules..."
    depmod -a -b $tmpdir -F "$sys_map" "$kernel_abi_ver"
    echo "${style_green}Run done.${style_normal}"

    # cleanup
    echo "Unmount rootfs partition..."
    umount ${DevicePath}2 || exit 1
    echo "${style_green}Unmount done.${style_normal}"
    rm -rf $tmpdir
}


CheckPrerequisite "time"
CheckPrerequisite "gunzip"
CheckPrerequisite "dd"
CheckPrerequisite "parted"
CheckPrerequisite "resize2fs"
CheckPrerequisite "depmod"

