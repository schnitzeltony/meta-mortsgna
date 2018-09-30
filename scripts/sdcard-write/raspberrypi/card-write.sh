#! /bin/bash

# write-card.sh
# (c) Copyright 2013-2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script writes image to sdcard and aligns rootfs partition to max size.

run_user() {
	if [ -z $DevicePath ]; then
		# DevicePath for memory card
		SelectCardDevice || exit 1
	fi

	if [ -z $RootFsFile ]; then
		# select rootfs
		SelectRootfs '-name *.rpi-sdimg -type l' || exit 1
	fi
	RootParams="$DevicePath $RootFsFile"
}

run_root() {
	# device node valid?
	if [ ! -b $DevicePath ] ; then
		echo "$DevicePath is not a valid block device!"
		exit 1
	fi
	# rootfs valid?
	if [ ! -e $RootFsFile ] ; then
		echo "$RootFsFile can not be found!"
		exit 1
	fi

	IMAGEDIR=$(dirname $RootFsFile)

	# check if the card is currently mounted
	MOUNTSTR=$(mount | grep $DevicePath)
	if [ -n "$MOUNTSTR" ] ; then
	    echo -e "\n$DevicePath is mounted. Unmounting..."
	    umount -f ${DevicePath}?*
	fi

	# rootfs write/resize to card fit
	time(
	    echo "Writing $RootFsFile to $DevicePath..."
		dd of=$DevicePath if=$RootFsFile bs=1024K
		sync
	    echo "Resizing ${DevicePath}2..."
		parted -s $DevicePath -- resizepart 2 -0
		resize2fs "${DevicePath}2"
	)
}

# includes
. `dirname $0`/machine.inc
. `dirname $0`/../tools.inc

CheckPrerequisite "dd"
CheckPrerequisite "time"
CheckPrerequisite "parted"
CheckPrerequisite "resize2fs"

if [ -z $MACHINE ]; then
	MACHINE=$DEFAULT_MACHINE
fi


DevicePath=$1
RootFsFile=$2

# On the 1st call: run user
# After the 2nd call: run root
RootParams='$DevicePath $RootFsFile'
chk_root&&run_root
