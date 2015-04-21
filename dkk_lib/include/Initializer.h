#ifndef DKK_INITIALIZER_H
#define DKK_INITIALIZER_H

#include "WindowsExport.h"
#include "Unit.h"
#include "Array2D.h"
#include "DistributedArray2D.h"

namespace DKK{

class DKK_EXPORT Initializer : public Unit{
public:
	const std::string &getType(){
		static std::string type = "INITIALIZER";
		return type;
	}

	virtual void label(Array2D<float> &data, DistributedArray2D<float> &K, DistributedArray2D<int> &labels)=0;
};

}

#endif
