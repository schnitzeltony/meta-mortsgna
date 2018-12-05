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
# * Additional build time for task do_copysourcestosysroot for most recipes is < 1s
#
# to enable debug sysroot build set:
# 'INHERIT += "instant-remote-debug"'
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
# * build gdb-cross-<TARGET_ARCH> (done automatically on images if this class
#   enabled - see EXTRA_IMAGEDEPENDS below)
# * in QtCreator select Menu Debug/Start Debugging/Attach to Running Debug Server
#   -> Dialog 'Start Debugger' opens
# * At the first session a so called 'Kit' has to be set up (1st line -> 'Manage').
#   The settings are kept so 1.-6. have to be done once only.
#   1. Create a Kit by 'Add' -> further dilaog opens
#   2. Select an name for the Kit e.g 'OE'
#   3. Set sysroot (see INSTANT_REMOTE_PATH below):
#      ${TMPDIR}/sysroot-instant-remote-${MACHINE}
#   4. Select compilers (it is not necessary for debug but without QTCreator won't enable Kit) for C and C++ e.g:
#      C:   '<TMDIR>/sysroot-instant-native/usr/bin/arm-mortsgna-linux-gnueabi/arm-mortsgna-linux-gnueabi-gcc'
#      C++: '<TMDIR>/sysroot-instant-native/usr/bin/arm-mortsgna-linux-gnueabi/arm-mortsgna-linux-gnueabi-g++'
#   5. Select debugger e.g:
#      GDB: '<TMDIR>/sysroot-instant-native/usr/bin/arm-mortsgna-linux-gnueabi/arm-mortsgna-linux-gnueabi-gdb'
#   6. Select 'OK' in Options dialog -> 'Start Debugger' should be back on top
# * Make sure 'OE' Kit is selected
# * Browse for executable e.g '<TMDIR>/sysroot-instant-remote/usr/bin/thunar'
# * Set IP:Port of target machine e.g '192.168.2.108:5000'
# * Select 'OK'
#
# Happy debugging!!
#
# TODO:
# * Class won't work with rm_work.bbclass / rm_work_and_downloads.bbclass
#   -> Implement error message
# * Class does not work properly when package data is taken from sstate cache
#   -> Help appreciated
# * Class does not work properly for when changing machine
#------------------------------------------------------------------------------

# ensure necessary gdb recipes are build
EXTRA_IMAGEDEPENDS += "gdb-cross-${TARGET_ARCH} gdb"

# This is where instant sysroot is installed into
INSTANT_REMOTE_PATH = "${TMPDIR}/sysroot-instant-remote-${MACHINE}"

addtask copysourcestosysroot before do_packagedata after do_package

do_copysourcestosysroot() {
    # remove old source code / files in old manifest
    rm -rf ${INSTANT_REMOTE_PATH}/usr/src/debug/${PN}
    if [ -f ${INSTANT_REMOTE_PATH}/manifests/${PN} ] ; then
        # remove old files from sysroot
        for file in `cat ${INSTANT_REMOTE_PATH}/manifests/${PN}` ; do
            rm -f ${INSTANT_REMOTE_PATH}/$file
        done
        # remove old manifest
        rm ${INSTANT_REMOTE_PATH}/manifests/${PN}
    fi

    # ---------- link source code files ----------
    if [ -d ${WORKDIR}/package/usr/src/debug/${PN} ] ; then
        mkdir -p ${INSTANT_REMOTE_PATH}/usr/src/debug/${PN}
        cd ${WORKDIR}/package/usr/src/debug/${PN}
        find . -print0 | cpio --null -pdlu ${INSTANT_REMOTE_PATH}/usr/src/debug/${PN}
    fi

    # ---------- names of binaries and debuginfo -> manifest ----------
    mkdir -p ${INSTANT_REMOTE_PATH}/manifests
    # get path to library-link once only
    if [ "${PN}" = "glibc-locale" ] ; then
        PACK_SPLIT_LIB_LINK_SEARCH_PATH=`find ${WORKDIR}/packages-split -mindepth 1 -maxdepth 1 -type d ! -name '*-dbg' ! -name '*-dev' ! -name '*-staticdev' ! -name '*-doc' ! -name 'glibc*-localedata-*' ! -name 'glibc-charmap-*' ! -name 'locale-base-*'`
    # other specials go here
    # elif...
    else
        PACK_SPLIT_LIB_LINK_SEARCH_PATH=`find ${WORKDIR}/packages-split -mindepth 1 -maxdepth 1 -type d ! -name '*-dbg' ! -name '*-dev' ! -name '*-staticdev' ! -name '*-doc' ! -name '${PN}*-locale-*'`
    fi
    echo "Search so-link in $PACK_SPLIT_LIB_LINK_SEARCH_PATH.."
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
	        echo $file >> ${INSTANT_REMOTE_PATH}/manifests/${PN}
	        echo $filestripped >> ${INSTANT_REMOTE_PATH}/manifests/${PN}
	        # check for so-file links
	        if echo $filestripped | grep -q '\.so'; then
	            soname=`basename $filestripped`
                for packsplit in  $PACK_SPLIT_LIB_LINK_SEARCH_PATH; do
	                for link in `find $packsplit -lname $soname` ; do
                        # do 'root' path
                        link=`echo $link | sed -e 's:'$packsplit'::'`
	                    echo $link >> ${INSTANT_REMOTE_PATH}/manifests/${PN}
	                done
	            done
	        fi
        done
    done
    # ---------- names of includes (they might contain debuggable code) -> manifest ----------
    if [ -d ${WORKDIR}/packages-split/${PN}-dev/${includedir} ]; then
        for include in `find ${WORKDIR}/packages-split/${PN}-dev/${includedir} -type f` ; do
            # do 'root' path
            include=`echo $include | sed -e 's:'${WORKDIR}/packages-split/${PN}-dev/'::'`
	        echo $include >> ${INSTANT_REMOTE_PATH}/manifests/${PN}
        done
    fi

    # ---------- link to files in package folder from manifest ----------
    if [ -f ${INSTANT_REMOTE_PATH}/manifests/${PN} ] ; then
        cd ${WORKDIR}/package
        for file in `cat ${INSTANT_REMOTE_PATH}/manifests/${PN}` ; do
            file=`echo $file | cut -c 2-`
            if [ -e $file ] ; then
                echo -n $file | cpio --null -pdlu ${INSTANT_REMOTE_PATH}
            fi
        done
    fi
}

# remove source code links
do_clean[cleandirs] += "${INSTANT_REMOTE_PATH}/usr/src/debug/${PN}"

do_clean_append() {
    # remove binaries from mainfest
    manifest = "%s/manifests/%s" % (d.expand("${INSTANT_REMOTE_PATH}"), d.expand("${PN}"))
    if os.path.isfile(manifest):
        bb.note("Removing all files from manifest " + manifest)
        manifestfile = open(manifest, "r")
        for filetoremove in manifestfile:
            filetoremove = filetoremove.rstrip()
            filetoremove = d.expand("${INSTANT_REMOTE_PATH}") + filetoremove
            if os.path.isfile(filetoremove):
                os.remove(filetoremove)
        manifestfile.close()

        os.remove(manifest)
}
