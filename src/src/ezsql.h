
#ifndef H_EZSQL
#define H_EZSQL

typedef struct ezsql_t ezsql_t;

#ifdef __cplusplus
extern "C" {
#endif

   ezsql_t *ezsql_load (const char *plugin_so);
   void ezsql_unload (ezsql_t *handle);


#ifdef __cplusplus
};
#endif

#endif

