#ifndef DKK_PRIMITIVE_H
#define DKK_PRIMITIVE_H

#include <string>

#include "WindowsExport.h"

namespace DKK{

class Worker;

class Primitive{
private:
	Worker *worker;

public:
	void setWorker(Worker &worker) {this->worker = &worker;}
	Worker &getWorker() {return *worker;}

	virtual const std::string &getName()=0;

	virtual void init()=0;
	virtual void execute()=0;
};
}

#endif