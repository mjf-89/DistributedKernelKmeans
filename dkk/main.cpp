#include <iostream>
#include <string>

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

	Array2D<float> *data;
	DistributedArray2D<float> *K;
	Array2D<int> *labels_;
	DistributedArray2D<int> *labels;

	//get algorithm units
	Reader &reader = conf.getReader();
	Kernel &kernel = conf.getKernel();
	Initializer &initializer = conf.getInitializer();
	Iterator &iterator = conf.getIterator();

	//load dataset
	data = new Array2D<float>(reader.getLength(), reader.getDimensionality());
	for (int i = 0; i < data->rows(); i++)
		reader.read(i, data->bff(i));
	
	//compute the kernel
	K = new DistributedArray2D<float>(comm, data->rows(), data->rows());
	kernel.compute(*data, *K);

	//init the labels
	labels = new DistributedArray2D<int>(comm, data->rows());
	labels_ = new Array2D<int>(data->rows());
	initializer.label(*data, *K, *labels);
	comm.allgather(*labels, *labels_);

	iterator.prepare(*K, *labels_);
	int notconverge=0;
	do{
		iterator.update(*K, *labels_);
		notconverge=iterator.reassign(*K, *labels);
		comm.allgather(*labels, *labels_);

		if(!comm.getRank()) std::cout<<notconverge<<"\n";
	}while(notconverge);


	delete data;
	delete K;
	delete labels;
	delete labels_;

	return 0;
}
