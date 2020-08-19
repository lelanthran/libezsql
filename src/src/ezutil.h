
#ifndef H_EZUTIL
#define H_EZUTIL

#define PRINTF(fmts,...)     do {\
   ezutil_diagnostic (__FILE__, __LINE__, BUILD_TIMESTAMP, fmts, __VA_ARGS__);\
} while (0);

#ifdef __cplusplus
extern "C" {
#endif

   void ezutil_diagnostic (const char *srcfile,
                           int         line,
                           const char *build_id,
                           const char *fmts,
                           ...);


#ifdef __cplusplus
};
#endif

#endif
