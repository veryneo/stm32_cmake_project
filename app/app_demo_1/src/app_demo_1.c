#include "app_demo_1.h"

#include "bsp.h"



extern int app_demo_1()
{
    bsp_led_init(E_BSP_LED_BLUE);
    bsp_led_init(E_BSP_LED_GREEN);
    bsp_led_init(E_BSP_LED_RED);

    bsp_led_on(E_BSP_LED_BLUE);
    bsp_led_on(E_BSP_LED_GREEN);

    bsp_uart_init();
    bsp_uart1_rx_enable();

    uint8_t move[80]   		=   {0};
    uint8_t move_size  		=   80;
    int     read_size   	=   0;
    int		write_size_once	=	0;
    int		write_size_all	=	0;

    while (1)
    {
    	read_size = bsp_uart1_rx_fifo_read(move, move_size);
    	if (read_size)
        {
            write_size_once	=	bsp_uart1_tx_fifo_write(move, read_size);
            write_size_all 	+= 	write_size_once;
        }

        bsp_uart1_tx_send();
    }

    return 0;
}
