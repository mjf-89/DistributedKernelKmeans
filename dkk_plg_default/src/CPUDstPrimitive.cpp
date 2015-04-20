#include "CPUDstPrimitive.h"

namespace DKK{
void CPUDstPrimitive::init()
{
}

void CPUDstPrimitive::execute()
{
	int gi, gj;
	float d;
	for (int i = 0; i < dst->rows(); i++){
		for (int j = 0; j < dst->cols(); j++){
			dst->ltgIdx(i, j, gi, gj);
			d=0;
			for(int k=0; k<data->cols(); k++)
				d += (data->idx(gi,k)-data->idx(gj,k))*(data->idx(gi,k)-data->idx(gj,k));
			dst->idx(i, j) = d;
		}
	}
	
}

CPUDstPrimitive::~CPUDstPrimitive()
{
}

}

