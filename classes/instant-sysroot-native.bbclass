#------------------------------------------------------------------------------
# class for instant native sysroot
#
# Hardlink recipe sysroot and generated sysroot to instant native sysroot
#------------------------------------------------------------------------------

inherit instant-paths

do_prepare_recipe_sysroot[postfuncs] += "${INSTANTSYSROOTFUNC}"
INSTANTSYSROOTFUNC = ""
INSTANTSYSROOTFUNC:class-native = "instant_sysroot_copy"
INSTANTSYSROOTFUNC:class-cross = "instant_sysroot_copy"

instant_sysroot_copy () {
    mkdir -p ${INSTANT_NATIVE_PATH}
    hardlinkdir ${RECIPE_SYSROOT_NATIVE} ${INSTANT_NATIVE_PATH}
}

do_populate_sysroot[postfuncs] += "${INSTANTPOPULATE} "
INSTANTPOPULATE = ""
INSTANTPOPULATE:class-native = "instant_populate_sysroot"
INSTANTPOPULATE:class-cross = "instant_populate_sysroot"

instant_populate_sysroot () {
    find "${SYSROOT_DESTDIR}${STAGING_DIR_NATIVE}" -type f -o -type l | while read file; do
        alignedpath=`echo "$file" | sed 's:${SYSROOT_DESTDIR}${STAGING_DIR_NATIVE}::'`
        targetdir=`dirname "${INSTANT_NATIVE_PATH}/${alignedpath}"`
        mkdir -p "$targetdir"
        cp -fld "${file}" "${targetdir}"
    done
}
