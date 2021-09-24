if (NOT "${ENGINE_VERSION_GITHASH}" STREQUAL "")
    add_definitions(-DENGINE_VERSION_GITHASH="${ENGINE_VERSION_GITHASH}")
    message("Current commit is ${ENGINE_VERSION_GITHASH}.")
endif()

if (NOT "${ENGINE_VERSION_PATCH}" STREQUAL "")
    add_definitions(-DENGINE_VERSION_PATCH=${ENGINE_VERSION_PATCH})
    message("Current patch(rev) is ${ENGINE_VERSION_PATCH}.")
endif()

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
        ../
    PCH
        App/pch.hh
    SOURCE
        Core/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

# Debug
LorrComponent(
    NAME
        LorrDebug
    INCLUDE
        ../
    PCH
        App/pch.hh
    SOURCE
        Debug/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

# Graphics
LorrComponent(
    NAME
        LorrGraphics
    INCLUDE
        ../
    PCH
        App/pch.hh
    SOURCE
        Graphics/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

# Managers
LorrComponent(
    NAME
        LorrManagers
    INCLUDE
        ../
    PCH
        App/pch.hh
    SOURCE
        Managers/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

# Systems
LorrComponent(
    NAME
        LorrSystems
    INCLUDE
        ../
    PCH
        App/pch.hh
    SOURCE
        Systems/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

LorrComponent(
    NAME
        Lorr
    SOURCE
        App/*.cc
    VENDOR
        ${ENGINE_COMPONENTS}
)