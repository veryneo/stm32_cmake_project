project(BSP)

add_library(lib_bsp STATIC)
target_sources(lib_bsp 
    PRIVATE 
    ./p_nucleo_wb55/src/bsp.c
    ./p_nucleo_wb55/src/bsp_led.c
    ./p_nucleo_wb55/src/bsp_uart.c   
    ./p_nucleo_wb55/src/bsp_switch.c 
    ./p_nucleo_wb55/src/bsp_nfc07a1.c
    ./p_nucleo_wb55/src/bsp_lowpower.c
    ./components/st25dvxxkc/src/st25dvxxkc.c
    ./components/st25dvxxkc/src/st25dvxxkc_reg.c
)
target_include_directories(lib_bsp 
    PUBLIC 
    ./p_nucleo_wb55/inc
    ./components/st25dvxxkc/inc
)
target_link_libraries(lib_bsp 
    PRIVATE 
    lib_hal     # HAL的头文件都在bsp的.c源文件，所以这里将lib_hal可见性设PRIVATE
    lib_lwrb    # lwrb的头文件都在bsp的.c源文件，所以这里将lib_lwrb可见性设PRIVATE
) 

# get_target_property (res lib_bsp INCLUDE_DIRECTORIES)
# message(STATUS "lib_bsp Include Directories = ${res}")
# get_target_property (res lib_bsp INTERFACE_INCLUDE_DIRECTORIES)
# message(STATUS "lib_bsp Interface Include Directories = ${res}")