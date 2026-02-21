include_directories(
	${LOOM_INCLUDE_DIR}
)

add_executable(octiTest TestMain.cpp)

# Conditionally link util library only for non-Windows platforms
if(NOT WIN32 AND NOT MINGW)
    target_link_libraries(octiTest octi_dep util ad_cppgtfs)
else()
    target_link_libraries(octiTest octi_dep ad_cppgtfs)
endif()
