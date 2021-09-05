# Third party applications
set(ENGINE_VENDOR
    bgfx
    bx
    bimg
    glm
    imgui
    bgfx-imgui
    lw
    spdlog
    tracy
    zlib1
)

set(ENGINE_COMPONENTS
    LorrCore
    LorrDebug
    LorrGraphics
    LorrManagers
    LorrSystems
)

######### Engine components ########
# Core
LorrComponent(
    NAME
        LorrCore
    INCLUDE
        src/
    PCH
        src/pch.hh
    SOURCE
        src/Core/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

# Debug
LorrComponent(
    NAME
        LorrDebug
    INCLUDE
        src/
    PCH
        src/pch.hh
    SOURCE
        src/Debug/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

# Graphics
LorrComponent(
    NAME
        LorrGraphics
    INCLUDE
        src/
    PCH
        src/pch.hh
    SOURCE
        src/Graphics/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

# Managers
LorrComponent(
    NAME
        LorrManagers
    INCLUDE
        src/
    PCH
        src/pch.hh
    SOURCE
        src/Managers/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

# Systems
LorrComponent(
    NAME
        LorrSystems
    INCLUDE
        src/
    PCH
        src/pch.hh
    SOURCE
        src/Systems/*.cc
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