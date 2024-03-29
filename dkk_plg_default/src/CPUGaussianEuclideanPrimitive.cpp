#include <math.h>
#include "CPUGaussianEuclideanPrimitive.h"

namespace DKK{
void CPUGaussianEuclideanPrimitive::init()
{
}

void CPUGaussianEuclideanPrimitive::execute()
{
	int gi, gj;
	float d;
	for (int i = 0; i < K->rows(); i++){
		for (int j = 0; j < K->cols(); j++){
			K->ltgIdx(i, j, gi, gj);
			d=0;
			for(int k=0; k<data_rows->cols(); k++)
				d += (data_rows->idx(gi,k)-data_cols->idx(gj,k))*(data_rows->idx(gi,k)-data_cols->idx(gj,k));
			K->idx(i, j) = exp(-d*sigma_k);
		}
	}
}

CPUGaussianEuclideanPrimitive::~CPUGaussianEuclideanPrimitive()
{
}

}
