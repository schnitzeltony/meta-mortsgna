DESCRIPTION = "This recipe adds a user 'morona' without password"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit useradd

USERNAME = "morona"
# groups user shall be member of (NO WHITESPACES ALLOWED)
USERGROUPS = "audio,video,${@bb.utils.contains('BBFILE_COLLECTIONS', 'openembedded-layer', 'datetime,network,', '', d)}systemd-journal"

# all those we are member of
USER_DEPS ?= " \
    systemd \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'openembedded-layer', 'polkit-group-rule-datetime polkit-group-rule-network', '', d)} \
    \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'meta-qt5-extra', 'audio-tweaks', '', d)} \
"

DEPENDS += "${USER_DEPS}"
RDEPENDS_${PN} = "${USER_DEPS} bash"

USERADD_PACKAGES = "${PN}"

USERADD_PARAM_${PN} = "-m -c Morona -d /home/${USERNAME} -s /bin/bash -k /etc/skel -g ${USERNAME} --groups ${USERGROUPS} ${USERNAME}"

GROUPADD_PARAM_${PN} = "${USERNAME}"

pkg_postinst_ontarget_${PN}() {
# empty password
passwd -d ${USERNAME}
}

ALLOW_EMPTY_${PN} = "1"
