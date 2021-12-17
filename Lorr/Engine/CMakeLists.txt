if (NOT "${ENGINE_VERSION_GITHASH}" STREQUAL "")
    add_definitions(-DENGINE_VERSION_GITHASH="${ENGINE_VERSION_GITHASH}")
    message("Current commit is ${ENGINE_VERSION_GITHASH}.")
endif()

if (NOT "${ENGINE_VERSION_PATCH}" STREQUAL "")
    add_definitions(-DENGINE_VERSION_PATCH=${ENGINE_VERSION_PATCH})
    message("Current patch(rev) is ${ENGINE_VERSION_PATCH}.")
endif()

option(LR_BACKEND_D3D11 "Support for D3D11 backend." ON)
option(LR_BACKEND_D3D12 "Support for D3D12 backend." ON)

set(ENGINE_GPU_LIBS)

if (LR_BACKEND_D3D11 OR LR_BACKEND_D3D12)
    set(ENGINE_GPU_LIBS
        d3dcompiler
        dxgi
        dxguid
    )

    if (LR_BACKEND_D3D11)
        set(ENGINE_GPU_LIBS ${ENGINE_GPU_LIBS} d3d11)
    endif()

    if (LR_BACKEND_D3D12)
        set(ENGINE_GPU_LIBS ${ENGINE_GPU_LIBS} d3d12)
    endif()
endif()

# Third party applications
set(ENGINE_VENDOR
    bx
    bimg
    bvh
    DirectX-Headers
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
    meshoptimizer
    msdf-atlas-gen
)

set(ENGINE_COMPONENTS
    LorrAudio
    LorrCore
    LorrDebug
    LorrECS
    LorrGraphics
    LorrGUI
    LorrJob
    LorrManagers
    LorrModel
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
        ../ ../Engine/Graphics
    PCH
        App/pch.hh ../Engine/Graphics/pch.hh
    SOURCE
        Graphics/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

# GUI
LorrComponent(
    NAME
        LorrGUI
    INCLUDE
        ../
    PCH
        App/pch.hh
    SOURCE
        GUI/*.cc
    VENDOR
        ${ENGINE_VENDOR}
)

# Job
LorrComponent(
    NAME
        LorrJob
    INCLUDE
        ../
    PCH
        App/pch.hh
    SOURCE
        Job/*.cc
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

LorrComponent(
    NAME
        Lorr
    SOURCE
        App/*.cc
    VENDOR
        ${ENGINE_COMPONENTS} ${ENGINE_GPU_LIBS} 
)


if (LR_BACKEND_D3D11)
    target_compile_definitions(LorrGraphics PUBLIC "LR_BACKEND_D3D11=1")
endif()

if (LR_BACKEND_D3D12)
    target_compile_definitions(LorrGraphics PUBLIC "LR_BACKEND_D3D12=1")
endif()