//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_SUPPORT_IRIX_XLOCALE_H
#define _LIBCPP_SUPPORT_IRIX_XLOCALE_H

#if defined(__sgi)

#include <cstdlib>
#include <clocale>
#include <cwctype>
#include <ctype.h>
#include <cstdarg>

#include <__support/xlocale/__nop_locale_mgmt.h>

static inline 
size_t wcsnrtombs(char *__dest, const wchar_t **__src, size_t __nwc,
    size_t __len, mbstate_t *__ps)
{
  return NULL;
}

static inline
size_t mbsnrtowcs(wchar_t * __dest, const char **__src, size_t __nms,
    size_t __len, mbstate_t *__ps)
{
  return NULL;
}

#include <__support/xlocale/__posix_l_fallback.h>
#include <__support/xlocale/__strtonum_fallback.h>

static inline
int vasprintf(char **strp, const char *fmt, va_list ap)
{
  const size_t buff_size = 256;
  int str_size;
  if ((*strp = (char *)malloc(buff_size)) == NULL)
  {
    return -1;
  }
  if ((str_size = vsnprintf(*strp, buff_size, fmt,  ap)) >= buff_size)
  {
    if ((*strp = (char *)realloc(*strp, str_size + 1)) == NULL)
    {
      return -1;
    }
    str_size = vsnprintf(*strp, str_size + 1, fmt,  ap);
  }
  return str_size;
}

#endif // __sgi

#endif // _LIBCPP_SUPPORT_IRIX_XLOCALE_H