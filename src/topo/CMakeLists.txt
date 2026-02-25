file(GLOB_RECURSE topo_all_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# remove test sources and the TopoMain from the library sources
set(topo_SRC ${topo_all_SRC})
foreach(_f IN LISTS topo_all_SRC)
    get_filename_component(_name "${_f}" NAME)
    if(_name STREQUAL "TestMain.cpp" OR _name STREQUAL "TopoMain.cpp" OR _f MATCHES "/tests/")
        list(REMOVE_ITEM topo_SRC "${_f}")
    endif()
endforeach()

list(APPEND topo_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Topo.cpp")

# generate config header if template exists
configure_file (
  "_config.h.in"
  "_config.h"
)

# Build object library (replace shared library)
add_library(topo_objects OBJECT ${topo_SRC})
set_target_properties(topo_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ensure generated headers are visible so includes like "topo/_config.h" resolve
target_include_directories(topo_objects PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}          # source includes
    ${CMAKE_CURRENT_BINARY_DIR}          # topo/_config.h location
    ${CMAKE_CURRENT_BINARY_DIR}/..       # parent build dir so "topo/_config.h" can be found
)

