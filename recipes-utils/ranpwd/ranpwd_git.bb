DESCRIPTION = "Random password generator"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://ranpwd.c;beginline=1;endline=11;md5=0e8585e19117526efedfaeb50c345d7a"
SECTION = "console/utils"

PV = "1.2+git${SRCPV}"

inherit autotools

SRC_URI = "git://github.com/koenkooi/ranpwd.git;protocol=https;branch=master \
           file://ranpwd_confgure.patch \
          "

SRCREV = "b62aab579e288715b82d5575befaa2b8ff210c2b"

S="${WORKDIR}/git"

do_configure:prepend () {
	( cd ${S} 
	touch NEWS README AUTHORS ChangeLog
	if [ ! -e acinclude.m4 -a -e aclocal.m4 ]; then
		cp aclocal.m4 acinclude.m4
	fi )
}

do_install() {
	install -d ${D}/${bindir}
	install -m 0755 ${B}/ranpwd ${D}/${bindir}/ranpwd
}
