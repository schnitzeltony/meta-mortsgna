# It is time I do suggest common / global PREFERRED_OPENGL variable

PACKAGECONFIG:remove:class-target = "opengl"

PACKAGECONFIG:append:class-target = " egl glesv2"
