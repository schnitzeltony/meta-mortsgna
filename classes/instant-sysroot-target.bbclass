#------------------------------------------------------------------------------
# class for instant remote debugging by building a debug sysroot
#
# It adresses a use case seen umptenth times: After creating or changing a
# recipe, the build result crashes on target e.g with segfault or without
# dropping helpful information. In these situations one wants to debug NOW
# without further waiting or quirky activities.
#
# To achieve, a debug sysroot is build with low cost:
#
# * All files are hard-linked to recipe's ${WORKDIR}/package
# * Additional build time for task do_copy_to_target_sysroot for most recipes is < 1s
#
# to enable debug sysroot build set:
# 'INHERIT += "instant-sysroot-target"'
# in your local.conf
#
# to debug by remote do:
#
# ON TARGET:
# * install gdbserver (build automatically for images if this class enabled - see
#   EXTRA_IMAGEDEPENDS below)
# * open a shell and enter 'gdbserver :<IP-Port> <full path of executable with optional args>' e.g
#   'gdbserver :5000 /usr/bin/thunar'
#
# ON BUILD HOST (suggested IDE: QtCreator)
# * build gdb-target-<TARGET_ARCH> (done automatically on images if this class
#   enabled - see EXTRA_IMAGEDEPENDS below)
# * in QtCreator select Menu Debug/Start Debugging/Attach to Running Debug Server
#   -> Dialog 'Start Debugger' opens
# * At the first session a so called 'Kit' has to be set up (1st line -> 'Manage').
#   The settings are kept so 1.-6. have to be done once only.
#   1. Create a Kit by 'Add' -> further dialog opens
#   2. Select an name for the Kit e.g 'OE'
#   3. Set sysroot (see INSTANT_TARGET_PATH in instant-path.bbclass):
#      ${TMPDIR}/sysroot-instant-target-${MACHINE_ARCH}
#   4. Select compilers (it is not necessary for debug but without QTCreator won't enable Kit) for C and C++ e.g:
#      C:   '<TMDIR>/sysroot-instant-native/usr/bin/arm-mortsgna-linux-gnueabi/arm-mortsgna-linux-gnueabi-gcc'
#      C++: '<TMDIR>/sysroot-instant-native/usr/bin/arm-mortsgna-linux-gnueabi/arm-mortsgna-linux-gnueabi-g++'
#   5. Select debugger e.g:
#      GDB: '<TMDIR>/sysroot-instant-native/usr/bin/arm-mortsgna-linux-gnueabi/arm-mortsgna-linux-gnueabi-gdb'
#   6. Select 'OK' in Options dialog -> 'Start Debugger' should be back on top
# * Make sure 'OE' Kit is selected
# * Browse for executable e.g '<TMDIR>/sysroot-instant-target/usr/bin/thunar'
# * Set IP:Port of target machine e.g '192.168.2.108:5000'
# * Select 'OK'
#
# Happy debugging!!
#
#------------------------------------------------------------------------------

inherit utils instant-paths

# ensure necessary gdb recipes are build
EXTRA_IMAGEDEPENDS += "gdb-cross-${TARGET_ARCH} gdb"

python __anonymous () {
    if d.getVar('CLASSOVERRIDE') != 'class-target':
        bb.build.deltask('do_copy_to_target_sysroot', d)
}

INSTANT_MANIFEST = "${INSTANT_TARGET_PATH}/manifests/${PN}"

