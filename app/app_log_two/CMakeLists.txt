project(APP_LOG_TWO)

# Interface library
add_library(lib_app_log_two_intf INTERFACE)
target_include_directories(lib_app_log_two_intf
    INTERFACE   
    ./inc
)

# Implement library
add_library(lib_app_log_two STATIC)
target_sources(lib_app_log_two 
    PRIVATE
    ./src/app_log_two.c
)
target_link_libraries(lib_app_log_two 
    PUBLIC
    lib_app_log_two_intf
    PRIVATE
    lib_shell_intf
    lib_threadx_intf
)