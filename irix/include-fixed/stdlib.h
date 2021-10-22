#ifndef FIXED_STDLIB_H_
#define FIXED_STDLIB_H_

#ifndef _WCHAR_T
#define _WCHAR_T
#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif
#endif

#include_next <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
extern void abort(void) __attribute__ ((__noreturn__));
extern void exit(int) __attribute__ ((__noreturn__));
#ifdef __cplusplus
}
#endif

#endif
