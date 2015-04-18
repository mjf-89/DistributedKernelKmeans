#ifndef DKK_CUDADSTPRIMITIVE_H
#define DKK_CUDADSTPRIMITIVE_H

#include "PluginCudaWindowsExport.h"
#include "DstPrimitive.h"
#include "Array2D.h"

namespace DKK{

class DKK_PLUGIN_CUDA_EXPORT CUDADstPrimitive : public DstPrimitive{
public:	
	~CUDADstPrimitive();
	void init();
	void execute();
private:
	float *d_data, *d_dst;
};

}

#endif