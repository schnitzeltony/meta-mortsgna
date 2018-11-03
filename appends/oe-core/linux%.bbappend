kernel_do_deploy_append() {
    # deploy an extra link containing abiversion - used by our
    # card-kernel-write scripts
    for imageType in ${KERNEL_IMAGETYPES} ; do

        # thud: KERNEL_IMAGE_NAME
        if [ ! "x${KERNEL_IMAGE_NAME}" = "x" ]; then
            base_name=${imageType}-${KERNEL_IMAGE_NAME}

        # sumo and earlier: KERNEL_IMAGE_BASE_NAME
        else
            base_name=${imageType}-${KERNEL_IMAGE_BASE_NAME}
        fi

        if [ -e $deployDir/${base_name}.bin ] ; then
            abilinkname=${imageType}-abiversion-${KERNEL_VERSION}
	        ln -sf ${base_name}.bin $deployDir/${abilinkname}
        fi
    done
}
