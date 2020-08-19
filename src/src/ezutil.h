
#ifndef H_EZUTIL
#define H_EZUTIL

#define PRINTF(...)     do {\
   ezutil_diagnostic (__FILE__, __LINE__, BUILD_TIMESTAMP, __VA_ARGS__);\
} while (0);

#ifdef __cplusplus
extern "C" {
#endif

   void ezutil_diagnostic (const char *srcfile,
                           int         line,
                           const char *build_id,
                           ...);


#ifdef __cplusplus
};
#endif

#endif
