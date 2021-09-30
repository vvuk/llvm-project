# This file sets up a CMakeCache for a IRIX cross build.

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I=/usr/xg/include" CACHE STRING "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I=/usr/xg/include" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L=/usr/xg/lib32 -lxg -lgen -lpthread" CACHE STRING "")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -L=/usr/xg/lib32 -lxg -lgen -lpthread" CACHE STRING "")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -L=/usr/xg/lib32 -lxg -lgen -lpthread" CACHE STRING "")

include(${CMAKE_CURRENT_LIST_DIR}/IRIX.cmake)

# Release by default
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
# But with assertions
set(LLVM_ENABLE_ASSERTIONS ON CACHE BOOL "")

