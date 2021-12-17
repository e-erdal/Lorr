# Third party applications
set(GAME_VENDOR
    Lorr
)

set(GAME_COMPONENTS
    GameApp
    Scenes
    Terrain
)

######### Game components ########
# GameApp
LorrComponent(
    NAME
        GameApp
    INCLUDE
        ./
    SOURCE
        ./GameApp/*.cc
    VENDOR
        ${GAME_VENDOR}
)

# Scenes
LorrComponent(
    NAME
        Scenes
    INCLUDE
        ./
    SOURCE
        ./Scenes/*.cc
    VENDOR
        ${GAME_VENDOR}
)

# Terrain
LorrComponent(
    NAME
        Terrain
    INCLUDE
        ./
    SOURCE
        ./Terrain/*.cc
    VENDOR
        ${GAME_VENDOR}
)

add_executable(Game main.cc)
    target_link_libraries(Game PUBLIC ${GAME_VENDOR} ${GAME_COMPONENTS})
    target_include_directories(Game PUBLIC .)