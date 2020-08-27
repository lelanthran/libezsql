
#include "ezplatform.h"

int ezplatform_setenv (const char *name, const char *value, int overwrite)
{
#ifdef PLATFORM_POSIX
   int setenv (const char *name, const char *value, int overwrite);
   return setenv (name, value, overwrite);
#else
#error Specified platform not supported yet.
#endif
}

