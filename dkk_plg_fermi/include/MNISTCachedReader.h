#ifndef DKK_MNISTCACHEDREADER_H
#define DKK_MNISTCACHEDREADER_H

#include <fstream>

#include "Types.h"
#include "PluginFermiWindowsExport.h"
#include "Reader.h"

namespace DKK{

class DKK_PLUGIN_FERMI_EXPORT MNISTCachedReader : public Reader{
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
	int cache_size, cache_N, cache_idx;
	unsigned char *cache_buffer, *cache;

	void goTo(const int &idx);
	int read32Int();
	unsigned char readByte();
};

}

#endif
