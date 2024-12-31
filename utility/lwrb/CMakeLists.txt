project(LWRB)

add_library(lib_lwrb STATIC)
target_sources(lib_lwrb 
    PRIVATE
    ./src/lwrb.c
    ./src/lwrb_ex.c
)
target_include_directories(lib_lwrb 
    PUBLIC 
    ./inc)              