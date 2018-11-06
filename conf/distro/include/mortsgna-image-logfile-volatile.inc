# override bitbake.conf default
VOLATILE_LOG_DIR = "false"

# Do alignment on journal and /var/log
ROOTFS_POSTPROCESS_COMMAND += "postinst_volatile_log; "

postinst_volatile_log () {
    if [ "${MORTSGNA_IMAGE_VOLATILE_LOG_DIR}" = "1" ]; then
        # systemd journal
        if [ -f "${IMAGE_ROOTFS}${sysconfdir}/systemd/journald.conf" ]; then
            sed -i \
                -e 's:.*Storage=.*:Storage=volatile:g' \
                ${IMAGE_ROOTFS}${sysconfdir}/systemd/journald.conf
        fi
        # link /var/log -> /run/log
        if [ -e ${IMAGE_ROOTFS}${localstatedir}/log ]; then
             rm -rf ${IMAGE_ROOTFS}${localstatedir}/log
        fi
        ln -sf ../run/log ${IMAGE_ROOTFS}${localstatedir}/log
    fi
}

