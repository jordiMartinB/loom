cmake_minimum_required (VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project (loom-python-plugin)

if (CMAKE_BUILD_TYPE)
	string(SUBSTRING ${CMAKE_BUILD_TYPE} 0 1 FIRST_CHAR)
	string(TOUPPER ${FIRST_CHAR} FIRST_CHAR)
	string(REGEX REPLACE "^.(.*)" "${FIRST_CHAR}\\1" CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/release")

option(BUILD_TESTS "Build tests" OFF)

find_package(OpenMP)
if(OPENMP_FOUND)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(LibZip)

if (LIBZIP_FOUND)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLIBZIP_FOUND=1")
else()
	message(STATUS "Configuring without libzip")
endif()

set(CMAKE_CXX_FLAGS_DEBUG          "-Og -g -DLOGLEVEL=3")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS} -DLOGLEVEL=2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS} -DLOGLEVEL=2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -g -DLOGLEVEL=3")

# export compile commands to tools like clang
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use modern FindPython with pybind11
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)

# Use modern Python3 module
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Include Python3 headers
include_directories(${Python3_INCLUDE_DIRS})

# --- added: provide LOOM_INCLUDE_DIR for subprojects and expose generated headers
set(LOOM_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src")
include_directories(${LOOM_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_BINARY_DIR}/src)
# --- end added

# Make project headers and generated headers available to all subdirectories/targets
# include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR})

# http://brianmilco.blogspot.de/2012/11/cmake-automatically-use-git-tags-as.html
include(GetGitRevisionDescription)
git_get_tag(VERSION_GIT)
get_git_is_dirty(VERSION_GIT_IS_DIRTY)
if ("${VERSION_GIT_IS_DIRTY}" STREQUAL "")
    set(VERSION_GIT_FULL "${VERSION_GIT}")
else()
    set(VERSION_GIT_FULL "${VERSION_GIT}-${VERSION_GIT_IS_DIRTY}")
endif()

add_subdirectory(src/util)
add_subdirectory(src/shared)
add_subdirectory(src/transitmap)
add_subdirectory(src/loom)
add_subdirectory(src/cppgtfs)
add_subdirectory(src/gtfs2graph)
add_subdirectory(src/topo)
add_subdirectory(src/octi)
add_subdirectory(src/dot)

# Use pybind11_add_module so PyInit_loom is generated correctly.
# The output will be named loom.cpython-<ver>-<arch>-linux-gnu.so
pybind11_add_module(loom
    src/Python.cpp
    $<TARGET_OBJECTS:util_objects>
    $<TARGET_OBJECTS:shared_objects>
    $<TARGET_OBJECTS:loom_objects>
    $<TARGET_OBJECTS:topo_objects>
    $<TARGET_OBJECTS:octi_objects>
    $<TARGET_OBJECTS:cppgtfs_objects>
    $<TARGET_OBJECTS:dot_objects>
    $<TARGET_OBJECTS:gtfs2graph_objects>
    $<TARGET_OBJECTS:transitmap_objects>
)

target_include_directories(loom PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(loom PRIVATE
    ${LIBZIP_LIBRARY}
    -lpthread
)

# Override the output filename to libloom-python-plugin.so
# while keeping the pybind11 PyInit_loom symbol intact
set_target_properties(loom PROPERTIES
    OUTPUT_NAME "loom-python-plugin"
    PREFIX "lib"
    SUFFIX ".so"
)