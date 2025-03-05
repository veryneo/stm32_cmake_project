project(APP_ENTRY)

# Interface library
add_library(lib_app_entry_intf INTERFACE)
target_include_directories(lib_app_entry_intf
    INTERFACE
    ./inc
)

# Implement library
add_library(lib_app_entry STATIC)
target_sources(lib_app_entry
    PRIVATE
    ./src/app_entry.c
)
target_link_libraries(lib_app_entry
    PUBLIC
    lib_app_entry_intf
    PRIVATE
    lib_app_shell_intf
    lib_app_led_intf
    lib_threadx_intf
)