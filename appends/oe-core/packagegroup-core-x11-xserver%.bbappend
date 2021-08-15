# we want xf86-input-libinput and some BSP layers exceed their allowed limits :(
XSERVER:remove = " xf86-input-evdev "

# at least for test add xserver's standard xf86-video-modesetting
XSERVER:append = " xf86-video-modesetting"
