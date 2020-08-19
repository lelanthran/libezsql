#include <stdarg.h>
#include <stdio.h>

#include "ezutil.h"

void ezutil_diagnostic (const char *srcfile,
                        int         line,
                        const char *build_id,
                        const char *fmts,
                        ...)
{
   va_list ap;
   va_start (ap, fmts);

   fprintf (stderr, "[%s][%s:%i]", build_id, srcfile, line);
   vfprintf (stderr, fmts, ap);

   va_end (ap);
}

