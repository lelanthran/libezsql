
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#include "ezdummy.h"

#include "ds_str.h"

#define PRINTF(...)     do {\
   printf ("[%s:%s:%i:%s()]", BUILD_TIMESTAMP, __FILE__, __LINE__, __func__);\
   printf (__VA_ARGS__);\
} while (0);


struct dummy_db_t {
   char        *path;
   char        *host;
   uint16_t     port;
   char        *username;
   char        *password;
   char        *database;
};

static void dummy_del (struct dummy_db_t *db)
{
   if (db) {
      free (db->path);
      free (db->host);
      free (db->username);
      free (db->password);
      free (db->database);
      free (db);
   }
}

static struct dummy_db_t *dummy_new (const char *path,
                                     const char *host, uint16_t port,
                                     const char *username, const char *password,
                                     const char *database)
{
   struct dummy_db_t *ret = calloc (1, sizeof *ret);
   if (!ret)
      return NULL;

   ret->path     = ds_str_dup (path);
   ret->host     = ds_str_dup (host);
   ret->port     = port;
   ret->username = ds_str_dup (username);
   ret->password = ds_str_dup (password);
   ret->database = ds_str_dup (database);

   if (!ret->path || !ret->host || !ret->port ||
       !ret->username || !ret->password || !ret->database) {
      dummy_del (ret);
      ret = NULL;
   }

   return ret;
}

const char *plugin_name (void)
{
   PRINTF ("Returning a plugin name\n");
   return "Dummy Plugin";
}

const char *plugin_version (void)
{
   PRINTF ("Returning a plugin version [%s]\n", ezsql_dummy_version);
   return ezsql_dummy_version;
}

void *plugin_connect (const char *path,
                      const char *host, uint16_t port,
                      const char *username, const char *password,
                      const char *database)
{
   PRINTF ("Pretending to connect to dummy database\n"
           "   path           [%s]\n"
           "   host           [%s:%hu]\n"
           "   credentials    [%s:%s]\n"
           "   database       [%s]\n",
               path, host, port, username, password, database);
   return dummy_new (path, host, port, username, password, database);;
}

void plugin_disconnect (void *handle)
{
   struct dummy_db_t *tmp = handle;

   PRINTF ("Disconnecting dummy database\n");
   dummy_del (tmp);
}
