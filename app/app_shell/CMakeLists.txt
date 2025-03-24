project(APP_SHELL)

add_library(lib_app_shell STATIC)
target_sources(lib_app_shell
    PRIVATE
    ./src/app_shell.c 
)
target_include_directories(lib_app_shell
    PUBLIC
    ./inc
)
target_link_libraries(lib_app_shell
    PRIVATE
    lib_shell
    lib_threadx
    lib_bsp
)