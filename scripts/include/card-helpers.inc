#! /bin/bash

# card-helpers.inc
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script contains some helper functions and can be sourced by other 
# scripts. To work properly common-helpers.inc in this directory must
# be sourced before this file.

# SelectCardDevice() creates a dialog to select one of all available removable
# devices.
# Output variables:
# DevicePath: path to the selected device
# DeviceSize: size of the selected device
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
                for name in `lsblk --raw --noheadings --output NAME $dev` ; do
                    # device
                    if [ "x$secondline" = "x" ]; then
                        # first line - it is for device
                        secondline="1"
                        model=`lsblk --nodeps --raw --noheadings --output MODEL $dev`
                        # a bug in lsblk?
                        model=`echo $model | sed 's:\\\x20::g'`
                        if [ -n "$model" ]; then
                            model="'$model'"
                        fi
                        display="$name $model size: `lsblk --nodeps --raw --noheadings --output SIZE $dev`"
                    # partition
                    else
                        label=`lsblk --nodeps --raw --noheadings --output LABEL /dev/$name`
                        if [ -n "$label" ]; then
                            label="'$label'"
                        fi
                        if [ "x$secondline" = "x1" ]; then
                            secondline="2"
                            # second: header + partition
                            display="$display | partitions: $name $label"
                         else
                            # third..: separator + partition
                            display="$display + $name $label"
                        fi
                    fi
                done
                display=`echo "$display" | sed 's:  : :g'`
                display=`echo "$display" | sed 's:  : :g'`
                display=`echo "$display" | sed 's:  : :g'`
                
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
            2>"$base_tempdir/menuitem.$$"

        # get OK/Cancel
        sel=$?
        # get selected menuitem
        menuitem=`cat "$base_tempdir/menuitem.$$"`
        rm -f "$base_tempdir/menuitem.$$"

        # Cancel Button or <ESC>
        if [ $sel -eq 1 -o $sel -eq 255 ] ; then
            ErrorOut 'Cancel selected at SelectCardDevice().'
        fi
        DevicePath=${DevicePathArr[$menuitem]}
        DeviceSize=`lsblk --nodeps --raw --noheadings --output SIZE --bytes $dev`
        unset menuitem
        unset menuitems
    fi
}

# SelectDeployedFile() opens a dialog to select file containing rootfs/kernel/...
# It expects one parameter which contains find parameters e.g for raspberrypi:
# '-name *.rpi-sdimg -type l'.
# Output variables:
# DeployedFile: The file selected
# DeployedFileSize: Size of the file selected
# DeployFileDir: Path of the file selected
SelectDeployedFile() {
    if [ -z "$DeployedFile" ]; then
        # OE environment found?
        if [ -z "$BUILDDIR" ]; then
            ErrorOut "The environment variable BUILDDIR is not set. It is usually set before running bitbake."
        fi
        iCount=0
        for grep_result in `grep -h TMPDIR $BUILDDIR/conf/*.conf | sed -e s/' '/''/g -e s/'\"'/''/g`; do
            # exclude comments
            tmp_dir=`echo $grep_result | grep '^TMPDIR='`
            if [ ! -z "$tmp_dir" ]; then
                OeTmpDir=`echo $tmp_dir | sed -e s/'TMPDIR='/''/g`
            fi
        done
        for BuildPath in ${OeTmpDir}-*; do
            for FilePath in `find ${BuildPath}/deploy/images/${Machine} $FindString | sort` ; do
                iCount=`expr $iCount + 1`
                RootFileNameArr[${iCount}]="$FilePath"
                FileSize=`du -Dh "$FilePath" | cut -f1`
                menuitems+=( "$iCount" "`basename $FilePath` / size: $FileSize" )
            done
        done

        # were files found?
        if [ $iCount -eq 0 ]; then
            ErrorOut "No files found in ${OeTmpDir}-\* matching $FindString"
        fi

        dialog --title 'Select file to write'\
            --menu 'Move using [UP] [DOWN],[Enter] to select' 30 100 $iCount\
            "${menuitems[@]}"\
            2>"$base_tempdir/menuitem.$$"

        # get OK/Cancel
        sel=$?
        # get selected menuitem
        menuitem=`cat "$base_tempdir/menuitem.$$"`
        rm -f "$base_tempdir/menuitem.$$"

        # Cancel Button or <ESC>
        if [ $sel -eq 1 -o $sel -eq 255 ] ; then
            ErrorOut "Cancel selected at SelectDeployedFile()."
        fi
        DeployedFile=${RootFileNameArr[$menuitem]}
        unset menuitem
        unset menuitems
    fi
    DeployedFileSize=`du -Db "$DeployedFile" | cut -f1`
    DeployFileDir=`dirname $DeployedFile`

    # Now that we know the file, exact machine can be set without wildcards
    Machine=`basename $DeployFileDir`
}

# SelectSuSudo opens a dialog in which the user can select how to login
# as root: either su with root password or sudo
# The selection result is found in variable SuSudoSelection:
# su -> 1
# sudo -> 2
SelectSuSudo() {
    if [ -z "$SuSudoSelection" ]; then
        # Select su/sudo
        dialog --title 'Select how you want to logon as root'\
            --menu 'Move using [UP] [DOWN],[Enter] to select' 10 100 2 1 su 2 sudo \
            2>"$base_tempdir/menuitem.$$"

        # get OK/Cancel
        sel=$?
        # get selected menuitem
        SuSudoSelection=`cat "$base_tempdir/menuitem.$$"`
        rm -f "$base_tempdir/menuitem.$$"

        # Cancel Button or <ESC>
        if [ $sel -eq 1 -o $sel -eq 255 ] ; then
            ErrorOut "Cancel selected at SelectSuSudo()."
        fi
    fi
}

