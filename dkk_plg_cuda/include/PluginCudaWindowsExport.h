#ifndef DKK_PLUGINCUDAWINDOWSEXPORT_H
#define DKK_PLUGINCUDAWINDOWSEXPORT_H

#ifdef WIN32
	#ifdef DKK_BUILDING_PLUGIN_CUDA
		#define DKK_PLUGIN_CUDA_EXPORT _declspec(dllexport)
	#else
		#define DKK_PLUGIN_CUDA_EXPORT _declspec(dllimport)
	#endif
#else
#define DKK_PLUGIN_CUDA_EXPORT
#endif

#endif