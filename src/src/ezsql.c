#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdbool.h>

#include <dlfcn.h>

#include "ezutil.h"
#include "ezsql.h"

struct ezsql_t {
   void *libh;
};

ezsql_t *ezsql_load (const char *plugin_so)
{
   bool error = true;
   ezsql_t *ret = NULL;

   if (!(ret = calloc (1, sizeof *ret))) {
      PRINTF ("OOM error\n");
      goto errorexit;
   }

   ret->libh = dlopen (plugin_so, RTLD_NOW);
   if (!ret->libh) {
      PRINTF ("Error loading [%s]: %s\n", plugin_so, dlerror ());
      goto errorexit;
   }

   error = true;
errorexit:
   if (error) {
      ezsql_unload (ret);
      ret = NULL;
   }
   return ret;
}

void ezsql_unload (ezsql_t *handle)
{

}