SelectInOut() {
    # DevicePath for target card
    SelectCardDevice

    # select rootfs source
    SelectDeployedFile

    # rudimental size check
    if [ "$DeployedFileSize" -gt "$DeviceSize" ]; then
        ErrorOut "`basename $DeployedFile` (size: $DeployedFileSize) is too large for $DevicePath (size: $DeviceSize)"
    fi
}

# EnsureUnmount() tests if a device partition is mounted. If so partition is
# unmounted. Device name is expected in first parameter.
EnsureUnmount() {
    # check if the card is currently mounted
    MOUNTSTR=$(mount | grep $1)
    if [ -n "$MOUNTSTR" ] ; then
        echo -e "${style_magenta}Unmount all partitions of $1...${style_normal}"
        umount -f ${1}?* 2>/dev/null
        echo "${style_green}Unmount done.${style_normal}"
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

# RunUserStartRoot() tests if we are running as root. If not, it calls
# SelectInOut and SelectSuSudo and starts the main script as root.
# One parameter can be set optionally containing the last-exit message.
# If not set the standard full-card message is displayed. If set the
# placeholders %DeployedFile% and %DeployedFile% are replaces by actual
# contents of the variables.
RunUserStartRoot() {
    # we are not already root?
    if [ ! $( id -u ) -eq 0 ]; then
        SelectInOut
        SelectSuSudo

        # prepare passing params to root-run - make sure they have same
        # signature as loaded at the end of this file and replace spaces -
        # otherwise param are realigned
        for setting in "$DevicePath" "$DeployedFile" "$DeployFileDir" "$Machine" "$FindString" "$KernelImageType"; do
            RootParams="$RootParams `echo $setting | sed 's: :%20:g'`"
        done
        # dialog's gui is done here
        clear
        echo "${style_bold}${style_red}IMPORTANT: THIS is YOUR LAST EXIT...${style_normal}"
        if [ "x$1" = "x" ] ; then
            # No message text passed: use standard
            echo "${style_bold}All data on $DevicePath will be overwritten by $DeployedFile${style_normal}"
        else
            # Use message text passed
            echo $1 | sed "s|\%DevicePath\%|$DevicePath|" | sed "s|\%DeployedFile\%|$DeployedFile|"
        fi

        if [ $SuSudoSelection -eq 1 ]; then
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

# CheckPartitionTable: Checks if device has a partition table. If not write one.
# The following parameters are expected:
# 1. Device: e.g /dev/sdc
# 2. Partition table type: This parameter is optional and default value is msdos
CheckPartitionTable() {
    # This function is supposed to be called fist before data is written
    # so do same safety checks (although it might be redundant)
    if [ -z "$1" ]; then
        ErrorOut "CheckPartitionTable: no parameter"
    fi
    if [ ! -b "$1" ]; then
        ErrorOut "CheckPartitionTable: parameter $1 does not point to a block device"
    fi
    dev=`basename $1`
    if [ ! `cat /sys/block/$dev/removable` = '1' ]; then
        ErrorOut "CheckPartitionTable: parameter $1 does not point to a removable device"
    fi
    # checks passed here
    parttype="$2"
    if [ -z "$parttype" ]; then
        parttype="msdos"
    fi
    # check for partition table
    partresult=`parted -s $1 -- print 2> /dev/null`
    PartTableReq=0
    if ! echo -e $partresult | grep -q "$parttype"; then
        PartTableReq=1
    fi
    # do we need to write part table?
    if [ $PartTableReq -eq 1 ]; then
        EvalExAuto "parted -s $1 -- mklabel $parttype" "\nWrite partition table $parttype..."
        # During tests I found that my card writer failed writing parttion-
        # table. So for others have similiar - check if partition table arrived.
        # Funny side note: Now my box with sdcard marked as 'broken' is empty again :)
        partresult=`parted -s $1 -- print 2> /dev/null`
        if ! echo -e $partresult | grep -q "$parttype"; then
            ErrorOut "Partition table '$parttype' was not written. Check your sdcard or another card-adapter!"
        fi
    fi

    unset retVal parttype partresult retVal PartTableReq
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

    RunUserStartRoot "${style_bold}Kernel on %DevicePath% will be overwritten by %DeployedFile%${style_normal}"
    PrepareWrite
    RootCardKernelWriteCallback
}

# check common prerequisites
CheckPrerequisite "dialog"
CheckPrerequisite "lsblk"

# Common script parameter handling for all scripts
if [ $( id -u ) -eq 0 ]; then
    # For root call all params are mandatory. It is important to
    # * have the same signature as set in RunUserStartRoot
    # * reintroduce spaces
    DevicePath="`echo $1 | sed 's:%20: :g'`"
    DeployedFile="`echo $2 | sed 's:%20: :g'`"
    DeployFileDir="`echo $3 | sed 's:%20: :g'`"
    Machine="`echo $4 | sed 's:%20: :g'`"
    FindString="`echo $5 | sed 's:%20: :g'`"
    KernelImageType="`echo $6 | sed 's:%20: :g'`"
else
    # User call params: By setiing them dialogs are not displayed. If
    # one really wants, custom parameters can be appended after those
    # set here
    DevicePath=$1
    DeployedFile=$2
    SuSudoSelection=$3
fi
