if(BUILD_TESTS)
    include_directories(
        ${LOOM_INCLUDE_DIR}
    )

    add_executable(sharedTest TestMain.cpp)

    # Conditionally link util library only for non-Windows platforms
    if(NOT WIN32 AND NOT MINGW)
        target_link_libraries(sharedTest shared_dep util ${GUROBI_LIBRARY} ${GLPK_LIBRARY} ${COIN_LIBRARIES})
    else()
        target_link_libraries(sharedTest shared_dep ${GUROBI_LIBRARY} ${GLPK_LIBRARY} ${COIN_LIBRARIES})
    endif()
endif()