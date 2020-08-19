
#include <stdlib.h>
#include <stdio.h>

#include "ezsql.h"

int main (void)
{
   int ret = EXIT_FAILURE;
   ezsql_t *handle = ezsql_load ("plugin_name.so");

   ret = EXIT_SUCCESS;

errorexit:
   ezsql_unload (handle);
   return ret;
}

