/**************************************************************
**  Include
**************************************************************/

#include "bsp_uart.h"

#include "stm32wbxx_hal.h"


/**************************************************************
**  Symbol
**************************************************************/

#define D_BSP_UART_RX_BUF_NUM	(5)
#define D_BSP_UART_RX_BUF_SIZE	(20)


/**************************************************************
**  Structure
**************************************************************/

typedef struct
{
	uint8_t 	buf_group[D_BSP_UART_RX_BUF_NUM][D_BSP_UART_RX_BUF_SIZE + 1];
	uint8_t		buf2read_idx;				/* Index of current writing buffer */
	uint8_t		buf2write_idx;				/* Index of current reading buffer */
	uint8_t*	buf2write_nxt_char_pos;		/* Next writing position of new received character in current writing buffer */
	uint8_t		buf2write_cur_char_cnt;		/* Current count of received character in current writing buffer */
} S_BSP_UART_RX_BUFFER;


/**************************************************************
**  Variable
**************************************************************/

static UART_HandleTypeDef 	gs_huart1;

static S_BSP_UART_RX_BUFFER gs_bsp_uart_rx_buffer;


/**************************************************************
**  Interface
**************************************************************/

/**
 * @brief       Initialization of UART
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_uart_init()
{
	gs_huart1.Instance 						= USART1;
	gs_huart1.Init.BaudRate 				= 115200;
	gs_huart1.Init.WordLength 				= UART_WORDLENGTH_8B;
	gs_huart1.Init.StopBits 				= UART_STOPBITS_1;
	gs_huart1.Init.Parity 					= UART_PARITY_NONE;
	gs_huart1.Init.Mode 					= UART_MODE_TX_RX;
	gs_huart1.Init.HwFlowCtl 				= UART_HWCONTROL_NONE;
	gs_huart1.Init.OverSampling 			= UART_OVERSAMPLING_16;
	gs_huart1.Init.OneBitSampling 			= UART_ONE_BIT_SAMPLE_DISABLE;
	gs_huart1.Init.ClockPrescaler 			= UART_PRESCALER_DIV1;
	gs_huart1.AdvancedInit.AdvFeatureInit 	= UART_ADVFEATURE_NO_INIT;

	if (HAL_UART_Init(&gs_huart1) != HAL_OK)
	{
		/* TBD */
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&gs_huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		/* TBD */
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&gs_huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		/* TBD */
	}
	if (HAL_UARTEx_DisableFifoMode(&gs_huart1) != HAL_OK)
	{
		/* TBD */
	}
}

/**
 * @brief       DeInitialization of UART
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_uart_deInit()
{

}

extern void bsp_uart_send(uint8_t* str)
{
	uint8_t* 	str_starAdd	=	str;
	uint8_t 	off 		=	0;

	do
    {
        HAL_UART_Transmit(&gs_huart1, (uint8_t *)(str_starAdd + off) , 1, 1000);
        off++;
    } while (*(str_starAdd + off) != '\0');
}

extern void bsp_uart_receive_start()
{
	/* Initiate or Reset the UART receiving buffer */
	memset(&gs_bsp_uart_rx_buffer, 0, sizeof(gs_bsp_uart_rx_buffer));
	gs_bsp_uart_rx_buffer.buf2write_nxt_char_pos = gs_bsp_uart_rx_buffer.buf_group[gs_bsp_uart_rx_buffer.buf2write_idx];

	/* Start receiving new character */
	HAL_UART_Receive_IT(&gs_huart1, gs_bsp_uart_rx_buffer.buf2write_nxt_char_pos, 1);
}

extern void bsp_uart_receive_echo(uint8_t* echo_flag)
{
	*echo_flag = 0;

	if (gs_bsp_uart_rx_buffer.buf2write_idx != gs_bsp_uart_rx_buffer.buf2read_idx)
	{
		uint8_t*	str_startAdd 	=	gs_bsp_uart_rx_buffer.buf_group[gs_bsp_uart_rx_buffer.buf2read_idx];
		uint8_t		off				=	0;

		do
	    {
	        HAL_UART_Transmit(&gs_huart1, (uint8_t *)(str_startAdd + off) , 1, 1000);
	        off++;
	    } while (*(str_startAdd + off) != '\0');

		gs_bsp_uart_rx_buffer.buf2read_idx = (gs_bsp_uart_rx_buffer.buf2read_idx + 1) % D_BSP_UART_RX_BUF_NUM;

		*echo_flag = 1;
	}
}

extern void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (huart->Instance == USART1)
	{
	    __HAL_RCC_USART1_CLK_ENABLE();
	    __HAL_RCC_GPIOB_CLK_ENABLE();

	    /**
	     * USART1 GPIO Configuration
	     * PB6     ------> USART1_TX
	     * PB7     ------> USART1_RX
	     */
	    GPIO_InitStruct.Pin 		= GPIO_PIN_6|GPIO_PIN_7;
	    GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull 		= GPIO_PULLUP;
	    GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate 	= GPIO_AF7_USART1;
	    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}

extern void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
	if (huart->Instance == USART1)
	{
		/* Reset peripherals */
		__HAL_RCC_USART1_FORCE_RESET();
		__HAL_RCC_USART1_RELEASE_RESET();

		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

	    /**
	     * USART1 GPIO Configuration
	     * PB6     ------> USART1_TX
	     * PB7     ------> USART1_RX
	     */
	    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

	    HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
}

extern void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	(void)UartHandle;

	gs_bsp_uart_rx_buffer.buf2write_cur_char_cnt++;
	gs_bsp_uart_rx_buffer.buf2write_nxt_char_pos++;

	if ('\n' == *(gs_bsp_uart_rx_buffer.buf2write_nxt_char_pos - 1) || D_BSP_UART_RX_BUF_SIZE == gs_bsp_uart_rx_buffer.buf2write_cur_char_cnt)
	{
		*gs_bsp_uart_rx_buffer.buf2write_nxt_char_pos = '\0';

		uint8_t next_buf2write_idx = (gs_bsp_uart_rx_buffer.buf2write_idx + 1) % D_BSP_UART_RX_BUF_NUM;
		if (gs_bsp_uart_rx_buffer.buf2read_idx != next_buf2write_idx)
		{
			gs_bsp_uart_rx_buffer.buf2write_idx = next_buf2write_idx;
		}

		gs_bsp_uart_rx_buffer.buf2write_nxt_char_pos	=	gs_bsp_uart_rx_buffer.buf_group[gs_bsp_uart_rx_buffer.buf2write_idx];
		gs_bsp_uart_rx_buffer.buf2write_cur_char_cnt 	= 	0;
	}

	HAL_UART_Receive_IT(&gs_huart1, gs_bsp_uart_rx_buffer.buf2write_nxt_char_pos, 1);
}

extern void USART1_IRQHandler()
{
	HAL_UART_IRQHandler(&gs_huart1);
}
