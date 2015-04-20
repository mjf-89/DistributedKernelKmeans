#ifndef DKK_CUDAWORKER_H
#define DKK_CUDAWORKER_H

#include "PluginCudaWindowsExport.h"
#include "Worker.h"

namespace DKK{

class DKK_PLUGIN_CUDA_EXPORT CUDAWorker : public Worker{
public:
	const std::string &getName();

	void init();
private:
};

}

#endif
