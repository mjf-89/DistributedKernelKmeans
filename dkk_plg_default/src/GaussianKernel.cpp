#include <math.h>
#include "GaussianKernel.h"
#include "GaussianEuclideanPrimitive.h"

namespace DKK{
const std::string &GaussianKernel::getName()
{
	static std::string name = "GaussianKernel";
	return name;
}

const std::vector<std::string> &GaussianKernel::getReqPrmNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrmNamesA[] = {
		"SIGMA"
	};
	static std::vector<std::string> reqPrmNames(reqPrmNamesA, reqPrmNamesA+1);

	return reqPrmNames;
}

const std::vector<std::string> &GaussianKernel::getReqPrimitiveNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrimitiveNamesA[] = {
		"GaussianEuclideanPrimitive"
	};
	static std::vector<std::string> reqPrimitiveNames(reqPrimitiveNamesA, reqPrimitiveNamesA+1);

	return reqPrimitiveNames;
}

void GaussianKernel::init()
{
	getParameter("SIGMA", sigma);
}

void GaussianKernel::compute(Array2D<DKK_TYPE_REAL> &dataset, DistributedArray2D<DKK_TYPE_REAL> &K)
{
	GaussianEuclideanPrimitive &ge = (GaussianEuclideanPrimitive&) getWorker().getPrimitive("GaussianEuclideanPrimitive");

	ge.setDataRows(dataset);
	ge.setDataCols(dataset);
	ge.setKernelArray(K);
	ge.setSigma(sigma);

	ge.execute();

	return;
}

void GaussianKernel::compute(Array2D<DKK_TYPE_REAL> &data_rows, Array2D<DKK_TYPE_REAL> &data_cols, DistributedArray2D<DKK_TYPE_REAL> &K)
{
	GaussianEuclideanPrimitive &ge = (GaussianEuclideanPrimitive&) getWorker().getPrimitive("GaussianEuclideanPrimitive");

	ge.setDataRows(data_rows);
	ge.setDataCols(data_cols);
	ge.setKernelArray(K);
	ge.setSigma(sigma);

	ge.execute();

	return;
}

void GaussianKernel::computeDiagonal(Array2D<DKK_TYPE_REAL> &dataset, DKK_TYPE_REAL *K_diag)
{
	for (int i = 0; i < dataset.rows(); i++)
		K_diag[i] = 1.0;
}

}
