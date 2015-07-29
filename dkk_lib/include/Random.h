#ifndef DKK_RANDOM_H
#define DKK_RANDOM_H

#include "WindowsExport.h"

#define DKK_RAND_MAX 2147483646

namespace DKK{

class DKK_EXPORT Random{
public:
	Random();

	void init(int s);
	int uniform();

private:
	int seed;
};

}

#endif
