# we want xf86-input-libinput and some BSP layers exceed their allowed limits :(
XSERVER_remove = " xf86-input-evdev "

# at least for test add xserver's standard xf86-video-modesetting
XSERVER_append = " xf86-video-modesetting"
