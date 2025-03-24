project(APP_LED)

add_library(lib_app_led STATIC)
target_sources(lib_app_led 
    PRIVATE
    ./src/app_led.c
)
target_include_directories(lib_app_led
    PUBLIC   
    ./inc
)
target_link_libraries(lib_app_led 
    PRIVATE
    lib_shell
    lib_threadx
    lib_bsp
)