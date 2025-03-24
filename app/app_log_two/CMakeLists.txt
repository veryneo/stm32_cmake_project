project(APP_LOG_TWO)

add_library(lib_app_log_two STATIC)
target_sources(lib_app_log_two 
    PRIVATE
    ./src/app_log_two.c
)
target_include_directories(lib_app_log_two
    PUBLIC   
    ./inc
)
target_link_libraries(lib_app_log_two 
    PRIVATE
    lib_shell
    lib_threadx
)