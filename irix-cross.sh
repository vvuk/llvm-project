#!/bin/sh

if [ -f clang/CMakeLists.txt ] ; then
    echo "Don't run this from the top level tree.  Make a new build directory and run with the path to the root:"
    echo ""
    echo "   mkdir build-native"
    echo "   cd build-native"
    echo "   ../$(basename $0)"
    exit 1
fi

NDIR=../build-full

if [ x"$1" != "x" ] ; then
    NDIR=$1
    shift
fi

if [ ! -f ${NDIR}/bin/llvm-tblgen ] ; then
    echo "Couldn't find ${NDIR}/bin/llvm-tblgen"
    echo "Please specify path to built llvm native cross-compiler binary directory, e.g."
    echo "   $0 ../build-full"
    exit 1
fi

if [ ! -f /opt/irix/root/lib32/libc.so.1 ] ; then
    echo "Expected to find IRIX root in /opt/irix/root"
    exit 1
fi

NDIR=$(readlink -f ${NDIR})

echo ${NDIR}

CROSS_CC=${NDIR}/bin/clang
CROSS_CXX=${NDIR}/bin/clang++
XLINK="-lpthread"

if [ ! -f ${CROSS_CC} ] ; then
    echo "Can't find cross compiler ${CROSS_CC}, is it installed?"
    exit 1
fi

if [ ! -f /opt/irix/root/usr/xg/include/setenv.h ] ; then
    echo "libxg stuff not found, run make install in libxg"
    exit 1
fi 

RELDIR=$(dirname $0)

set -x

cmake -G Ninja \
    -DCMAKE_CROSSCOMPILING=True \
    -C ${RELDIR}/clang/cmake/caches/IRIX-cross.cmake \
    -DCMAKE_INSTALL_PREFIX=/usr/sgug/llvm \
    -DGCC_INSTALL_PREFIX=/usr/sgug \
    -DLLVM_TABLEGEN=${NDIR}/bin/llvm-tblgen \
    -DCLANG_TABLEGEN=${NDIR}/bin/clang-tblgen \
    -DLLVM_OPTIMIZED_TABLEGEN=On \
    -DCMAKE_C_COMPILER="${CROSS_CC}" -DCMAKE_CXX_COMPILER="${CROSS_CXX}" \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DCMAKE_C_FLAGS="-I=/usr/xg/include" \
    -DCMAKE_CXX_FLAGS="-I=/usr/xg/include" \
    -DCMAKE_EXE_LINKER_FLAGS="-L=/usr/xg/lib32 -lxg -lgen ${XLINK}" \
    -DCMAKE_SHARED_LINKER_FLAGS="-L=/usr/xg/lib32 -lxg -lgen ${XLINK}" \
    -DCMAKE_MODULE_LINKER_FLAGS="-L=/usr/xg/lib32 -lxg -lgen ${XLINK}" \
    $* \
    ${RELDIR}/llvm

