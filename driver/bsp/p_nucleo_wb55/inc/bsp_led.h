#ifndef __BSP_LED_H
#define __BSP_LED_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************
**  Include
**************************************************************/

#include "bsp_def.h"


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

/**
 * @brief       Initialization of LED
 * @param[in]   led: Specifies the Led to be initialized
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_led_init(E_BSP_LED led);

/**
 * @brief       Deinitialization of LED
 * @param[in]   led: Specifies the Led to be deinitialized
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_led_deinit(E_BSP_LED led);

/**
 * @brief       Turn on selected LED
 * @param[in]   led: Specifies the Led to be set on
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_led_on(E_BSP_LED led);

/**
 * @brief       Turn off selected LED
 * @param[in]   led: Specifies the Led to be set off
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_led_off(E_BSP_LED led);

/**
 * @brief       Toggle selected LED
 * @param[in]   led: Specifies the Led to be toggled
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_led_toggle(E_BSP_LED led);


#ifdef __cplusplus
}
#endif

#endif
