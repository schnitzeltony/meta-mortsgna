# glibc:
PNBLACKLIST[libiconv] = "the glibc provided iconv library is used"

TARGET_OS = "linux"

# sane-toolchain.inc has this, and explains the GNU and ABI suffices:
#TARGET_OS = "${@compute_os_portion_of_target_triplet(d)}"

TARGET_OS .= "${@['','-gnueabi'][d.getVar('TARGET_ARCH') in ['arm', 'armeb']]}"
TARGET_OS .= "${@['','-gnuspe'][d.getVar('TUNE_PKGARCH') in ['ppce500', 'ppce500v2']]}"
