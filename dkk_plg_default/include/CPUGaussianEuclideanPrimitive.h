#ifndef DKK_CPUGAUSSIANEUCLIDEANPRIMITIVE_H
#define DKK_CPUGAUSSIANEUCLIDEANPRIMITIVE_H

#include "PluginDefaultWindowsExport.h"
#include "GaussianEuclideanPrimitive.h"
#include "Array2D.h"

namespace DKK{

class DKK_PLUGIN_DEFAULT_EXPORT CPUGaussianEuclideanPrimitive : public GaussianEuclideanPrimitive{
public:	
	~CPUGaussianEuclideanPrimitive();
	void init();
	void execute();
private:
};

}

#endif
