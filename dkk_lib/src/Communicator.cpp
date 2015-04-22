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

void Communicator::allgather(DistributedArray2D<DKK_TYPE_REAL> &src, Array2D<DKK_TYPE_REAL> &dst)
{
	for(int i=0; i<size; i++){
		cnts[i] = (int)(src.grows()/size)+(i<src.grows()%size?1:0);
		cnts[i] *= src.cols();
		dsps[i] = (i>0)?dsps[i-1]+cnts[i-1]:0;  
	}
	MPI_Allgatherv(src.bff(0), src.length(), DKK_TYPE_MPI_REAL, dst.bff(0), cnts, dsps, DKK_TYPE_MPI_REAL, MPI_COMM_WORLD);
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

void Communicator::allreducesum(DKK_TYPE_REAL &val)
{
	MPI_Allreduce(MPI_IN_PLACE, &val, 1, DKK_TYPE_MPI_REAL, MPI_SUM, MPI_COMM_WORLD);
}

void Communicator::allreducesum(Array2D<DKK_TYPE_REAL> &arr)
{
	MPI_Allreduce(MPI_IN_PLACE, arr.bff(0), arr.length(), DKK_TYPE_MPI_REAL, MPI_SUM, MPI_COMM_WORLD);
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
