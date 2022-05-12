SUMMARY = "W3C XML schema to C++ data binding compiler"
HOMEPAGE = "https://www.codesynthesis.com/products/xsd/"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://xsd/LICENSE;md5=79e31466c4d9f3a85f2f987c11ebcd83"

BBCLASSEXTEND = "native"

DEPENDS = "xerces-c"

SRC_URI = "https://codesynthesis.com/~boris/tmp/xsd/${PV}.a11/xsd-${PV}.a11+dep.tar.bz2"
SRC_URI[sha256sum] = "4fbe2d1e17ad4451bb3a9d9101ac89f7b465205470f1c7ad5e2c1386ac2c87d2"

S = "${WORKDIR}/xsd-${PV}.a11+dep"

do_install() {
    install -d ${D}${includedir}
    install -m 0755 -D ${S}/xsd/xsd/xsd ${D}${bindir}/xsdcxx
    cp -r ${S}/xsd/libxsd/xsd ${D}${includedir}/xsd
}

