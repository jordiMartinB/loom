file(GLOB_RECURSE gtfs2graph_SRC *.cpp)

set(gtfs2graph_main Gtfs2GraphMain.cpp)

list(REMOVE_ITEM gtfs2graph_SRC ${gtfs2graph_main})
list(REMOVE_ITEM gtfs2graph_SRC TestMain.cpp)

# Configure header file
configure_file (
  "_config.h.in"
  "_config.h"
)

# Build object library so sources compile to object files that can be linked into
# a single library later.
add_library(gtfs2graph_objects OBJECT ${gtfs2graph_SRC})
set_target_properties(gtfs2graph_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Add include directories for object files
target_include_directories(gtfs2graph_objects PUBLIC
    ${PROJECT_SOURCE_DIR}/src/cppgtfs/src
    ${CMAKE_CURRENT_BINARY_DIR}
)
