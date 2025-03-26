#ifndef BSP_LED_H
#define BSP_LED_H

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************
**  Include
**************************************************************/

#include "stdint.h"


/**************************************************************
**  Structure
**************************************************************/

/**
 * @brief List of LED type
 */
typedef enum
{
    E_BSP_LED1  =   0,
    E_BSP_LED2  =   1,
    E_BSP_LED3  =   2,
    /* Color LED aliases */
    E_BSP_LED_BLUE  = E_BSP_LED1,
    E_BSP_LED_GREEN = E_BSP_LED2,
    E_BSP_LED_RED   = E_BSP_LED3
} E_BSP_LED;


/**************************************************************
**  Interface
**************************************************************/

extern void bsp_led_init(E_BSP_LED led);

extern void bsp_led_deinit(E_BSP_LED led);

extern void bsp_led_on(E_BSP_LED led);

extern void bsp_led_off(E_BSP_LED led);

extern void bsp_led_toggle(E_BSP_LED led);

extern void bsp_led_delay(uint32_t delay_ms);


#ifdef __cplusplus
}
#endif

#endif
