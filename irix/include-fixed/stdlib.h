#ifdef FIXED_STDLIB_H_CHAIN_
// This is a hack for when two fixed stdlib.h's chain to eachother.  Without this, we won't
// actually get the real stdlib.h included!
#include_next <stdlib.h>

#else

#ifndef FIXED_STDLIB_H_
#define FIXED_STDLIB_H_

#ifndef _WCHAR_T
#define _WCHAR_T
#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif
#endif

#define FIXED_STDLIB_H_CHAIN_
#include_next <stdlib.h>
#undef FIXED_STDLIB_H_CHAIN_

#ifdef __cplusplus
extern "C" {
#endif
extern void abort(void) __attribute__ ((__noreturn__));
extern void exit(int) __attribute__ ((__noreturn__));
#ifdef __cplusplus
}
#endif

#endif

#endif
