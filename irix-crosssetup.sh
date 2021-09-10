#!/bin/sh

if [ -f clang/CMakeLists.txt ] ; then
    echo "Don't run this from the top level tree.  Make a new build directory and run with the path to the root:"
    echo ""
    echo "   mkdir build-native"
    echo "   cd build-native"
    echo "   ../$(basename $0)"
    exit 1
fi

NDIR=$1

if [ x"$NDIR" = "x" ] ; then
    if [ -f ../build-irix/bin/llvm-tblgen ] ; then
        echo "Assuming ../build-irix/bin"
        NDIR=../build-irix/bin
    else
        echo "Please specify path to built llvm native cross-compiler, e.g."
        echo "   $0 ../build-irix/bin"
        exit 1
    fi
fi

if [ ! -f /opt/irix/root/lib32/libc.so.1 ] ; then
    echo "Expected to find IRIX root in /opt/irix/root"
    exit 1
fi

# One day soon..
#CROSS_CC=${NDIR}/clang
#CROSS_CXX=${NDIR}/clang++
CROSS_CC=/opt/irix/sgug/bin/mips-sgi-irix6.5-gcc
CROSS_CXX=/opt/irix/sgug/bin/mips-sgi-irix6.5-g++

if [ ! -f ${CROSS_CC} ] ; then
    echo "Can't find cross compiler ${CROSS_CC}, is it installed?"
    exit 1
fi

RELDIR=$(dirname $0)

set -x

cmake -G Ninja \
    -DLLVM_ENABLE_PROJECTS="clang;lld;compiler-rt" \
    -DCMAKE_CROSSCOMPILING=True \
    -DCMAKE_INSTALL_PREFIX=/usr/sgug \
    -DGCC_INSTALL_PREFIX=/usr/sgug \
    -DLLVM_TABLEGEN=${NDIR}/llvm-tblgen \
    -DCLANG_TABLEGEN=${NDIR}/clang-tblgen \
    -DCLANG_DEFAULT_LINKER=lld \
    -DLLVM_DEFAULT_TARGET_TRIPLE=mips-sgi-irix6.5 \
    -DLLVM_TARGET_ARCH=Mips \
    -DLLVM_TARGETS_TO_BUILD=Mips \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DLLVM_OPTIMIZED_TABLEGEN=On \
    -DCMAKE_C_COMPILER="${CROSS_CC}" -DCMAKE_CXX_COMPILER="${CROSS_CXX}" \
    -DLLVM_INCLUDE_TESTS=Off -DLLVM_INCLUDE_EXAMPLES=Off \
    ${RELDIR}/llvm


