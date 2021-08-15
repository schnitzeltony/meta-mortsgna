do_install:append() {
    sed -i 's|root::0:0:root:/home/root:/bin/sh|root::0:0:root:/home/root:/bin/bash|' ${D}${datadir}/${BPN}/passwd.master
}
