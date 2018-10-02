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
        echo "Write $DeployedFile to $DevicePath..."
        if echo $DeployedFile | grep -q '.wic.gz'; then
            gunzip -c $DeployedFile | dd of=$DevicePath bs=1024K
        else
            dd of=$DevicePath if=$DeployedFile bs=1024K
        fi
        sync
        echo "${style_green}Write done.${style_normal}"
        # resize
	    echo "Resize ${DevicePath}2..."
        parted -s $DevicePath -- resizepart 2 -0
        resize2fs "${DevicePath}2"
        echo "${style_green}Resize done.${style_normal}"
     )
}

# callback for card-kernel-write
RootCardKernelWriteCallback() {
    tmpdir=`mktemp -d`

    # initial kernel
    mount ${DevicePath}1 $tmpdir || exit 1
    echo "Write initial kernel and devicetrees to boot partition..."
    rm -f $tmpdir/${KernelImageType}*
    cp -f $DeployedFile $tmpdir/$KernelImageType
    # devicetrees
    rm -f $tmpdir/*.dtb
    for dtb in `find ${DeployFileDir} -name "${KernelImageType}*.dtb" -type l`; do
        dtbname=`basename $dtb | sed 's:'${KernelImageType}'-::'`
        cp -f $dtb $tmpdir/${dtbname}
    done
    echo "${style_green}Write done.${style_normal}"

    echo "Unmount boot partition..."
    sleep 1
    umount ${DevicePath}1 || exit 1
    echo "${style_green}Unmount done.${style_normal}"

    # rootfs/boot
    mount ${DevicePath}2 $tmpdir || exit 1
    echo "Write kernel to rootfs..."
    rm -f $tmpdir/boot/${KernelImageType}*
    KernelWithAbiName=`basename $DeployedFile | sed -e 's:-abiversion-::'`
    cp $DeployedFile $tmpdir/boot/$KernelWithAbiName
    ln -sf $KernelWithAbiName $tmpdir/boot/$KernelImageType
    echo "${style_green}Write done.${style_normal}"

    # rootfs/lib/modules
    echo "Write modules to rootfs..."
    kernel_abi_ver=`echo $KernelWithAbiName | sed 's:'${KernelImageType}'::g'`
    for modules in `find ${DeployFileDir} -name "modules-${Machine}.tgz"`; do
        tar xvzf ${modules} -C $tmpdir/
    done
    echo "${style_green}Write done.${style_normal}"

    # run depmod (stolen from dempodwrapper)
    sys_map=`realpath ${DeployFileDir}/../../../pkgdata/${Machine}/kernel-depmod/System.map-$kernel_abi_ver`
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

