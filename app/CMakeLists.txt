project(APP)

set(APP_SRC   	./main.c 
                ./syscalls.c
                ./sysmem.c
                ${PRJ_SYSINIT_SRC}
                ${PRJ_STARTUP_SRC})

add_executable(${PRJ_EXE_NAME} ${APP_SRC})
target_include_directories(${PRJ_EXE_NAME} PRIVATE ./inc)
target_link_libraries(${PRJ_EXE_NAME} PRIVATE lib_app_demo_1 lib_st_hal)


add_subdirectory(app_demo_1)