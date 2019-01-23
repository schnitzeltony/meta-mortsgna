#!/bin/sh

# raspberrypi-card-write.sh
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script updates checksums in recipes after version bump. It is supposed
# to run run in same environment as bitbake:
#
# update-recipe-checksums.sh <recipe>
#
# where <recipe> can be a single recipe, a packagegroup an image or...

# Includes
. `dirname $0`/include/common-helpers.inc

if [ -z "$1" ]; then
    ErrorOut "No fetch target set in first parameter!"
fi

# Ask bitbake for recipe directory
GetBitbakeEnvVar "TOPDIR"
_TOPDIR="$BitbakeEnvVar"

echo
echo -e "${style_bold}Run bitbake -k --runall=fetch $@...${style_normal}"

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
               EvalExAuto "sed -i '"s:$oldchecksum:$newchecksum:"' "$recipe"" "Change checksum in $recipe to $newchecksum"
           fi
       done
   fi
done
