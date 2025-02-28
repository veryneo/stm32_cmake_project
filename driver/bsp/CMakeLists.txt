project(BSP)

# Interface library
add_library(lib_bsp_intf INTERFACE)
target_include_directories(lib_bsp_intf
    INTERFACE   
    ./p_nucleo_wb55/inc
)

# Implement library
add_library(lib_bsp STATIC)
target_sources(lib_bsp 
    PRIVATE 
    ./p_nucleo_wb55/src/bsp_led.c
    ./p_nucleo_wb55/src/bsp_serialport.c    
)
target_link_libraries(lib_bsp 
    # INTERFACE 
    # 这里如果使用 INTERFACE，意思就是借助 lib_bsp 继续传递 lib_bsp_intf 相关属性，
    # 并且不参与到 lib_bsp 的编译中，lib_bsp 无法获取 lib_bsp_intf 的依赖
    # 因此会出现 bsp_led.c 找不到对应头文件 bsp_led.h 的情况
    PUBLIC
    lib_bsp_intf    
    PRIVATE 
    lib_hal_intf
    lib_lwrb_intf
)

# target_include_directories(lib_bsp 
#     PUBLIC 
#     ./p_nucleo_wb55/inc
#     PRIVATE
#     # HAL
#     ../hal/STM32WBxx_HAL_Driver/inc
#     ../hal/STM32WBxx_HAL_Driver/inc/legacy
#     ../cmsis/core/inc
#     ../cmsis/device/ST/STM32WBxx/inc
#     # lwrb
#     ../../utility/lwrb/inc
# )
# target_compile_definitions(lib_bsp 
#     PRIVATE   
#     -D${PRJ_MCU_MODEL} 
#     -DUSE_HAL_DRIVER
# )

# target_link_libraries(lib_bsp 
#     PRIVATE 
#     lib_hal     # HAL的头文件都在bsp的.c源文件，所以这里将lib_hal可见性设PRIVATE
#     lib_lwrb    # lwrb的头文件都在bsp的.c源文件，所以这里将lib_lwrb可见性设PRIVATE
# ) 

# get_target_property (res lib_bsp INCLUDE_DIRECTORIES)
# message(STATUS "lib_bsp Include Directories = ${res}")
# get_target_property (res lib_bsp INTERFACE_INCLUDE_DIRECTORIES)
# message(STATUS "lib_bsp Interface Include Directories = ${res}")