#ifndef DKK_READER_H
#define DKK_READER_H

#include "WindowsExport.h"
#include "Unit.h"

namespace DKK{

class DKK_EXPORT Reader : public Unit{
public:
	const std::string &getType(){
		static std::string type = "READER";
		return type;
	}

	virtual int getDimensionality()=0;
	virtual int getLength()=0;
	virtual int read(int idx, float *data)=0;	
};

}

#endif