project(SHELL)

add_library(lib_shell STATIC)
target_sources(lib_shell
    PRIVATE
    ./core/src/shell.c
    ./core/src/shell_cmd_list.c
    ./core/src/shell_ext.c
    ./core/src/shell_companion.c
    # ./port/src/shell_io.c
    ./port/src/shell_mem.c
    ./extensions/log/src/log.c
)
target_include_directories(lib_shell
    PUBLIC
    ./core/inc
    ./port/inc
    ./extensions/log/inc
)
target_compile_definitions(lib_shell
    PUBLIC
    -DSHELL_CFG_USER="shell_cfg_user.h"
)
target_link_libraries(lib_shell
    PRIVATE
    lib_threadx
    lib_bsp
)