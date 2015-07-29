#include "Configurator.h"

#include "CPUWorker.h"
#include "CPUDstPrimitive.h"
#include "CPUGaussianEuclideanPrimitive.h"

#include "GaussianKernel.h"

#include "CSVReader.h"
#include "MNISTReader.h"
#include "XTCReader.h"


#include "ExplicitInitializer.h"
#include "RandomInitializer.h"
#include "KPPInitializer.h"

#include "SimpleIterator.h"

#include "SimpleDriver.h"

extern "C" DKK_PLUGIN_DEFAULT_EXPORT void registerUnits()
{
	DKK::Configurator::registerUnit(new DKK::CSVReader());
	DKK::Configurator::registerUnit(new DKK::MNISTReader());
	DKK::Configurator::registerUnit(new DKK::XTCReader());

	DKK::Configurator::registerUnit(new DKK::GaussianKernel());

	DKK::Configurator::registerUnit(new DKK::ExplicitInitializer());
	DKK::Configurator::registerUnit(new DKK::RandomInitializer());
	DKK::Configurator::registerUnit(new DKK::KPPInitializer());

	DKK::Configurator::registerUnit(new DKK::SimpleIterator());

	DKK::Configurator::registerUnit(new DKK::SimpleDriver());

	DKK::Worker *worker = new DKK::CPUWorker();
	DKK::Configurator::registerWorker(worker);
	worker->registerPrimitive(new DKK::CPUDstPrimitive());
	worker->registerPrimitive(new DKK::CPUGaussianEuclideanPrimitive());

	return;
}
