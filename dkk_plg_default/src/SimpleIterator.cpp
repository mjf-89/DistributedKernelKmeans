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

void SimpleIterator::prepare(const DistributedArray2D<DKK_TYPE_REAL> &K, const DistributedArray2D<DKK_TYPE_INT> &labels)
{
	f = new DistributedArray2D<double>(Configurator::getCommunicator(), K.grows(), NC);
	g = new Array2D<double>(NC);
	C = new Array2D<DKK_TYPE_INT>(NC);

	C->fill(0);
	for(int i=0; i<labels.length(); i++)
		C->idx(labels.idx(i)) += 1;

	Configurator::getCommunicator().allreducesum(*C);

	return;
}

void SimpleIterator::update(const DistributedArray2D<DKK_TYPE_REAL> &K, const Array2D<DKK_TYPE_INT> &labels)
{
	Array2D<double> invC(NC), invC2(NC);

	int gr, gc;
	int cr, cc;

	f->fill(0.0);
	g->fill(0.0);

	DKK_TYPE_REAL inv;
	for(int i=0; i<NC; i++){
		inv = 1.0/C->idx(i);
		invC.idx(i) = inv;
		invC2.idx(i) = inv*inv;
	}

	for(int i=0; i<K.rows(); i++){
		K.ltgIdx(i, 0, gr, gc);
		cr = labels.idx(gr);
		for(int j=0; j<K.cols(); j++){
			cc = labels.idx(j);

			f->idx(i, cc) -= 2 * K.idx(i,j);
			if(cr==cc)
				g->idx(cc) += K.idx(i,j) ;
		}
		for(int j=0; j<NC; j++)
			f->idx(i,j) *= invC.idx(j);
	}
	for(int j=0; j<NC; j++)
		g->idx(j) *= invC2.idx(j);

	Configurator::getCommunicator().allreducesum(*g);

	return;
}

int SimpleIterator::reassign(const DistributedArray2D<DKK_TYPE_REAL> &K, DistributedArray2D<DKK_TYPE_INT> &labels)
{
	DKK_TYPE_INT reassign=0;

	C->fill(0);
	for(int i=0; i<labels.rows(); i++){
		DKK_TYPE_REAL min=f->idx(i,0)+g->idx(0);
		DKK_TYPE_INT min_idx=0;
		for(int j=1; j<NC; j++){
			if(f->idx(i,j)+g->idx(j)<min){
				min = f->idx(i,j)+g->idx(j);
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

double SimpleIterator::cost(const DistributedArray2D<DKK_TYPE_REAL> &K, const DistributedArray2D<DKK_TYPE_INT> &labels){
	double c=0.0;
	int ci, gi, gj;
	for(int i=0; i<K.rows(); i++){
		labels.ltgIdx(i,0,gi,gj);
		ci = labels.idx(i);
		c += K.gidx(gi,gi) + f->idx(i, ci) + g->idx(ci);
	}

	Configurator::getCommunicator().allreducesum(c);

	return c;
}

void SimpleIterator::medoids(const DistributedArray2D<DKK_TYPE_REAL> &K, const DistributedArray2D<DKK_TYPE_INT> &labels, Array2D<DKK_TYPE_INT> &medoids)
{
	int ci, gi, gj;

	Array2D<DKK_TYPE_REAL_INT> medoidsMinDst(NC);
	DKK_TYPE_REAL_INT init;
	init.r = 1e64;
	init.i = -1;
	medoidsMinDst.fill(init);

	DKK_TYPE_REAL dst;

	for(int i=0; i<labels.rows(); i++){
		labels.ltgIdx(i,0,gi,gj);
		ci = labels.idx(i);
		dst=K.gidx(gi,gi)+f->idx(i, ci)+g->idx(ci);
		if(medoidsMinDst.idx(ci).i<0 || dst<medoidsMinDst.idx(ci).r){
			medoidsMinDst.idx(ci).i = gi;
			medoidsMinDst.idx(ci).r = dst;	
		}
	}

	Configurator::getCommunicator().allreduceminloc(medoidsMinDst);

	for(int i=0; i<medoids.rows(); i++){
		medoids.idx(i)=medoidsMinDst.idx(i).i;
	}
}

DKK_TYPE_INT SimpleIterator::getClusterSize(int cluster_idx)
{
	return C->idx(cluster_idx);
}

double SimpleIterator::getClusterAvgSimilarity(int cluster_idx)
{
	return g->idx(cluster_idx);
}

}
