#!/bin/sh

# raspberrypi-card-write.sh
# (c) Copyright 2020 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script updates checksums in recipes after version bump. It is supposed
# to run run in same environment as bitbake:
#
# update-recipe-checksums.sh [-d <recipedir> <recipes>
#
# where <recipe> can be a single recipe, a packagegroup an image or...

# Includes
. `dirname $0`/include/common-helpers.inc

echo
if [ "$1" = "-d" ]; then
    shift
    _TOPDIR="$1"
    echo -e "${style_bold}Use $1 as recipe directory...${style_normal}"
    shift
fi

if [ -z "$1" ]; then
    ErrorOut "No fetch target set in first parameter!"
fi

if [ "x$_TOPDIR" = "x" ]; then
    echo -e "${style_bold}Ask bitbake for recipe directory...${style_normal}"
    GetBitbakeEnvVar "TOPDIR"
    _TOPDIR="$BitbakeEnvVar"
fi

echo
echo -e "${style_bold}Run bitbake -k --runall=fetch $@...${style_normal}"

while true; do
    replaced=""
    bitbake -k --runall=fetch "$@" 2>&1 | while read line; do
        if echo "$line" | grep -q "was expected"; then
            # Shorten line to ensure not being confused by filenames containing spaces
            line=`echo "$line" | sed 's:.*checksum ::'`
            # Extract checksums
            newchecksum=`echo "$line" | awk '{print $1}'`
            oldchecksum=`echo "$line" | awk '{print $3}'`
            # Lazy way: just grep for old checksums to find recipes
            for recipe in `grep -rl "$oldchecksum" "${_TOPDIR}"`; do
                if [ -f "$recipe" ]; then
                    replaced="1"
                    EvalExAuto "sed -i '"s:$oldchecksum:$newchecksum:"' "$recipe"" "Change checksum in $recipe to $newchecksum"
                fi
            done
        fi
    done
    if [ $replaced != "1" ]
        break
    fi
done
    
