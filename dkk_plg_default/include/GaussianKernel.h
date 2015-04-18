#ifndef DKK_GAUSSIANKERNEL_H
#define DKK_GAUSSIANKERNEL_H

#include "PluginDefaultWindowsExport.h"
#include "Kernel.h"
#include "DistributedArray2D.h"

namespace DKK{

class DKK_PLUGIN_DEFAULT_EXPORT GaussianKernel : public Kernel{
public:
	const std::string &getName();
	const std::vector<std::string> &getReqPrmNames();
	const std::vector<std::string> &getReqPrimitiveNames();

	void init();

	void compute(Array2D<float> &dataset, DistributedArray2D<float> &K);
private:
	float k;

	float dst(float *va, float *vb, int d);
};

}

#endif