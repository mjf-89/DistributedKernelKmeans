#ifndef DKK_RANDOMINITIALIZER_H
#define DKK_RANDOMINITIALIZER_H

#include <fstream>

#include "PluginDefaultWindowsExport.h"
#include "Initializer.h"
#include "Array2D.h"
#include "DistributedArray2D.h"

namespace DKK{

class DKK_PLUGIN_DEFAULT_EXPORT RandomInitializer : public Initializer{
public:
	const std::string &getName();
	const std::vector<std::string> &getReqPrmNames();
	const std::vector<std::string> &getReqPrimitiveNames();

	void init();

	void label(Array2D<float> &data, DistributedArray2D<float> &K, DistributedArray2D<int> &labels);
private:
	int seed, NC;
};

}

#endif
