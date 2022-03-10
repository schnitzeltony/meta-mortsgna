EXTRA_OECMAKE += "-DBUILD_SHARED_LIBS=ON"

RDEPENDS:${PN}-dev:remove = "${PN}-staticdev"
