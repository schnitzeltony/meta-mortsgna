#! /bin/bash

# raspberrypi-card-kernel-write.sh
# (c) Copyright 2018 Andreas Müller <schnitzeltony@gmail.com>
# Licensed under terms of GPLv2
#
# This script writes kernel+modules to SDCard. To
# select card device a dialog based GUI is used.

# Includes
. `dirname $0`/include/common-helpers.inc
. `dirname $0`/include/card-helpers.inc
. `dirname $0`/include/machine-raspberrypi.inc

StartCardKernelWrite
