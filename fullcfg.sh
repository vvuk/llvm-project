#!/bin/bash

set -x

cmake -G Ninja \
    -C ../clang/cmake/caches/IRIX.cmake \
    -DIRIX_mips64_SYSROOT=/opt/irix/root \
    -DDEFAULT_SYSROOT=/opt/irix/root \
    -DCLANG_DEFAULT_LINKER=lld \
    -DCMAKE_INSTALL_PREFIX=/opt/irix/sgug/llvm \
    -DGCC_INSTALL_PREFIX=/opt/irix/root/usr/sgug \
    ../llvm
