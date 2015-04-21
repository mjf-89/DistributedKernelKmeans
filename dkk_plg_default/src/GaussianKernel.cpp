#include <math.h>

#include "GaussianKernel.h"
#include "DstPrimitive.h"

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
		"DstPrimitive"
	};
	static std::vector<std::string> reqPrimitiveNames(reqPrimitiveNamesA, reqPrimitiveNamesA+1);

	return reqPrimitiveNames;
}

void GaussianKernel::init()
{
	float sigma;
	getParameter("SIGMA", sigma);
	k = 0.5 / sigma / sigma;
}

void GaussianKernel::compute(Array2D<float> &dataset, DistributedArray2D<float> &K)
{
	DstPrimitive &dst = (DstPrimitive&) getWorker().getPrimitive("DstPrimitive");

	dst.setDataArray(dataset);
	dst.setDstArray(K);
	dst.execute();

	for(int i=0; i<K.rows(); i++)
		for(int j=0; j<K.cols(); j++)
			K.idx(i,j) =  exp(-K.idx(i,j)*k);

	return;
}

float GaussianKernel::dst(float *va, float *vb, int d)
{
	float dst = 0.0;
	for (int i = 0; i < d; i++)
		dst += (va[i] - vb[i])*(va[i] - vb[i]);

	return dst;
}

}
