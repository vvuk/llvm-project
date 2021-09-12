#ifndef _POSIX_MEMALIGN_H
#define _POSIX_MEMALIGN_H

#ifdef __cplusplus
extern "C" {
#endif

int posix_memalign (void **memptr, size_t alignment, size_t size);

#ifdef __cplusplus
}
#endif

#endif
