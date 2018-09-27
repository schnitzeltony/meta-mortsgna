kernel_do_deploy_append() {
    # deploy an extra link containing abiversion - used by our
    # card-kernel-write scripts
    abilinkname=${type}-abiversion-${KERNEL_VERSION}
    if [ -e $deployDir/${base_name}.bin ] ; then
		ln -sf ${base_name}.bin $deployDir/${abilinkname}
    fi
}
