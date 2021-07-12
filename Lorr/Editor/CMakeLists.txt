# Third party applications
set(EDITOR_VENDOR
    Lorr
)

set(EDITOR_COMPONENTS
    # LorrEditor
)

######### Editor components ########
# # Editor
# file(GLOB EDITOR_SOURCES
#     src/*.cc
# )

# add_library(LorrEditor ${EDITOR_SOURCES})
#     target_include_directories(LorrEditor PUBLIC src/)

# # Link them all
# target_link_libraries(LorrEditor PUBLIC ${EDITOR_VENDOR})

add_executable(Editor src/main.cc)
    target_link_libraries(Editor PUBLIC ${EDITOR_VENDOR} PUBLIC ${EDITOR_COMPONENTS})
    target_include_directories(Editor PUBLIC src/)