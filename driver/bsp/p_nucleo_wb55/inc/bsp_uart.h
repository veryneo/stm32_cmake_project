#ifndef __BSP_UART_H
#define __BSP_UART_H

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************
**  Include
**************************************************************/

#include "bsp_def.h"


/**************************************************************
**  Interface
**************************************************************/

/**
 * @brief       Initialization of UART
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_uart_init();

/**
 * @brief       Deinitialization of UART
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_uart_deInit();

extern void bsp_uart_send(uint8_t* str);

extern void bsp_uart_receive_start();

extern void bsp_uart_receive_echo(uint8_t* echo_flag);


#ifdef __cplusplus
}
#endif

#endif
