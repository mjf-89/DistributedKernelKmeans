#ifndef DKK_UNIT_H
#define DKK_UNIT_H

#include <string>
#include <vector>
#include <map>

#include "Types.h"
#include "WindowsExport.h"
#include "Worker.h"

namespace DKK{

class DKK_EXPORT Unit{
private:
	std::map < std::string, std::vector<std::string> > prms;
	Worker *worker;

public:
	virtual const std::string &getType() = 0;
	virtual const std::string &getName() = 0;
	virtual const std::vector<std::string> &getReqPrmNames() = 0;
	virtual const std::vector<std::string> &getReqPrimitiveNames()=0;

	virtual void init() = 0;

	void setWorker(Worker &worker);
	Worker &getWorker();

	void setParameter(const std::string &name, const std::string &value);
	void setParameter(const std::string &name, const std::vector<std::string> &value);

	int getParameter(const std::string &name, std::string &prm);
	int getParameter(const std::string &name, std::vector<std::string> &prm);
	int getParameter(const std::string &name, int &prm);
	int getParameter(const std::string &name, std::vector<int> &prm);
	int getParameter(const std::string &name, float &prm);
	int getParameter(const std::string &name, std::vector<float> &prm);
};

}
#endif