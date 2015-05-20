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
	if(!comm.getRank()) std::cerr<<"Loading dataset ...\n";
	data = new Array2D<DKK_TYPE_REAL>(reader.getLength(), reader.getDimensionality());
	for (int i = 0; i < data->rows(); i++)
		reader.read(i, data->bff(i));

	//compute the kernel
	if(!comm.getRank()) std::cerr<<"Computing kernel ...\n";
	K = new DistributedArray2D<DKK_TYPE_REAL>(comm, data->rows(), data->rows());
	kernel.compute(*data, *K);

	//init the labels
	if(!comm.getRank()) std::cerr<<"Initializing labels ...\n";
	labels = new DistributedArray2D<DKK_TYPE_INT>(comm, data->rows());
	labels_ = new Array2D<DKK_TYPE_INT>(data->rows());
	initializer.label(*data, *K, *labels);
	comm.allgather(*labels, *labels_);

	//iterate
	if(!comm.getRank()) std::cerr<<"Iterating kernel k-means ...\n";
	iterator.prepare(*K, *labels);
	int notconverge=0;
	DKK_TYPE_REAL cost;
	do{
		comm.allgather(*labels, *labels_);
		iterator.update(*K, *labels_);
		notconverge=iterator.reassign(*K, *labels);
		cost = iterator.cost(*K, *labels);
		if(!comm.getRank()) std::cerr<<cost<<"\n";
	}while(notconverge);


	for(int i=0; i<labels->rows(); i++){
		for(int j=0; j<labels->cols(); j++){
			std::cout<<labels->idx(i,j)<<"\n";	
		}
	}

	delete data;
	delete K;
	delete labels;
	delete labels_;

	return 0;
}
