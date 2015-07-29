#ifndef DKK_GAUSSIANEUCLIDEANPRIMITIVE_H
#define DKK_GAUSSIANEUBLIDEANPRIMITIVE_H

#include "PluginDefaultWindowsExport.h"

#include "Types.h"
#include "Worker.h"
#include "Primitive.h"
#include "DistributedArray2D.h"

namespace DKK{

class GaussianEuclideanPrimitive : public Primitive{
public:	
	const std::string &getName()
	{
		static std::string name = "GaussianEuclideanPrimitive";
		return name;
	}

	void setDataRows(Array2D<DKK_TYPE_REAL> &data) 
	{
		this->data_rows = &data;
	}

	void setDataCols(Array2D<DKK_TYPE_REAL> &data) 
	{
		this->data_cols = &data;
	}

	void setKernelArray(DistributedArray2D<DKK_TYPE_REAL> &K)
	{
		this->K = &K;
	}

	void setSigma(DKK_TYPE_REAL sigma)
	{
		this->sigma = sigma;
		sigma_k = 0.5/sigma/sigma;
	}

protected:
	Array2D<DKK_TYPE_REAL> *data_rows, *data_cols;
	DistributedArray2D<DKK_TYPE_REAL> *K;
	DKK_TYPE_REAL sigma, sigma_k;
};

}

#endif
