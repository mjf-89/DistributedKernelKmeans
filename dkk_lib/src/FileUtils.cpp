#include "Config.h"
#include "Types.h"

int dkk_fseek(FILE *stream, DKK_TYPE_OFF offset, int whence)
{
#ifdef HAVE_FSEEKO
	return fseeko(stream, offset, whence);
#else
#ifdef HAVE__FSEEKI64
	return _fseeki64(stream, offset, whence);
#else
	return fseek(stream, offset, whence);
#endif
#endif
}

DKK_TYPE_OFF dkk_ftell(FILE *stream)
{
#ifdef HAVE_FSEEKO
	return ftello(stream);
#else
#ifdef HAVE__FSEEKI64
	return _ftelli64(stream);
#else
	return ftell(stream);
#endif
#endif
}
