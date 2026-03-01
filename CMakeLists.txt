cmake_minimum_required (VERSION 3.18)
set(CMAKE_CXX_STANDARD 17)

project (loom-python-plugin)

# -----------------------------------------------------------------------
# OS / Platform detection
# -----------------------------------------------------------------------
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(LOOM_OS "windows")
    set(LOOM_WINDOWS TRUE)
    message(STATUS "Target OS: Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LOOM_OS "linux")
    set(LOOM_LINUX TRUE)
    message(STATUS "Target OS: Linux")
else()
    message(WARNING "Unknown target OS: ${CMAKE_SYSTEM_NAME}")
endif()

# -----------------------------------------------------------------------
# Build type normalisation
# -----------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Build type: Debug | Release | RelWithDebInfo | MinSizeRel" FORCE)
endif()

string(SUBSTRING ${CMAKE_BUILD_TYPE} 0 1 FIRST_CHAR)
string(TOUPPER ${FIRST_CHAR} FIRST_CHAR)
string(REGEX REPLACE "^.(.*)" "${FIRST_CHAR}\\1" CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# -----------------------------------------------------------------------
# Paths
# -----------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# Remove these — they override what setup.py passes via CMAKE_LIBRARY_OUTPUT_DIRECTORY
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/lib/debug")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/release")

# -----------------------------------------------------------------------
# Options
# -----------------------------------------------------------------------
option(BUILD_TESTS "Build tests" OFF)

# -----------------------------------------------------------------------
# Compiler flags per build type
# -----------------------------------------------------------------------
if(MSVC)
    set(LOOM_COMMON_FLAGS "/W3 /EHsc /wd4244 /wd4267 /wd4996 /wd4251 /wd4127")
    add_compile_definitions(
        _CRT_SECURE_NO_WARNINGS
        _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
    set(CMAKE_CXX_FLAGS_DEBUG          "/Od /Zi /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE        "/O2 /MD")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Zi /MD")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "/O1 /MD")
else()
    set(LOOM_COMMON_FLAGS
        "-Wall -Wextra"
        "-Wno-unused-parameter"
        "-Wno-unused-variable"
        "-Wno-sign-compare"
        "-Wno-deprecated-declarations"
        "-Wno-missing-field-initializers"
    )
    string(JOIN " " LOOM_COMMON_FLAGS ${LOOM_COMMON_FLAGS})
    if(LOOM_WINDOWS)
        set(LOOM_COMMON_FLAGS "${LOOM_COMMON_FLAGS} -D_WIN32_WINNT=0x0601")
    endif()
    set(CMAKE_CXX_FLAGS_DEBUG          "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LOOM_COMMON_FLAGS}")

# -----------------------------------------------------------------------
# OpenMP
# -----------------------------------------------------------------------
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# -----------------------------------------------------------------------
# LibZip
# -----------------------------------------------------------------------
find_package(LibZip)
if(LIBZIP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLIBZIP_FOUND=1")
else()
    message(STATUS "Configuring without libzip")
endif()

# -----------------------------------------------------------------------
# Export compile commands (for clangd / clang-tidy)
# -----------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------
# Python + pybind11
# -----------------------------------------------------------------------
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)
# Use Development.Module — only needs headers, not libpython (works on manylinux)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)
include_directories(${Python3_INCLUDE_DIRS})

# -----------------------------------------------------------------------
# Project include dirs
# -----------------------------------------------------------------------
set(LOOM_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src")
include_directories(${LOOM_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_BINARY_DIR}/src)

# -----------------------------------------------------------------------
# Git version
# -----------------------------------------------------------------------
include(GetGitRevisionDescription)
git_get_tag(VERSION_GIT)
get_git_is_dirty(VERSION_GIT_IS_DIRTY)
if("${VERSION_GIT_IS_DIRTY}" STREQUAL "")
    set(VERSION_GIT_FULL "${VERSION_GIT}")
else()
    set(VERSION_GIT_FULL "${VERSION_GIT}-${VERSION_GIT_IS_DIRTY}")
endif()

# -----------------------------------------------------------------------
# Subdirectories
# -----------------------------------------------------------------------
add_subdirectory(src/util)
add_subdirectory(src/shared)
add_subdirectory(src/transitmap)
add_subdirectory(src/loom)
# add_subdirectory(src/cppgtfs)
# add_subdirectory(src/gtfs2graph)
add_subdirectory(src/topo)
add_subdirectory(src/octi)
add_subdirectory(src/dot)

# -----------------------------------------------------------------------
# pybind11 module
# -----------------------------------------------------------------------
pybind11_add_module(loom
    src/Python.cpp
    $<TARGET_OBJECTS:util_objects>
    $<TARGET_OBJECTS:shared_objects>
    $<TARGET_OBJECTS:loom_objects>
    $<TARGET_OBJECTS:topo_objects>
    $<TARGET_OBJECTS:octi_objects>
    # $<TARGET_OBJECTS:cppgtfs_objects>
    $<TARGET_OBJECTS:dot_objects>
    # $<TARGET_OBJECTS:gtfs2graph_objects>
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

# -----------------------------------------------------------------------
# Output naming  (platform-aware)
# -----------------------------------------------------------------------
# Remove the custom output naming — pybind11_add_module already sets the
# correct name, prefix and suffix for the platform
# if(LOOM_WINDOWS)
#     set_target_properties(loom PROPERTIES
#         OUTPUT_NAME "loom-python-plugin"
#         PREFIX      "lib"
#         SUFFIX      ".dll"
#     )
# else()
#     set_target_properties(loom PROPERTIES
#         OUTPUT_NAME "loom-python-plugin"
#         PREFIX      "lib"
#         SUFFIX      ".so"
#     )
# endif()