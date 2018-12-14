#------------------------------------------------------------------------------
# class for instant toolchain
#------------------------------------------------------------------------------

inherit instant-paths

PACKAGE_ARCH = "${MACHINE_ARCH}"

do_compile[postfuncs] += "${COMPILE_POST_TARGET}"
COMPILE_POST_TARGET = ""
COMPILE_POST_TARGET_class-target = "instant_compile_target"

INSTANTPOPULATE_TOOLCHAIN_VARS = " \
    CC \
    CFLAGS \
"

python instant_compile_target () {
    # create sourceable script that exports variables necessary
    for envvar in d.getVar('INSTANTPOPULATE_TOOLCHAIN_VARS').split():
        content = d.getVar(envvar).strip()
        line = 'export %s="%s"' % (envvar, content)
        line = line.replace(d.getVar('DEBUG_PREFIX_MAP'),'-fdebug-prefix-map=%s= -fdebug-prefix-map=%s=' % (d.getVar('INSTANT_TARGET_PATH'), d.getVar('INSTANT_NATIVE_PATH') ))
        line = line.replace(d.getVar('STAGING_DIR_NATIVE'),d.getVar('INSTANT_NATIVE_PATH'))
        line = line.replace(d.getVar('STAGING_DIR_HOST'),d.getVar('INSTANT_TARGET_PATH'))
        print(line)
        print(d.getVar('DEBUG_PREFIX_MAP'))
}
