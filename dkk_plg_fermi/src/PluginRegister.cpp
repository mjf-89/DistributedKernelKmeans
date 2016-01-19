#include "Configurator.h"

#include "PluginFermiWindowsExport.h"
#include "MNISTCachedReader.h"
#include "GlobalCostDriver.h"
#include "SubsampleDriver.h"

extern "C" DKK_PLUGIN_FERMI_EXPORT void registerUnits()
{
	DKK::Configurator::registerUnit(new DKK::MNISTCachedReader());
	DKK::Configurator::registerUnit(new DKK::GlobalCostDriver());
	DKK::Configurator::registerUnit(new DKK::SubsampleDriver());

	return;
}
