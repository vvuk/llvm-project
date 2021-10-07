# This file sets up a CMakeCache for a IRIX toolchain build.

set(LLVM_ENABLE_ASSERTIONS ON CACHE BOOL "")
# Eventually just do release builds
#set(CMAKE_BUILD_TYPE Release CACHE STRING "")
set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "")

set(PACKAGE_VENDOR IRIX CACHE STRING "")

# Build this for N32
set(LLVM_DEFAULT_TARGET_TRIPLE "mips64-sgi-irix6.5-gnuabin32" CACHE STRING "")
set(LLVM_LIBDIR_SUFFIX "32" CACHE STRING "")

set(LLVM_ENABLE_PROJECTS "clang;clang-tools-extra;lld;llvm" CACHE STRING "")
set(LLVM_ENABLE_RUNTIMES "libunwind;compiler-rt;libcxxabi;libcxx" CACHE STRING "")
set(LLVM_RUNTIME_TARGETS "mips64-sgi-irix6.5-gnuabin32;mips64-sgi-irix6.5" CACHE STRING "")
set(LLVM_BUILTIN_TARGETS "mips64-sgi-irix6.5-gnuabin32;mips64-sgi-irix6.5" CACHE STRING "")

set(LLVM_TARGETS_TO_BUILD Mips CACHE STRING "")
set(LLVM_TARGET_ARCH Mips CACHE STRING "")
set(LLVM_ENABLE_LLD ON CACHE BOOL "")

###set(ENABLE_EXPERIMENTAL_NEW_PASS_MANAGER ON CACHE BOOL "")

set(LLVM_ENABLE_BACKTRACES OFF CACHE BOOL "")
#set(LLVM_ENABLE_PER_TARGET_RUNTIME_DIR ON CACHE BOOL "")
set(LLVM_ENABLE_TERMINFO OFF CACHE BOOL "")
set(LLVM_ENABLE_UNWIND_TABLES OFF CACHE BOOL "")
## TODO we have zlib
set(LLVM_ENABLE_ZLIB OFF CACHE BOOL "")

set(LLVM_INCLUDE_DOCS OFF CACHE BOOL "")
set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "")
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "")
set(LLVM_INCLUDE_GO_TESTS OFF CACHE BOOL "")
set(LLVM_INCLUDE_BENCHMARKS OFF CACHE BOOL "")

set(CLANG_DEFAULT_CXX_STDLIB libc++ CACHE STRING "")
set(CLANG_DEFAULT_LINKER lld CACHE STRING "")
set(CLANG_DEFAULT_OBJCOPY llvm-objcopy CACHE STRING "")
set(CLANG_DEFAULT_RTLIB compiler-rt CACHE STRING "")
set(CLANG_DEFAULT_UNWINDLIB libunwind CACHE STRING "")

set(CLANG_ENABLE_ARCMT OFF CACHE BOOL "")
set(CLANG_ENABLE_STATIC_ANALYZER OFF CACHE BOOL "")
set(CLANG_PLUGIN_SUPPORT OFF CACHE BOOL "")

# Skip the builtins checks
set(BUILTINS_CMAKE_ARGS "-DCAN_TARGET_mipsn32=YES;-DCAN_TARGET_mips64=YES" CACHE STRING "")

