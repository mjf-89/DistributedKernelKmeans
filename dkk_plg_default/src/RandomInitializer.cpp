#include <cstdlib>
#include <set>

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

	random.init(seed);
}

void RandomInitializer::label(Array2D<DKK_TYPE_REAL> &data, DistributedArray2D<DKK_TYPE_REAL> &K, DistributedArray2D<DKK_TYPE_INT> &labels)
{	
	std::set<int> medoids;

	while(medoids.size()<NC)
		medoids.insert(random.uniform()%data.rows());

	for(int i=0; i<labels.length(); i++){
		float m_dst;
		int m_idx=0, lbl=0;
		for(std::set<int>::iterator j=medoids.begin(); j!=medoids.end(); j++){
			float dst = 2.0-2.0*K.idx(i,*j);
			if(lbl==0) m_dst=dst;
			if(dst<m_dst){
				m_dst=dst;
				m_idx=lbl;
			}
			lbl++;
		}
		labels.idx(i)=m_idx;
	}
}

}
