#!/bin/bash

if [ "$CC" == "" ] ; then
    CC="./bin/clang"
fi

CFLAGS="-Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wmissing-field-initializers -Wimplicit-fallthrough -Wcovered-switch-default -Wno-comment -Wstring-conversion -Wmisleading-indentation -Wno-pedantic -fdiagnostics-color -ffunction-sections -fdata-sections -Wall -fno-lto -std=c11 -fPIC -DEH_USE_FRAME_REGISTRY"
RTDIR=lib32/clang/14.0.0/lib/irix6.5

mkdir -p $RTDIR

set -e
set -x

${CC} ${CFLAGS} -mips3 -mabi=n32 -c ../compiler-rt/lib/crt/crtbegin.c -o ${RTDIR}/clang_rt.crtbegin32.o
${CC} ${CFLAGS} -mips3 -mabi=n32 -c ../compiler-rt/lib/crt/crtend.c   -o ${RTDIR}/clang_rt.crtend32.o
${CC} ${CFLAGS} -mips4 -mabi=64  -c ../compiler-rt/lib/crt/crtbegin.c -o ${RTDIR}/clang_rt.crtbegin64.o
${CC} ${CFLAGS} -mips4 -mabi=64  -c ../compiler-rt/lib/crt/crtend.c   -o ${RTDIR}/clang_rt.crtend64.o
