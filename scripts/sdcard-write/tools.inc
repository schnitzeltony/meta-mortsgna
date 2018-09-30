#! /bin/bash

# tools.inc
# (c) Copyright 2013-2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script contains some helper functions and can be sourced by other 
# scripts

# set terminal styles
# are we on terrminal?
if [ -t 1 ] ; then
    # tput available?
    if [ ! "x`which tput 2>/dev/null`" = "x" ] ; then
        # supports colors ?
        ncolors=`tput colors`
        if [ -n "$ncolors" -a  $ncolors -ge 8 ] ; then
            style_bold="`tput bold`"
            style_underline="`tput smul`"
            style_standout="`tput smso`"
            style_normal="`tput sgr0`"
            style_black="`tput setaf 0`"
            style_red="`tput setaf 1`"
            style_green="`tput setaf 2`"
            style_yellow="`tput setaf 3`"
            style_blue="`tput setaf 4`"
            style_magenta="`tput setaf 5`"
            style_cyan="`tput setaf 6`"
            style_white="`tput setaf 7`"
        fi
    fi
fi

ErrorOut() {
    echo
    echo "${style_red}${style_bold}${1}!${style_normal}"
    echo
    exit -1
}

# CheckPrerequisite checks if required hosttool is found on host. In case not
# it outputs a message and exits script.
CheckPrerequisite() {
    if [ "x`which $1 2>/dev/null`" = "x" ] ; then
        ErrorOut "You need to install $1 first!"
    fi
}


# SelectCardDevice() creates a dialog to select one of all available removable
# devices. The path to the selected device is stored in the variale DevicePath.
SelectCardDevice() {
	iCount=0
	for dev in /dev/sd[a-z] ; do
		DeviceFile=`basename $dev`
		# we are only interested in removable devices
		if [ `cat  /sys/block/$DeviceFile/removable` = '1' ] && [ `cat  /sys/block/$DeviceFile/size` -gt "0" ]; then
			iCount=`expr $iCount + 1`
			DevicePathArr[${iCount}]=$dev
            # pretify display
            secondline=
            display=
            IFS=$'\n'
            for line in `lsblk --raw --noheadings --output NAME,LABEL,MODEL $dev` ; do
                # a bug in lsblk?
                line=`echo $line | sed 's:\\\x20: :g'`
                if [ "x$secondline" = "x" ]; then
                    # first line - it is for device
                    secondline="1"
                    display="$line"
                else
                    if [ "x$secondline" = "x1" ]; then
                        secondline="2"
                        # second line - header + partition
                        display="$display / partitions: $line"
                    else
                        # second line - separator + partition
                        display="$display + $line"
                    fi
                fi
            done
            display=`echo $display | sed 's:  : :g'`
            display=`echo $display | sed 's:  : :g'`
            display=`echo $display | sed 's:  : :g'`
            
            unset IFS
            menuitems+=( "$iCount" "$display" )
		fi
	done
	
	if [ $iCount -eq 0 ]; then
		ErrorOut 'No removable devices found!'
	fi

	dialog --title 'Select card device'\
	--menu 'Move using [UP] [DOWN],[Enter] to select' 10 100 $iCount\
	"${menuitems[@]}"\
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

# common prerequisites
CheckPrerequisite "dialog"
CheckPrerequisite "lsblk"

