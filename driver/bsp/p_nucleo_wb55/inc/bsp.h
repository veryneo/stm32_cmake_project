#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************
**  Include
**************************************************************/

#include "bsp_def.h"
#include "bsp_com_it.h"
#include "bsp_led.h"
#include "bsp_uart.h"


/**************************************************************
**  Interface
**************************************************************/

/**
 * @brief       Initialization of BSP basic function (such as HAL and clock configuration)
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_basic_init();

/**
 * @brief       Deinitialization of BSP basic function (such as HAL and clock configuration)
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_basic_deinit();


#ifdef __cplusplus
}
#endif

#endif
