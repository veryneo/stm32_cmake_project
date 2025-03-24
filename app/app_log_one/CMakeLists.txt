project(APP_LOG_ONE)

# Interface library
add_library(lib_app_log_one_intf INTERFACE)
target_include_directories(lib_app_log_one_intf
    INTERFACE   
    ./inc
)

# Implement library
add_library(lib_app_log_one STATIC)
target_sources(lib_app_log_one 
    PRIVATE
    ./src/app_log_one.c
)
target_link_libraries(lib_app_log_one 
    PUBLIC
    lib_app_log_one_intf
    PRIVATE
    lib_shell_intf
    lib_threadx_intf
)