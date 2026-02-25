file(GLOB_RECURSE shared_SRC *.cpp)

include_directories(
    SYSTEM ${GUROBI_INCLUDE_DIR}
    SYSTEM ${GLPK_INCLUDE_DIR}
    SYSTEM ${COIN_INCLUDE_DIR}
)

# remove test mains from library sources
foreach(_f IN LISTS shared_SRC)
    get_filename_component(_name "${_f}" NAME)
    if(_name STREQUAL "TestMain.cpp" OR _f MATCHES "/tests/")
        list(REMOVE_ITEM shared_SRC "${_f}")
    endif()
endforeach()

# Build as object library (to be combined later)
add_library(shared_objects OBJECT ${shared_SRC})
set_target_properties(shared_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Add the shared directory and its subdirectories to the include paths
target_include_directories(shared_objects PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}  # Include the shared directory
)

# Conditionally link the util library for non-Windows platforms
if(NOT WIN32 AND NOT MINGW)
    target_link_libraries(shared_objects PUBLIC util)
endif()
