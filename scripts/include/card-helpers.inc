#! /bin/bash

# card-helpers.inc
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script contains some helper functions and can be sourced by other 
# scripts

# SelectCardDevice() creates a dialog to select one of all available removable
# devices. The path to the selected device is stored in the variale DevicePath.
SelectCardDevice() {
    if [ -z "$DevicePath" ]; then
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
            ErrorOut 'Cancel selected at SelectCardDevice().'
        fi
        DevicePath=${DevicePathArr[$menuitem]}
    fi
}

# SelectDeployedFile() opens a dialog to select file containing rootfs/kernel/...
# It expects one parameter which contains find parameters e.g for raspberrypi:
# '-name *.rpi-sdimg -type l'.
# Output variables:
# DeployedFile: The file selected
# DeployFileDir: Path of the file selected
SelectDeployedFile() {
    if [ -z "$DeployedFile" ]; then
        # OE environment found?
        if [ -z "$BUILDDIR" ]; then
            ErrorOut "The environment variable BUILDDIR is not set. It is usually set before running bitbake."
        fi
        iCount=0
        strSelection=
        for grep_result in `grep -h TMPDIR $BUILDDIR/conf/*.conf | sed -e s/' '/''/g -e s/'\"'/''/g`; do
            # exclude comments
            tmp_dir=`echo $grep_result | grep '^TMPDIR='`
            if [ ! -z "$tmp_dir" ]; then
                OeTmpDir=`echo $tmp_dir | sed -e s/'TMPDIR='/''/g`
            fi
        done
        for BuildPath in ${OeTmpDir}-*; do
            for i in `find ${BuildPath}/deploy/images/${Machine} $FindString | sort` ; do
                iCount=`expr $iCount + 1`
                RootFileNameArr[${iCount}]=$i
                strSelection="$strSelection $iCount "`basename $i`
            done
        done

        # were files found?
        if [ $iCount -eq 0 ]; then
            ErrorOut "No files found in ${OeTmpDir}-\* matching $FindString"
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
            ErrorOut "Cancel selected at SelectDeployedFile()."
        fi
        DeployedFile=${RootFileNameArr[$menuitem]}
    fi
    DeployFileDir=`dirname $DeployedFile`

    # Now that we know the file, exact_machine can be set without wildcards
    Machine=`basename $DeployFileDir`
}

SelectSuSudo() {
    # Select su/sudo
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
        ErrorOut "Cancel selected at SelectSuSudo()."
    fi
}

SelectInOut() {
    # DevicePath for target card
    SelectCardDevice

    # select rootfs source
    SelectDeployedFile
}

# EnsureUnmount() tests if a device partition is mounted. If so partition is
# unmounted. Device name is expected in first parameter.
EnsureUnmount() {
    # check if the card is currently mounted
    MOUNTSTR=$(mount | grep $1)
    if [ -n "$MOUNTSTR" ] ; then
        echo -e "\n$1 is mounted. Unmounting..."
        umount -f ${1}?*
        echo "${style_green}Unmount done${style_normal}"
    fi
}

# PrepareWrite performs actions required before data write can start:
# * Check if source and destination locations are valid
# * Extract source path to variable DeployFileDir
# * eventually unmount destination
# The following variables are expected:
# * DevicePath: destination block-device e.g '/dev/sdc'
# * DeployedFile (optional): source file (rootfs/kernel/..)
PrepareWrite() {
    # device node valid?
    if [ ! -b "${DevicePath}" ] ; then
        ErrorOut "$DevicePath is not a valid block device!"
    fi

    if [ -n "${DeployedFile}" ] ; then
        # rootfs/kernel valid?
        if [ ! -e "${DeployedFile}" ] ; then
            ErrorOut "$DeployedFile can not be found!"
        fi
    fi
    EnsureUnmount $DevicePath
}

# RunUserStartRoot() tests if we are running as root. If not, it calls SelectInOut. It expects
# SelectInOut to leave the required parameters for root call in variable RootParams.
# After this it asks user how to log on as root. Then the main script is started
# as root.
RunUserStartRoot() {
    # we are not already root?
    if [ ! $( id -u ) -eq 0 ]; then
        SelectInOut
        SelectSuSudo

        # prepare passing params to root-run - make sure they have same
        # signature as loaded at the end of this file
        RootParams="$DevicePath $DeployedFile $DeployFileDir $Machine $FindString $KernelImageType"
        
        # dialog's gui is done here
        clear
        echo
        if [ "x$1" = "x" ] ; then
            # No message text passed: use standard
            echo "${style_bold}All data currenly stored on $DevicePath will be overwritten!${style_normal}"
        else
            # Use message text passed
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
        # sice we're 'execing' above, we wont reach this exit unless something
        # goes wrong.
        ErrorOut "RunUserStartRoot() should not have reached here!"
    fi
}

StartCardWrite() {
    # If not set on call - get defaults from machine.inc
    if [ -z "$Machine" ]; then
        Machine="$DEFAULT_MACHINE_FAMILY"
    fi
    if [ -z "$FindString" ]; then
        FindString="$DEFAULT_FIND_ROOTFS"
    fi

    RunUserStartRoot
    PrepareWrite
    RootCardWriteCallback
}

StartCardKernelWrite() {
    # If not set on call - get defaults from machine.inc
    if [ -z "$Machine" ]; then
        Machine="$DEFAULT_MACHINE_FAMILY"
    fi
    if [ -z "$FindString" ]; then
        FindString="$DEFAULT_FIND_KERNEL"
    fi
    if [ -z "$KernelImageType" ]; then
        KernelImageType="$DEFAULT_KERNEL_IMAGE_TYPE"
    fi

    RunUserStartRoot "${style_bold}The kernel images on %DevicePath% will be overwritten!${style_normal}"
    PrepareWrite
    RootCardKernelWriteCallback
}


# check common prerequisites
CheckPrerequisite "dialog"
CheckPrerequisite "lsblk"

# For user call the first two params can be set optionally - the first
# target/source dialogs won't be displayed then
DevicePath=$1
DeployedFile=$2
# For root call all params are mandatory
if [ $( id -u ) -eq 0 ]; then
    DeployFileDir=$3
    Machine=$4
    FindString=$5
    KernelImageType=$6
fi
