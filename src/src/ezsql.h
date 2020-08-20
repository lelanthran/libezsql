
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
   void *ezsql_connect (ezsql_t *handle, const char *host, uint16_t port,
                                         const char *username, const char *password,
                                         const char *database);

#ifdef __cplusplus
};
#endif

#endif

