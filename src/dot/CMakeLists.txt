file(GLOB_RECURSE dot_SRC *.cpp)

set(dot2tg_main Dot2TgMain.cpp)

list(REMOVE_ITEM dot_SRC ${dot2tg_main})
list(REMOVE_ITEM dot_SRC TestMain.cpp)

include_directories(
    ${LOOM_INCLUDE_DIR}
)

# Conditionally add the tests directory
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

configure_file (
  "_config.h.in"
  "_config.h"
)

add_executable(dot2tg ${dot2tg_main})
add_library(dot_dep ${dot_SRC})

target_include_directories(dot_dep PUBLIC ${PROJECT_SOURCE_DIR}/src/cppgtfs/src)
target_link_libraries(dot_dep PUBLIC loom_includes)

# Conditionally link util library only for non-Windows platforms
if(NOT WIN32 AND NOT MINGW)
    target_link_libraries(dot2tg dot_dep util -lpthread)
else()
    target_link_libraries(dot2tg dot_dep -lpthread)
endif()
