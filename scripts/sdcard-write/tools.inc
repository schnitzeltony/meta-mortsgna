#! /bin/bash

# tools.inc
# (c) Copyright 2013-2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script contains some helper functions and can be sourced by other 
# scripts


# SelectCardDevice() creates a dialog to select one of all available removable
# devices. The path to the selected device is stored in the variale DevicePath.
SelectCardDevice() {
	strSelection=""
	iCount=0
	for dev in /dev/sd[a-z] ; do
		DeviceFile=`basename $dev`
		# we are only interested in removable devices
		if [ `cat  /sys/block/$DeviceFile/removable` = '1' ] && [ `cat  /sys/block/$DeviceFile/size` -gt "0" ]; then
			iCount=`expr $iCount + 1`
			DevicePathArr[${iCount}]=$dev
			strSelection="$strSelection $iCount $dev"
		fi
	done
	
	if [ $iCount -eq 0 ]; then
		echo 'No removable devices found!'
		exit 1
	fi

	dialog --title 'Select card device'\
	--menu 'Move using [UP] [DOWN],[Enter] to select' 10 100 $iCount\
	${strSelection}\
	2>/tmp/menuitem.$$

	# get OK/Cancel
	sel=$?
	# get selected menuitem
	menuitem=`cat /tmp/menuitem.$$`
	rm -f /tmp/menuitem.$$

	# Cancel Button or <ESC>
	if [ $sel -eq 1 -o $sel -eq 255 ] ; then
		echo 'Cancel selected SelectCardDevice()'
		return 1
	fi
	DevicePath=${DevicePathArr[$menuitem]}
}

# chk_root() tests if we are running as root. If not, it calls run_user and then
# asks operator how to log on as root. Then the main script is started as root
chk_root() {
	# we are not already root?
	if [ ! $( id -u ) -eq 0 ]; then
		# do all non root operations
		run_user
		# abort in case run_user was performed without success
		if [ $? -ne 0 ] ; then
			return 1
		fi

		dialog --title 'Select how you want to logon as root'\
		--menu 'Move using [UP] [DOWN],[Enter] to select' 10 100 2 1 su 2 sudo \
		2>/tmp/menuitem.$$

		# get OK/Cancel
		sel=$?
		# get selected menuitem
		menuitem=`cat /tmp/menuitem.$$`
		rm -f /tmp/menuitem.$$

		# Cancel Button or <ESC>
		if [ $sel -eq 1 -o $sel -eq 255 ] ; then
			echo 'Cancel selected chk_root()'
			return 1
		fi

		clear
		if [ "x$1" = "x" ] ; then
			echo "All data currenly stored on $DevicePath will be overwritten!!"
		else
			echo $1 | sed "s|\%DevicePath\%|$DevicePath|"
		fi

		if [ $menuitem -eq 1 ]; then
			echo -e "\nEnter valid root password if you are sure you want to continue"
			# Call this prog as root
			exec su -c "${0} $RootParams"
		else
			echo -e "\nEnter valid sudo password if you are sure you want to continue"
			sudo -k
			# Call this prog as root
			exec sudo ${0} $RootParams
		fi
		return 1	# sice we're 'execing' above, we wont reach this exit
				# unless something goes wrong.
	fi
}

