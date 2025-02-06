/*
 * bsp.c
 *
 *  Created on: Jan 17, 2025
 *      Author: cgu
 */

#include "bsp.h"
#include "stm32wbxx_hal.h"

extern void bsp_delay(uint32_t delay_ms)
{
	HAL_Delay(delay_ms);
}