do_copy_to_target_sysroot() {
    # ---------- bail out on package-less recipes ----------
    if [ ! -d "${WORKDIR}/packages-split" -o ! -d ${WORKDIR}/package ]; then
        exit 0
    fi

    # ---------- remove old sources ----------
    rm -rf ${INSTANT_TARGET_PATH}/usr/src/debug/${PN}

    # ---------- remove old files in manifest and manifest ----------
    if [ -f ${INSTANT_MANIFEST} ] ; then
        echo "Old manifest ${INSTANT_MANIFEST} found - remove files..."
        # remove old files from sysroot
        for file in `cat ${INSTANT_MANIFEST}` ; do
            if ! rm "${INSTANT_TARGET_PATH}/$file" 2> /dev/null; then
                echo "Tried to delete '${INSTANT_TARGET_PATH}/$file' but it is not there! A look into mainfest creation at '${INSTANT_MANIFEST}' might help."
            fi
        done
        # remove old manifest
        rm ${INSTANT_MANIFEST}
    fi

    # ---------- hard link source code files ----------
    if [ -d ${WORKDIR}/package/usr/src/debug/${PN} ] ; then
        mkdir -p ${INSTANT_TARGET_PATH}/usr/src/debug/${PN}
        hardlinkdir ${WORKDIR}/package/usr/src/debug/${PN} ${INSTANT_TARGET_PATH}/usr/src/debug/${PN}
    fi

    # ---------- names of binaries and debuginfo -> manifest ----------
    mkdir -p ${INSTANT_TARGET_PATH}/manifests
    # get path to library-link once only
    if [ "${PN}" = "glibc-locale" ] ; then
        PACK_SPLIT_LIB_LINK_SEARCH_PATH=`find ${WORKDIR}/packages-split -mindepth 1 -maxdepth 1 -type d ! -name '*-dbg' ! -name '*-dev' ! -name '*-staticdev' ! -name '*-doc' ! -name 'glibc*-localedata-*' ! -name 'glibc-charmap-*' ! -name 'locale-base-*'`
    # other specials go here
    # elif...
    else
        PACK_SPLIT_LIB_LINK_SEARCH_PATH=`find ${WORKDIR}/packages-split -mindepth 1 -maxdepth 1 -type d ! -name '*-dbg' ! -name '*-dev' ! -name '*-staticdev' ! -name '*-doc' ! -name '${PN}*-locale-*'`
    fi
    echo "Search so-links in:"
    echo "$PACK_SPLIT_LIB_LINK_SEARCH_PATH"
    echo
    # add new
    for pkgdbg in `find ${WORKDIR}/packages-split -mindepth 1 -maxdepth 1 -type d -name '*-dbg'` ; do
        debug_binaries=
        # Note: hardcoding is used in package.bbclass either (search for PACKAGE_DEBUG_SPLIT_STYLE)
        if [ "${PACKAGE_DEBUG_SPLIT_STYLE}" = "debug-file-directory" ] ; then
            if [ -d $pkgdbg/usr/lib/debug ] ; then
                debug_binaries=`find $pkgdbg/usr/lib/debug -name '*.debug'`
            fi
        else
            debug_binaries=`find $pkgdbg -wholename '*.debug/*'`
        fi
        for file in $debug_binaries; do
            # do 'root' path
            file=`echo $file | sed -e 's:'$pkgdbg'::'`

            # stripped binary (non debug)
            if [ "${PACKAGE_DEBUG_SPLIT_STYLE}" = "debug-file-directory" ] ; then
                filestripped=`echo $file | sed -e 's:/usr/lib/debug::' -e 's:\.debug::'`
            else
                filestripped=`echo $file | sed -e 's:\.debug/::'`
            fi
            # keep files in manifest
            echo $file >> ${INSTANT_MANIFEST}
            echo $filestripped >> ${INSTANT_MANIFEST}
            # check for so-file links
            if echo $filestripped | grep -q '\.so'; then
                soname=`basename $filestripped`
                for packsplit in  $PACK_SPLIT_LIB_LINK_SEARCH_PATH; do
                    for link in `find $packsplit -lname $soname` ; do
                        # do 'root' path
                        link=`echo $link | sed -e 's:'$packsplit'::'`
                        echo $link >> ${INSTANT_MANIFEST}
                    done
                done
            fi
        done
    done

    # ---------- get -dev/-mkspecs packet contents -> manifest ----------
    for package in "${PN}-dev" "${PN}-mkspecs" ; do
        if [ -d ${WORKDIR}/packages-split/$package ]; then
            echo "Add files from ${WORKDIR}/packages-split/$package to manifest..."
            for file in `find ${WORKDIR}/packages-split/$package -type f` ; do
                # do 'root' path
                file=`echo $file | sed -e 's:${WORKDIR}/packages-split/'$package'::'`
                echo $file >> ${INSTANT_MANIFEST}
            done
        fi
    done

    # ---------- manifest: do the hardlinks (optimization welcome..) ----------
    if [ -f ${INSTANT_MANIFEST} ] ; then
        echo "Create hardlinks from manifest ${INSTANT_MANIFEST}..."
        cd ${WORKDIR}/package
        for file in `cat ${INSTANT_MANIFEST}` ; do
            file=`echo $file | cut -c 2-`
            if [ -e $file ] ; then
                echo -n $file | cpio --null -pdlu ${INSTANT_TARGET_PATH} > /dev/null 2>&1
            fi
        done
    fi
}

addtask copy_to_target_sysroot after do_package before do_build

# same as do package
do_copy_to_target_sysroot[vardeps] = "${PACKAGEBUILDPKGD} ${PACKAGESPLITFUNCS} ${PACKAGEFUNCS} ${@gen_packagevar(d)}"

do_copy_to_target_sysroot[stamp-extra-info] = "${MACHINE_ARCH}"

do_build[recrdeptask] += "do_copy_to_target_sysroot"
