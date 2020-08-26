#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "ezutil.h"
#include "ezplatform.h"

#include "ds_str.h"

void ezutil_diagnostic (const char *srcfile,
                        int         line,
                        const char *build_id,
                        ...)
{
   va_list ap;
   const char *fmts = "";
   va_start (ap, build_id);

   fmts = va_arg (ap, const char *);

   fprintf (stderr, "[%s][%s:%i] | ", build_id, srcfile, line);
   vfprintf (stderr, fmts, ap);

   va_end (ap);
}

static char *find_first_whitespace (char *src)
{
   while (src && *src) {
      if (isspace (*src))
         return src;
      src++;
   }
   return src;
}

bool ezutil_parse_connection_string (const char *connstring)
{
   bool error = true;
   char *tmp = NULL;

   static const char *fields[] = {
      EZENV_FILE,
      EZENV_USERNAME,
      EZENV_PASSWORD,
      EZENV_HOST,
      EZENV_PORT,
      EZENV_DATABASE,
      NULL,
   };

   if (!(tmp = ds_str_dup (connstring)))
      goto errorexit;

   for (size_t i=0; fields[i]; i++) {
      char *name = strstr (tmp, fields[i]);

      if (!name)
         continue;

      char *value = NULL;
      char *delim = &name[strlen (fields[i])];
      if (delim && *delim == '=') {
         value = &delim[1];
         *delim = 0;
       } else {
         value = "";
      }


      char *eov = find_first_whitespace (delim);
      char tmp_eov = 0;
      if (eov) {
         tmp_eov = *eov;
         *eov = 0;
      }

      if (tmp_eov) {
         *eov = tmp_eov;
      }
      if (delim && *delim == 0)
         *delim = '=';

      ezplatform_setenv (name, value, 1);
   }

   error = false;
errorexit:
   free (tmp);
   return !error;
}
