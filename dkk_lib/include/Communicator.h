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

	void bcast(DKK_TYPE_INT &val, int root);
	void bcast(float &val, int root);
	void bcast(double &val, int root);

	void allgather(DistributedArray2D<float> &src, Array2D<float> &dst);
	void allgather(DistributedArray2D<double> &src, Array2D<double> &dst);
	void allgather(DistributedArray2D<DKK_TYPE_INT> &src, Array2D<DKK_TYPE_INT> &dst);

	void allreducemax(float &val);
	void allreducemax(double &val);

	void allreducesum(float &val);
	void allreducesum(double &val);
	void allreducesum(Array2D<float> &arr);
	void allreducesum(Array2D<double> &arr);
	void allreducesum(DKK_TYPE_INT &val);
	void allreducesum(Array2D<DKK_TYPE_INT> &arr);

	void allreduceminloc(Array2D<DKK_TYPE_REAL_INT> &arr);

	double wtime();
private:
	int rank, size;
	int *cnts, *dsps;
};

}

#endif
