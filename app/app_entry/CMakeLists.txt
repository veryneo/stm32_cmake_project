project(APP_ENTRY)

add_library(lib_app_entry STATIC)
target_sources(lib_app_entry
    PRIVATE
    ./src/app_entry.c
)
target_include_directories(lib_app_entry
    PUBLIC
    ./inc
)
target_link_libraries(lib_app_entry
    PRIVATE
    lib_app_shell
    lib_app_led
    lib_app_log_one
    lib_app_log_two
    lib_threadx
)