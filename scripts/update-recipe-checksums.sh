#!/bin/sh

# raspberrypi-card-write.sh
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script updates checksums in recipes after version bump. It run in same
# environment as bitbake by:
#
# update-recipe-checksums.sh <recipe>
#
# where recipe can be a single recipe, a packagegroup an image or...

# Includes
. `dirname $0`/include/common-helpers.inc

# Do we find bitbake
if [ "x`which bitbake 2>/dev/null`" = "x" ] ; then
    ErrorOut "bitbake not found! Run this script in same environment as bitbake."
fi


OLDIFS=$IFS
IFS=$'\n'

echo
echo -e "${style_bold}Run bitbake -k ${1}...${style_normal}"

for line in `bitbake -k $1 2>&1`; do
   if echo "$line" | grep -q "was expected"; then
       # Shorten line to ensure not being confused by filenames containing spaces
       line=`echo "$line" | sed 's:.*checksum ::'`
       # Extract checksums
       newchecksum=`echo "$line" | awk '{print $1}'`
       oldchecksum=`echo "$line" | awk '{print $3}'`
       # Lazy way: just grep for old checksums to find recipes
       for recipe in `grep -rl "$oldchecksum" "$OE_SOURCE_DIR"`; do
           if [ -f "$recipe" ]; then
               EvalExAuto "sed -i '"s:$oldchecksum:$newchecksum:"' "$recipe"" "Change checksum in $recipe to $newchecksum"
           fi
       done
   fi
done
IFS=$OLDIFS

