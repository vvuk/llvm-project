#!/bin/bash

rm -rf DEST
DESTDIR=`pwd`/DEST ninja install

pushd DEST/usr/llvm
cp -r ../../../../irix/include-fixed lib32/clang/14.0.0/include-fixed
pushd lib64/clang/14.0.0
ln -s ../../../lib32/clang/14.0.0/include-fixed .
popd
rm -rf lib32/libLLVM*.a lib32/libclang*.a lib32/liblld*.a lib32/libfindAll*.a lib32/cmake
rm -rf include/llvm* include/clang* include/lld* 
popd

echo Creating tarball..
pushd DEST
tar czf ../irix-llvm.tar.gz .
popd

