//===-- crtbegin.c - Start of constructors and destructors ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <stddef.h>

#ifdef __sgi
#include <pthread.h>
#endif

__attribute__((visibility("hidden"))) void *__dso_handle = &__dso_handle;

#ifdef EH_USE_FRAME_REGISTRY
__extension__ static void *__EH_FRAME_LIST__[]
    __attribute__((section(".eh_frame"), aligned(sizeof(void *)))) = {};

extern void __register_frame_info(const void *, void *) __attribute__((weak));
extern void *__deregister_frame_info(const void *) __attribute__((weak));
#endif

#ifndef CRT_HAS_INITFINI_ARRAY
typedef void (*fp)(void);

static fp __CTOR_LIST__[]
    __attribute__((section(".ctors"), aligned(sizeof(fp)))) = {(fp)-1};
extern fp __CTOR_LIST_END__[];
#endif

// This is declared here
#ifndef __sgi
extern void __cxa_finalize(void *) __attribute__((weak));
#endif

static void __attribute__((used)) __do_init() {
  static _Bool __initialized;
  if (__builtin_expect(__initialized, 0))
    return;
  __initialized = 1;

#ifdef EH_USE_FRAME_REGISTRY
  static struct { void *p[8]; } __object;
  if (__register_frame_info)
    __register_frame_info(__EH_FRAME_LIST__, &__object);
#endif
#ifndef CRT_HAS_INITFINI_ARRAY
  const size_t n = __CTOR_LIST_END__ - __CTOR_LIST__ - 1;
  for (size_t i = n; i >= 1; i--) __CTOR_LIST__[i]();
#endif
}

#ifdef CRT_HAS_INITFINI_ARRAY
__attribute__((section(".init_array"),
               used)) static void (*__init)(void) = __do_init;
#elif defined(__i386__) || defined(__x86_64__)
__asm__(".pushsection .init,\"ax\",@progbits\n\t"
    "call " __USER_LABEL_PREFIX__ "__do_init\n\t"
    ".popsection");
#elif defined(__riscv)
__asm__(".pushsection .init,\"ax\",%progbits\n\t"
        "call " __USER_LABEL_PREFIX__ "__do_init\n\t"
        ".popsection");
#elif defined(__arm__) || defined(__aarch64__)
__asm__(".pushsection .init,\"ax\",%progbits\n\t"
    "bl " __USER_LABEL_PREFIX__ "__do_init\n\t"
    ".popsection");
#elif defined(__powerpc__) || defined(__powerpc64__)
__asm__(".pushsection .init,\"ax\",@progbits\n\t"
    "bl " __USER_LABEL_PREFIX__ "__do_init\n\t"
    "nop\n\t"
    ".popsection");
#elif defined(__sparc__)
__asm__(".pushsection .init,\"ax\",@progbits\n\t"
    "call " __USER_LABEL_PREFIX__ "__do_init\n\t"
    ".popsection");
#elif defined(__sgi)
// IRIX crt1.o/crtn.o begin and end an __istart function
// in the .init section, which gets called from __start.
// We want our own DT_INIT/DT_FINI, so we define an _init
// and _fini, which the dynamic linker will call before
// it calls __start -- and more importantly will call
// for shared libraries, unlike __istart.
void
    __attribute__((section(".gcc_init")))
    __attribute__((used))
_init()
{
  __do_init();
}
#else
#error "crtbegin without .init_fini array unimplemented for this architecture"
#endif // CRT_HAS_INITFINI_ARRAY

#ifndef CRT_HAS_INITFINI_ARRAY
static fp __DTOR_LIST__[]
    __attribute__((section(".dtors"), aligned(sizeof(fp)))) = {(fp)-1};
extern fp __DTOR_LIST_END__[];
#endif

#ifdef __sgi
/*
 * On IRIX, we'd like to have __cxa_atexit since the OS atexit has the
 * standard POSIX 32 item limit.  We implement a hacky version here.
 */
typedef void (*__cxa_atexit_fn)(void *);

// TODO make this more optimal; maybe have a smaller initial value,
// and then a bigger growth value
#define __CXA_ATEXIT_NUM 256
struct __cxa_atexit_chunk {
    int count;
    struct __cxa_atexit_chunk *next;
    __cxa_atexit_fn funs[__CXA_ATEXIT_NUM];
    void *args[__CXA_ATEXIT_NUM];
    void *dsos[__CXA_ATEXIT_NUM];
};

// Delcare some things from libc that we may use, if they're available.
void* malloc(size_t size) __attribute__((weak));
void free(void *ptr) __attribute__((weak));
int pthread_mutex_lock(pthread_mutex_t *) __attribute__((weak));
int pthread_mutex_unlock(pthread_mutex_t *) __attribute__((weak));

