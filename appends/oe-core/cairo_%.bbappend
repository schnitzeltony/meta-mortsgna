# It is time I do suggest common / global PREFERRED_OPENGL variable

PACKAGECONFIG_remove_class-target = "opengl"

PACKAGECONFIG_append_class-target = " egl glesv2"
