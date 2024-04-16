cmake_minimum_required(VERSION 3.20)

##
## Constant Variable
##
set(MCPU_CORTEX_M0                     	"-mcpu=cortex-m0"		)
set(MCPU_CORTEX_M0PLUS				    "-mcpu=cortex-m0plus"	)
set(MCPU_CORTEX_M3				        "-mcpu=cortex-m3"		)
set(MCPU_CORTEX_M4				        "-mcpu=cortex-m4"		)
set(MCPU_CORTEX_M7				        "-mcpu=cortex-m7"		)
set(MCPU_CORTEX_M33				    	"-mcpu=cortex-m33"	    )
set(MCPU_CORTEX_M55				    	"-mcpu=cortex-m55"	    )
set(MCPU_CORTEX_M85				    	"-mcpu=cortex-m85"	    )
set(MFPU_FPV4_SP_D16                   	"-mfpu=fpv4-sp-d16"	    )
set(MFPU_FPV5_D16                      	"-mfpu=fpv5-d16"		)
set(RUNTIME_LIBRARY_REDUCED_C          	"--specs=nano.specs"	)
set(RUNTIME_LIBRARY_STD_C              	""					    )
set(RUNTIME_LIBRARY_SYSCALLS_MINIMAL   	"--specs=nosys.specs"	)
set(RUNTIME_LIBRARY_SYSCALLS_NONE      	""					    )
set(MFLOAT_ABI_SOFTWARE                	"-mfloat-abi=soft"	    )
set(MFLOAT_ABI_HARDWARE             	"-mfloat-abi=hard"	    )
set(MFLOAT_ABI_MIX                     	"-mfloat-abi=softfp"	)

set(MCU_MODEL_STM32WB55xx              	"STM32WB55xx"           )
set(MCU_MODEL_STM32U5A5xx              	"STM32U5A5xx"           ) 

##
## Project Build Configuration
##
project(STM32PRJ LANGUAGES C ASM)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(PRJ_MCU_LINE                       	"STM32WBxx"                                                                                 )
set(PRJ_MCU_MODEL                      	"STM32WB55xx"                                                                               )
set(PRJ_LINKER_SCRIPT            	    "${CMAKE_SOURCE_DIR}/driver/cmsis/device/ST/${PRJ_MCU_LINE}/src/stm32wb55xx_flash_cm4.ld"   )
set(PRJ_SYSINIT_SRC					    "${CMAKE_SOURCE_DIR}/driver/cmsis/device/ST/${PRJ_MCU_LINE}/src/system_stm32wbxx.c"			)
set(PRJ_STARTUP_SRC						"${CMAKE_SOURCE_DIR}/driver/cmsis/device/ST/${PRJ_MCU_LINE}/src/startup_stm32wb55xx_cm4.s"	)
set(PRJ_MCPU                     	    "-mcpu=Cortex-M4"			                                                                )
set(PRJ_MFPU						    "-mfpu=fpv4-sp-d16"			                                                                )
set(PRJ_MFLOAT_ABI             	    	"-mfloat-abi=hard"			                                                                )
set(PRJ_RUNTIME_LIBRARY         	    "--specs=nano.specs"		                                                                )
set(PRJ_RUNTIME_LIBRARY_SYSCALLS 	    "--specs=nosys.specs"		                                                                )
set(PRJ_EXE_NAME						"${CMAKE_PROJECT_NAME}"											                            )
set(PRJ_MAP_NAME						"${CMAKE_PROJECT_NAME}"											                            )
	
set(CMAKE_EXECUTABLE_SUFFIX            	".elf")
set(CMAKE_STATIC_LIBRARY_SUFFIX        	".a")
set(CMAKE_C_FLAGS                      	"${PRJ_MCPU} -std=gnu11 ${PRJ_MFPU} ${PRJ_MFLOAT_ABI} ${PRJ_RUNTIME_LIBRARY} -mthumb -Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS             	"-T${PRJ_LINKER_SCRIPT} ${PRJ_RUNTIME_LIBRARY_SYSCALLS} -Wl,-Map=${PRJ_MAP_NAME}.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")
set(CMAKE_ASM_FLAGS                    	"${CMAKE_C_FLAGS} -x assembler-with-cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 		${CMAKE_SOURCE_DIR}/output)

add_subdirectory(app)
add_subdirectory(driver)








