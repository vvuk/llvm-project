#!/bin/sh

if [ -f clang/CMakeLists.txt ] ; then
    echo "Don't run this from the top level tree."
    echo "Make a new build directory and run this script with a relative path:"
    echo ""
    echo "   mkdir build-irix"
    echo "   cd build-irix"
    echo "   ../$(basename $0)"
    exit 1
fi

if [ ! -f /opt/irix/root/lib32/libc.so.1 ] ; then
    echo "Expected to find IRIX root in /opt/irix/root"
    exit 1
fi

if [ ! -f /opt/irix/root/usr/xg/include/setenv.h ] ; then
    echo "libxg stuff not found, run make install in libxg"
    exit 1
fi 

FIRSTTIME=1
if [ -f CMakeCache.txt ] ; then
    FIRSTTIME=
fi

RELDIR=$(dirname $0)

# set -x in subshell to trace the cmake invocation
(set -x ; \
cmake -G Ninja \
    -C ${RELDIR}/clang/cmake/caches/IRIX.cmake \
    -DIRIX_mips64_SYSROOT=/opt/irix/root \
    -DDEFAULT_SYSROOT=/opt/irix/root \
    -DCLANG_DEFAULT_LINKER=lld \
    -DCMAKE_INSTALL_PREFIX=/opt/irix/sgug/llvm \
    -DGCC_INSTALL_PREFIX=/opt/irix/root/usr/sgug \
    ${RELDIR}/llvm \
)

if [ "$FIRSTTIME" = "1" ] ; then
    echo "====="
    echo "NOTE: there are still some patches that need to happen to libgcc -- specifically, you must run"
    echo "objcopy on irix-crti.o and irix-crtn.o, and patch some headers.  A patch coming soon."
fi

