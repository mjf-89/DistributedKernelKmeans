#include "Configurator.h"

#include "PluginFermiWindowsExport.h"
#include "MNISTCachedReader.h"

extern "C" DKK_PLUGIN_FERMI_EXPORT void registerUnits()
{
	DKK::Configurator::registerUnit(new DKK::MNISTCachedReader());

	return;
}
