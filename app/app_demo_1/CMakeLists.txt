project(APP_DEMO_1)

add_library(lib_app_demo_1 STATIC)
target_sources(lib_app_demo_1 
    PRIVATE
    ./src/app_demo_1.c
)
target_include_directories(lib_app_demo_1 
    PUBLIC 
    ./inc
)
target_link_libraries(lib_app_demo_1 
    PRIVATE
    lib_bsp
)