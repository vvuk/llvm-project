# This file sets up a CMakeCache for a IRIX native build

# Release build only; no debug symbols for the native build
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
# No assertions
set(LLVM_ENABLE_ASSERTIONS OFF CACHE BOOL "")

include(${CMAKE_CURRENT_LIST_DIR}/IRIX.cmake)

