project(APP_LOG_ONE)

add_library(lib_app_log_one STATIC)
target_sources(lib_app_log_one 
    PRIVATE
    ./src/app_log_one.c
)
target_include_directories(lib_app_log_one
    PUBLIC   
    ./inc
)
target_link_libraries(lib_app_log_one 
    PRIVATE
    lib_shell
    lib_threadx
)