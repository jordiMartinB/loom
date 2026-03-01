set(LOOM_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${LOOM_INCLUDE_DIR})

if (LIBZIP_FOUND)
    include_directories( SYSTEM ${LIBZIP_INCLUDE_DIR} )
    include_directories( SYSTEM ${LIBZIP_CONF_INCLUDE_DIR} )
endif( LIBZIP_FOUND )

# Platform-specific libraries
if(WIN32)
    set(PLATFORM_LIBS ws2_32)
else()
    set(PLATFORM_LIBS pthread)
endif()

add_subdirectory(shared)
add_subdirectory(util)
add_subdirectory(transitmap)
add_subdirectory(loom)
add_subdirectory(cppgtfs)
add_subdirectory(topo)
add_subdirectory(octi)
add_subdirectory(dot)
add_subdirectory(topoeval)
