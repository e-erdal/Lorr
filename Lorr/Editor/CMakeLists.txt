# Third party applications
set(EDITOR_VENDOR
    Lorr
)

set(EDITOR_COMPONENTS
    LorrEditorLayers
)

######### Editor components ########
# # Editor
LorrComponent(
    NAME
        LorrEditorLayers
    INCLUDE
        ./
    SOURCE
        ./Layers/*.cc
    VENDOR
        ${EDITOR_VENDOR}
)

add_executable(Editor ./main.cc)
    target_link_libraries(Editor PUBLIC ${EDITOR_VENDOR} PUBLIC ${EDITOR_COMPONENTS})
    target_include_directories(Editor PUBLIC ./)