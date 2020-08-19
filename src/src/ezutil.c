#include <stdarg.h>
#include <stdio.h>

#include "ezutil.h"

void ezutil_diagnostic (const char *srcfile,
                        int         line,
                        const char *build_id,
                        ...)
{
   va_list ap;
   const char *fmts = "";
   va_start (ap, build_id);

   fmts = va_arg (ap, const char *);

   fprintf (stderr, "[%s][%s:%i]", build_id, srcfile, line);
   vfprintf (stderr, fmts, ap);

   va_end (ap);
}