static __attribute__((visibility("hidden"))) struct __cxa_atexit_chunk  __cxa_atexit_list_init = { 0 };
static __attribute__((visibility("hidden"))) struct __cxa_atexit_chunk *__cxa_atexit_list = &__cxa_atexit_list_init;
static __attribute__((visibility("hidden"))) pthread_mutex_t            __cxa_atexit_mutex = PTHREAD_MUTEX_INITIALIZER;

// __cxa_atexit needs to be safe to call concurrently.  We're going to assume
// that this is not performance critical and will just use a regular pthread mutex.
// We should really assert that we're never called with a NULL dso.
int __cxa_atexit(void (*func)(void *), void *arg, void *dso) {
    if (pthread_mutex_lock && pthread_mutex_lock(&__cxa_atexit_mutex) != 0)
        return -1;

    if (__cxa_atexit_list->count == __CXA_ATEXIT_NUM) {
        // if we can't malloc, we can't add more entries
        if (!malloc) {
            if (pthread_mutex_unlock)
                pthread_mutex_unlock(&__cxa_atexit_mutex);
            return -1;
        }

        struct __cxa_atexit_chunk *new_chunk = (struct __cxa_atexit_chunk *)
            malloc(sizeof(struct __cxa_atexit_chunk));
        if (!new_chunk)
            return -1;
        new_chunk->count = 0;
        new_chunk->next = __cxa_atexit_list;
        __cxa_atexit_list = new_chunk;
    }
    const int index = __cxa_atexit_list->count++;
    __cxa_atexit_list->funs[index] = func;
    __cxa_atexit_list->args[index] = arg;
    __cxa_atexit_list->dsos[index] = dso;

    if (pthread_mutex_unlock)
        pthread_mutex_unlock(&__cxa_atexit_mutex);

    return 0;
}

void __cxa_finalize(void *dso) {
    struct __cxa_atexit_chunk *chunk = __cxa_atexit_list;
    while (chunk) {
        for (int i = chunk->count-1; i >= 0; i--) {
            // TODO: any dso == NULL calls will get cleared on the first
            // shlib unload, when these should be treated like atexit().
            // Not sure if that's correct?
            if (chunk->dsos[i] == NULL || chunk->dsos[i] == dso)
            {
                chunk->funs[i](chunk->args[i]);
                chunk->dsos[i] = (void*) -1;
            }
        }
        struct __cxa_atexit_chunk *next = chunk->next;
        //if (free && next) free(chunk); // initial chunk is static
        chunk = next;
    }

    //__cxa_atexit_list = &__cxa_atexit_list_init;
    //__cxa_atexit_list->count = 0;
}
#endif

static void __attribute__((used)) __do_fini() {
  static _Bool __finalized;
  if (__builtin_expect(__finalized, 0))
    return;
  __finalized = 1;

  if (__cxa_finalize)
    __cxa_finalize(__dso_handle);

#ifndef CRT_HAS_INITFINI_ARRAY
  const size_t n = __DTOR_LIST_END__ - __DTOR_LIST__ - 1;
  for (size_t i = 1; i <= n; i++) __DTOR_LIST__[i]();
#endif
#ifdef EH_USE_FRAME_REGISTRY
  if (__deregister_frame_info)
    __deregister_frame_info(__EH_FRAME_LIST__);
#endif
}

#ifdef CRT_HAS_INITFINI_ARRAY
__attribute__((section(".fini_array"),
               used)) static void (*__fini)(void) = __do_fini;
#elif defined(__i386__) || defined(__x86_64__)
__asm__(".pushsection .fini,\"ax\",@progbits\n\t"
    "call " __USER_LABEL_PREFIX__ "__do_fini\n\t"
    ".popsection");
#elif defined(__arm__) || defined(__aarch64__)
__asm__(".pushsection .fini,\"ax\",%progbits\n\t"
    "bl " __USER_LABEL_PREFIX__ "__do_fini\n\t"
    ".popsection");
#elif defined(__powerpc__) || defined(__powerpc64__)
__asm__(".pushsection .fini,\"ax\",@progbits\n\t"
    "bl " __USER_LABEL_PREFIX__ "__do_fini\n\t"
    "nop\n\t"
    ".popsection");
#elif defined(__riscv)
__asm__(".pushsection .fini,\"ax\",@progbits\n\t"
        "call " __USER_LABEL_PREFIX__ "__do_fini\n\t"
        ".popsection");
#elif defined(__sparc__)
__asm__(".pushsection .fini,\"ax\",@progbits\n\t"
    "call " __USER_LABEL_PREFIX__ "__do_fini\n\t"
    ".popsection");
#elif defined(__sgi)
void
    __attribute__((section(".gcc_fini")))
    __attribute__((used))
_fini()
{
  __do_fini();
}
#else
#error "crtbegin without .init_fini array unimplemented for this architecture"
#endif  // CRT_HAS_INIT_FINI_ARRAY
