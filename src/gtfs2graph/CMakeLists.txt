file(GLOB_RECURSE gtfs2graph_SRC *.cpp)

set(gtfs2graph_main Gtfs2GraphMain.cpp)

list(REMOVE_ITEM gtfs2graph_SRC ${gtfs2graph_main})
list(REMOVE_ITEM gtfs2graph_SRC TestMain.cpp)

add_subdirectory(tests)

configure_file(
  "_config.h.in"
  "_config.h"
)

# Build as a shared library
add_library(gtfs2graph_dep SHARED ${gtfs2graph_SRC})
set_target_properties(gtfs2graph_dep PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Add include directories for gtfs2graph_dep
target_include_directories(gtfs2graph_dep PUBLIC ${PROJECT_SOURCE_DIR}/src/cppgtfs/src)

# Conditionally link util library only for non-Windows platforms
if(NOT WIN32 AND NOT MINGW)
    target_link_libraries(gtfs2graph_dep PUBLIC pb_util_geo pb_util_graph pb_util_json ad_cppgtfs -lpthread util)
else()
    target_link_libraries(gtfs2graph_dep PUBLIC pb_util_geo pb_util_graph pb_util_json ad_cppgtfs -lpthread)
endif()
