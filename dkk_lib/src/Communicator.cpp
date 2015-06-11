#include <stdlib.h>
#include "Communicator.h"
#include "DistributedArray2D.h"

namespace DKK{
Communicator::Communicator(int *argc, char*** argv)
{
	MPI_Init(argc, argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	dsps = (int*)malloc(sizeof(int)*size);
	cnts = (int*)malloc(sizeof(int)*size);
}

Communicator::~Communicator()
{	
	free(dsps);
	free(cnts);


	MPI_Finalize();
}

void Communicator::allgather(DistributedArray2D<float> &src, Array2D<float> &dst)
{
	for(int i=0; i<size; i++){
		cnts[i] = (int)(src.grows()/size)+(i<src.grows()%size?1:0);
		cnts[i] *= src.cols();
		dsps[i] = (i>0)?dsps[i-1]+cnts[i-1]:0;  
	}
	MPI_Allgatherv(src.bff(0), src.length(), MPI_FLOAT, dst.bff(0), cnts, dsps, MPI_FLOAT, MPI_COMM_WORLD);
}

void Communicator::allgather(DistributedArray2D<double> &src, Array2D<double> &dst)
{
	for(int i=0; i<size; i++){
		cnts[i] = (int)(src.grows()/size)+(i<src.grows()%size?1:0);
		cnts[i] *= src.cols();
		dsps[i] = (i>0)?dsps[i-1]+cnts[i-1]:0;  
	}
	MPI_Allgatherv(src.bff(0), src.length(), MPI_DOUBLE, dst.bff(0), cnts, dsps, MPI_DOUBLE, MPI_COMM_WORLD);
}

void Communicator::allgather(DistributedArray2D<DKK_TYPE_INT> &src, Array2D<DKK_TYPE_INT> &dst)
{
	for(int i=0; i<size; i++){
		cnts[i] = (int)(src.grows()/size)+(i<src.grows()%size?1:0);
		cnts[i] *= src.cols();
		dsps[i] = (i>0)?dsps[i-1]+cnts[i-1]:0;  
	}
	MPI_Allgatherv(src.bff(0), src.length(), DKK_TYPE_MPI_INT, dst.bff(0), cnts, dsps, DKK_TYPE_MPI_INT, MPI_COMM_WORLD);
}

void Communicator::allreducesum(float &val)
{
	MPI_Allreduce(MPI_IN_PLACE, &val, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
}

void Communicator::allreducesum(double &val)
{
	MPI_Allreduce(MPI_IN_PLACE, &val, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
}

void Communicator::allreducesum(Array2D<float> &arr)
{
	MPI_Allreduce(MPI_IN_PLACE, arr.bff(0), arr.length(), MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
}

void Communicator::allreducesum(Array2D<double> &arr)
{
	MPI_Allreduce(MPI_IN_PLACE, arr.bff(0), arr.length(), MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
}

void Communicator::allreducesum(DKK_TYPE_INT &val)
{
	MPI_Allreduce(MPI_IN_PLACE, &val, 1, DKK_TYPE_MPI_INT, MPI_SUM, MPI_COMM_WORLD);
}

void Communicator::allreducesum(Array2D<DKK_TYPE_INT> &arr)
{
	MPI_Allreduce(MPI_IN_PLACE, arr.bff(0), arr.length(), DKK_TYPE_MPI_INT, MPI_SUM, MPI_COMM_WORLD);
}

}
