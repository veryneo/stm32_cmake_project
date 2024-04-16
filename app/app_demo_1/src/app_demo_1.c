#include "app_demo_1.h"

#include "bsp.h"

extern int app_demo_1()
{
    bsp_basic_init();

    bsp_led_init(E_BSP_LED_BLUE);
    bsp_led_init(E_BSP_LED_GREEN);
    bsp_led_init(E_BSP_LED_RED);
    bsp_led_on(E_BSP_LED_BLUE);

    bsp_uart_init();
    const char 	str1[] = "hello world\r\n";
    char 		str2[] = "hello world again\r\n";
    bsp_uart_send( (uint8_t*)str1);
    bsp_uart_send( (uint8_t*)str2);
    bsp_uart_send( (uint8_t*)"hello world again again\r\n");

    bsp_uart_receive_start();

    while (1)
    {
    	uint8_t echo_flag = 0;
    	bsp_uart_receive_echo(&echo_flag);
    	if (1 == echo_flag)
    	{
    		bsp_led_toggle(E_BSP_LED_BLUE);
    	}
    }
}
