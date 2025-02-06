#ifndef __BSP_UART_H
#define __BSP_UART_H

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************
**  Include
**************************************************************/

#include "bsp_errno.h"

#include "stdint.h"


/**************************************************************
**  Interface
**************************************************************/

extern int32_t bsp_uart1_init();

extern void bsp_uart1_deInit();

extern int32_t bsp_uart1_tx_send_ch(const char ch);

extern int32_t bsp_uart1_tx_send_str(const char* p_str);

extern int32_t bsp_uart1_tx_send_dma();

extern int32_t bsp_uart1_rx_enable();

extern int32_t bsp_uart1_tx_fifo_write(const uint8_t* buf, uint8_t size);

extern int32_t bsp_uart1_rx_fifo_read(uint8_t* buf, uint8_t size);


#ifdef __cplusplus
}
#endif

#endif
