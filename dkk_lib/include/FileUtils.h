#ifndef DKK_FILEUTILS_H
#define DKK_FILEUTILS_H

#include "FileUtils.h"
#include "Config.h"
#include "Types.h"

int dkk_fseek(FILE *stream, DKK_TYPE_OFF offset, int whence);
int dkk_ftell(FILE *stream);

#endif
