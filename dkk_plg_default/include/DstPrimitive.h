#ifndef DKK_DSTPRIMITIVE_H
#define DKK_DSTPRIMITIVE_H

#include "PluginDefaultWindowsExport.h"

#include "Worker.h"
#include "Primitive.h"
#include "DistributedArray2D.h"

namespace DKK{

class DstPrimitive : public Primitive{
public:	
	const std::string &getName()
	{
		static std::string name = "DstPrimitive";
		return name;
	}

	void setDataArray(Array2D<float> &data) 
	{
		this->data = &data;
	}

	void setDstArray(DistributedArray2D<float> &dst)
	{
		this->dst = &dst;
	}

protected:
	Array2D<float> *data;
	DistributedArray2D<float> *dst;
};

}

#endif
