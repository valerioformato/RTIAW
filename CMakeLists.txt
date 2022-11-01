# CMakeList.txt : CMake project for RTIAW, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.13)

project ("RTIAW")

set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY TRUE)
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

if (NOT DEFINED CMAKE_INSTALL_RPATH)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
else ()
    message(WARNING "Manually specified install rpath: ${CMAKE_INSTALL_RPATH}")
endif ()

# add the automatically determined parts of the RPATH which point to directories outside the build tree to the install
# RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Check for C++ standard and set it to C++17 if not already set
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)

include(cmake/FetchDependencies.cmake)

add_subdirectory(vendor)
add_subdirectory(src)
