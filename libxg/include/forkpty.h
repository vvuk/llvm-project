/* 
 * libxg forkpty.h
 */

#ifndef _FORKPTY_H
#define _FORKPTY_H

#ifdef __cplusplus 
extern "C" {
#endif

pid_t forkpty(int *master, char *name, struct termios *tio, struct winsize *ws);

#ifdef __cplusplus
}
#endif

#endif
