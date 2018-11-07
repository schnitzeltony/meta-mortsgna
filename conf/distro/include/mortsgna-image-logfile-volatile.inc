# override bitbake.conf default
VOLATILE_LOG_DIR = "false"

IMAGE_FEATURES[validitems] += "volatile-log"

# Do alignment on journal and /var/log
ROOTFS_POSTPROCESS_COMMAND += "postinst_volatile_log; "

postinst_volatile_log () {
    if [ "${@bb.utils.contains_any('IMAGE_FEATURES', 'volatile-log read-only-rootfs', '1', '0', d)}" = "1" ]; then
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

