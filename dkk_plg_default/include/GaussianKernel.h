#ifndef DKK_GAUSSIANKERNEL_H
#define DKK_GAUSSIANKERNEL_H

#include "Types.h"
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

	void compute(Array2D<DKK_TYPE_REAL> &dataset, DistributedArray2D<DKK_TYPE_REAL> &K);
	void compute(Array2D<DKK_TYPE_REAL> &data_rows, Array2D<DKK_TYPE_REAL> &data_cols, DistributedArray2D<DKK_TYPE_REAL> &K);
	void computeDiagonal(Array2D<DKK_TYPE_REAL> &dataset, DKK_TYPE_REAL *K_diag);
private:
	DKK_TYPE_REAL sigma;
};

}

#endif
