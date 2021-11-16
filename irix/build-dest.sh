#!/bin/bash

echo running install..
rm -rf DEST
DESTDIR=`pwd`/DEST ninja install

if [ ! -d DEST/usr/llvm ] ; then
    echo nothing created DEST
    exit 1
fi

echo fixing includes..
pushd DEST/usr/llvm
cp -r ../../../../irix/include-fixed lib32/clang/14.0.0/include-fixed
pushd lib64/clang/14.0.0
ln -s ../../../lib32/clang/14.0.0/include-fixed .
popd
rm -rf lib32/libLLVM*.a lib32/libclang*.a lib32/liblld*.a lib32/libfindAll*.a lib32/cmake
rm -rf include/llvm* include/clang* include/lld* 
popd

echo moving around runtime..
pushd DEST/usr/llvm
cp -r lib/clang/14.0.0/lib/* lib32/clang/14.0.0/lib
cp lib64/clang/14.0.0/lib/mips64-sgi-irix6.5/* lib32/clang/14.0.0/lib/mips64-sgi-irix6.5
rm -rf lib64/clang/14.0.0/lib
rm -rf lib
popd

echo Creating tarball..
pushd DEST
tar czf ../irix-llvm.tar.gz .
popd

