#ifdef FIXED_SYS_ENDIAN_H_CHAIN_
// This is a hack for when two fixed headers chain to eachother.  Without this, we won't
// actually get the real header included!
#include_next <sys/endian.h>

#else

#ifndef FIXED_SYS_ENDIAN_H_
#define FIXED_SYS_ENDIAN_H_

#include <standards.h>

#define FIXED_SYS_ENDIAN_H_CHAIN_
#include_next <sys/endian.h>
#undef FIXED_SYS_ENDIAN_H_CHAIN_

#endif

#endif
