#include <iostream>
#include <string>

#include "Utils.h"
#include "Types.h"
#include "Configurator.h"
#include "Worker.h"
#include "Primitive.h"
#include "Array2D.h"
#include "DistributedArray2D.h"
#include "Communicator.h"

using namespace DKK;

int main(int argc, char** argv)
{
	//read configuration file
	Configurator conf(&argc, &argv);
	conf.loadPlugins();

	//get algorithm units
	Reader &reader = conf.getReader();
	Kernel &kernel = conf.getKernel();
	Initializer &initializer = conf.getInitializer();
	Iterator &iterator = conf.getIterator();

	//get algorithm driver
	Driver &driver = conf.getDriver();
	//execute the algorithm
	driver.execute(reader, kernel, initializer, iterator);

	return 0;
}
