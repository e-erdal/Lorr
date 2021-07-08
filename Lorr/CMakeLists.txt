# Third party applications
set(ENGINE_VENDOR
    glm
    imgui
    lw
    spdlog
)

set(ENGINE_COMPONENTS
    LorrCoreEngine
    LorrCoreGraphics
)

######### Engine components ########
# Engine
file(GLOB_RECURSE ENGINE_RESOURCES
   src/Core/Engine/*.cc
)

add_library(LorrCoreEngine ${ENGINE_RESOURCES})
    target_precompile_headers(LorrCoreEngine PUBLIC src/pch.hh)
    target_include_directories(LorrCoreEngine PUBLIC src/)

# Graphics
file(GLOB_RECURSE GRAPHICS_RESOURCES
   src/Core/Graphics/*.cc
)

add_library(LorrCoreGraphics ${GRAPHICS_RESOURCES})
    target_precompile_headers(LorrCoreGraphics PUBLIC src/pch.hh)
    target_include_directories(LorrCoreGraphics PUBLIC src/)

# Link them all
target_link_libraries(LorrCoreEngine PUBLIC ${ENGINE_VENDOR} PUBLIC LorrCoreGraphics)
target_link_libraries(LorrCoreGraphics PUBLIC ${ENGINE_VENDOR} PUBLIC LorrCoreEngine)

# Create application
add_executable(${PROJECT_NAME} ${GAME_SOURCES} src/main.cc)
        target_precompile_headers(${PROJECT_NAME} PUBLIC src/pch.hh)
        target_link_libraries(${PROJECT_NAME} PUBLIC ${ENGINE_VENDOR} PUBLIC ${ENGINE_COMPONENTS})
        target_include_directories(${PROJECT_NAME} PUBLIC src/)