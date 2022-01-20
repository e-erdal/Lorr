# Third party applications
set(RC_VENDOR
    Lorr
)

set(RC_COMPONENTS
)

add_executable(ResComp main.cc)
    target_link_libraries(ResComp PUBLIC ${RC_VENDOR} ${RC_COMPONENTS})
    target_include_directories(ResComp PUBLIC .)
    set_target_properties(ResComp PROPERTIES OUTPUT_NAME "ResComp-${CMAKE_BUILD_TYPE}")