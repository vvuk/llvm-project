#!/bin/bash

set -x

IROOT=/opt/irix/root
SROOT=/home/vladimir/proj/trunk-llvm
CROOT=/opt/irix/sgug/llvm
NROOT=${SROOT}/build-active/native-root/usr/sgug

FLAGS="-DO_CLOEXEC=0 -DO_NOFOLLOW=0"

cmake -G Ninja \
    -DCMAKE_CROSSCOMPILING=1 \
    -DCMAKE_MODULE_PATH=${SROOT}/irix/cmake \
    -DCMAKE_SYSTEM_NAME=IRIX \
    -DCMAKE_HOST_SYSTEM_NAME=Linux \
    -DCMAKE_C_COMPILER=${CROOT}/bin/clang \
    -DCMAKE_CXX_COMPILER=${CROOT}/bin/clang++ \
    -DLLVM_HOST_TRIPLE=mips-sgi-irix6.5-gnuabin32 \
    -DLLVM_DIR=${NROOT}/lib32/cmake/llvm \
    -DNATIVE_LLVM_DIR=${CROOT}/lib32/cmake/llvm \
    -DNATIVE_Clang_DIR=${CROOT}/lib32/cmake/clang \
    -DLLVM_TABLEGEN=${CROOT}/bin/llvm-tblgen \
    -DCLANG_TABLEGEN=${CROOT}/bin/clang-tblgen \
    -DLLDB_ENABLE_PYTHON=0 \
    -DLLDB_ENABLE_LIBEDIT=0 \
    -DLLDB_ENABLE_CURSES=0 \
    -DLLDB_ENABLE_TERMINFO=0 \
    -DCMAKE_C_FLAGS="${FLAGS}" \
    -DCMAKE_CXX_FLAGS="${FLAGS}" \
    -DCROSS_TOOLCHAIN_FLAGS_NATIVE='-DCMAKE_C_COMPILER=clang;-DCMAKE_CXX_COMPILER=clang++' \
    -DLIBXML2_INCLUDE_DIR:PATH=${IROOT}/usr/sgug/include/libxml2 \
    -DLIBXML2_LIBRARY:FILEPATH=${IROOT}/usr/sgug/lib32/libxml2.so \
    ../lldb
