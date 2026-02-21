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
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/_config.h.in")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/_config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/_config.h" @ONLY)
endif()

# do not compile main into the shared lib
add_library(topo_dep SHARED ${topo_SRC})
set_target_properties(topo_dep PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ensure generated headers are visible so includes like "topo/_config.h" resolve
target_include_directories(topo_dep PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}          # source includes
    ${CMAKE_CURRENT_BINARY_DIR}          # topo/_config.h location
    ${CMAKE_CURRENT_BINARY_DIR}/..       # parent build dir so "topo/_config.h" can be found
)

# link dependencies into the shared library
target_link_libraries(topo_dep PUBLIC shared_dep dot_dep pb_util_geo pb_util_graph pb_util_json pb_util)
