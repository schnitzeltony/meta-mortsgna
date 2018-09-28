#! /bin/bash

# card-kernel-write.sh
# (c) Copyright 2013-2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script writes kernel+modules to SDCard. To
# select card device a dialog based GUI is used.

SelectKernel() {
	# OE environment found?
	if [ -z $BUILDDIR ]; then
		echo "The environment variable BUILDDIR is not set. It is usually set before running bitbake."
		exit 1
	fi
	iCount=0
	strSelection=
	for grep_result in `grep -h TMPDIR $BUILDDIR/conf/*.conf | sed -e s/' '/''/g -e s/'\"'/''/g`; do
		# exclude comments
		tmp_dir=`echo $grep_result | grep '^TMPDIR='`
		if [ ! -z $tmp_dir ]; then
			TMPDIR=`echo $tmp_dir | sed -e s/'TMPDIR='/''/g`
		fi
	done
	for BuildPath in ${TMPDIR}-*; do
		# Find avaliable kernels
		for i in `find ${BuildPath}/deploy/images/${MACHINE} -name ${KERNEL_IMAGE_TYPE}-abiversion-* | sort` ; do
			iCount=`expr $iCount + 1`
			KernelNameArr[${iCount}]=$i
			strSelection="$strSelection $iCount "`basename $i`
		done
	done

	# were files found?
	if [ $iCount -eq 0 ]; then
		echo "No kernel images found in ${TMPDIR}-\*"
		exit 1
	fi

	dialog --title 'Select kernel'\
	--menu 'Move using [UP] [DOWN],[Enter] to select' 30 100 $iCount\
	${strSelection}\
	2>/tmp/menuitem.$$

	# get OK/Cancel
	sel=$?
	# get selected menuitem
	menuitem=`cat /tmp/menuitem.$$`
	rm -f /tmp/menuitem.$$

	# Cancel Button or <ESC>
	if [ $sel -eq 1 -o $sel -eq 255 ] ; then
		echo Cancel selected 1
		return 1
	fi
	KernelImage=${KernelNameArr[$menuitem]}
}

run_user() {
	if [ -z $DevicePath ]; then
		# DevicePath for memory card
		SelectCardDevice || exit 1
	fi

	if [ -z $KernelImage ]; then
		# select rootfs
		SelectKernel || exit 1
	fi
	RootParams="$DevicePath $KernelImage $KERNEL_IMAGE_TYPE"
}

run_root() {
	# device node valid?
	if [ ! -b $DevicePath ] ; then
		echo "$DevicePath is not a valid block device!"
		exit 1
	fi
	# rootfs valid?
	if [ ! -e $KernelImage ] ; then
		echo "$KernelImage can not be found!"
		exit 1
	fi

	IMAGEDIR=$(dirname $KernelImage)

	# check if the card is currently mounted
	MOUNTSTR=$(mount | grep $DevicePath)
	if [ -n "$MOUNTSTR" ] ; then
	    echo -e "\n$DevicePath is mounted. Unmounting..."
	    umount -f ${DevicePath}?*
	fi

	# create temp mount path
	if [ ! -d /tmp/tmp_mount$$ ] ; then
		mkdir /tmp/tmp_mount$$
	fi


	# initial kernel
	mount ${DevicePath}1 /tmp/tmp_mount$$ || exit 1
	echo "Writing initial kernel and devicetrees to boot partition"
	rm -f /tmp/tmp_mount$$/${KernelImageType}*
	cp -f $KernelImage /tmp/tmp_mount$$/$KernelImageType

	# devicetrees
    rm -f /tmp/tmp_mount$$/*.dtb
	for dtb in `find ${IMAGEDIR} -name "${KernelImageType}*.dtb" -type l`; do
        dtbname=`basename $dtb | sed 's:'${KernelImageType}'-::'`
		cp -f $dtb /tmp/tmp_mount$$/${dtbname}
	done

	sleep 1
	umount ${DevicePath}1 || exit 1

	# rootfs/boot
	mount ${DevicePath}2 /tmp/tmp_mount$$ || exit 1
	echo "Writing kernel to rootfs"
	rm -f /tmp/tmp_mount$$/boot/${KernelImageType}*
    KernelWithAbiName=`basename $KernelImage | sed -e 's:-abiversion-::'`
	cp $KernelImage /tmp/tmp_mount$$/boot/$KernelWithAbiName
    ln -sf $KernelWithAbiName /tmp/tmp_mount$$/boot/$KernelImageType

	# rootfs/lib/modules
	echo "Writing modules to rootfs..."
    kernel_abi_ver=`echo $KernelWithAbiName | sed 's:'${KernelImageType}'::g'`

	for modules in `find ${IMAGEDIR} -name "modules-${MACHINE}.tgz"`; do
		tar xvzf ${modules} -C /tmp/tmp_mount$$/
	done
	# run depmod (stolen from dempodwrapper
	sys_map=`realpath ${IMAGEDIR}/../../../pkgdata/${MACHINE}/kernel-depmod/System.map-$kernel_abi_ver`
	echo "Running depmod on modules"
	depmod -a -b /tmp/tmp_mount$$ -F "$sys_map" "$kernel_abi_ver"

	echo "Unmount / write cache..."
	umount ${DevicePath}2 || exit 1

	rm -rf /tmp/tmp_mount$$
}

. `dirname $0`/machine.inc
. `dirname $0`/../tools.inc

if [ -z $MACHINE ]; then
	MACHINE=$DEFAULT_MACHINE
fi
if [ -z $KERNEL_IMAGE_TYPE ]; then
	KERNEL_IMAGE_TYPE=$DEFAULT_KERNEL_IMAGE_TYPE
fi

DevicePath=$1
KernelImage=$2
KernelImageType=$3

# On the 1st call: run user
# After the 2nd call: run root
RootParams='$DevicePath $KernelImage $KERNEL_IMAGE_TYPE'
chk_root "The kernel images on %DevicePath% will be overwritten!!" && run_root
