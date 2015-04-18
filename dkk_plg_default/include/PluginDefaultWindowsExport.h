#ifndef DKK_PLUGINDEFAULTWINDOWSEXPORT_H
#define DKK_PLUGINDEFAULTWINDOWSEXPORT_H

#ifdef WIN32
	#ifdef DKK_BUILDING_PLUGIN_DEFAULT
		#define DKK_PLUGIN_DEFAULT_EXPORT _declspec(dllexport)
	#else
		#define DKK_PLUGIN_DEFAULT_EXPORT _declspec(dllimport)
	#endif
#else
#define DKK_PLUGIN_DEFAULT_EXPORT
#endif

#endif