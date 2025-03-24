#ifndef __BSP_SERIALPORT_H
#define __BSP_SERIALPORT_H

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

extern void bsp_serialport_init();
extern void bsp_serialport_deInit();
extern int32_t bsp_serialport_send();
extern int32_t bsp_serialport_receive_enable();
extern int32_t bsp_serialport_send_cache_data_set(const uint8_t* buf, uint16_t size);
extern int32_t bsp_serialport_send_cache_data_get(uint8_t* buf, uint16_t size);
extern int32_t bsp_serialport_send_cache_used_size_get();
extern int32_t bsp_serialport_receive_cache_data_get(uint8_t* buf, uint16_t size);
extern int32_t bsp_serialport_receive_callback_set(void (*p_uart_rx_callcack)(void) );


#ifdef __cplusplus
}
#endif

#endif
