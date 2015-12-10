#ifndef DKK_PLUGINFERMIWINDOWSEXPORT_H
#define DKK_PLUGINFERMIWINDOWSEXPORT_H

#ifdef WIN32
	#ifdef DKK_BUILDING_PLUGIN_FERMI
		#define DKK_PLUGIN_FERMI_EXPORT _declspec(dllexport)
	#else
		#define DKK_PLUGIN_FERMI_EXPORT _declspec(dllimport)
	#endif
#else
#define DKK_PLUGIN_FERMI_EXPORT
#endif

#endif
