/*	$NetBSD: strerror_r.c,v 1.5 2020/03/25 16:15:41 kre Exp $	*/

/*
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
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
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>	/* for sys_nerr on FreeBSD */

int
_strerror_lr(int num, char *buf, size_t buflen)
{
#define	UPREFIX	"Unknown error: %d"
	unsigned int errnum = num;
	int retval = 0;
	size_t slen;
	int saved_errno = errno;

	if (errnum < (unsigned int) sys_nerr) {
		slen = strlcpy(buf, sys_errlist[errnum], buflen); 
	} else {
		slen = snprintf(buf, buflen, UPREFIX, num);
		retval = EINVAL;
	}

	if (slen >= buflen)
		retval = ERANGE;

	errno = saved_errno;

	return retval;
}

int
strerror_r(int num, char *buf, size_t buflen)
{
	return _strerror_lr(num, buf, buflen);
}
