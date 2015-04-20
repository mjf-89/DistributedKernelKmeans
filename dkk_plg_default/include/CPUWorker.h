#ifndef DKK_CPUWORKER_H
#define DKK_CPUWORKER_H

#include "PluginDefaultWindowsExport.h"
#include "Worker.h"

namespace DKK{

class DKK_PLUGIN_DEFAULT_EXPORT CPUWorker : public Worker{
public:
	const std::string &getName();

	void init();
private:
};

}

#endif
