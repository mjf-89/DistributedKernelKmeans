#ifndef DKK_COMMUNICATOR_H
#define DKK_COMMUNICATOR_H

#include <mpi.h>
#include <string>
#include <iostream>

#include "WindowsExport.h"

#include "Array2D.h"
#include "DistributedArray2D.h"

namespace DKK{

class DKK_EXPORT Communicator{
public:
	Communicator(int *argc, char ***argv);
	~Communicator();

	inline const int &getRank() const {return rank;}
	inline const int &getSize() const {return size;}

	void allgather(DistributedArray2D<float> &src, Array2D<float> &dst);
private:
	int rank, size;
	int *cnts, *dsps;
};

}

#endif