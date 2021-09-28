# This file sets up a CMakeCache for the second stage of a IRIX toolchain build.

set(LLVM_TARGETS_TO_BUILD Mips CACHE STRING "")

set(PACKAGE_VENDOR IRIX CACHE STRING "")

set(LLVM_ENABLE_PROJECTS "clang;clang-tools-extra;lld;llvm" CACHE STRING "")
set(LLVM_ENABLE_RUNTIMES "compiler-rt;libcxx;libcxxabi;libunwind" CACHE STRING "")

set(LLVM_ENABLE_BACKTRACES OFF CACHE BOOL "")
if(NOT APPLE)
  set(LLVM_ENABLE_LLD ON CACHE BOOL "")
endif()
set(LLVM_ENABLE_LTO ON CACHE BOOL "")
set(LLVM_ENABLE_PER_TARGET_RUNTIME_DIR ON CACHE BOOL "")
set(LLVM_ENABLE_TERMINFO OFF CACHE BOOL "")
set(LLVM_ENABLE_UNWIND_TABLES OFF CACHE BOOL "")
set(LLVM_ENABLE_ZLIB ON CACHE BOOL "")
set(LLVM_INCLUDE_DOCS OFF CACHE BOOL "")
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "")
set(LLVM_INCLUDE_GO_TESTS OFF CACHE BOOL "")
set(LLVM_INCLUDE_BENCHMARKS OFF CACHE BOOL "")
set(LLVM_USE_RELATIVE_PATHS_IN_FILES ON CACHE BOOL "")
set(LLVM_ENABLE_Z3_SOLVER OFF CACHE BOOL "")

if(MSVC)
  set(LLVM_USE_CRT_RELEASE "MT" CACHE STRING "")
endif()

set(CLANG_DEFAULT_CXX_STDLIB libc++ CACHE STRING "")
set(CLANG_DEFAULT_LINKER lld CACHE STRING "")
set(CLANG_DEFAULT_OBJCOPY llvm-objcopy CACHE STRING "")
set(CLANG_DEFAULT_RTLIB compiler-rt CACHE STRING "")
set(CLANG_ENABLE_ARCMT OFF CACHE BOOL "")
set(CLANG_ENABLE_STATIC_ANALYZER OFF CACHE BOOL "")
set(CLANG_PLUGIN_SUPPORT OFF CACHE BOOL "")

set(ENABLE_EXPERIMENTAL_NEW_PASS_MANAGER ON CACHE BOOL "")
set(ENABLE_LINKER_BUILD_ID ON CACHE BOOL "")
set(ENABLE_X86_RELAX_RELOCATIONS ON CACHE BOOL "")

set(CMAKE_BUILD_TYPE Release CACHE STRING "")
if (APPLE)
  set(MACOSX_DEPLOYMENT_TARGET 10.7 CACHE STRING "")
