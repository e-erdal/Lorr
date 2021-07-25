# Third party applications
set(ENGINE_VENDOR
    glm
    imgui
    lw
    spdlog
    tracy
)

set(ENGINE_COMPONENTS
    LorrCoreDebug
    LorrCoreGraphics
    LorrCoreSystems
)

######### Engine components ########
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

# Systems
LorrComponent(
    NAME
        LorrCoreSystems
    INCLUDE
        src/
    PCH
        src/pch.hh
    SOURCE
        src/Core/Systems/*.cc
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