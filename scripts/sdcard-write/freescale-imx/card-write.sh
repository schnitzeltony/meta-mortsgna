#! /bin/bash

# write-card.sh
# (c) Copyright 2013-2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script writes image to sdcard and aligns rootfs partition to max size.

SelectRootfs() {
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
		for i in `find ${BuildPath}/deploy/images/${MACHINE} -name *.sdcard -o -name *.wic.gz | sort` ; do
			iCount=`expr $iCount + 1`
			RootFileNameArr[${iCount}]=$i
			strSelection="$strSelection $iCount "`basename $i`
		done
	done

	# were files found?
	if [ $iCount -eq 0 ]; then
		echo "No rootfs files found in ${TMPDIR}-\*"
		exit 1
	fi
	
	dialog --title 'Select rootfs'\
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
	RootFsFile=${RootFileNameArr[$menuitem]}
	echo 
}

run_user() {
	if [ -z $DevicePath ]; then
		# DevicePath for memory card
		SelectCardDevice || exit 1
	fi

	if [ -z $RootFsFile ]; then
		# select rootfs
		SelectRootfs || exit 1
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
		if echo $RootFsFile | grep -q '.wic.gz'; then
			gunzip -c $RootFsFile | dd of=$DevicePath bs=1024K
		else
			dd of=$DevicePath if=$RootFsFile bs=1024K
		fi
		sync
	    echo "Resizing ${DevicePath}2..."
		parted -s $DevicePath -- resizepart 2 -0
		resize2fs "${DevicePath}2"
	)
}

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



