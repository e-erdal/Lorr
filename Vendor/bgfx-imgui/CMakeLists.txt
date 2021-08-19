add_library(bgfx-imgui STATIC imgui_impl_bgfx.cpp)
    target_include_directories(bgfx-imgui PUBLIC .)
    target_link_libraries(bgfx-imgui PUBLIC imgui bgfx bx)
