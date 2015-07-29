#include <cstdlib>
#include <set>

#include "ExplicitInitializer.h"

namespace DKK{

const std::string &ExplicitInitializer::getName()
{
	static std::string name = "ExplicitInitializer";
	return name;
}

const std::vector<std::string> &ExplicitInitializer::getReqPrmNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrmNamesA[] = {
		"NC"
	};
	static std::vector<std::string> reqPrmNames(reqPrmNamesA, reqPrmNamesA+1);

	return reqPrmNames;
}

const std::vector<std::string> &ExplicitInitializer::getReqPrimitiveNames()
{
	static std::vector<std::string> reqPrimitiveNames;

	return reqPrimitiveNames;
}

void ExplicitInitializer::init()
{
	getParameter("NC", NC); 
}

void ExplicitInitializer::label(Array2D<DKK_TYPE_REAL> &data, DistributedArray2D<DKK_TYPE_REAL> &K, DistributedArray2D<DKK_TYPE_INT> &labels)
{	
}

}
