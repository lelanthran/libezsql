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
   void *(*fptr_connect) (const char *ez_path,
                          const char *ez_host, uint16_t ez_port,
                          const char *ez_username, const char *ez_password,
                          const char *ez_database);

#define SYMB_DISCONNECT ("plugin_disconnect")
   void (*fptr_disconnect) (void *handle);

#define SYMB_LAST_ERRCODE ("plugin_last_errcode")
   int (*fptr_last_errcode) (void *handle);

#define SYMB_LAST_ERRMSG ("plugin_last_errmsg")
   const char *(*fptr_last_errmsg) (void *handle);

#define SYMB_EXEC ("plugin_exec")
   void * (*fptr_exec) (void *handle, const char           *stmt,
                                      size_t                nparams,
                                      enum ezsql_coltype_t *param_types,
                                      void                **params);

#define SYMB_RES_DEL ("plugin_res_del")
   void (*fptr_res_del) (void *result);

#define SYMB_RES_BIND ("plugin_res_bind")
   int (*fptr_res_bind) (void *result, size_t nfields,
                                       enum ezsql_coltype_t *field_types,
                                       void                **fields);

};

struct ezsql_result_t {
   ezsql_t *handle;
   void    *result;
};

static void res_del (ezsql_result_t *res)
{
   if (res) {
      res->handle->fptr_res_del (res->result);
      free (res);
   }
}

static ezsql_result_t *res_new (void *handle, void *result)
{
   ezsql_result_t *ret = NULL;

   if (!(ret = calloc (1, sizeof *ret)))
      return NULL;

   ret->handle = handle;
   ret->result = result;

   return ret;
}

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
   LOAD_SYMBOL (fptr_disconnect, SYMB_DISCONNECT);
   LOAD_SYMBOL (fptr_last_errcode, SYMB_LAST_ERRCODE);
   LOAD_SYMBOL (fptr_last_errmsg, SYMB_LAST_ERRMSG);
   LOAD_SYMBOL (fptr_exec, SYMB_EXEC);
   LOAD_SYMBOL (fptr_res_del, SYMB_EXEC);
   LOAD_SYMBOL (fptr_res_bind, SYMB_RES_BIND);

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

int ezsql_last_errcode (ezsql_t *handle)
{
   return handle==NULL ? -1
                       : handle->fptr_last_errcode (handle->dbconn);
}

const char *ezsql_last_errmsg (ezsql_t *handle)
{
   return handle==NULL ? "Handle is NULL"
                       : handle->fptr_last_errmsg (handle->dbconn);
}

const char *ezsql_plugin_name (ezsql_t *handle)
{
   return handle->fptr_name ();
}

const char *ezsql_plugin_version (ezsql_t *handle)
{
   return handle->fptr_version ();
}

void *ezsql_connect (ezsql_t *handle, const char *ez_path,
                                      const char *ez_host, uint16_t ez_port,
                                      const char *ez_username, const char *ez_password,
                                      const char *ez_database)
{
   return handle->dbconn = handle->fptr_connect (ez_path,
                                                 ez_host, ez_port,
                                                 ez_username, ez_password,
                                                 ez_database);
}

void ezsql_disconnect (ezsql_t *handle)
{
   if (!handle)
      return;

   handle->fptr_disconnect (handle->dbconn);
}

ezsql_result_t *ezsql_exec (ezsql_t *handle, const char *stmt,
                            size_t                nparams,
                            enum ezsql_coltype_t *param_types,
                            void                **params)
{
   if (!handle)
      return NULL;

   return res_new (handle,
                   handle->fptr_exec (handle->dbconn,
                                      stmt,
                                      nparams,
                                      param_types,
                                      params));
}

void ezsql_res_del (ezsql_result_t *res)
{
   if (res)
      res_del (res);
}

bool ezsql_res_bind (ezsql_result_t       *res,
                     size_t                nfields,
                     enum ezsql_coltype_t *field_types,
                     void                **fields)
{
   if (!res)
      return false;

   return res->handle->fptr_res_bind (res, nfields, field_types, fields);
}

