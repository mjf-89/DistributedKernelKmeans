#ifndef DKK_ITERATOR_H
#define DKK_ITERATOR_H

#include "WindowsExport.h"
#include "Unit.h"
#include "Array2D.h"
#include "DistributedArray2D.h"

namespace DKK{

class DKK_EXPORT Iterator : public Unit{
public:
	const std::string &getType(){
		static std::string type = "ITERATOR";
		return type;
	}

	virtual void prepare(const DistributedArray2D<float> &K, const Array2D<int> &labels)=0;
	virtual void update(const DistributedArray2D<float> &K, const Array2D<int> &labels)=0;
	virtual int reassign(const DistributedArray2D<float> &K, DistributedArray2D<int> &labels)=0;
};

}

#endif
