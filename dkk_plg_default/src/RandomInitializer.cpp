#include <cstdlib>
#include <set>

#include "Configurator.h"
#include "Random.h"
#include "RandomInitializer.h"

namespace DKK{

const std::string &RandomInitializer::getName()
{
	static std::string name = "RandomInitializer";
	return name;
}

const std::vector<std::string> &RandomInitializer::getReqPrmNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrmNamesA[] = {
		"SEED",
		"NC"
	};
	static std::vector<std::string> reqPrmNames(reqPrmNamesA, reqPrmNamesA+2);

	return reqPrmNames;
}

const std::vector<std::string> &RandomInitializer::getReqPrimitiveNames()
{
	static std::vector<std::string> reqPrimitiveNames;

	return reqPrimitiveNames;
}

void RandomInitializer::init()
{
	getParameter("SEED", seed);
	getParameter("NC", NC); 

	Random::init(seed);
}

void RandomInitializer::label(Array2D<DKK_TYPE_REAL> &data, DistributedArray2D<DKK_TYPE_REAL> &K, Kernel &kernel, Array2D<DKK_TYPE_REAL> &medoids)
{	
	std::set<int> medoids_id;
	int i;

	Array2D<DKK_TYPE_REAL> missing_data(1,data.cols());
	DistributedArray2D<DKK_TYPE_REAL> missing_K(Configurator::getCommunicator(), data.rows());

	while(medoids_id.size()<NC)
		medoids_id.insert(Random::uniform()%data.rows());

	i = 0;
	for(std::set<int>::iterator j=medoids_id.begin(); j!=medoids_id.end(); j++){
		for(int k=0; k<medoids.cols(); k++)
			medoids.idx(i,k) = data.idx(*j,k);
		i++;
	}
}

}
