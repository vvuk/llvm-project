#!/bin/bash

if [ -f clang/CMakeLists.txt ] ; then
	echo "Don't run this from the top level tree.  Make a new build directory and run with the path to the root:"
	echo ""
	echo "   mkdir build-irix"
	echo "   cd build-irix"
	echo "   ../irix-buildsetup.sh"
	exit 1
fi

if [ ! -f /opt/irix/root/lib32/libc.so.1 ] ; then
    echo "Expected to find IRIX root in /opt/irix/root"
    exit 1
fi

RELDIR=$(dirname $0)

set -x
cmake -G Ninja \
	-DLLVM_ENABLE_PROJECTS="clang;lld;compiler-rt" \
	-DCLANG_DEFAULT_LINKER=lld \
    -DDEFAULT_SYSROOT=/opt/irix/root \
	-DCMAKE_INSTALL_PREFIX=/opt/irix/sgug \
	-DCMAKE_BUILD_TYPE=RelWithDebInfo \
	-DLLVM_TARGETS_TO_BUILD=Mips \
	-DLLVM_OPTIMIZED_TABLEGEN=On \
	-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
	-DLLVM_INCLUDE_TESTS=Off -DLLVM_INCLUDE_EXAMPLES=Off \
    ${RELDIR}/llvm


