project(BSP)

set(BSP_SRC	./p_nucleo_wb55/src/bsp.c
			./p_nucleo_wb55/src/bsp_com_it.c
			./p_nucleo_wb55/src/bsp_led.c
			./p_nucleo_wb55/src/bsp_uart.c)

add_library(lib_bsp STATIC ${BSP_SRC})
target_include_directories(lib_bsp PUBLIC ./p_nucleo_wb55/inc)
target_link_libraries(lib_bsp PRIVATE lib_st_hal) # HAL的头文件都在bsp的.c源文件，所以这里可见性可以设PRIVATE