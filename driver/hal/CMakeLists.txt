project(HAL)

string(CONCAT HAL_PATH ${PRJ_MCU_LINE} "_HAL_Driver")

file(GLOB HAL_SRC ${HAL_PATH}/src/*.c)
file(GLOB HAL_REMOVE_SRC ${HAL_PATH}/src/*template.c)
list(REMOVE_ITEM HAL_SRC ${HAL_REMOVE_SRC})

# Interface library
add_library(lib_hal_intf INTERFACE)
target_include_directories(lib_hal_intf
    INTERFACE   
    ./${HAL_PATH}/inc 
    ./${HAL_PATH}/inc/legacy
    ../cmsis/core/inc
    ../cmsis/device/ST/${PRJ_MCU_LINE}/inc
)
target_compile_definitions(lib_hal_intf
    INTERFACE   
    -D${PRJ_MCU_MODEL} 
    -DUSE_HAL_DRIVER
)

# Implement library
add_library(lib_hal STATIC)
target_sources(lib_hal 
    PRIVATE 
    ${HAL_SRC}
)
target_link_libraries(lib_hal 
    PUBLIC 
    lib_hal_intf
)

                            