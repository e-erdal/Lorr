cmake_minimum_required(VERSION 3.0.0)
project(freetype-gl)

file(GLOB_RECURSE source *.c)
add_library(freetype-gl STATIC
    ${source}
)

target_link_libraries(freetype-gl PUBLIC freetype)
target_include_directories(freetype-gl PUBLIC include)