/*
 * libxg endian.h
 * derived from NetBSD's endian.h 
 */

/*
 * Copyright (c) 1987, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef _ENDIAN_H_
#define _ENDIAN_H_


#include <standards.h> 
#include <sys/endian.h> 
#include <netinet/in.h>
#include <inttypes.h>
#include <sys/bswap.h>


#define htobe16(x)	(x)
#define htobe32(x)	(x)
#define htobe64(x)	(x)

#define htole16(x)	bswap16(uint16_t, (x))
#define htole32(x)	bswap32(uint32_t, (x))
#define htole64(x)	bswap64(uint64_t, (x))

#define HTOBE16(x)	(void, (x))
#define HTOBE32(x)	(void, (x))
#define HTOBE64(x)	(void, (x))

#define HTOLE16(x)  (x) = bswap16(uint16_t, (x))
#define HTOLE32(x)  (x) = bswap32(uint32_t, (x))
#define HTOLE64(x)  (x) = bswap64(uint64_t, (x))

#define be16toh(x)	htobe16(x)
#define be32toh(x)	htobe32(x)
#define be64toh(x)	htobe64(x)
#define le16toh(x)	htole16(x)
#define le32toh(x)	htole32(x)
#define le64toh(x)	htole64(x)

#define BE16TOH(x)	HTOBE16(x)
#define BE32TOH(x)	HTOBE32(x)
#define BE64TOH(x)	HTOBE64(x)
#define LE16TOH(x)	HTOLE16(x)
#define LE32TOH(x)	HTOLE32(x)
#define LE64TOH(x)	HTOLE64(x)

#endif
