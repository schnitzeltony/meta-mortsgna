# Problem with copying /etc/skel to home forder by useradd only is that
# user-recipe must RDEPEND on all recipes installing files to /etc/skel.
# Otherwise content found in home folder is unpredictable.
#
# To get around, this class was created. It is dual role and can be inherited
# by:
#
# * recipes creating a user whose home folder shall be populated by /etc/skel.
#   The only action performed is dropping a file
#   /home/${USERNAME}/${SKEL_INIT_MARKER} to mark the folder for later
#   population.
# * images which get a postprocess command copying /etc/skel for those home
#   folders marked.

SKEL_INIT_MARKER = "force-skel-full-init"

# user recipe part
pkg_postinst_${PN}_prepend() {
    if [ -n "$D" -a -n "${USERNAME}" ]; then
        touch $D/home/${USERNAME}/${SKEL_INIT_MARKER}
    fi
}

do_package[vardeps] += "USERNAME"

# image recipe part
ROOTFS_POSTPROCESS_COMMAND += "postinst_copy_skel; "

postinst_copy_skel () {
    if [ -d "${IMAGE_ROOTFS}/home" -a -d "${IMAGE_ROOTFS}/${sysconfdir}/skel" ]; then
        for home_target in `find "${IMAGE_ROOTFS}/home" -name ${SKEL_INIT_MARKER}`; do
            homedir=`dirname "$home_target"`
            echo "Copying ${IMAGE_ROOTFS}${sysconfdir}/skel to $homedir..."
            user=`basename "$homedir"`
            # 1. copy -> /home/user/skel
            cp -rf  --preserve=mode,ownership,timestamps,links ${IMAGE_ROOTFS}${sysconfdir}/skel "$homedir"
            chown -R "$user:$user" "$homedir/skel"
            # 2. copy -> /home/user - TBD mv?
            cp -rfT  --preserve=mode,ownership,timestamps,links "$homedir/skel" "$homedir"
            # 3. remove /home/user/skel
            rm -rf "$homedir/skel"
            # remove marker
            rm "$home_target"
        done
    fi
}
