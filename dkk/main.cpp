#include <iostream>
#include <string>

#include "Types.h"
#include "Configurator.h"
#include "Worker.h"
#include "Primitive.h"
#include "Array2D.h"
#include "DistributedArray2D.h"
#include "Communicator.h"

using namespace DKK;

int main(int argc, char** argv)
{
	//read configuration file
	Configurator conf(&argc, &argv);
	conf.loadPlugins();

	Communicator &comm = conf.getCommunicator();

	Array2D<DKK_TYPE_REAL> *data;
	DistributedArray2D<DKK_TYPE_REAL> *K;
	Array2D<DKK_TYPE_INT> *labels_;
	DistributedArray2D<DKK_TYPE_INT> *labels;

	//get algorithm units
	Reader &reader = conf.getReader();
	Kernel &kernel = conf.getKernel();
	Initializer &initializer = conf.getInitializer();
	Iterator &iterator = conf.getIterator();

	//load dataset
	data = new Array2D<DKK_TYPE_REAL>(reader.getLength(), reader.getDimensionality());
	for (int i = 0; i < data->rows(); i++)
		reader.read(i, data->bff(i));
	
	//compute the kernel
	K = new DistributedArray2D<DKK_TYPE_REAL>(comm, data->rows(), data->rows());
	kernel.compute(*data, *K);

	//init the labels
	labels = new DistributedArray2D<DKK_TYPE_INT>(comm, data->rows());
	labels_ = new Array2D<DKK_TYPE_INT>(data->rows());
	initializer.label(*data, *K, *labels);
	comm.allgather(*labels, *labels_);

	iterator.prepare(*K, *labels);
	int notconverge=0;
	do{
		comm.allgather(*labels, *labels_);
		iterator.update(*K, *labels_);
		notconverge=iterator.reassign(*K, *labels);

		if(!comm.getRank()) std::cout<<notconverge<<"\n";
	}while(notconverge);


	delete data;
	delete K;
	delete labels;
	delete labels_;

	return 0;
}
