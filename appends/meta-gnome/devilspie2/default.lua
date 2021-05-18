-- Copyright (c) 2018 Andreas Müller <schnitzeltony@gmail.com>
--
-- this is an example 
-- * maximizing and undecorating all appplication's windows in apps_list
-- for further information see
-- http://git.savannah.gnu.org/cgit/devilspie2.git/tree/README

wnd_type = get_window_type()

if(wnd_type == "WINDOW_TYPE_NORMAL") then

	-- add only applications you want maximized+undecorated and
	-- which don't keep maximized state
	apps_list = 
	{
		"Terminal",
		"ristretto",
		"xarchiver",
		"Thunar",
		"geany",
	}

	app_name = get_application_name()

	-- to have some informational output, start devilspie2 with --debug 
	-- option and uncomment the following lines:

	--debug_print ("Window Name: " .. get_window_name())
	--debug_print ("Application name: " .. app_name)
	--debug_print ("window-type: " .. wnd_type)

	-- maximize/undecorate all windows in apps_list
	-- (unfortunately for some also their settings)
	for line, str in ipairs(apps_list) do
		if (string.find(app_name, str)) then
			maximize()
			undecorate_window()
			break
		end
	end
end
