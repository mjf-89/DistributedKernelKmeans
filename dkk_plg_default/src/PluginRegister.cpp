#include "Configurator.h"

#include "CPUWorker.h"
#include "CPUDstPrimitive.h"
#include "GaussianKernel.h"
#include "CSVReader.h"
#include "MNISTReader.h"
#include "RandomInitializer.h"
#include "SimpleIterator.h"

#include <iostream>
extern "C" DKK_PLUGIN_DEFAULT_EXPORT void registerUnits()
{
	DKK::Configurator::registerUnit(new DKK::CSVReader());
	DKK::Configurator::registerUnit(new DKK::MNISTReader());
	DKK::Configurator::registerUnit(new DKK::GaussianKernel());
	DKK::Configurator::registerUnit(new DKK::RandomInitializer());
	DKK::Configurator::registerUnit(new DKK::SimpleIterator());

	DKK::Worker *worker = new DKK::CPUWorker();
	DKK::Configurator::registerWorker(worker);
	worker->registerPrimitive(new DKK::CPUDstPrimitive());

	return;
}
