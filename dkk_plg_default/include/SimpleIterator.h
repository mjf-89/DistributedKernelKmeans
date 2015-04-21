#ifndef DKK_SIMPLEITERATOR_H
#define DKK_SIMPLEITERATOR_H

#include <fstream>

#include "PluginDefaultWindowsExport.h"
#include "Iterator.h"
#include "Array2D.h"
#include "DistributedArray2D.h"

namespace DKK{

class DKK_PLUGIN_DEFAULT_EXPORT SimpleIterator : public Iterator{
public:
	const std::string &getName();
	const std::vector<std::string> &getReqPrmNames();
	const std::vector<std::string> &getReqPrimitiveNames();

	void init();

	void prepare(const DistributedArray2D<float> &K, const Array2D<int> &labels);
	void update(const DistributedArray2D<float> &K, const Array2D<int> &labels);
	int reassign(const DistributedArray2D<float> &K, DistributedArray2D<int> &labels);
private:
	int NC;

	DistributedArray2D<float> *f;
	Array2D<float> *g;
	Array2D<int> *C;
};

}

#endif
