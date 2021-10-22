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
#ROOT=/home/vladimir/tmp/irix655
LLVMVER=14

NDIR=${RELDIR}/build-full
NDIR=$(readlink -f ${NDIR})
CROSS_CC=${NDIR}/bin/clang
CROSS_CXX=${NDIR}/bin/clang++
XLINK="-lpthread"

if [ ! -f ${ROOT}/lib32/libc.so.1 ] ; then
    echo "Expected to find IRIX root in /opt/irix/root"
    exit 1
fi

# set -x in subshell to trace the cmake invocation
(set -x ; \
cmake -G Ninja \
    -DCMAKE_SYSTEM_NAME=Linux \
    -C ${RELDIR}/clang/cmake/caches/IRIX-cross.cmake \
    -DCMAKE_INSTALL_PREFIX=/usr/llvm \
    -DCMAKE_C_COMPILER="${CROSS_CC}" \
    -DCMAKE_CXX_COMPILER="${CROSS_CXX}" \
    -DLLVM_TABLEGEN=${NDIR}/bin/llvm-tblgen \
    -DLLVM_CONFIG_PATH=${NDIR}/bin/llvm-config \
    -DCLANG_TABLEGEN=${NDIR}/bin/clang-tblgen \
    -DCLANG_TOOLING_BUILD_AST_INTROSPECTION=OFF \
    -DCLANG_DEFAULT_LINKER=lld \
    $* \
    ${RELDIR}/llvm
)
