#include "app_demo_1.h"

#include "bsp.h"



extern int app_demo_1()
{
    bsp_nfc07a1_nfctag_init();
    bsp_nfc07a1_gpo_init();
    bsp_nfc07a1_led_init(E_BSP_NFC07A1_LED_GREEN);

    uint32_t nfctag_password_msb = 0;
    uint32_t nfctag_password_lsb = 0;
    bsp_nfc07a1_nfctag_i2cPassword_present(nfctag_password_msb, nfctag_password_lsb);

    bsp_nfc07a1_nfctag_it_configure(D_BSP_NFC07A1_NFCTAG_GPO1_ENABLE_MASK | D_BSP_NFC07A1_NFCTAG_GPO1_FILEDCHQNGE_MASK);

    uint8_t nfctag_activeFlag = 0;
    while(1)
    {
        bsp_nfc07a1_gpo_activeFlag_check(&nfctag_activeFlag);

        if (nfctag_activeFlag)
        {
            bsp_nfc07a1_led_toggle(E_BSP_NFC07A1_LED_GREEN);
            bsp_nfc07a1_gpo_activeFlag_reset();
        }
    }

    // bsp_uart1_init();
    // bsp_uart1_rx_enable();

    // uint8_t move[80]   		=   {0};
    // uint8_t move_size  		=   80;
    // int     read_size   	=   0;
    // int		write_size_once	=	0;
    // int		write_size_all	=	0;

    // while (1)
    // {
    // 	read_size = bsp_uart1_rx_fifo_read(move, move_size);
    // 	if (read_size)
    //     {
    //         write_size_once	=	bsp_uart1_tx_fifo_write(move, read_size);
    //         write_size_all 	+= 	write_size_once;
    //     }

    //     bsp_uart1_tx_send();
    // }

    return 0;
}
