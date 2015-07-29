#ifndef DKK_KPPINITIALIZER_H
#define DKK_KPPINITIALIZER_H

#include <fstream>

#include "Random.h"
#include "Types.h"
#include "PluginDefaultWindowsExport.h"
#include "Initializer.h"
#include "Array2D.h"
#include "DistributedArray2D.h"

namespace DKK{

class DKK_PLUGIN_DEFAULT_EXPORT KPPInitializer : public Initializer{
public:
	const std::string &getName();
	const std::vector<std::string> &getReqPrmNames();
	const std::vector<std::string> &getReqPrimitiveNames();

	void init();

	void label(Array2D<DKK_TYPE_REAL> &data, DistributedArray2D<DKK_TYPE_REAL> &K, DistributedArray2D<DKK_TYPE_INT> &labels);
private:
	Random random;
	int seed, NC;
	DistributedArray2D<DKK_TYPE_REAL> *D2;
};

}

#endif
