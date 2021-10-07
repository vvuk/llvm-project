#!/bin/bash

if [ -f clang/CMakeLists.txt ] ; then
    echo "Don't run this from the top level tree."
    echo "Make a new build directory and run this script with a relative path:"
    echo ""
    echo "   mkdir build-irix"
    echo "   cd build-irix"
    echo "   ../$(basename $0)"
    exit 1
fi

ROOT=/opt/irix/root
#ROOT=/home/vladimir/tmp/irix655
LLVMVER=14

if [ ! -f ${ROOT}/lib32/libc.so.1 ] ; then
    echo "Expected to find IRIX root in /opt/irix/root"
    exit 1
fi

#if [ ! -f ${ROOT}/usr/xg/include/setenv.h ] ; then
#    echo "libxg stuff not found, run make install in libxg"
#    exit 1
#fi 

#if [[ ! -f "${ROOT}/usr/lib/clang/mips64-sgi-irix6.5/gcc-irix-crti.o" || ! -f "lib/clang/${LLVMVER}.0.0/lib/mips64-sgi-irix6.5/gcc-irix-crtn.o" ]] ; then
#    echo "${ROOT}/usr/lib/clang/mips64-sgi-irix6.5 needs to be populated with:"
#    echo "    gcc/mips-sgi-irix6.5/9/irix-crtn.o -> gcc-irix-crtn.o"
#    echo "    gcc/mips-sgi-irix6.5/9/irix-crti.o -> gcc-irix-crti.o"
#    echo "    gcc/mips-sgi-irix6.5/9/include -> include"
#    echo "    gcc/mips-sgi-irix6.5/9/include-fixed -> include-fixed"
#    echo ""
#    echo "    gcc-irix-crtn.o and gcc-irix-crti.o also need to be copied to lib/clang/${LLVMVER}.0.0/lib/mips64-sgi-irix6.5 (under this build dir)"
#    exit 1
#fi

RELDIR=$(dirname $0)

# set -x in subshell to trace the cmake invocation
(set -x ; \
cmake -G Ninja \
    -C ${RELDIR}/clang/cmake/caches/IRIX.cmake \
    -DDEFAULT_SYSROOT=${ROOT} \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCLANG_DEFAULT_LINKER=lld \
    -DCMAKE_INSTALL_PREFIX=/opt/irix/sgug/llvm \
    $* \
    ${RELDIR}/llvm
)
