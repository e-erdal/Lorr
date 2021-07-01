set(GAME_LIBS
    glad
    glfw
    glm
    lw
)

file(GLOB_RECURSE GAME_SOURCES
   src/*.cc
)

add_executable(${PROJECT_NAME} ${GAME_SOURCES} src/main.cc)
        target_precompile_headers(${PROJECT_NAME} PUBLIC src/pch.hh)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GAME_LIBS})
        target_include_directories(${PROJECT_NAME} PRIVATE src/)