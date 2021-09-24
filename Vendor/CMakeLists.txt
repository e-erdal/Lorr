###############################################################################################

add_subdirectory(bgfx-imgui)

###############################################################################################

option( BGFX_BUILD_TOOLS      "Build bgfx tools."                             OFF )
option( BGFX_BUILD_EXAMPLES   "Build bgfx examples."                          OFF )
option( BGFX_INSTALL          "Create installation target."                   ON  )
option( BGFX_INSTALL_EXAMPLES "Install examples and their runtimes."          OFF )
option( BGFX_CUSTOM_TARGETS   "Include convenience custom targets."           ON  )
option( BGFX_USE_OVR          "Build with OVR support."                       OFF )
option( BGFX_AMALGAMATED      "Amalgamated bgfx build for faster compilation" ON  )
option( BX_AMALGAMATED        "Amalgamated bx build for faster compilation"   ON  )
option( BGFX_CONFIG_DEBUG     "Enables debug configuration on all builds"     OFF )
option( BGFX_CONFIG_RENDERER_WEBGPU     "Enables the webgpu renderer"         OFF )

add_subdirectory(bgfx.cmake)

###############################################################################################

add_subdirectory(glm)

###############################################################################################

add_subdirectory(imgui)

###############################################################################################

add_subdirectory(lw)

###############################################################################################

add_subdirectory(soloud/contrib)

###############################################################################################

set(LEVEL_NAMES "{\"TRACE\", \"DEBUG\", \"INFO\", \"WARN\", \"ERROR\", \"FATAL\", \"OFF\"}")
add_subdirectory(spdlog)
target_compile_definitions(spdlog PRIVATE SPDLOG_LEVEL_NAMES=${LEVEL_NAMES})

###############################################################################################

add_subdirectory(tracy)

###############################################################################################

add_definitions(-DZLIB_DLL)
add_subdirectory(zlib)

###############################################################################################
