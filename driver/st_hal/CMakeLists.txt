project(ST_HAL)

file(GLOB ST_HAL_SRC src/*.c)
file(GLOB ST_HAL_REMOVE_SRC src/*template.c)
list(REMOVE_ITEM ST_HAL_SRC ${ST_HAL_REMOVE_SRC})

add_library(lib_st_hal STATIC ${ST_HAL_SRC})
target_include_directories(lib_st_hal PUBLIC    ./inc 
                                                ./inc/legacy
                                                ../cmsis/core/inc
                                                ../cmsis/device/ST/${PRJ_MCU_LINE}/inc)
target_compile_definitions(lib_st_hal PUBLIC -D${PRJ_MCU_MODEL} -DUSE_HAL_DRIVER)

                                        
