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

    # copy to cross sysroot
    qtconf=`basename ${OE_QMAKE_QTCONF_PATH}`
    targetpath=${INSTANT_CROSS_PATH}${OE_QMAKE_PATH_HOST_BINS}
    cp -f "${OE_QMAKE_QTCONF_PATH}" "$targetpath"
    echo "${OE_QMAKE_PATH_HOST_BINS}/$qtconf" >> ${INSTANT_MANIFEST}

    # adjust to instant sysroot
    sed -i \
        -e 's:${STAGING_DIR_NATIVE}:${INSTANT_NATIVE_PATH}:g' \
        -e 's:${STAGING_DIR_HOST}:${INSTANT_CROSS_PATH}:g' \
        -e 's:^HostBinaries =.*:HostBinaries = ${INSTANT_NATIVE_PATH}${bindir}${QT_DIR_NAME}:g' \
        "$targetpath/$qtconf"

    # ---------- mkspecs qmake.conf ----------
    rm ${INSTANT_CROSS_PATH}${libdir}${QT_DIR_NAME}/mkspecs/${XPLATFORM}/qmake.conf
    cp -f "${WORKDIR}/packages-split/${PN}-mkspecs${libdir}${QT_DIR_NAME}/mkspecs/${XPLATFORM}/qmake.conf" \
        "${INSTANT_CROSS_PATH}${libdir}${QT_DIR_NAME}/mkspecs/${XPLATFORM}/"
    sed -i \
        -e 's:${DEBUG_PREFIX_MAP}:-fdebug-prefix-map=${INSTANT_CROSS_PATH}= -fdebug-prefix-map=${INSTANT_NATIVE_PATH}=:g' \
        "${INSTANT_CROSS_PATH}${libdir}${QT_DIR_NAME}/mkspecs/${XPLATFORM}/qmake.conf"

}
