project(LWRB)

set(LWRB_SRC    ./src/lwrb.c
                ./src/lwrb_ex.c)

add_library(lib_lwrb STATIC ${LWRB_SRC})
target_include_directories(lib_lwrb PUBLIC ./inc)              