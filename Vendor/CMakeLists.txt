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

option(ALSOFT_DLOPEN  "Check for the dlopen API for loading optional libs"  ON)
option(ALSOFT_WERROR  "Treat compile warnings as errors"      OFF)
option(ALSOFT_UTILS "Build utility programs"  OFF)
option(ALSOFT_NO_CONFIG_UTIL "Disable building the alsoft-config utility" OFF)
option(ALSOFT_EXAMPLES  "Build example programs"  OFF)

option(ALSOFT_INSTALL "Install main library" ON)
option(ALSOFT_INSTALL_CONFIG "Install alsoft.conf sample configuration file" ON)
option(ALSOFT_INSTALL_HRTF_DATA "Install HRTF data files" ON)
option(ALSOFT_INSTALL_AMBDEC_PRESETS "Install AmbDec preset files" ON)
option(ALSOFT_INSTALL_EXAMPLES "Install example programs (alplay, alstream, ...)" OFF)
option(ALSOFT_INSTALL_UTILS "Install utility programs (openal-info, alsoft-config, ...)" OFF)
option(ALSOFT_UPDATE_BUILD_VERSION "Update git build version info" ON)

add_subdirectory(openal)

###############################################################################################

set(LEVEL_NAMES "{\"TRACE\", \"DEBUG\", \"INFO\", \"WARN\", \"ERROR\", \"FATAL\", \"OFF\"}")
add_subdirectory(spdlog)
target_compile_definitions(spdlog PRIVATE SPDLOG_LEVEL_NAMES=${LEVEL_NAMES})

###############################################################################################

add_subdirectory(tracy)

###############################################################################################

add_subdirectory(ogg)
add_subdirectory(vorbis)

###############################################################################################

add_definitions(-DZLIB_DLL)
add_subdirectory(zlib)

###############################################################################################
