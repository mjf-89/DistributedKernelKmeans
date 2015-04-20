#include "CPUWorker.h"

namespace DKK{

const std::string &CPUWorker::getName()
{
	static std::string name = "CPUWorker";
	return name;
}

void CPUWorker::init()
{
}

}
