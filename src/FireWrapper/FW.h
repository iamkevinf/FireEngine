#ifndef __FW_H__
#define __FW_H__

#if defined _WIN32 || defined __CYGWIN__
#define API __declspec(dllexport)
#ifndef __GNUC__
#endif
#else
#ifdef __GNUC__
#define API  __attribute__((__visibility__("default")))
#else
#define API
#endif
#endif

#if defined __cplusplus
#define EXTERN extern "C"
#else
#include <stdarg.h>
#include <stdbool.h>
#define EXTERN extern
#endif

#define FW_API EXTERN API


#endif // __FW_H__