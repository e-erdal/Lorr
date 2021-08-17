file(GLOB source *c)
add_library(zlib1 SHARED
    ${source}
)

target_include_directories(zlib1 PUBLIC .)
