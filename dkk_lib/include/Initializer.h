#ifndef DKK_INITIALIZER_H
#define DKK_INITIALIZER_H

#include "Types.h"
#include "WindowsExport.h"
#include "Unit.h"
#include "Array2D.h"
#include "DistributedArray2D.h"
#include "Kernel.h"

namespace DKK{

class DKK_EXPORT Initializer : public Unit{
public:
	const std::string &getType(){
		static std::string type = "INITIALIZER";
		return type;
	}

	virtual void label(Array2D<DKK_TYPE_REAL> &data, DistributedArray2D<DKK_TYPE_REAL> &K, Kernel &kernel, Array2D<DKK_TYPE_REAL> &medoids)=0;
};

}

#endif
