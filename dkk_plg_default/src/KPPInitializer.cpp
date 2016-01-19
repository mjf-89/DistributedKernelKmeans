#include <cstdlib>
#include <set>

#include "Configurator.h"
#include "Random.h"
#include "KPPInitializer.h"

namespace DKK{

const std::string &KPPInitializer::getName()
{
	static std::string name = "KPPInitializer";
	return name;
}

const std::vector<std::string> &KPPInitializer::getReqPrmNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrmNamesA[] = {
		"SEED",
		"NC"
	};
	static std::vector<std::string> reqPrmNames(reqPrmNamesA, reqPrmNamesA+2);

	return reqPrmNames;
}

const std::vector<std::string> &KPPInitializer::getReqPrimitiveNames()
{
	static std::vector<std::string> reqPrimitiveNames;

	return reqPrimitiveNames;
}

void KPPInitializer::init()
{
	getParameter("SEED", seed);
	getParameter("NC", NC); 

	Random::init(seed);
}

void KPPInitializer::label(Array2D<DKK_TYPE_REAL> &data, DistributedArray2D<DKK_TYPE_REAL> &K, Kernel &kernel, Array2D<DKK_TYPE_REAL> &medoids)
{	
	Communicator &comm = Configurator::getCommunicator();

	int accepted;
	int N = data.rows();

	int id, owner;
	Array2D<DKK_TYPE_INT> medoids_id(NC);

	DistributedArray2D<DKK_TYPE_REAL> D2(comm, N);

	Array2D<DKK_TYPE_REAL> missing_data(1,data.cols());
	DistributedArray2D<DKK_TYPE_REAL> missing_K(comm, N);

	DKK_TYPE_REAL D2_max, threshold, dst;

	for(int i=0; i<NC; i++){
		accepted = 0;
		while(!accepted){
			id = Random::uniform()%N;
			if(i==0){
				accepted = 1;		
			}else{
				owner = D2.ownerRank(id,0);

				if(comm.getRank()==owner)
					threshold = D2.gidx(id);
				comm.bcast(threshold, owner);

				if(Random::uniform()*D2_max < threshold*DKK_RAND_MAX)
					accepted = 1;
			}
		}

		medoids_id.idx(i) = id;
		D2_max = 0.0;

		if(id>=K.cols()){
			for(int j=0; j<data.cols(); j++)
				missing_data.idx(0,j) = data.idx(id,j);

			kernel.compute(data, missing_data ,missing_K); 

			for(int j=0; j<K.rows(); j++){
				dst = 2.0 - 2.0*missing_K.idx(j);
				if(i==0 || dst < D2.idx(j))
					D2.idx(j) = dst;

				if(D2_max < D2.idx(j))
					D2_max = D2.idx(j);
			}
		}else{
			for(int j=0; j<K.rows(); j++){
				dst = 2.0 - 2.0*K.idx(j,id);
				if(i==0 || dst < D2.idx(j))
					D2.idx(j) = dst;

				if(D2_max < D2.idx(j))
					D2_max = D2.idx(j);
			}
		}


		comm.allreducemax(D2_max);
	}

	for(int i=0; i<medoids.rows(); i++)
		for(int j=0; j<medoids.cols(); j++)
			medoids.idx(i,j) = data.idx(medoids_id.idx(i),j);

}

}
