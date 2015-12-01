#ifndef DKK_SIMPLEDRIVER_H
#define DKK_SIMPLEDRIVER_H

#include <fstream>

#include "Types.h"
#include "PluginDefaultWindowsExport.h"
#include "Driver.h"
#include "Array2D.h"
#include "DistributedArray2D.h"

namespace DKK{

class DKK_PLUGIN_DEFAULT_EXPORT SimpleDriver : public Driver{
public:
	~SimpleDriver();

	const std::string &getName();
	const std::vector<std::string> &getReqPrmNames();
	const std::vector<std::string> &getReqPrimitiveNames();

	void init();

	void execute(Reader &reader, Kernel &kernel, Initializer &initializer, Iterator &iterator);
	
private:
	std::ofstream fLbl, fCst, fMed, fTim;

	int NC;
	int BS, nB;
	std::string fname;

	double global_timer, init_timer, kernel_timer, iterator_timer, medoid_timer;

	Array2D<DKK_TYPE_REAL> *data;
	Array2D<DKK_TYPE_INT> *labels;

	DistributedArray2D<DKK_TYPE_REAL> *K_D;
	DistributedArray2D<DKK_TYPE_INT> *labels_D;

	Array2D<DKK_TYPE_INT> *medoids_id;
	Array2D<DKK_TYPE_REAL> *medoids;
	DistributedArray2D<DKK_TYPE_REAL> *K_medoids;

	Array2D<DKK_TYPE_INT> *clusters_size;
	Array2D<double> *clusters_similarity;

	void initBatch(Kernel &kernel);

	void writeLabels();
	void writeMedoids();
	void writeCost(Iterator &iterator);
	void writeTime();
};

}

#endif
