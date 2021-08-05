# Third party applications
set(GAME_VENDOR
    Lorr
)

# set(GAME_COMPONENTS
#     LorrEditorLayers
# )

# ######### Editor components ########
# # # Editor
# LorrComponent(
#     NAME
#         LorrEditorLayers
#     INCLUDE
#         src/
#     SOURCE
#         src/Layers/*.cc
#     VENDOR
#         ${EDITOR_VENDOR}
# )

add_executable(Game src/main.cc)
    target_link_libraries(Game PUBLIC ${GAME_VENDOR})
    target_include_directories(Game PUBLIC src/)