elseif(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded" CACHE STRING "")
endif()

if(IRIX_ROOT)
  foreach(target mips64)
    set(IRIX_${target}_COMPILER_FLAGS "--target=mips64-sgi-irix6.5 -I${IRIX_ROOT}/usr/include")
    set(IRIX_${target}_LINKER_FLAGS "-L${IRIX_ROOT}/usr/lib32")
    set(IRIX_${target}_SYSROOT "${IRIX_ROOT}")
  endforeach()

  foreach(target mips64)
    # Set the per-target builtins options.
    list(APPEND BUILTIN_TARGETS "mips64-sgi-irix6.5")
    set(BUILTINS_${target}-sgi-irix6.5_CMAKE_SYSTEM_NAME IRIX CACHE STRING "")
    set(BUILTINS_${target}-sgi-irix6.5_CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "")
    set(BUILTINS_${target}-sgi-irix6.5_CMAKE_ASM_FLAGS ${IRIX_${target}_COMPILER_FLAGS} CACHE STRING "")
    set(BUILTINS_${target}-sgi-irix6.5_CMAKE_C_FLAGS ${IRIX_${target}_COMPILER_FLAGS} CACHE STRING "")
    set(BUILTINS_${target}-sgi-irix6.5_CMAKE_CXX_FLAGS ${IRIX_${target}_COMPILER_FLAGS} CACHE STRING "")
    set(BUILTINS_${target}-sgi-irix6.5_CMAKE_SHARED_LINKER_FLAGS ${IRIX_${target}_LINKER_FLAGS} CACHE STRING "")
    set(BUILTINS_${target}-sgi-irix6.5_CMAKE_MODULE_LINKER_FLAGS ${IRIX_${target}_LINKER_FLAGS} CACHE STRING "")
    set(BUILTINS_${target}-sgi-irix6.5_CMAKE_EXE_LINKER_FLAGS ${IRIX_${target}_LINKER_FLAGS} CACHE STRING "")
    set(BUILTINS_${target}-sgi-irix6.5_CMAKE_SYSROOT ${IRIX_${target}_SYSROOT} CACHE PATH "")
  endforeach()

  foreach(target mips64)
    # Set the per-target runtimes options.
    list(APPEND RUNTIME_TARGETS "${target}-sgi-irix6.5")
    set(RUNTIMES_${target}-sgi-irix6.5_CMAKE_SYSTEM_NAME IRIX CACHE STRING "")
    set(RUNTIMES_${target}-sgi-irix6.5_CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "")
    set(RUNTIMES_${target}-sgi-irix6.5_CMAKE_BUILD_WITH_INSTALL_RPATH ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_CMAKE_ASM_FLAGS ${IRIX_${target}_COMPILER_FLAGS} CACHE STRING "")
    set(RUNTIMES_${target}-sgi-irix6.5_CMAKE_C_FLAGS ${IRIX_${target}_COMPILER_FLAGS} CACHE STRING "")
    set(RUNTIMES_${target}-sgi-irix6.5_CMAKE_CXX_FLAGS ${IRIX_${target}_COMPILER_FLAGS} CACHE STRING "")
    set(RUNTIMES_${target}-sgi-irix6.5_CMAKE_SHARED_LINKER_FLAGS ${IRIX_${target}_LINKER_FLAGS} CACHE STRING "")
    set(RUNTIMES_${target}-sgi-irix6.5_CMAKE_MODULE_LINKER_FLAGS ${IRIX_${target}_LINKER_FLAGS} CACHE STRING "")
    set(RUNTIMES_${target}-sgi-irix6.5_CMAKE_EXE_LINKER_FLAGS ${IRIX_${target}_LINKER_FLAGS} CACHE STRING "")
    set(RUNTIMES_${target}-sgi-irix6.5_CMAKE_SYSROOT ${IRIX_${target}_SYSROOT} CACHE PATH "")
    set(RUNTIMES_${target}-sgi-irix6.5_COMPILER_RT_USE_BUILTINS_LIBRARY ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBUNWIND_USE_COMPILER_RT ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBUNWIND_HERMETIC_STATIC_LIBRARY ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBUNWIND_INSTALL_STATIC_LIBRARY OFF CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBCXXABI_USE_COMPILER_RT ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBCXXABI_USE_LLVM_UNWINDER ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBCXXABI_ENABLE_STATIC_UNWINDER ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBCXXABI_HERMETIC_STATIC_LIBRARY ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBCXXABI_INSTALL_STATIC_LIBRARY OFF CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBCXXABI_STATICALLY_LINK_UNWINDER_IN_SHARED_LIBRARY OFF CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBCXX_USE_COMPILER_RT ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBCXX_ENABLE_STATIC_ABI_LIBRARY ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBCXX_HERMETIC_STATIC_LIBRARY ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBCXX_STATICALLY_LINK_ABI_IN_SHARED_LIBRARY ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LIBCXX_ABI_VERSION 2 CACHE STRING "")
    set(RUNTIMES_${target}-sgi-irix6.5_LLVM_ENABLE_ASSERTIONS ON CACHE BOOL "")
    set(RUNTIMES_${target}-sgi-irix6.5_LLVM_ENABLE_RUNTIMES "compiler-rt;libcxx;libcxxabi;libunwind" CACHE STRING "")

    # Use .build-id link.
    list(APPEND RUNTIME_BUILD_ID_LINK "${target}-sgi-irix6.5")
  endforeach()
endif()

set(LLVM_BUILTIN_TARGETS "${BUILTIN_TARGETS}" CACHE STRING "")
set(LLVM_RUNTIME_TARGETS "${RUNTIME_TARGETS}" CACHE STRING "")
set(LLVM_RUNTIME_BUILD_ID_LINK_TARGETS "${RUNTIME_BUILD_ID_LINK}" CACHE STRING "")

# Setup toolchain.
set(LLVM_INSTALL_TOOLCHAIN_ONLY ON CACHE BOOL "")
set(LLVM_TOOLCHAIN_TOOLS
  dsymutil
  llvm-ar
  llvm-cov
  llvm-cxxfilt
  llvm-dlltool
  llvm-dwarfdump
  llvm-dwp
  llvm-elfabi
  llvm-gsymutil
  llvm-lib
  llvm-mt
  llvm-nm
  llvm-objcopy
  llvm-objdump
  llvm-profdata
  llvm-rc
  llvm-ranlib
  llvm-readelf
  llvm-readobj
  llvm-size
  llvm-strip
  llvm-symbolizer
  llvm-xray
  sancov
  CACHE STRING "")

set(LLVM_DISTRIBUTION_COMPONENTS
  clang
  lld
  LTO
  clang-apply-replacements
  clang-doc
  clang-format
  clang-resource-headers
  clang-include-fixer
  clang-refactor
  clang-scan-deps
  clang-tidy
  clangd
  builtins
  runtimes
  ${LLVM_TOOLCHAIN_TOOLS}
  CACHE STRING "")
