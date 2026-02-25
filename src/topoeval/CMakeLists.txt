file(GLOB_RECURSE topoeval_SRC *.cpp)

set(topoeval_main TopoEvalMain.cpp)

list(REMOVE_ITEM topoeval_SRC ${topoeval_main})

include_directories(
    ${LOOM_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file (
  "_config.h.in"
  "_config.h"
)

# build sources as an OBJECT library so their object files can be linked later
add_library(topoeval_objects OBJECT ${topoeval_SRC})
set_target_properties(topoeval_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

# provide an interface target to propagate link requirements to consumers
add_library(topoeval_dep INTERFACE)

add_executable(topoeval ${topoeval_main})
target_link_libraries(topoeval topoeval_dep)
