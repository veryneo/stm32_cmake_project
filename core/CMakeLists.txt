project(CORE)

add_executable(${PRJ_EXE_NAME})
target_sources(${PRJ_EXE_NAME} 
    PRIVATE
    ./src/main.c
    ./src/syscalls.c
    ./src/sysmem.c
    ./src/stm32wbxx_it.c
    ./src/stm32wbxx_hal_timebase_tim.c
    ${PRJ_SYSINIT_SRC}
    ${PRJ_STARTUP_SRC}
    ../middlewares/threadx/ports/cortex_m4/gnu/tx_initialize_low_level.S
)
target_include_directories(${PRJ_EXE_NAME} 
    PRIVATE 
    ./inc
)
target_link_libraries(${PRJ_EXE_NAME} 
    PRIVATE   
    lib_app_entry
    lib_app_shell
    lib_app_led
    lib_app_log_one
    lib_app_log_two
    lib_shell
    lib_threadx
    lib_bsp
    lib_hal
    lib_lwrb
)
add_dependencies(${PRJ_EXE_NAME}    
    lib_app_entry
    lib_app_shell
    lib_app_led
    lib_app_log_one
    lib_app_log_two
    lib_shell
    lib_threadx
    lib_bsp
    lib_hal
    lib_lwrb
)

# Generate .bin file (Transfer .elf file to .bin file)
if(CMAKE_OBJCOPY)
    add_custom_command(
        TARGET ${PRJ_EXE_NAME} 
        POST_BUILD 
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PRJ_EXE_NAME}> $<TARGET_FILE_DIR:${PRJ_EXE_NAME}>/${PRJ_EXE_NAME}.bin) 
endif()
