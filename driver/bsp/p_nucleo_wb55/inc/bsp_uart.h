#ifndef __BSP_UART_H
#define __BSP_UART_H

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************
**  Include
**************************************************************/

#include "stdint.h"


/**************************************************************
**  Interface
**************************************************************/

extern void bsp_uart_init();

extern void bsp_uart_deInit();

extern int bsp_uart1_tx_send();

extern int bsp_uart1_rx_enable();

extern int bsp_uart1_tx_fifo_write(const uint8_t* buf, uint8_t size);

extern int bsp_uart1_rx_fifo_read(uint8_t* buf, uint8_t size);


#ifdef __cplusplus
}
#endif

#endif
