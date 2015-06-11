#ifndef DKK_SIMPLEITERATOR_H
#define DKK_SIMPLEITERATOR_H

#include <fstream>

#include "Types.h"
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

	void prepare(const DistributedArray2D<DKK_TYPE_REAL> &K, const DistributedArray2D<DKK_TYPE_INT> &labels);
	void update(const DistributedArray2D<DKK_TYPE_REAL> &K, const Array2D<DKK_TYPE_INT> &labels);
	int reassign(const DistributedArray2D<DKK_TYPE_REAL> &K, DistributedArray2D<DKK_TYPE_INT> &labels);
	DKK_TYPE_REAL cost(const DistributedArray2D<DKK_TYPE_REAL> &K, const DistributedArray2D<DKK_TYPE_INT> &labels);
private:
	int NC;

	DistributedArray2D<double> *f;
	Array2D<double> *g;
	Array2D<DKK_TYPE_INT> *C;
};

}

#endif
