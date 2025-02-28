project(LWRB)

# Interface library
add_library(lib_lwrb_intf INTERFACE)
target_include_directories(lib_lwrb_intf
    INTERFACE   
    ./inc
)

# Implement library
add_library(lib_lwrb STATIC)
target_sources(lib_lwrb 
    PRIVATE
    ./src/lwrb.c
    ./src/lwrb_ex.c
)
target_link_libraries(lib_lwrb 
    PRIVATE
    lib_lwrb_intf
)              