# Check if BUILD_TESTS is enabled
if(BUILD_TESTS)
    include_directories(
        ${LOOM_INCLUDE_DIR}
    )

    add_executable(dotTest TestMain.cpp)

    # Conditionally link util library only for non-Windows platforms
    if(NOT WIN32 AND NOT MINGW)
        target_link_libraries(dotTest dot_dep util ad_cppgtfs)
    else()
        target_link_libraries(dotTest dot_dep ad_cppgtfs)
    endif()
endif()
