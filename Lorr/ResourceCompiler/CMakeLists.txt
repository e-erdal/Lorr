# Third party applications
set(RC_VENDOR
    Lorr
)

set(RC_COMPONENTS
)

######### Editor components ########
# # GameApp
# LorrComponent(
#     NAME
#         GameApp
#     INCLUDE
#         src/
#     SOURCE
#         src/GameApp/*.cc
#     VENDOR
#         ${GAME_VENDOR}
# )

add_executable(RC src/main.cc)
    target_link_libraries(RC PUBLIC ${RC_VENDOR} ${RC_COMPONENTS})
    target_include_directories(RC PUBLIC src/)