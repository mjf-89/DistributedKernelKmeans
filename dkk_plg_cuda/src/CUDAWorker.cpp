#include "CUDAWorker.h"

namespace DKK{

const std::string &CUDAWorker::getName()
{
	static std::string name = "CUDAWorker";
	return name;
}

void CUDAWorker::init()
{
}

}