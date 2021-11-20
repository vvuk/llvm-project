#!/bin/bash

set -e

LLVMDIR="`dirname $0`/.."
cd $LLVMDIR
LLVMDIR="`pwd`"

if [ ! -f "$LLVMDIR/llvm/CMakeLists.txt" ] ; then
    echo "ERROR: This script should live inside the irix directory in the llvm tree"
    exit 1
fi

# Prefix where the cross compiler is potentially going to live (linux binaries)
: "${CROSSPREFIX:=/opt/irix/sgug/llvm}"

# Prefix where the native compiler is going to live (irix binaries)
: "${NATIVEPREFIX:=/usr/sgug}"

# Build directory prefix
: "${BUILDDIR:=build-clean}"

# IRIX root
: "${IRIXROOT:=/opt/irix/root}"

: "${LLVMVER:=14.0.0}"

echo "Building cross and native IRIX Clang with the following configuration:"
echo ""
echo "Location where Linux cross-compiler will live:"
echo "   CROSSPREFIX = ${CROSSPREFIX}"
echo "Location where native compiler will live under IRIX:"
echo "   NATIVEPREIFX = ${NATIVEPREFIX}"
echo "Name to use for build directories:"
echo "   BUILDDIR = ${BUILDDIR}"
echo "Location of IRIX root filesystem:"
echo "   IRIXROOT = ${IRIXROOT}"
echo ""

if [ ! -f ${IRIXROOT}/lib32/libc.so.1 ] ; then
    echo "ERROR: Expected to find IRIX root in ${IRIXROOT} (missing ${IRIXROOT}/lib32/libc.so.1)"
    exit 1
fi

echo "If any of the above doesn't look correct, hit ^C within the next 5 seconds,"
echo "and set environment variables before running this script."

if [ "$GO" == "" ] ; then
    sleep 5
    echo "Continuing."
    sleep 1
fi

fix_dest_dir () {
    DD="$1"

    # move runtimes so that they all live in lib32/clang/14.0.0/lib
    cp -r ${DD}/lib/clang/14.0.0/lib/* ${DD}/lib32/clang/14.0.0/lib
    cp -r ${DD}/lib64/clang/14.0.0/lib/* ${DD}/lib32/clang/14.0.0/lib
    rm -rf ${DD}/lib ${DD}/lib64/clang

    # copy in includes-fixed into the cross tree
    cp -r ${LLVMDIR}/irix/include-fixed ${DD}/lib32/clang/14.0.0
}

echo "==== Cross compiler build ===="

mkdir -p ${BUILDDIR}-cross
cd ${BUILDDIR}-cross
../irix/setup-irix-cross.sh -DCMAKE_INSTALL_PREFIX=${CROSSPREFIX}

# Build clang first so we can build the CRT
ninja bin/clang

CC=`pwd`/bin/clang ../irix/build-crt.sh
ninja

# sort out DEST
rm -rf DEST
DESTDIR=`pwd`/DEST ninja install
fix_dest_dir DEST/${CROSSPREFIX}

# move runtimes so that they all live in lib32/clang/14.0.0/lib
#cp -r DEST/${CROSSPREFIX}/lib/clang/14.0.0/lib/* DEST/${CROSSPREFIX}/lib32/clang/14.0.0/lib
#cp -r DEST/${CROSSPREFIX}/lib64/clang/14.0.0/lib/* DEST/${CROSSPREFIX}/lib32/clang/14.0.0/lib
#rm -rf DEST/${CROSSPREFIX}/lib DEST/${CROSSPREFIX}/lib64/clang
#cp -r lib32/clang/14.0.0/lib/* DEST/${CROSSPREFIX}/lib32/clang/14.0.0/lib

# copy in includes-fixed into the cross tree
#cp -r ../irix/include-fixed DEST/${CROSSPREFIX}/lib32/clang/14.0.0

# clang-tblgen doesn't get installed, which complicates things.  just copy it in.
cp bin/clang-tblgen DEST/${CROSSPREFIX}/bin

cd ..

echo "===="
echo "==== Native compiler build ===="

mkdir -p ${BUILDDIR}-native
cd ${BUILDDIR}-native

# copy in the runtimes first, though we may not need them here
mkdir -p lib32/clang/14.0.0/lib
cp -r ../${BUILDDIR}-cross/DEST/${CROSSPREFIX}/lib32/clang/14.0.0/lib/* lib32/clang/14.0.0/lib

CROSSDIR=../${BUILDDIR}-cross/DEST/${CROSSPREFIX} ../irix/setup-irix-native.sh -DCMAKE_INSTALL_PREFIX=${NATIVEPREFIX} \
    -DCMAKE_INSTALL_RPATH=/usr/sgug/lib32:/usr/lib32:/lib32 -DRUNTIMES_INSTALL_RPATH=/usr/sgug/lib32:/usr/lib32:/lib32 -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON
ninja

rm -rf DEST
DESTDIR=`pwd`/DEST ninja install
fix_dest_dir DEST/${NATIVEPREFIX}

cd ..

echo "===="

echo "Creating llvm-${LLVMVER}-irix-sgug.tar ..."
tar -c -f llvm-${LLVMVER}-irix-sgug.tar -C build-clean-native/DEST usr
echo "Creating llvm-${LLVMVER}-irix-linux-cross.tar ..."
tar -c -f llvm-${LLVMVER}-irix-linux-cross.tar -C build-clean-cross/DEST opt

