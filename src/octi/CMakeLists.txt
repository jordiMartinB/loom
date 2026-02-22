file(GLOB_RECURSE octi_all_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# remove test sources and main sources from library sources
set(octi_SRC ${octi_all_SRC})
foreach(_f IN LISTS octi_all_SRC)
    get_filename_component(_name "${_f}" NAME)
    if(_name STREQUAL "TestMain.cpp" OR _name STREQUAL "OctiMain.cpp" OR _f MATCHES "/tests/")
        list(REMOVE_ITEM octi_SRC "${_f}")
    endif()
endforeach()

# compile RunMain into the shared lib so run_main symbol is provided
list(APPEND octi_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Octi.cpp")

# generate config header if template exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/_config.h.in")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/_config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/_config.h" @ONLY)
endif()

# Build as a shared library
add_library(octi_dep SHARED ${octi_SRC})
set_target_properties(octi_dep PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Add the octi directory to the include paths (make generated headers visible)
target_include_directories(octi_dep PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/..
)

# Link dependencies
target_link_libraries(octi_dep PUBLIC loom_includes shared_dep pb_util_geo pb_util_graph pb_util_json pb_util dot_dep ${GLPK_LIBRARY} ${GUROBI_LIBRARY} ${COIN_LIBRARIES} -lpthread)
