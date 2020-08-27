
#ifndef H_EZSQL
#define H_EZSQL

#include <stdint.h>
#include <stdbool.h>

typedef struct ezsql_t ezsql_t;
typedef struct ezsql_result_t ezsql_result_t;

enum ezsql_coltype_t {
   ezsql_STRING,
   ezsql_INT8,
   ezsql_UINT8,
   ezsql_INT16,
   ezsql_UINT16,
   ezsql_INT32,
   ezsql_UINT32,
   ezsql_INT64,
   ezsql_UINT64,
   ezsql_TIMESTAMP,
};

#ifdef __cplusplus
extern "C" {
#endif

   ezsql_t *ezsql_load (const char *plugin_so);
   void ezsql_unload (ezsql_t *handle);

   int ezsql_last_errcode (ezsql_t *handle);
   const char *ezsql_last_errmsg (ezsql_t *handle);

   const char *ezsql_plugin_name (ezsql_t *handle);
   const char *ezsql_plugin_version (ezsql_t *handle);
   void *ezsql_connect (ezsql_t *handle, const char *ez_path,
                                         const char *ez_host, uint16_t ez_port,
                                         const char *ez_username, const char *ez_password,
                                         const char *ez_database);

   void ezsql_disconnect (ezsql_t *handle);


   // TODO: These are to be converted to static functions once the correct variadic wrappers
   // are written.
   ezsql_result_t *ezsql_exec (ezsql_t *handle, const char *stmt,
                               size_t                nparams,
                               enum ezsql_coltype_t *param_types,
                               void                **params);

   bool ezsql_res_bind (ezsql_result_t       *res,
                        size_t                nfields,
                        enum ezsql_coltype_t *field_types,
                        void                **fields);

   void ezsql_res_del (ezsql_result_t *res);

#ifdef __cplusplus
};
#endif

#endif

