/* 
 * libxg strtonum.h
 */

#ifndef _STRTONUM_H
#define _STRTONUM_H

#ifdef __cplusplus 
extern "C" {
#endif

long long strtonum(const char *numstr, long long minval, long long maxval, const char **errstrp);

#ifdef __cplusplus
}
#endif

#endif
