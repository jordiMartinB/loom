set(dot2tg_main Dot2TgMain.cpp)

file(GLOB_RECURSE dot_SRC *.cpp)

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

# Build object library so sources compile to object files that can be linked into
# a single library later.
add_library(dot_objects OBJECT ${dot_SRC})
set_target_properties(dot_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Expose include dirs and usage requirements on the object library.
target_include_directories(dot_objects PUBLIC ${PROJECT_SOURCE_DIR}/src/cppgtfs/src)