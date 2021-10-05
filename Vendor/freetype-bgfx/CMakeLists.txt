cmake_minimum_required(VERSION 3.0.0)
project(freetype-bgfx)

file(GLOB_RECURSE source *.c)
add_library(freetype-bgfx STATIC
    ${source}
)

target_link_libraries(freetype-bgfx PUBLIC freetype)
target_include_directories(freetype-bgfx PUBLIC include)