foreach(target "mips64-sgi-irix6.5-gnuabin32" "mips64-sgi-irix6.5")
    if("${target}" MATCHES "-gnuabin32$")
        set(libdir_suffix "32")
    elseif("${target}" MATCHES "^mips64")
        set(libdir_suffix "64")
    endif()

    set(RUNTIMES_${target}_LLVM_LIBDIR_SUFFIX "${libdir_suffix}" CACHE STRING "")
    # I think we need to set COMPILER_RT_INSTALL_LIBRARY_DIR and COMPILER_RT_OUTPUT_LIBRARY_DIR?

    # CMake doesn't know this as a valid system.  We pretend to be linux for most things,
    # though that gets us into trouble in a few places
    #set(RUNTIMES_${target}_CMAKE_SYSTEM_NAME "IRIX" CACHE STRING "")
    set(RUNTIMES_${target}_LIBCXX_BUILD_FOR_IRIX ON CACHE BOOL "")

    #set(RUNTIMES_${target}_LLVM_ENABLE_PER_TARGET_RUNTIME_DIR ON CACHE BOOL "")

    set(RUNTIMES_${target}_COMPILER_RT_BUILD_CRT ON CACHE BOOL "")
    set(RUNTIMES_${target}_COMPILER_RT_BUILD_SANITIZERS OFF CACHE BOOL "")
    set(RUNTIMES_${target}_COMPILER_RT_BUILD_XRAY OFF CACHE BOOL "")
    set(RUNTIMES_${target}_COMPILER_RT_BUILD_ORC OFF CACHE BOOL "")
    set(RUNTIMES_${target}_COMPILER_RT_BUILD_LIBFUZZER OFF CACHE BOOL "")
    set(RUNTIMES_${target}_COMPILER_RT_BUILD_PROFILE OFF CACHE BOOL "")
    set(RUNTIMES_${target}_COMPILER_RT_BUILD_MEMPROF OFF CACHE BOOL "")
    set(RUNTIMES_${target}_COMPILER_RT_BUILD_XRAY_NO_PREINIT OFF CACHE BOOL "")
    set(RUNTIMES_${target}_COMPILER_RT_CRT_USE_EH_FRAME_REGISTRY ON CACHE BOOL "")

    # Note that we force set INSTALL_LIBRARY_DIR here; a runtimes build for two separate
    # targets will automatically set PER_TARGET_RUNTIME_DIRS to true.  I'm not sure
    # how to make the runtimes build treat irix as a single default "target", just with 3 "architectures",
    # each with a separate lib suffix.

    set(RUNTIMES_${target}_LIBUNWIND_ENABLE_SHARED ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBUNWIND_INSTALL_LIBRARY ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBUNWIND_USE_COMPILER_RT ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBUNWIND_LINK_FLAGS "-nodefaultlibs" CACHE STRING "")
    set(RUNTIMES_${target}_LIBUNWIND_INSTALL_LIBRARY_DIR "./lib${libdir_suffix}" CACHE PATH "")

    # We can't enable shared libcxxabi -- it needs posix_memalign, which is in support for libcxx (they both need it).
    # In the future we could move it to libcxxabi but this is simpler
    ###set(RUNTIMES_${target}_LIBCXXABI_ENABLE_SHARED ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXXABI_ENABLE_SHARED OFF CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXXABI_ENABLE_STATIC_UNWINDER ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXXABI_INSTALL_LIBRARY ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXXABI_USE_COMPILER_RT ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXXABI_USE_LLVM_UNWINDER ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXXABI_INSTALL_LIBRARY_DIR "./lib${libdir_suffix}" CACHE PATH "")

    set(RUNTIMES_${target}_LIBCXX_ENABLE_SHARED ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXX_ENABLE_STATIC_ABI_LIBRARY ON CACHE BOOL "")
    set(RUNTIMES_${target}_LIBCXX_INSTALL_LIBRARY_DIR "./lib${libdir_suffix}" CACHE PATH "")

endforeach()

##set(BOOTSTRAP_LLVM_ENABLE_LTO ON CACHE BOOL "")
##set(BOOTSTRAP_LLVM_ENABLE_LLD ON CACHE BOOL "")
##
##set(CLANG_BOOTSTRAP_TARGETS
##  check-all
##  check-llvm
##  check-clang
##  check-lld
##  llvm-config
##  test-suite
##  test-depends
##  llvm-test-depends
##  clang-test-depends
##  lld-test-depends
##  distribution
##  install-distribution
##  install-distribution-stripped
##  install-distribution-toolchain
##  clang CACHE STRING "")
##
##get_cmake_property(variableNames VARIABLES)
##foreach(variableName ${variableNames})
##  if(variableName MATCHES "^STAGE2_")
##    string(REPLACE "STAGE2_" "" new_name ${variableName})
##    list(APPEND EXTRA_ARGS "-D${new_name}=${${variableName}}")
##  endif()
##endforeach()
##
### Setup the bootstrap build.
##set(CLANG_ENABLE_BOOTSTRAP ON CACHE BOOL "")
##set(CLANG_BOOTSTRAP_EXTRA_DEPS
#####  builtins
##  runtimes
##  CACHE STRING "")
##set(CLANG_BOOTSTRAP_CMAKE_ARGS
##  ${EXTRA_ARGS}
##  -C ${CMAKE_CURRENT_LIST_DIR}/IRIX-stage2.cmake
##  CACHE STRING "")
##
