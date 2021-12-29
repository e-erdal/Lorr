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
add_subdirectory(Audio/openal)

set(BUILD_TESTING OFF)
add_subdirectory(Audio/ogg)
add_subdirectory(Audio/vorbis)

###############################################################################################

add_subdirectory(EA/EABase)
add_subdirectory(EA/EAAssert)
add_subdirectory(EA/EAStdC)
add_subdirectory(EA/EASTL)
add_subdirectory(EA/EAThread)

###############################################################################################

add_subdirectory(Utility/bx-cmake)
add_subdirectory(Utility/freetype)
add_subdirectory(Utility/glm)
add_subdirectory(Utility/lw)
add_subdirectory(Utility/spdlog)
add_subdirectory(Utility/tiny-utf8)
add_subdirectory(Utility/tracy)

add_definitions(-DZLIB_DLL)
add_subdirectory(Utility/zlib)

###############################################################################################

add_subdirectory(Graphics/bimg-cmake)
add_subdirectory(Graphics/bvh)
add_subdirectory(Graphics/DirectX-Headers)
add_subdirectory(Graphics/imgui)
add_subdirectory(Graphics/meshoptimizer)
add_subdirectory(Graphics/msdf-atlas-gen)

###############################################################################################

option(BOX2D_BUILD_UNIT_TESTS "Build the Box2D unit tests" OFF)
option(BOX2D_BUILD_TESTBED "Build the Box2D testbed" OFF)
option(BOX2D_BUILD_DOCS "Build the Box2D documentation" OFF)
option(BOX2D_USER_SETTINGS "Override Box2D settings with b2UserSettings.h" OFF)
add_subdirectory(Physics/box2d)