#ifndef DKK_FILEUTILS_H
#define DKK_FILEUTILS_H

#include "FileUtils.h"
#include "Config.h"
#include "Types.h"

#include "WindowsExport.h"

namespace DKK{
	DKK_EXPORT int dkk_fseek(FILE *stream, DKK_TYPE_OFF offset, int whence);
	DKK_EXPORT DKK_TYPE_OFF dkk_ftell(FILE *stream);
}

#endif
