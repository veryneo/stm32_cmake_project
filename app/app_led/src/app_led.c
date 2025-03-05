#include "app_led.h"

#include "log.h"

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
        logInfo("LED Toggle ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
        bsp_led_toggle(E_BSP_LED_BLUE);
        tx_thread_sleep(1000);
    }
}   
