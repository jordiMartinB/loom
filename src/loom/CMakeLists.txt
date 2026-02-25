file(GLOB_RECURSE loom_all_SRC *.cpp)

# remove test sources and main sources from library sources
set(loom_SRC ${loom_all_SRC})

list(APPEND loom_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Loom.cpp")

# generate config header if template exists
configure_file (
  "_config.h.in"
  "_config.h"
)

# Build as an object library (to be combined later)
add_library(loom_objects OBJECT ${loom_SRC})
set_target_properties(loom_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Add include directories for loom_objects (make generated headers visible)
target_include_directories(loom_objects PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/..
)

