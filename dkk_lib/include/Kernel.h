#ifndef DKK_KERNEL_H
#define DKK_KERNEL_H

#include "WindowsExport.h"
#include "Unit.h"
#include "DistributedArray2D.h"

namespace DKK{

class DKK_EXPORT Kernel : public Unit{
public:
	const std::string &getType(){
		static std::string type = "KERNEL";
		return type;
	}

	virtual void compute(Array2D<float> &dataset, DistributedArray2D<float> &K)=0;
};

}

#endif