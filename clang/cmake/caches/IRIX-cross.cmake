# This file sets up a CMakeCache for a IRIX cross build.

# Release by default (in contrast with IRIX.cmake)
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
# But with assertions
set(LLVM_ENABLE_ASSERTIONS ON CACHE BOOL "")

include(${CMAKE_CURRENT_LIST_DIR}/IRIX.cmake)

