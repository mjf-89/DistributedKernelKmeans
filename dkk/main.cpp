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
	Communicator comm(&argc, &argv);

	//read configuration file
	Configurator conf(argv[1]);
	conf.loadPlugins();

	Array2D<float> *data, *K_;
	DistributedArray2D<float> *K;

	//get algorithm units
	Reader &reader = conf.getReader();
	Kernel &kernel = conf.getKernel();
	
	//load dataset
	data = new Array2D<float>(reader.getLength(), reader.getDimensionality());
	for (int i = 0; i < data->rows(); i++)
		reader.read(i, data->bff(i));
	
	//compute the kernel
	K = new DistributedArray2D<float>(comm, data->rows(), data->rows());
	K_ = new Array2D<float>(data->rows(), data->rows());
	std::cout<<K->length()<<"\n";
	kernel.compute(*data, *K);
	comm.allgather(*K, *K_);
	
	delete data;
	delete K;
	
	return 0;
}
