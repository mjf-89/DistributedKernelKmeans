#include "PluginCudaWindowsExport.h"
#include "Configurator.h"

#include "CUDAWorker.h"
#include "CUDADstPrimitive.h"

extern "C" DKK_PLUGIN_CUDA_EXPORT void registerUnits()
{
	DKK::Worker *worker;

	worker = new DKK::CUDAWorker();
	worker->registerPrimitive(new DKK::CUDADstPrimitive());
	DKK::Configurator::registerWorker(worker);
	return;
}