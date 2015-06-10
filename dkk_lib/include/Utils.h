#ifndef DKK_UTILS_H
#define DKK_UTILS_H

#include <time.h>

namespace DKK{

static double wallclock(const double * ref)
{
	struct timespec t;
	double ret;

	clock_gettime(CLOCK_REALTIME, &t);
	ret = ((double) t.tv_sec)*1.0e6 + 1.0e-3*((double) t.tv_nsec);

	return ref ? (ret - *ref) : ret;
}

}

#endif
