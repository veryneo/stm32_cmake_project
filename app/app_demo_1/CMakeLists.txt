project(APP_DEMO_1)

set(APP_DEMO_1_SRC ./src/app_demo_1.c)

add_library(lib_app_demo_1 STATIC ${APP_DEMO_1_SRC})
target_include_directories(lib_app_demo_1 PUBLIC ./inc)
target_link_libraries(lib_app_demo_1 PRIVATE -Wl,--whole-archive lib_bsp -Wl,--no-whole-archive)