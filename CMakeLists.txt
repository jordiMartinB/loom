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
    set(LOOM_COMMON_FLAGS "/W3 /EHsc /utf-8 /wd4244 /wd4267 /wd4996 /wd4251 /wd4127 /wd4101 /wd4305 /wd4309 /wd4715")
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
if(OpenMP_CXX_FOUND)
    # Use compile flags for object libraries (they don't link themselves).
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    # OpenMP::OpenMP_CXX is added to target_link_libraries(loom) below so
    # lld-link / ld receives libomp.lib / -lgomp at link time.
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

# Pin the Python interpreter for both the Python3-namespaced module (used by
# our explicit find_package call below) and the unversioned Python-namespaced
# module that pybind11 invokes internally via PYBIND11_FINDPYTHON.
# setup.py passes -DPython_EXECUTABLE / -DPython3_EXECUTABLE; map legacy
# all-caps PYTHON_EXECUTABLE as a fallback.
foreach(_pyexe Python_EXECUTABLE Python3_EXECUTABLE)
  if(NOT DEFINED ${_pyexe} AND DEFINED PYTHON_EXECUTABLE)
    set(${_pyexe} "${PYTHON_EXECUTABLE}" CACHE FILEPATH "Python interpreter (from PYTHON_EXECUTABLE)" FORCE)
  endif()
endforeach()
if(DEFINED Python3_EXECUTABLE)
  message(STATUS "Python3 executable pinned to: ${Python3_EXECUTABLE}")
endif()
if(DEFINED Python_EXECUTABLE)
  message(STATUS "Python executable pinned to: ${Python_EXECUTABLE}")
endif()

# Find Python3 FIRST so the result is cached before pybind11's internal
# find_package(Python ...) call.  This ensures pybind11 reuses the same
# interpreter and produces the correct .pyd suffix (cp313, cp314, …).
# Use Development.Module — only needs headers, not libpython (works on manylinux)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)
include_directories(${Python3_INCLUDE_DIRS})
message(STATUS "Found Python3: ${Python3_EXECUTABLE} (ABI: ${Python3_SOABI})")

find_package(pybind11 REQUIRED)

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
add_subdirectory(src/loom)
add_subdirectory(src/topo)
add_subdirectory(src/octi)

# -----------------------------------------------------------------------
# Platform-specific libraries
# -----------------------------------------------------------------------
if(WIN32)
    set(PLATFORM_LIBS ws2_32)
else()
    find_package(Threads REQUIRED)
    set(PLATFORM_LIBS Threads::Threads)
endif()


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
)

target_include_directories(loom PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
)

# pybind11_add_module enables LTO by default, which makes clang-cl emit
# -flto as a *linker* flag that lld-link does not understand (it expects
# /lto).  Disable LTO here; sccache also does not support LTO wrappers.
set_target_properties(loom PROPERTIES INTERPROCEDURAL_OPTIMIZATION OFF)

target_link_libraries(loom PRIVATE
    ${LIBZIP_LIBRARY}
    $<$<NOT:$<PLATFORM_ID:Windows>>:-lpthread>
    $<$<PLATFORM_ID:Windows>:ws2_32>
    Python3::Module
    # Link the OpenMP runtime so lld-link (Windows) / ld (Linux) can
    # resolve omp_get_num_procs, __kmpc_fork_call, etc.
    $<$<BOOL:${OpenMP_CXX_FOUND}>:OpenMP::OpenMP_CXX>
)