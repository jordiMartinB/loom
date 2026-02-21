file(GLOB_RECURSE loom_all_SRC *.cpp)

# remove test sources and main sources from library sources
set(loom_SRC ${loom_all_SRC})
foreach(_f IN LISTS loom_all_SRC)
    get_filename_component(_name "${_f}" NAME)
    if(_name STREQUAL "TestMain.cpp" OR _name STREQUAL "LoomMain.cpp" OR _f MATCHES "/tests/")
        list(REMOVE_ITEM loom_SRC "${_f}")
    endif()
endforeach()

# Avoid duplicate symbols: don't compile config/ConfigReader.cpp into loom_dep
list(REMOVE_ITEM loom_SRC "${CMAKE_CURRENT_SOURCE_DIR}/config/ConfigReader.cpp")

list(APPEND loom_SRC "${CMAKE_CURRENT_SOURCE_DIR}/Loom.cpp")

# generate config header if template exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/_config.h.in")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/_config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/_config.h" @ONLY)
endif()

# Build as a shared library
add_library(loom_dep SHARED ${loom_SRC})
set_target_properties(loom_dep PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Add include directories for loom_dep (make generated headers visible)
target_include_directories(loom_dep PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/..
)

# Link dependencies
target_link_libraries(loom_dep PUBLIC loom_includes shared_dep dot_dep pb_util_graph pb_util pb_util_json pb_util_geo ${GLPK_LIBRARY} ${GUROBI_LIBRARY} ${COIN_LIBRARIES} -lpthread)
