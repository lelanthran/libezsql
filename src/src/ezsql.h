
#ifndef H_EZSQL
#define H_EZSQL

#include <stdint.h>

typedef struct ezsql_t ezsql_t;

#ifdef __cplusplus
extern "C" {
#endif

   ezsql_t *ezsql_load (const char *plugin_so);
   void ezsql_unload (ezsql_t *handle);


   const char *ezsql_plugin_name (ezsql_t *handle);
   const char *ezsql_plugin_version (ezsql_t *handle);
   void *ezsql_connect (ezsql_t *handle, const char *ez_path,
                                         const char *ez_host, uint16_t ez_port,
                                         const char *ez_username, const char *ez_password,
                                         const char *ez_database);

#ifdef __cplusplus
};
#endif

#endif

