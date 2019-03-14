#
# Writes build information to target filesystem on /etc/buildinfo
#
# Copyright (C) 2014 Intel Corporation
# Author: Alejandro Enedino Hernandez Samaniego <alejandro.hernandez@intel.com>
#
# Licensed under the MIT license, see COPYING.MIT for details
#
# Usage: add INHERIT += "image-buildinfo_mortsgna-mortsgna" to your conf file
#
# This is a slightly reworked version oe-core's version. Patch was sent [1] 
# but...
# [1] http://lists.openembedded.org/pipermail/openembedded-core/2018-September/274747.html

# Desired variables to display 
IMAGE_BUILDINFO_MORTSGNA_VARS ?= "DISTRO DISTRO_VERSION"

# Desired location of the output file in the image.
IMAGE_BUILDINFO_MORTSGNA_FILE ??= "${sysconfdir}/buildinfo"

# Gets git branch's status (clean or dirty)
def get_layer_mortsgna_git_status(path):
    import subprocess
    try:
        subprocess.check_output("""cd %s; export PSEUDO_UNLOAD=1; set -e;
                                git diff --quiet --no-ext-diff
                                git diff --quiet --no-ext-diff --cached""" % path,
                                shell=True,
                                stderr=subprocess.STDOUT)
        return ""
    except subprocess.CalledProcessError as ex:
        # Silently treat errors as "modified", without checking for the
        # (expected) return code 1 in a modified git repo. For example, we get
        # output and a 129 return code when a layer isn't a git repo at all.
        return "-- modified"

# Returns layer revisions along with their respective status
def get_layer_mortsgna_revs(d):
    layers = (d.getVar("BBLAYERS") or "").split()
    medadata_revs = ["%-17s = %s:%s %s" % (os.path.basename(i), \
        base_get_metadata_git_branch(i, None).strip(), \
        base_get_metadata_git_revision(i, None), \
        get_layer_mortsgna_git_status(i)) \
            for i in layers]
    return '\n'.join(medadata_revs)

def buildinfo_mortsgna_target(d):
        # Get context
        if d.getVar('BB_WORKERCONTEXT') != '1':
                return ""
        # taken from base.bbclass
        localdata = bb.data.createCopy(d)
        statuslines = []
        g = globals()
        func = 'buildcfg_vars'
        if func not in g:
            bb.warn("Build configuration function '%s' does not exist" % func)
        else:
            flines = g[func](localdata)
            if flines:
                statuslines.extend(flines)
        return ('\n%s\n' % '\n'.join(statuslines))

# Write build information to target filesystem
python buildinfo_mortsgna () {
    if not d.getVar('IMAGE_BUILDINFO_MORTSGNA_FILE'):
        return
    with open(d.expand('${IMAGE_ROOTFS}${IMAGE_BUILDINFO_MORTSGNA_FILE}'), 'w') as build:
        build.writelines((
            '''-----------------------
Build Configuration:  |
-----------------------
''',
            buildinfo_mortsgna_target(d),
            '''
-----------------------
Layer Revisions:      |
-----------------------
''',
            get_layer_mortsgna_revs(d),
            '''
'''
       ))
}

IMAGE_PREPROCESS_COMMAND += "buildinfo_mortsgna;"
