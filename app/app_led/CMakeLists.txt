project(APP_LED)

# Interface library
add_library(lib_app_led_intf INTERFACE)
target_include_directories(lib_app_led_intf
    INTERFACE   
    ./inc
)

# Implement library
add_library(lib_app_led STATIC)
target_sources(lib_app_led 
    PRIVATE
    ./src/app_led.c
)
target_link_libraries(lib_app_led 
    PUBLIC
    lib_app_led_intf
    PRIVATE
    lib_shell_intf
    lib_threadx_intf
    lib_bsp_intf
)