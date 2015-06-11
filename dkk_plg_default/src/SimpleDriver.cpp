#include <cstdlib>
#include <set>

#include "Configurator.h"
#include "SimpleDriver.h"

#include "Utils.h"

namespace DKK{

const std::string &SimpleDriver::getName()
{
	static std::string name = "SimpleDriver";
	return name;
}

const std::vector<std::string> &SimpleDriver::getReqPrmNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrmNamesA[] = {
		"NC",
		"READER",
		"KERNEL",
		"INITIALIZER",
		"ITERATOR"
	};
	static std::vector<std::string> reqPrmNames(reqPrmNamesA, reqPrmNamesA+5);
	return reqPrmNames;
}

const std::vector<std::string> &SimpleDriver::getReqPrimitiveNames()
{
	static std::vector<std::string> reqPrimitiveNames;

	return reqPrimitiveNames;
}

void SimpleDriver::init()
{
	getParameter("NC", NC);

	return;
}


void SimpleDriver::execute(Reader &reader, Kernel &kernel, Initializer &initializer, Iterator &iterator)
{
	Communicator &comm = Configurator::getCommunicator();

	//load dataset
	double timer = wallclock(NULL);	
	if(!comm.getRank()) std::cerr<<"Loading dataset ...\n";
	data = new Array2D<DKK_TYPE_REAL>(reader.getLength(), reader.getDimensionality());
	for (int i = 0; i < data->rows(); i++)
		reader.read(i, data->bff(i));
	if(!comm.getRank()) std::cerr<<"("<<wallclock(&timer)<<" us)\n";

	//compute the kernel
	timer = wallclock(NULL);
	if(!comm.getRank()) std::cerr<<"Computing kernel ...\n";
	K_D = new DistributedArray2D<DKK_TYPE_REAL>(comm, data->rows(), data->rows());
	kernel.compute(*data, *K_D);
	if(!comm.getRank()) std::cerr<<"("<<wallclock(&timer)<<" us)\n";

	//init the labels
	timer = wallclock(NULL);
	if(!comm.getRank()) std::cerr<<"Initializing labels ...\n";
	labels_D = new DistributedArray2D<DKK_TYPE_INT>(comm, data->rows());
	labels = new Array2D<DKK_TYPE_INT>(data->rows());
	initializer.label(*data, *K_D, *labels_D);
	if(!comm.getRank()) std::cerr<<"("<<wallclock(&timer)<<" us)\n";

	//iterate
	timer = wallclock(NULL);
	if(!comm.getRank()) std::cerr<<"Iterating kernel k-means ...\n";
	iterator.prepare(*K_D, *labels_D);
	int notconverge=0;
	DKK_TYPE_REAL cost;
	do{
		comm.allgather(*labels_D, *labels);
		iterator.update(*K_D, *labels);
		notconverge=iterator.reassign(*K_D, *labels_D);
		cost = iterator.cost(*K_D, *labels_D);
		if(!comm.getRank()) std::cerr<<cost<<"\n";
	}while(notconverge);
	if(!comm.getRank()) std::cerr<<"("<<wallclock(&timer)<<" us)\n";

	//output the labels
	if(!comm.getRank()){
		for(int i=0; i<labels->rows(); i++){
			for(int j=0; j<labels->cols(); j++){
				std::cout<<labels->idx(i,j)<<"\n";	
			}
		}
	}

	delete data;
	delete labels;

	delete K_D;
	delete labels_D;

}

}
