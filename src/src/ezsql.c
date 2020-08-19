#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdbool.h>

#include <dlfcn.h>

#include "ezutil.h"
#include "ezsql.h"

struct ezsql_t {
   void *libh;

   // All the functions that must be defined by the plugin
#define SYMB_NAME       ("plugin_name")
   const char *fptr_name;
#define SYMB_VERSION    ("plugin_version")
   const char *fptr_version;
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

#define LOAD_SYMBOL(dst,name) \
   if (!(ret->dst = dlsym (ret->libh, name))) { \
      PRINTF ("Failed to find symbol [%s] in plugin [%s]\n", name, plugin_so); \
      goto errorexit; \
   } \

   LOAD_SYMBOL (fptr_name, SYMB_NAME);
   LOAD_SYMBOL (fptr_version, SYMB_VERSION);

#undef LOAD_SYMBOL

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


