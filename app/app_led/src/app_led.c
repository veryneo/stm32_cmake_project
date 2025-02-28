#include "app_led.h"

#include "tx_api.h"

#include "bsp.h"


extern void app_led_init()
{
    bsp_led_init(E_BSP_LED_BLUE);
}

extern void app_led_run()
{
    while(1)
    {
        bsp_led_toggle(E_BSP_LED_BLUE);
        tx_thread_sleep(1000);
    }
}   
