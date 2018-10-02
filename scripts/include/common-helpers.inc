#! /bin/bash

# common-helpers.inc
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
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

# base tempdir
base_tempdir=`mktemp -u`
base_tempdir=`dirname "$base_tempdir"`

# ErrorOut outputs error text in first param and exits script
ErrorOut() {
    clear
    echo "${style_red}${style_bold}${1}${style_normal}"
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


