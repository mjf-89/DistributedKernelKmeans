#ifndef DKK_WORKER_H
#define DKK_WORKER_H

#include <map>
#include <string>

#include "WindowsExport.h"

namespace DKK{

class Primitive;

class DKK_EXPORT Worker{
private:
	std::map<std::string, Primitive *> primitives;

public:
	virtual const std::string &getName()=0;

	void registerPrimitive(Primitive *primitive);
	Primitive &getPrimitive(const std::string &name);
};

}


#endif