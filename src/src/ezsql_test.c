
#include <stdlib.h>
#include <stdio.h>

#include "ezutil.h"
#include "ezsql.h"

int main (int argc, char **argv)
{
   int ret = EXIT_FAILURE;
   ezsql_t *handle = NULL;

   if (!argv[1]) {
      PRINTF ("No plugin specified\n");
      goto errorexit;
   }

   if (!(handle = ezsql_load (argv[1]))) {
      PRINTF ("Unable to load plugin [%s]\n", argv[1]);
      goto errorexit;
   }

   PRINTF ("Loaded plugin [%s] [%s]\n", ezsql_plugin_name (handle),
                                        ezsql_plugin_version (handle));

   if (!(ezsql_connect (handle, "localhost", 5432, "lelanthran", "a", "lelanthran"))) {
      PRINTF ("Unable to connect to test database\n");
      goto errorexit;
   }

   ret = EXIT_SUCCESS;

errorexit:
   ezsql_unload (handle);
   return ret;
}

