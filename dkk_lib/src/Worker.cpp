#include "Worker.h"
#include "Primitive.h"

namespace DKK{

void Worker::registerPrimitive(Primitive *primitive)
{
	primitives.insert(std::pair<std::string, Primitive*>(primitive->getName(), primitive));
	primitive->setWorker(*this);
}

Primitive &Worker::getPrimitive(const std::string &name)
{
	if (primitives.find(name) != primitives.end())
		return *primitives.find(name)->second;

	throw 1;
}

}