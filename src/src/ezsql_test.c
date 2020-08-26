
#include <stdlib.h>
#include <stdio.h>

#include "ezutil.h"
#include "ezsql.h"

int main (int argc, char **argv)
{
   int ret = EXIT_FAILURE;
   ezsql_t *handle = NULL;

   if (!argv[1]) {
      PRINTF ("No plugin specified, aborting\n");
      goto errorexit;
   }

   if (!argv[2]) {
      PRINTF ("No connection string provided, aborting\n");
      goto errorexit;
   }

   // Set the default connection string. If the user leaves
   // out a field we'll get the ones set here.
   ezutil_parse_connection_string (" ez_file=./testfile"
                                   " ez_user=lelanthran"
                                   " ez_password=a"
                                   " ez_host=localhost"
                                   " ez_port=5432"
                                   " ez_database=eztest");

   if (!(ezutil_parse_connection_string (argv[2]))) {
      PRINTF ("Error parsing connection string [%s]\n" ,argv[2]);
      goto errorexit;
   }

   if (!(handle = ezsql_load (argv[1]))) {
      PRINTF ("Unable to load plugin [%s]\n", argv[1]);
      goto errorexit;
   }

   PRINTF ("Loaded plugin [%s] [%s]\n", ezsql_plugin_name (handle),
                                        ezsql_plugin_version (handle));

   if (!(ezsql_connect (handle, "path", "localhost", 5432, "lelanthran", "a", "lelanthran"))) {
      PRINTF ("Unable to connect to test database\n");
      goto errorexit;
   }

   ret = EXIT_SUCCESS;

   errorexit:
   ezsql_disconnect (handle);
   ezsql_unload (handle);
   return ret;
}

