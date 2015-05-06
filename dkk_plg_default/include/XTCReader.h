#ifndef DKK_XTCREADER_H
#define DKK_XTCREADER_H

#include <fstream>

#include "Types.h"
#include "PluginDefaultWindowsExport.h"
#include "Reader.h"

#include "xdrfile.h"

namespace DKK{

class DKK_PLUGIN_DEFAULT_EXPORT XTCReader : public Reader{
public:
	const std::string &getName();
	const std::vector<std::string> &getReqPrmNames();
	const std::vector<std::string> &getReqPrimitiveNames();

	void init();
	
	int getDimensionality();
	int getLength();
	int read(int idx, DKK_TYPE_REAL* data);
private:
	std::string fin_name;
	XDRFILE *fin;
	std::vector<int> atoms;
	std::vector<DKK_TYPE_OFF> frameIndex;

	int n_atoms, step;
	float time, prec;
	matrix box;
	rvec *coord;
	
	int D, N;

	void createFrameIndex();
	void openFile();
	void goTo(const int &idx);
	DKK_TYPE_OFF skipFrame();
};

}

#endif
