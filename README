.-.    .-.
|  \  /  |            .-.
|   \/   |.-----.----.| |   .-----.-----.-----..----.
|        ||     |  .-'| '--.|  ---' __  |     |.--  |
|    |   ||  '  |  |  |  --'|---  |     |  |  |  '  \
'----'---''-----'--'  '----''-----'--.  |--'--''--`--`
                                   .-'  |
                                   '----'

meta-mortsgna is yet another distribution layer. It is based on Ångström
distribution  - check https://github.com/Angstrom-distribution/meta-angstrom.

The target of this layer is to build images with the best out-of-the-box
experience and to support developers with helpers for common tasks.

-------------------------------------------------------------------------------
Layer maintainer: Andreas Müller <schnitzeltony@gmail.com>


-------------------------------------------------------------------------------
Additionally this layer checks the following variable in local.conf:

* MORTSGNA_ARMV7_TUNE:
  If this variable is set to "1", all armv7a / cortex variants are build as
  armv7a.

  + Reduce build time/disk-space required when building for multiple machines
  - Build optimization (e.g vfp/NEON) is not the best your machine can get

  Rules of thumb:
  * If you build images for multiple armv7 machines and don't need to tickle
    the last quarter of your machine's performance:
    Set MORTSGNA_ARMV7_TUNE="1"
  * If you are looking for best optimization for your machine and/or you have
    a fast build host with lots of disk-space: don't!
  see conf/distro/include/mortsgna-armv7-tune.inc for more information

-------------------------------------------------------------------------------
The following image-features are (re-)added:

* x11-base: Let systemd boot into grahical target

* volatile-log: If set, journal goes to volatile /run/log and /var/log is
  linked to /run/log which is volatile.
  NOTE: We change OE-Core's default here: In bitbake.conf VOLATILE_LOG_DIR is
  enabled so that all images are build for volatile log. Since this is far off
  what others do change the default.
  See conf/distro/include/mortsgna-image-logfile-volatile.inc for more
  information.

-------------------------------------------------------------------------------
This layer contains the following folders:

appends:
  All bbappends are stored in this folder. For each layer a sub-folder exists
  and the appends are dynamic: If a layer is not in bblayers.conf the folder
  for the layer is ignored (see conf/layer.conf).

classes:
  * instant-sysroot-target.bbclass: Create a sysroot for target remote
    building/debugging - Yocto's suggested workflows are cumbersome for ad-hoc
    usage.
  * instant-sysroot-native.bbclass: Native recipes inheriting this class
    install everything necessary to run into a native sysroot. It's like a
    quick & minimal native SDK. Currently gcc-cross and gdb-cross inherit this
    class to enable instant remote debugging.
  * user-with-full-skel.bbclass: Helper to populate /etc/skel in user's
    home at image creation time.

conf:
  layer.conf / and distro configurations

recipes-defaultconfig:
  * default configurations for lxqt and xfce to get a preconfigured
    environment. Note that all data is stored in th image at /etc/skel
  * unpriv-user: A default non root user (named morona) member of many groups
    required for daily operations. It is shipped with home folder populated
    form /etc/skel.

recipes-image:
  All images for console / kde / lxqt / xfce ar found. allgui-images build
  images for all graphical environments.

recipes-utlis:
  * udev-rules-udisks-hide-bootpart: Hide boot partition from udisks
  * Some tools (these are the only left from angstrom). Honestly: Have never
    used them but kept them if users come from angstrom and might miss
    them.

scripts:
  A script collection
  * to write images or kernel only to sdcards
  * for changing recipe checksums automatically after version bump


-------------------------------------------------------------------------------
This layer depends on:

URI: git://git.openembedded.org/openembedded-core
branch: master
revision: HEAD


For building console images:

URI: git://git.openembedded.org/meta-openembedded
Layer: meta-oe
branch: master
revision: HEAD

URI: git://git.openembedded.org/meta-openembedded
Layer: meta-networking
branch: master
revision: HEAD


-------------------------------------------------------------------------------
Other layers are required when building 'GUI images'

URI: git://git.openembedded.org/meta-openembedded
Layer: meta-gnome
branch: master
revision: HEAD

URI: git://git.openembedded.org/meta-openembedded
Layer: meta-multimedia
branch: master
revision: HEAD

URI: git://git.openembedded.org/meta-openembedded
Layer: meta-python
branch: master
revision: HEAD

URI: git://git.openembedded.org/meta-openembedded
Layer: meta-xfce
branch: master
revision: HEAD

URI: git://github.com/meta-qt5/meta-qt5.git
branch: master
revision: HEAD

URI: git://github.com/schnitzeltony/meta-qt5-extra.git
branch: master
revision: HEAD

URI: git://github.com/schnitzeltony/meta-office.git
branch: master
revision: HEAD

URI: git://github.com/OSSystems/meta-browser.git
branch: master
revision: HEAD

URI: git://github.com/cazfi/meta-games.git
branch: master
revision: HEAD

URI: git://github.com/schnitzeltony/meta-retro
branch: master
revision: HEAD

-------------------------------------------------------------------------------
To add plymouth to 'GUI images'

URI: git://git.openembedded.org/meta-openembedded
Layer: meta-initramfs
branch: master
revision: HEAD

-------------------------------------------------------------------------------
To hear radio

URI: git://github.com/balister/meta-sdr.git
Layer: meta-initramfs
branch: master
revision: HEAD
