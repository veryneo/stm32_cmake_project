project(BSP)

file(GLOB BSP_SRC ./p_nucleo_wb55/src/*.c)

add_library(lib_bsp STATIC ${BSP_SRC})
target_include_directories(lib_bsp PUBLIC ./p_nucleo_wb55/inc)
target_link_libraries(lib_bsp PRIVATE lib_hal lib_lwrb) # HAL的头文件都在bsp的.c源文件，所以这里将lib_hal可见性设PRIVATE

# get_target_property (res lib_bsp INCLUDE_DIRECTORIES)
# message(STATUS "lib_bsp Include Directories = ${res}")
# get_target_property (res lib_bsp INTERFACE_INCLUDE_DIRECTORIES)
# message(STATUS "lib_bsp Interface Include Directories = ${res}")