#!/bin/bash

: "${LLVMVER:=14.0.6}"

RELDIR=$(dirname $0)/..
if [ ! -f ${RELDIR}/llvm/CMakeLists.txt ] ; then
    echo bad RELDIR ${RELDIR}
    exit 1
fi

if [ "$CC" == "" ] ; then
    if [ -f "./bin/clang" ] ; then
        CC="./bin/clang"
    elif [ -f "${RELDIR}/build-full/bin/clang" ] ; then
        CC="${RELDIR}/build-full/bin/clang"
    else
        echo "Where is clang? Set CC"
        exit 1
    fi
fi

CFLAGS="-Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wmissing-field-initializers -Wimplicit-fallthrough -Wcovered-switch-default -Wno-comment -Wstring-conversion -Wmisleading-indentation -Wno-pedantic -fdiagnostics-color -ffunction-sections -fdata-sections -Wall -fno-lto -std=c11 -fPIC -DEH_USE_FRAME_REGISTRY"

#RTDIR=lib32/clang/${LLVMVER}/lib/irix6.5
RTDIR=lib32/clang/${LLVMVER}/lib

mkdir -p $RTDIR/mips64-sgi-irix6.5{,-gnuabin32}

set -e
set -x

## These _must_ be named with a -xxx. 
##${CC} ${CFLAGS} -mips3 -mabi=o32 -c ${RELDIR}/compiler-rt/lib/crt/crtbegin.c -o ${RTDIR}/clang_rt.crtbegin.o
##${CC} ${CFLAGS} -mips3 -mabi=o32 -c ${RELDIR}/compiler-rt/lib/crt/crtend.c   -o ${RTDIR}/clang_rt.crtend.o
#${CC} ${CFLAGS} -mips3 -mabi=n32 -c ${RELDIR}/compiler-rt/lib/crt/crtbegin.c -o ${RTDIR}/clang_rt.crtbegin-32.o
#${CC} ${CFLAGS} -mips3 -mabi=n32 -c ${RELDIR}/compiler-rt/lib/crt/crtend.c   -o ${RTDIR}/clang_rt.crtend-32.o
#${CC} ${CFLAGS} -mips4 -mabi=64  -c ${RELDIR}/compiler-rt/lib/crt/crtbegin.c -o ${RTDIR}/clang_rt.crtbegin-64.o
#${CC} ${CFLAGS} -mips4 -mabi=64  -c ${RELDIR}/compiler-rt/lib/crt/crtend.c   -o ${RTDIR}/clang_rt.crtend-64.o

${CC} ${CFLAGS} -mips3 -mabi=n32 -c ${RELDIR}/compiler-rt/lib/crt/crtbegin.c -o ${RTDIR}/mips64-sgi-irix6.5-gnuabin32/clang_rt.crtbegin.o
${CC} ${CFLAGS} -mips3 -mabi=n32 -c ${RELDIR}/compiler-rt/lib/crt/crtend.c   -o ${RTDIR}/mips64-sgi-irix6.5-gnuabin32/clang_rt.crtend.o
${CC} ${CFLAGS} -mips4 -mabi=64  -c ${RELDIR}/compiler-rt/lib/crt/crtbegin.c -o ${RTDIR}/mips64-sgi-irix6.5/clang_rt.crtbegin.o
${CC} ${CFLAGS} -mips4 -mabi=64  -c ${RELDIR}/compiler-rt/lib/crt/crtend.c   -o ${RTDIR}/mips64-sgi-irix6.5/clang_rt.crtend.o
