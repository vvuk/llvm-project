/* 
 * libxg setenv.h
 */

#ifndef _SETENV_H
#define _SETENV_H

#ifdef __cplusplus
extern "C" {
#endif

int setenv(const char *name, const char *value, int rewrite);
void unsetenv(const char *name);

#ifdef __cplusplus
}
#endif


#endif
