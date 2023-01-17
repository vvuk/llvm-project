#!/bin/bash

set -e

: "${NATIVEBUILDTYPE=Debug}"

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
: "${BUILDDIR:=build-debug}"
: "${NATIVEBUILDDIR:=build-clean}"

# IRIX root
: "${IRIXROOT:=/opt/irix/root}"

: "${LLVMVER:=14.0.6}"

echo "Building cross and native IRIX Clang with the following configuration:"
echo ""
echo "Location where Linux cross-compiler will live:"
echo "   CROSSPREFIX = ${CROSSPREFIX}"
echo "Location where native compiler will live under IRIX:"
echo "   NATIVEPREIFX = ${NATIVEPREFIX}"
echo "Name prefix to use for build directories:"
echo "   BUILDDIR = ${BUILDDIR}"
echo "Location of IRIX root filesystem:"
echo "   IRIXROOT = ${IRIXROOT}"
echo ""
echo $"${CROSSBUILDTYPE} ${NATIVEBUILDTYPE}"

if [ ! -f ${IRIXROOT}/lib32/libc.so.1 ] ; then
    echo "ERROR: Expected to find IRIX root in ${IRIXROOT} (missing ${IRIXROOT}/lib32/libc.so.1)"
    exit 1
fi

if [ ! -d ${IRIXROOT}/usr/lib/clang/include-fixed ] ; then
    echo "Warning: For bootstrapping, ${IRIXROOT}/usr/lib/clang/include-fixed needs to be"
    echo "a symlink to ${LLVMDIR}/irix/include-fixed.  Attempting to fix..."

    mkdir -p ${IRIXROOT}/usr/lib/clang
    ln -s ${LLVMDIR}/irix/include-fixed ${IRIXROOT}/usr/lib/clang/include-fixed
    if [ ! -d ${IRIXROOT}/usr/lib/clang/include-fixed ] ; then
        echo "Failed (permissions?).  Please create manually and rerun."
        exit 1
    fi

    echo "Success, continuing..."
fi

if [ -f ${IRIXROOT}/usr/sgug/include/elf.h ] ; then
    echo "ERROR: ${IRIXROOT}/usr/sgug/include/elf.h exists.  It conflicts with the IRIX"
    echo "system include files.  Please uninstall SGUG elfutils-libelf-devel and rerun."
    exit 1
fi

if [ "$1" == "clean" ] ; then
    echo "WARNING: Will clean build directories"
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

    # move runtimes so that they all live in lib32/clang/${LLVMVER}/lib
    cp -u -r ${DD}/lib/clang/${LLVMVER}/lib/* ${DD}/lib32/clang/${LLVMVER}/lib
    cp -u -r ${DD}/lib64/clang/${LLVMVER}/lib/* ${DD}/lib32/clang/${LLVMVER}/lib
    rm -rf ${DD}/lib ${DD}/lib64/clang

    # set up target symlinks, mainly for distcc
    ln -s clang-14 ${DD}/bin/mips64-sgi-irix6.5-gnuabin32-clang
    ln -s clang-14 ${DD}/bin/mips64-sgi-irix6.5-gnuabin32-clang++
    ln -s clang-14 ${DD}/bin/mips64-sgi-irix6.5-clang
    ln -s clang-14 ${DD}/bin/mips64-sgi-irix6.5-clang++

    # copy in includes-fixed into the cross tree
    cp -u -r ${LLVMDIR}/irix/include-fixed ${DD}/lib32/clang/${LLVMVER}
}

if [ "$NO_BUILD_NATIVE" == "" ] ; then
    echo "==== Native compiler build ===="

    mkdir -p ${BUILDDIR}-native
    cd ${BUILDDIR}-native

    # copy in the runtimes first, though we may not need them here
    mkdir -p lib32/clang/${LLVMVER}/lib
    cp -u -r ../${NATIVEBUILDDIR}-cross/DEST/${CROSSPREFIX}/lib32/clang/${LLVMVER}/lib/* lib32/clang/${LLVMVER}/lib

    CROSSDIR=../${NATIVEBUILDDIR}-cross/DEST/${CROSSPREFIX} \
        ../irix/setup-irix-native.sh \
        -DCMAKE_INSTALL_PREFIX=${NATIVEPREFIX} \
        -DCMAKE_INSTALL_RPATH=/usr/sgug/lib32:/usr/lib32:/lib32 \
        -DRUNTIMES_INSTALL_RPATH=/usr/sgug/lib32:/usr/lib32:/lib32 \
        -DCMAKE_BUILD_TYPE=${NATIVEBUILDTYPE} \
        -DCMAKE_INSTALL_STRIP=OFF \
        -DCMAKE_STRIP=echo \
        -DCMAKE_C_FLAGS="-g -O1" \
        -DCMAKE_CXX_FLAGS="-g -O1" \
        -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON

    ninja

    rm -rf DEST
    DESTDIR=`pwd`/DEST ninja install
    fix_dest_dir DEST/${NATIVEPREFIX}

    cd ..
    echo "===="
else
    echo "==== Skipping native compiler build ===="
fi

echo "Creating llvm-${LLVMVER}-irix-sgug.tar ..."
tar -c -f llvm-${LLVMVER}-irix-sgug.tar -C build-clean-native/DEST usr
echo "Creating llvm-${LLVMVER}-irix-linux-cross.tar ..."
tar -c -f llvm-${LLVMVER}-irix-linux-cross.tar -C build-clean-cross/DEST opt

