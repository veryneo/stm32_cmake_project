project(LWBTN)

add_library(lib_lwbtn STATIC)
target_sources(lib_lwbtn 
    PRIVATE 
    ./src/lwbtn.c
)
target_include_directories(lib_lwbtn
    PUBLIC  
    ./inc
)