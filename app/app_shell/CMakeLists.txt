project(APP_SHELL)

add_library(lib_app_shell_intf INTERFACE)
target_include_directories(lib_app_shell_intf
    INTERFACE
    ./inc
)

add_library(lib_app_shell STATIC)
target_sources(lib_app_shell
    PRIVATE
    ./src/app_shell.c 
)
target_link_libraries(lib_app_shell
    PUBLIC
    lib_app_shell_intf
    PRIVATE
    lib_shell_intf
    lib_threadx_intf
    lib_bsp_intf
)