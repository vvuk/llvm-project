#ifndef _LIBCPP_SUPPORT_IRIX_STDDEF_H
#define _LIBCPP_SUPPORT_IRIX_STDDEF_H

#include <sys/ptimers.h>
#include <strerror_r.h>
#include <posix_memalign.h>

#define CLOCK_MONOTONIC CLOCK_SGI_CYCLE

#ifdef __cplusplus 
#define _WCHAR_T

#include_next <stddef.h>
#include <wctype.h>

// For whatever reason, iswblank is stored as __iswblank inside 
// IRIX's libc.so

#define iswblank __iswblank

#endif

#endif