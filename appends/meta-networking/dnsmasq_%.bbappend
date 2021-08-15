# In our environment dnsmasq is used by networkmanager only. That takes care
# of all the configuration parameters necessary. Unfortunately this is broken
# by dnsmasq running as service: It is set-up by configuration files and that
# are not matching networkmanagers's needs.
SYSTEMD_AUTO_ENABLE:${PN} = "disable"
