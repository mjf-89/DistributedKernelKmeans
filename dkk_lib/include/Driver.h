#ifndef DKK_DRIVER_H
#define DKK_DRIVER_H

#include "Types.h"
#include "WindowsExport.h"
#include "Unit.h"
#include "Array2D.h"
#include "DistributedArray2D.h"

namespace DKK{

class DKK_EXPORT Driver : public Unit{
public:
	const std::string &getType(){
		static std::string type = "DRIVER";
		return type;
	}

	virtual void execute(Reader &reader, Kernel &kernel, Initializer &initializer, Iterator &iterator) = 0;
};

}

#endif
