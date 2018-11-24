#------------------------------------------------------------------------------
# class for instant native sysroot
#------------------------------------------------------------------------------

inherit utils

# This is where instant sysroot is installed into
INSTANT_SYSROOT_PATH ??= "${TMPDIR}/sysroot-instant-native"

do_prepare_recipe_sysroot[postfuncs] += "${INSTANTSYSROOTFUNC}"
INSTANTSYSROOTFUNC = ""
INSTANTSYSROOTFUNC_class-native = "instant_sysroot_copy"
INSTANTSYSROOTFUNC_class-cross = "instant_sysroot_copy"

instant_sysroot_copy () {
    mkdir -p ${INSTANT_SYSROOT_PATH}
    hardlinkdir ${RECIPE_SYSROOT_NATIVE} ${INSTANT_SYSROOT_PATH}
}

do_populate_sysroot[postfuncs] += "${INSTANTPOPULATE}"
INSTANTPOPULATE = ""
INSTANTPOPULATE_class-native = "instant_populate_sysroot"
INSTANTPOPULATE_class-cross = "instant_populate_sysroot"

instant_populate_sysroot () {
    for executable in `find ${SYSROOT_DESTDIR}/${STAGING_DIR_NATIVE} -type f`; do
        alignedpath=`echo ${executable} | sed 's:${SYSROOT_DESTDIR}/${STAGING_DIR_NATIVE}::'`
        targetdir=`dirname ${INSTANT_SYSROOT_PATH}/${alignedpath}`
        mkdir -p "$targetdir"
        cp -fl "${executable}" "${targetdir}"
    done
}
