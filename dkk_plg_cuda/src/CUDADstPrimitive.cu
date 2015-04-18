#include "cuda.h"
#include "cuda_runtime_api.h"

#include "CUDADstPrimitive.h"

__global__ void cuKernelDst(float *data, float *dst)
{
	extern __shared__ float s_dst[];

	int t_id = threadIdx.x;
	int dst_id = blockIdx.y * gridDim.x + blockIdx.x;
	int data_i = blockIdx.x * blockDim.x + t_id;
	int data_j = blockIdx.y * blockDim.x + t_id;
	
	//compute components distance on shared memory
	s_dst[t_id]= (data[data_i]-data[data_j])*(data[data_i]-data[data_j]);
	
	//binary-tree reduction of components distance
	__syncthreads();
	for(int i=1; i<blockDim.x; i*=2){
		if(t_id%(i*2)==0)
			s_dst[t_id] += s_dst[t_id+i];
		__syncthreads();
	}

	//output distance
	if(t_id==0) dst[dst_id] = s_dst[0];
}

namespace DKK{
void CUDADstPrimitive::init()
{
}

void CUDADstPrimitive::execute()
{
	dim3 blocks(data->rows(),data->rows(),1), threads(data->cols(),1,1);

	cudaMalloc((void**)&d_data, data->length()*sizeof(float));
	cudaMemcpy(d_data, data->bff(0), data->length()*sizeof(float),cudaMemcpyHostToDevice);

	cudaMalloc((void**)&d_dst, dst->length()*sizeof(float));

	cuKernelDst<<<blocks,threads,threads.x>>> (d_data, d_dst);

	cudaMemcpy(dst->bff(0), d_dst, dst->length()*sizeof(float),cudaMemcpyDeviceToHost);
}

CUDADstPrimitive::~CUDADstPrimitive()
{
	cudaFree(d_data);
}

}

