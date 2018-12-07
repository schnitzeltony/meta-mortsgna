PACKAGECONFIG_DISTRO += " \
    icu \
    gif \
    gtk \
    harfbuzz \
"

inherit instant-paths

do_copy_to_cross_sysroot_append() {
    # ---------- qt.conf ----------
    # create
    generate_qt_config_file_paths
    generate_qt_config_file_effective_paths

    # copy to cross sysroot
    cross_sysroot_qtconf="${INSTANT_CROSS_PATH}${OE_QMAKE_PATH_QT_BINS}/`basename ${OE_QMAKE_QTCONF_PATH}`"
    cp -f ${OE_QMAKE_QTCONF_PATH} "$cross_sysroot_qtconf"
    echo "$cross_sysroot_qtconf" >> ${INSTANT_MANIFEST}

    # adjust to instant sysroot
    qtconf_cross_sysroot=
    sed -i \
        -e 's:${STAGING_DIR_NATIVE}:${INSTANT_NATIVE_PATH}:g' \
        -e 's:${STAGING_DIR_HOST}:${INSTANT_CROSS_PATH}:g' \
        "$cross_sysroot_qtconf"

    # ---------- mkspecs ----------
    # qmake.conf replace -fdebug-prefix-map ( see bitbake.conf )

}
