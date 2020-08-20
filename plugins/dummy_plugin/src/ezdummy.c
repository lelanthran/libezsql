
#include <stdint.h>
#include <stdlib.h>


#include "ezdummy.h"


const char *plugin_name (void)
{
   return "Dummy Plugin";
}

const char *plugin_version (void)
{
   return ezsql_dummy_version;
}

void *plugin_connect (const char *host, uint16_t port,
                    const char *username, const char *password,
                    const char *database)
{
   return NULL;
}
