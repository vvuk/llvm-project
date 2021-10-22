#!/bin/bash

RELDIR=$(dirname $0)/..
if [ ! -f ${RELDIR}/llvm/CMakeLists ] ; then
    echo bad RELDIR ${RELDIR}
    exit 1
fi

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
LLVMVER=14

if [ ! -f ${ROOT}/lib32/libc.so.1 ] ; then
    echo "Expected to find IRIX root in /opt/irix/root"
    exit 1
fi

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
