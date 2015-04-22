#ifndef DKK_ITERATOR_H
#define DKK_ITERATOR_H

#include "Types.h"
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

	virtual void prepare(const DistributedArray2D<DKK_TYPE_REAL> &K, const DistributedArray2D<DKK_TYPE_INT> &labels)=0;
	virtual void update(const DistributedArray2D<DKK_TYPE_REAL> &K, const Array2D<DKK_TYPE_INT> &labels)=0;
	virtual int reassign(const DistributedArray2D<DKK_TYPE_REAL> &K, DistributedArray2D<DKK_TYPE_INT> &labels)=0;
	virtual DKK_TYPE_REAL cost(const DistributedArray2D<DKK_TYPE_REAL> &K, const DistributedArray2D<DKK_TYPE_INT> &labels)=0;
};

}

#endif
