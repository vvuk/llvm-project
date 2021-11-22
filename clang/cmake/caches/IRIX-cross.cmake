# This file sets up a CMakeCache for a IRIX cross build

# Build this one with debugging symbols and assertions
set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "")
set(LLVM_ENABLE_ASSERTIONS ON CACHE BOOL "")

# We only care about MIPS for the cross-compiler.  X86 makes things easier for rust.
set(LLVM_TARGETS_TO_BUILD "X86;Mips" CACHE STRING "")

include(${CMAKE_CURRENT_LIST_DIR}/IRIX.cmake)

