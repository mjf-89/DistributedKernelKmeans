#ifndef DKK_COMMUNICATOR_H
#define DKK_COMMUNICATOR_H

#include <mpi.h>
#include <string>
#include <iostream>

#include "Types.h"
#include "WindowsExport.h"

namespace DKK{

template<class T> class Array2D;
template<class T> class DistributedArray2D;

class DKK_EXPORT Communicator{
public:
	Communicator(int *argc, char ***argv);
	~Communicator();

	inline const int &getRank() const {return rank;}
	inline const int &getSize() const {return size;}

	void allgather(DistributedArray2D<DKK_TYPE_REAL> &src, Array2D<DKK_TYPE_REAL> &dst);
	void allgather(DistributedArray2D<DKK_TYPE_INT> &src, Array2D<DKK_TYPE_INT> &dst);

	void allreducesum(DKK_TYPE_REAL &val);
	void allreducesum(Array2D<DKK_TYPE_REAL> &arr);
	void allreducesum(DKK_TYPE_INT &val);
	void allreducesum(Array2D<DKK_TYPE_INT> &arr);

private:
	int rank, size;
	int *cnts, *dsps;
};

}

#endif
