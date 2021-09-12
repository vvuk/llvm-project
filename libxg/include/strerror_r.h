#ifndef _STRERROR_R_H
#define _STRERROR_R_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int strerror_r(int num, char *buf, size_t buflen);

#ifdef __cplusplus
}
#endif

#endif
