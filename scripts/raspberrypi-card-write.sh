#! /bin/bash

# raspberrypi-card-write.sh
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script writes image to sdcard and aligns rootfs partition to max size.

# Includes
. `dirname $0`/include/common-helpers.inc
. `dirname $0`/include/card-helpers.inc
. `dirname $0`/include/machine-raspberrypi.inc

StartCardWrite
