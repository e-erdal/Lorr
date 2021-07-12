# Third party applications
set(ENGINE_VENDOR
    glm
    imgui
    lw
    spdlog
)

set(ENGINE_COMPONENTS
    LorrCoreGraphics
    LorrCoreDebug
)

######### Engine components ########
# Graphics
LorrComponent(
    NAME
        LorrCoreGraphics
    INCLUDE
        src/
    PCH
        src/pch.hh
    SOURCE
        src/Core/Graphics/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

# Debug
LorrComponent(
    NAME
        LorrCoreDebug
    INCLUDE
        src/
    PCH
        src/pch.hh
    SOURCE
        src/Core/Debug/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

LorrComponent(
    NAME
        Lorr
    SOURCE
        src/Engine.cc
    VENDOR
        ${ENGINE_COMPONENTS}
)