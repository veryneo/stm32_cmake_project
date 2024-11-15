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

extern void bsp_basic_init();

extern void bsp_basic_deinit();

extern void bsp_delay(uint32_t msec);

#ifdef __cplusplus
}
#endif

#endif
