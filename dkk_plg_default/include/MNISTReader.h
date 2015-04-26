#ifndef DKK_MNISTREADER_H
#define DKK_MNISTREADER_H

#include <fstream>

#include "Types.h"
#include "PluginDefaultWindowsExport.h"
#include "Reader.h"

namespace DKK{

class DKK_PLUGIN_DEFAULT_EXPORT MNISTReader : public Reader{
public:
	const std::string &getName();
	const std::vector<std::string> &getReqPrmNames();
	const std::vector<std::string> &getReqPrimitiveNames();

	void init();
	
	int getDimensionality();
	int getLength();
	int read(int idx, DKK_TYPE_REAL* data);
private:
	std::ifstream fin;
	unsigned char buffer[4];
	int magic, D, N;

	void goTo(const int &idx);
	int read32Int();
	unsigned char readByte();
};

}

#endif
