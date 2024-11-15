project(APP)

set(APP_SRC   	./main.c 
                ./syscalls.c
                ./sysmem.c
                ${PRJ_SYSINIT_SRC}
                ${PRJ_STARTUP_SRC})

add_executable(${PRJ_EXE_NAME} ${APP_SRC})
target_include_directories(${PRJ_EXE_NAME} PRIVATE ./inc)
target_link_libraries(${PRJ_EXE_NAME} PRIVATE lib_app_demo_1 lib_hal)

# Generate .bin file (Transfer .elf file to .bin file)
if(CMAKE_OBJCOPY)
    add_custom_command(
        TARGET ${PRJ_EXE_NAME} 
        POST_BUILD 
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PRJ_EXE_NAME}> $<TARGET_FILE_DIR:${PRJ_EXE_NAME}>/${PRJ_EXE_NAME}.bin) 
endif()

add_subdirectory(app_demo_1)