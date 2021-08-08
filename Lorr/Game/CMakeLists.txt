# Third party applications
set(GAME_VENDOR
    Lorr
)

set(GAME_COMPONENTS
    GameApp
)

######### Editor components ########
# # Editor
LorrComponent(
    NAME
        GameApp
    INCLUDE
        src/
    SOURCE
        src/GameApp/*.cc
    VENDOR
        ${GAME_VENDOR}
)

add_executable(Game src/main.cc)
    target_link_libraries(Game PUBLIC ${GAME_VENDOR} ${GAME_COMPONENTS})
    target_include_directories(Game PUBLIC src/)