# Third party applications
set(EDITOR_VENDOR
    Lorr
)

set(EDITOR_COMPONENTS
    LorrEditorLayers
    LorrEditorApp
)

######### Editor components ########
# App
LorrComponent(
    NAME
    LorrEditorApp
    INCLUDE
        ./
    SOURCE
        ./App/*.cc
    VENDOR
        ${EDITOR_VENDOR}
)

# Layers
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
    set_target_properties(Editor PROPERTIES OUTPUT_NAME "Editor-${CMAKE_BUILD_TYPE}")