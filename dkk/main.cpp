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
	data = new Array2D<float>(5000, reader.getDimensionality());
	for (int i = 0; i < data->rows(); i++)
		reader.read(i, data->bff(i));
	
	//compute the kernel
	K = new DistributedArray2D<float>(comm, data->rows(), data->rows());
	K_ = new Array2D<float>(data->rows(), data->rows());
	//kernel.compute(*data, *K);
	//comm.allgather(*K, *K_);
	
	/*if(!comm.getRank()){
		for(int i=0; i<K_->rows(); i++){
			for(int j=0; j<K_->cols(); j++)
				std::cout<<K_->idx(i,j)<<"\t";
			std::cout<<"\n";
		}
	}*/

	delete data;
	delete K;
	
	return 0;
}
