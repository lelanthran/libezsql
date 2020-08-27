
#ifndef H_EZPLATFORM
#define H_EZPLATFORM

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PLATFORM_POSIX
   int setenv (const char *name, const char *value, int overwrite);
#else
#error Specified platform not supported yet.
#endif

#ifdef __cplusplus
};
#endif


#endif



