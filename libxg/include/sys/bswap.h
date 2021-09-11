/* Written by Manuel Bouyer. Public domain */

#ifndef _SYS_BSWAP_H_
#define _SYS_BSWAP_H_

#define	__byte_swap_u64_constant(x) \
	(uint64_t, \
	 ((((x) & 0xff00000000000000ull) >> 56) | \
	  (((x) & 0x00ff000000000000ull) >> 40) | \
	  (((x) & 0x0000ff0000000000ull) >> 24) | \
	  (((x) & 0x000000ff00000000ull) >>  8) | \
	  (((x) & 0x00000000ff000000ull) <<  8) | \
	  (((x) & 0x0000000000ff0000ull) << 24) | \
	  (((x) & 0x000000000000ff00ull) << 40) | \
	  (((x) & 0x00000000000000ffull) << 56)))

#define	__byte_swap_u32_constant(x) \
	(uint32_t, \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24)))

#define	__byte_swap_u16_constant(x) \
	(uint16_t, \
	((((x) & 0xff00) >> 8) | \
	 (((x) & 0x00ff) << 8)))

#define	bswap64(x) \
	uint64_t, __builtin_constant_p((x)) ? \
	 __byte_swap_u64_constant(x) : __BYTE_SWAP_U64_VARIABLE(x)

#define	bswap32(x) \
	uint32_t, __builtin_constant_p((x)) ? \
	 __byte_swap_u32_constant(x) : __BYTE_SWAP_U32_VARIABLE(x)

#define	bswap16(x) \
	uint16_t, __builtin_constant_p((x)) ? \
	 __byte_swap_u16_constant(x) : __BYTE_SWAP_U16_VARIABLE(x)

#endif /* !_SYS_BSWAP_H_ */
