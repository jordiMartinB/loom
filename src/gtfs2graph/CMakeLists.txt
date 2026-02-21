file(GLOB_RECURSE gtfs2graph_SRC *.cpp)

set(gtfs2graph_main Gtfs2GraphMain.cpp)

list(REMOVE_ITEM gtfs2graph_SRC ${gtfs2graph_main})
list(REMOVE_ITEM gtfs2graph_SRC TestMain.cpp)

add_subdirectory(tests)

configure_file (
  "_config.h.in"
  "_config.h"
)

add_executable(gtfs2graph ${gtfs2graph_main})
add_library(gtfs2graph_dep ${gtfs2graph_SRC})

target_include_directories(gtfs2graph_dep PUBLIC ${PROJECT_SOURCE_DIR}/src/cppgtfs/src)

# Conditionally link util library only for non-Windows platforms
if(NOT WIN32 AND NOT MINGW)
    target_link_libraries(gtfs2graph gtfs2graph_dep pb_util_geo pb_util_graph pb_util_json ad_cppgtfs -lpthread util)
else()
    target_link_libraries(gtfs2graph gtfs2graph_dep pb_util_geo pb_util_graph pb_util_json ad_cppgtfs -lpthread)
endif()
