project(APP_DEMO)

add_library(lib_app_demo STATIC)
target_sources(lib_app_demo 
    PRIVATE
    ./src/app_demo.c
)
target_include_directories(lib_app_demo 
    PUBLIC 
    ./inc
)
target_link_libraries(lib_app_demo
    PRIVATE
    lib_bsp
    lib_nfc7
)
add_dependencies(lib_app_demo
    lib_bsp
    lib_nfc7
)