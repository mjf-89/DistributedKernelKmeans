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
	const std::string &getName();
	const std::vector<std::string> &getReqPrmNames();
	const std::vector<std::string> &getReqPrimitiveNames();

	void init();

	void execute(Reader &reader, Kernel &kernel, Initializer &initializer, Iterator &iterator);
	
	//TODO void medoids();
private:
	int NC;

	Array2D<DKK_TYPE_REAL> *data;
	Array2D<DKK_TYPE_INT> *labels;

	DistributedArray2D<DKK_TYPE_REAL> *K_D;
	DistributedArray2D<DKK_TYPE_INT> *labels_D;
};

}

#endif
