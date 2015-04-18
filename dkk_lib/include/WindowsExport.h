#ifndef DKK_WINDOWSEXPORT_H
#define DKK_WINDOWSEXPORT_H

#ifdef WIN32
	#ifdef DKK_BUILDING_LIB
		#define DKK_EXPORT _declspec(dllexport)
	#else
		#define DKK_EXPORT _declspec(dllimport)
	#endif
#else
	#define DKK_EXPORT
#endif

#endif