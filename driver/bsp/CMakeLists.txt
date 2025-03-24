project(BSP)

add_library(lib_bsp STATIC)
target_sources(lib_bsp 
    PRIVATE 
    ./${PRJ_BOARD_NAME}/src/bsp_led.c
    ./${PRJ_BOARD_NAME}/src/bsp_serialport.c    
)
target_include_directories(lib_bsp
    PUBLIC  
    ./${PRJ_BOARD_NAME}/inc
)
target_link_libraries(lib_bsp     
    PRIVATE 
    lib_hal
    lib_lwrb
)
