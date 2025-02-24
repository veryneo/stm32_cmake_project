#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************
**  Include
**************************************************************/

#include "bsp_led.h"
#include "bsp_uart.h"
#include "bsp_switch.h"
#include "bsp_nfc07a1.h"
#include "bsp_lowpower.h"

extern void bsp_delay(uint32_t delay_ms);


#ifdef __cplusplus
}
#endif

#endif
