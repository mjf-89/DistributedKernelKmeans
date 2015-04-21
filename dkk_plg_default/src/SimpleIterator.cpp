#include <cstdlib>
#include <set>

#include "Configurator.h"
#include "SimpleIterator.h"

namespace DKK{

const std::string &SimpleIterator::getName()
{
	static std::string name = "SimpleIterator";
	return name;
}

const std::vector<std::string> &SimpleIterator::getReqPrmNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrmNamesA[] = {
		"NC"
	};
	static std::vector<std::string> reqPrmNames(reqPrmNamesA, reqPrmNamesA+1);
	return reqPrmNames;
}

const std::vector<std::string> &SimpleIterator::getReqPrimitiveNames()
{
	static std::vector<std::string> reqPrimitiveNames;

	return reqPrimitiveNames;
}

void SimpleIterator::init()
{
	getParameter("NC", NC);

	return;
}

void SimpleIterator::prepare(const DistributedArray2D<float> &K, const Array2D<int> &labels)
{
	f = new DistributedArray2D<float>(Configurator::getCommunicator(), K.grows(), NC);
	g = new Array2D<float>(NC);
	C = new Array2D<int>(NC);

	C->fill(0);
	for(int i=0; i<labels.length(); i++)
		C->idx(labels.idx(i)) += 1;

	return;
}

void SimpleIterator::update(const DistributedArray2D<float> &K, const Array2D<int> &labels)
{
	int gr, gc;
	int cr, cc;

	f->fill(0.0);
	g->fill(0.0);

	for(int i=0; i<K.rows(); i++){
		for(int j=0; j<K.cols(); j++){
			K.ltgIdx(i, j, gr, gc);

			cr = labels.idx(gr);
			cc = labels.idx(gc);

			f->idx(i, cc) -= 2 * K.idx(i,j) / C->idx(cc);
			if (cr == cc)
				g->idx(cr) += K.idx(i,j) / (C->idx(cr) * C->idx(cr));
		}
	}

	return;
}

int SimpleIterator::reassign(const DistributedArray2D<float> &K, DistributedArray2D<int> &labels)
{
	int reassign=0;

	Configurator::getCommunicator().allreducesum(*g);

	C->fill(0);
	for(int i=0; i<labels.rows(); i++){
		float min=f->idx(i,0)+g->idx(0);
		int min_idx=0;
		for(int j=1; j<NC; j++){
			if(f->idx(i,j)+g->idx(i,j)<min){
				min = f->idx(i,j)+g->idx(i,j);
				min_idx = j;
			}
		}
		if(labels.idx(i) != min_idx)
			reassign++;

		labels.idx(i) = min_idx;
		C->idx(min_idx) += 1;
	}

	Configurator::getCommunicator().allreducesum(*C);
	Configurator::getCommunicator().allreducesum(reassign);

	return reassign;
}

}
