#include "Configurator.h"

#include "CPUWorker.h"
#include "CPUDstPrimitive.h"
#include "GaussianKernel.h"
#include "CSVReader.h"
#include "RandomInitializer.h"

#include <iostream>
extern "C" DKK_PLUGIN_DEFAULT_EXPORT void registerUnits()
{
	DKK::Unit *unit;
	DKK::Worker *worker;

	unit = new DKK::CSVReader();
	DKK::Configurator::registerUnit(unit);

	unit = new DKK::GaussianKernel();
	DKK::Configurator::registerUnit(unit);

	unit = new DKK::RandomInitializer();
	DKK::Configurator::registerUnit(unit);

	worker = new DKK::CPUWorker();
	DKK::Configurator::registerWorker(worker);
	worker->registerPrimitive(new DKK::CPUDstPrimitive());

	return;
}
