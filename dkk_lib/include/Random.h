#ifndef DKK_RANDOM_H
#define DKK_RANDOM_H

#include "WindowsExport.h"

namespace DKK{

static int seed=0;

int rand()
{
	int aa, mm, qq, rr, hh, lo, test;
	aa = 16807;
	mm = 2147483647;
	qq = 127773;
	rr = 2836;
	if ((seed/qq) < 0)
		hh = (int)(seed/qq - 0.5);
	else
		hh = (int)(seed/qq + 0.5);
	lo   = seed - hh * qq;
	test = aa*lo-rr*hh;
	if (test >= 0)
	    seed = test;
	else
	    seed = test + mm;

	return seed;
}

void srand(int s)
{
	seed = s;
}

}

#endif
