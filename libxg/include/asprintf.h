/* 
 * libxg asprintf.h
 */

#ifndef _ASPRINTF_H
#define _ASPRINTF_H

#include <stdarg.h>

#ifdef __cplusplus 
extern "C" {
#endif

int vasprintf(char **str, const char *format, va_list ap);

int asprintf(char **str, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
