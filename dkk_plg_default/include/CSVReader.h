#ifndef DKK_CSVREADER_H
#define DKK_CSVREADER_H

#include <fstream>

#include "Types.h"
#include "PluginDefaultWindowsExport.h"
#include "Reader.h"

namespace DKK{

class DKK_PLUGIN_DEFAULT_EXPORT CSVReader : public Reader{
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
	std::string fs;
	int D, N, record_idx;

	void rewind();
	void goTo(const int &idx);

	int getField(std::string &record, DKK_TYPE_REAL &field);
	int getRecord(std::string &record);
};

}

#endif