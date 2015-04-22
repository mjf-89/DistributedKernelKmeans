#ifndef DKK_CPUDSTPRIMITIVE_H
#define DKK_CPUDSTPRIMITIVE_H

#include "PluginDefaultWindowsExport.h"
#include "DstPrimitive.h"
#include "Array2D.h"

namespace DKK{

class DKK_PLUGIN_DEFAULT_EXPORT CPUDstPrimitive : public DstPrimitive{
public:	
	~CPUDstPrimitive();
	void init();
	void execute();
private:
	DKK_TYPE_REAL *d_data, *d_dst;
};

}

#endif
