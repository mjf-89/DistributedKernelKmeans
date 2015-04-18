#include "Configurator.h"

#include "GaussianKernel.h"
#include "CSVReader.h"

extern "C" DKK_PLUGIN_DEFAULT_EXPORT void registerUnits()
{
	DKK::Unit *unit;

	unit = new DKK::CSVReader();
	DKK::Configurator::registerUnit(unit);

	unit = new DKK::GaussianKernel();
	DKK::Configurator::registerUnit(unit);

	return;
}