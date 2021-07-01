file(GLOB source *.cpp)
add_library(lw STATIC
    ${source}
)

target_include_directories(lw PUBLIC .)