
#ifndef H_EZUTIL
#define H_EZUTIL

#include <stdbool.h>


#define PRINTF(...)     do {\
   ezutil_diagnostic (__FILE__, __LINE__, BUILD_TIMESTAMP, __VA_ARGS__);\
} while (0);


#define EZENV_FILE           ("ez_path")
#define EZENV_USERNAME       ("ez_username")
#define EZENV_PASSWORD       ("ez_password")
#define EZENV_HOST           ("ez_host")
#define EZENV_PORT           ("ez_port")
#define EZENV_DATABASE       ("ez_database")


#ifdef __cplusplus
extern "C" {
#endif

   // Emit a diagnostic
   void ezutil_diagnostic (const char *srcfile,
                           int         line,
                           const char *build_id,
                           ...);

   // Parse the filename, username, password, host, IP and database
   // name from the specified connection string. The parsed values
   // are stored in environment variables as defined above in the
   // `EZENV_* #defines`.
   //
   bool ezutil_parse_connection_string (const char *connstring);



#ifdef __cplusplus
};
#endif

#endif
