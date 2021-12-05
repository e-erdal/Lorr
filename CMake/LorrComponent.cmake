function(LorrComponent)

cmake_parse_arguments(
    COMPONENT
        "" # options
        "NAME" # one value
        "INCLUDE;SOURCE;VENDOR;PCH" # multi value
        ${ARGN}
    )

    if (NOT COMPONENT_NAME)
        message(FATAL_ERROR "Component name has to be set!")
    endif()

    if (NOT COMPONENT_SOURCE)
        message(FATAL_ERROR "You cannot create a component without source code.")
    endif()

    file(GLOB_RECURSE SOURCES
        ${COMPONENT_SOURCE}
    )

    add_library(${COMPONENT_NAME} STATIC ${SOURCES})

    if (COMPONENT_PCH)
        target_precompile_headers(${COMPONENT_NAME} PUBLIC ${COMPONENT_PCH})
    endif()

    if(COMPONENT_VENDOR)
        target_link_libraries(${COMPONENT_NAME} PUBLIC ${COMPONENT_VENDOR})
    endif()
    
        target_include_directories(${COMPONENT_NAME} PUBLIC ${COMPONENT_INCLUDE})

endfunction(LorrComponent)
