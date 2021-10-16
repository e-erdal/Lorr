if (NOT "${ENGINE_VERSION_GITHASH}" STREQUAL "")
    add_definitions(-DENGINE_VERSION_GITHASH="${ENGINE_VERSION_GITHASH}")
    message("Current commit is ${ENGINE_VERSION_GITHASH}.")
endif()

if (NOT "${ENGINE_VERSION_PATCH}" STREQUAL "")
    add_definitions(-DENGINE_VERSION_PATCH=${ENGINE_VERSION_PATCH})
    message("Current patch(rev) is ${ENGINE_VERSION_PATCH}.")
endif()

set(ENGINE_GPU_LIBS)

if (WIN32)
    set(ENGINE_GPU_LIBS
        d3dcompiler
        d3d11
    )
endif()

# Third party applications
set(ENGINE_VENDOR
    bx
    bimg
    glm
    imgui
    lw
    OpenAL
    spdlog
    tinyutf8
    tracy
    vorbis
    vorbisfile
    zlib1
    box2d
    freetype-gl
)

set(ENGINE_COMPONENTS
    LorrAudio
    LorrCore
    LorrDebug
    LorrECS
    LorrGraphics
    LorrManagers
    LorrModel
    LorrSystems
)

######### Engine components ########
# Audio
LorrComponent(
    NAME
        LorrAudio
    INCLUDE
        ../
    PCH
        App/pch.hh
    SOURCE
        Audio/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

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

# ECS
LorrComponent(
    NAME
        LorrECS
    INCLUDE
        ../
    PCH
        App/pch.hh
    SOURCE
        ECS/*.cc
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

# Model
LorrComponent(
    NAME
        LorrModel
    INCLUDE
        ../
    PCH
        App/pch.hh
    SOURCE
        Model/*.cc
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
        ${ENGINE_COMPONENTS} ${ENGINE_GPU_LIBS} 
)