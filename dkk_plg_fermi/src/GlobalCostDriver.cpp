#include <cstdlib>
#include <math.h>

#include "Configurator.h"
#include "GlobalCostDriver.h"

#include "Utils.h"

namespace DKK{

const std::string &GlobalCostDriver::getName()
{
	static std::string name = "GlobalCostDriver";
	return name;
}

const std::vector<std::string> &GlobalCostDriver::getReqPrmNames()
{
	//stupid fix to avoid c++ 11 init lists
	static std::string reqPrmNamesA[] = {
		"OUTPUT_PREFIX",
		"NC",
		"READER",
		"KERNEL",
		"INITIALIZER",
		"ITERATOR"
	};
	static std::vector<std::string> reqPrmNames(reqPrmNamesA, reqPrmNamesA+6);
	return reqPrmNames;
}

const std::vector<std::string> &GlobalCostDriver::getReqPrimitiveNames()
{
	static std::vector<std::string> reqPrimitiveNames;

	return reqPrimitiveNames;
}

void GlobalCostDriver::init()
{
	//load parameters
	getParameter("NC", NC);
	getParameter("BS", BS);
	getParameter("OUTPUT_PREFIX",fname);

	//clear files content
	fLbl.open((fname+".lbl").c_str());
	fLbl.close();
	fCst.open((fname+".cst").c_str());
	fCst.close();
	fMed.open((fname+".med").c_str());
	fMed.close();

	fTim.open((fname+".t").c_str());
	fTim.close();
	global_timer=0.0;
	init_timer=0.0;
	kernel_timer=0.0;
	iterator_timer=0.0;
	medoid_timer=0.0;

	return;
}

GlobalCostDriver::~GlobalCostDriver()
{
}

void GlobalCostDriver::execute(Reader &reader, Kernel &kernel, Initializer &initializer, Iterator &iterator)
{
	double global_timer_loc, init_timer_loc, kernel_timer_loc, iterator_timer_loc, medoid_timer_loc;

	int cnt = 0;

	Communicator &comm = Configurator::getCommunicator();

	global_timer_loc = comm.wtime();

	DS = reader.getLength();
	nB = ceil(DS / BS);

	dataset = new Array2D<DKK_TYPE_REAL>(DS, reader.getDimensionality());
	minibatch = new Array2D<DKK_TYPE_REAL>(BS, reader.getDimensionality());

	K_D = new DistributedArray2D<DKK_TYPE_REAL>(comm, minibatch->rows(), minibatch->rows());

	K_medoids = new DistributedArray2D<DKK_TYPE_REAL>(comm, minibatch->rows(), NC);
	K_dataset_medoids_D = new DistributedArray2D<DKK_TYPE_REAL>(comm, dataset->rows(), NC);

	labels_D = new DistributedArray2D<DKK_TYPE_INT>(comm, minibatch->rows());
	labels = new Array2D<DKK_TYPE_INT>(minibatch->rows());

	medoids_id = new Array2D<DKK_TYPE_INT>(NC);
	medoids = new Array2D<DKK_TYPE_REAL>(NC, minibatch->cols());

	clusters_size = new Array2D<DKK_TYPE_INT>(NC);
	clusters_similarity = new Array2D<double>(NC);

	//read the whole dataset in order to compute the global cost
	for (int i = 0; i < dataset->rows(); i++)
		reader.read(i, dataset->bff(i));

	for(int i=0; i<nB; i++){
		//load one batch
		if(!comm.getRank()) std::cerr<<"Loading dataset ...\n";
		for (int j = 0; j < minibatch->rows(); j++)
			reader.read(i*BS+j, minibatch->bff(j));

		//compute kernel for the current batch
		kernel_timer_loc = comm.wtime();
		if(!comm.getRank()) std::cerr<<"Computing kernel ...\n";
		kernel.compute(*minibatch, *K_D);
		kernel_timer += comm.wtime()-kernel_timer_loc;

		//init first batch using the given initializer
		if(!comm.getRank()) std::cerr<<"Initializing labels ...\n";
		init_timer_loc = comm.wtime();
		if(i==0){
			initializer.label(*minibatch, *K_D, kernel, *medoids);
			initBatch(kernel);
		}else{
			initBatch(kernel);
		}
		init_timer += comm.wtime()-init_timer_loc;

		//compute global cost
		if(!comm.getRank()) std::cerr<<"Compute global cost ...\n";
		computeDatasetCost(kernel,cnt);	
		
		//iterate until convergence
		iterator_timer_loc = comm.wtime();
		if(!comm.getRank()) std::cerr<<"Iterating kernel k-means ...\n";
		iterator.prepare(*K_D, *labels_D);
		int notconverge=0;
		do{
			comm.allgather(*labels_D, *labels);
			iterator.update(*K_D, *labels);
			notconverge=iterator.reassign(*K_D, *labels_D);
			writeCost(iterator,cnt);
			cnt++;
		}while(notconverge);
		iterator_timer += comm.wtime()-iterator_timer_loc;

		//find new medoids
		medoid_timer_loc = comm.wtime();
		if(!comm.getRank()) std::cerr<<"Finding medoids ...\n";
		iterator.medoids(*K_D, *labels_D, *medoids_id);
		for(int j=0; j<NC; j++){
			if(i>0){
				DKK_TYPE_INT old_size = clusters_size->idx(j);
				DKK_TYPE_INT size = iterator.getClusterSize(j);

				DKK_TYPE_REAL old_similarity = clusters_similarity->idx(j);
				DKK_TYPE_REAL similarity = iterator.getClusterAvgSimilarity(j);

				DKK_TYPE_REAL old_T = old_size;//*old_similarity;//(1.0-old_similarity);
				DKK_TYPE_REAL T = size;//*similarity;//(1.0-similarity);

				similarity = old_size * old_size * old_similarity + size * size * similarity + 2*old_size*size*K_medoids->idx(j,medoids_id->idx(j));
				//similarity /= old_size*old_size + size*size;
				size = old_size + size;
				similarity /= size*size;

				clusters_size->idx(j) = size;
				clusters_similarity->idx(j) = similarity;

				for(int k=0; k<medoids->cols(); k++){
					if(medoids_id->idx(j) > 0)
						medoids->idx(j,k) = (medoids->idx(j,k)*old_T + minibatch->idx(medoids_id->idx(j),k)*T)/(old_T+T);
					else
						medoids->idx(j,k) = medoids->idx(j,k);
				}
			}else{
				clusters_size->idx(j) = iterator.getClusterSize(j);
				clusters_similarity->idx(j) = iterator.getClusterAvgSimilarity(j);
			
				for(int k=0; k<medoids->cols(); k++)
					medoids->idx(j,k) = minibatch->idx(medoids_id->idx(j),k);
			}
		}
		medoid_timer += comm.wtime()-medoid_timer_loc;

		writeLabels();
		writeMedoids();
		writeEndBatch();
	}
	//compute global cost
	if(!comm.getRank()) std::cerr<<"Compute global cost ...\n";
	computeDatasetCost(kernel,cnt);	

	global_timer = comm.wtime()-global_timer_loc;

	writeTime();

	delete dataset;
	delete K_dataset_medoids_D;

	delete minibatch;
	delete labels;

	delete K_D;
	delete labels_D;

	delete medoids;
	delete medoids_id;
	delete K_medoids;
}

void GlobalCostDriver::initBatch(Kernel &kernel)
{
	kernel.compute(*minibatch, *medoids, *K_medoids);

	for(int i=0; i<labels_D->length(); i++){
		double m_dst;
		int m_idx=0;
		for(int j=0; j<medoids->rows(); j++){
			double dst = 2.0-2.0*K_medoids->idx(i,j);
			if(j==0 || dst<m_dst){
				m_dst=dst;
				m_idx=j;
			}
		}
		labels_D->idx(i)=m_idx;
	}
}

double GlobalCostDriver::computeDatasetCost(Kernel &kernel, int cnt)
{
	double cost;

	kernel.compute(*dataset, *medoids, *K_dataset_medoids_D);

	cost = 0;
	for(int i=0; i<K_dataset_medoids_D->rows(); i++){
		double m_dst;
		int m_idx=0;
		for(int j=0; j<medoids->rows(); j++){
			double dst = 2.0-2.0*K_dataset_medoids_D->idx(i,j);
			if(j==0 || dst<m_dst){
				m_dst=dst;
				m_idx=j;
			}
		}
		cost += m_dst;
	}

	Configurator::getCommunicator().allreducesum(cost);

	if(!Configurator::getCommunicator().getRank()) std::cout << "mjf-dbg> global cost: " << cnt << " " << cost << "\n";

	return cost;
}

void GlobalCostDriver::writeLabels()
{
	if(!Configurator::getCommunicator().getRank()){
		fLbl.open((fname+".lbl").c_str(),std::ofstream::app);

		for(int i=0; i<labels->rows(); i++){
			for(int j=0; j<labels->cols(); j++)
				fLbl<<labels->idx(i,j)<<" ";	
			fLbl<<"\n";	
		}

		fLbl.close();
	}
}

void GlobalCostDriver::writeMedoids()
{
	if(!Configurator::getCommunicator().getRank()){
		fMed.open((fname+".med").c_str(),std::ofstream::app);

		for(int i=0; i<medoids->rows(); i++){
			for(int j=0; j<medoids->cols(); j++)
				fMed<<medoids->idx(i,j)<<" ";	
			fMed<<"\n";
		}

		fMed.close();
	}
}

void GlobalCostDriver::writeCost(Iterator &iterator, int cnt)
{
	double cost = iterator.cost(*K_D, *labels_D);
	if(!Configurator::getCommunicator().getRank()){
		fCst.open((fname+".cst").c_str(),std::ofstream::app);

		fCst<<cnt<<" "<<cost<<"\n";

		fCst.close();
	}
}

void GlobalCostDriver::writeTime()
{
	if(!Configurator::getCommunicator().getRank()){
		fTim.open((fname+".t").c_str(),std::ofstream::app);
		fTim<<"global:   "<<global_timer<<"\n";
		fTim<<"kernel:   "<<kernel_timer<<"\n";
		fTim<<"iterator: "<<iterator_timer<<"\n";
		fTim<<"medoid:   "<<medoid_timer<<"\n";
		fTim<<"init:     "<<init_timer<<"\n";
		fTim.close();
	}
}

void GlobalCostDriver::writeEndBatch()
{
	if(!Configurator::getCommunicator().getRank()){
		fCst.open((fname+".cst").c_str(),std::ofstream::app);
		fMed.open((fname+".med").c_str(),std::ofstream::app);
		fLbl.open((fname+".lbl").c_str(),std::ofstream::app);

		fCst<<"\n\n";
		fMed<<"\n\n";
		fLbl<<"\n\n";

		fCst.close();
		fMed.close();
		fLbl.close();
	}

}

}
