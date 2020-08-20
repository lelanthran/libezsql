#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdbool.h>

#include <dlfcn.h>

#include "ezutil.h"
#include "ezsql.h"

struct ezsql_t {
   // The librrary handle
   void *libh;

   // The opaque pointer to the db implementation
   void *dbconn;

   // All the functions that must be defined by the plugin
#define SYMB_NAME       ("plugin_name")
   const char * (*fptr_name) (void);

#define SYMB_VERSION    ("plugin_version")
   const char * (*fptr_version) (void);

#define SYMB_CONNECT    ("plugin_connect")
   void *(*fptr_connect) (const char *host, uint16_t port,
                          const char *username, const char *password,
                          const char *database);
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
   LOAD_SYMBOL (fptr_connect, SYMB_CONNECT);

#undef LOAD_SYMBOL

   error = false;
errorexit:
   if (error) {
      ezsql_unload (ret);
      ret = NULL;
   }
   return ret;
}

void ezsql_unload (ezsql_t *handle)
{
   if (!handle)
      return;

   if (handle->libh)
      dlclose (handle->libh);

   free (handle);
}

const char *ezsql_plugin_name (ezsql_t *handle)
{
   return handle->fptr_name ();
}

const char *ezsql_plugin_version (ezsql_t *handle)
{
   return handle->fptr_version ();
}

void *ezsql_connect (ezsql_t *handle, const char *host, uint16_t port,
                                      const char *username, const char *password,
                                      const char *database)
{
   return handle->dbconn = handle->fptr_connect (host, port, username, password, database);
}


