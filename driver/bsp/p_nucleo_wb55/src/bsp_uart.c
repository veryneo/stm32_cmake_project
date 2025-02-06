/**************************************************************
**  Include
**************************************************************/

#include "bsp_uart.h"

#include "stm32wbxx_hal.h"

#include "lwrb.h"


/**************************************************************
**  Symbol
**************************************************************/

#define D_BSP_UART_TX_FIFO_BUFFER_SIZE	(256+1)
#define D_BSP_UART_RX_FIFO_BUFFER_SIZE	(256+1)
#define D_BSP_UART_TX_DMA_BUFFER_SIZE	(256)	/* DMA buffer size should be equal to FIFO buffer to maximize transmission efficiency*/
#define D_BSP_UART_RX_DMA_BUFFER_SIZE	(256)


/**************************************************************
**  Structure
**************************************************************/

typedef enum
{
	E_BSP_UART_TX_DMA_STATUS_READY	= 0,
	E_BSP_UART_TX_DMA_STATUS_BUSY
} E_BSP_UART_TX_DMA_STATUS_T;

/**
 * @brief           BSP UART handle
 * @author			chenwei.gu@murata.com
 */
typedef struct
{
	UART_HandleTypeDef 			huart;			/* HAL UART handle */
	DMA_HandleTypeDef 			hdma_usart_tx;	/* HAL DMA handle (for UART TX) */
	DMA_HandleTypeDef 			hdma_usart_rx;	/* HAL DMA handle (for UART RX) */
	lwrb_t*						p_tx_fifo_buf;	/* UART TX FIFO buffer */
	lwrb_t*						p_rx_fifo_buf;	/* UART RX FIFO buffer */
	uint8_t*					p_tx_dma_buf;	/* UART TX DMA buffer */
	uint8_t*					p_rx_dma_buf;	/* UART RX DMA buffer */
	E_BSP_UART_TX_DMA_STATUS_T	tx_dma_status;	/* UART TX DMA status */
} S_BSP_UART_HANDLE_T;


/**************************************************************
**  Variable
**************************************************************/

static S_BSP_UART_HANDLE_T gs_bsp_uart1_handle;
static lwrb_t	gs_bsp_uart1_tx_fifo_buf;
static lwrb_t	gs_bsp_uart1_rx_fifo_buf;
static uint8_t 	gs_bsp_uart1_tx_fifo_buf_int[D_BSP_UART_TX_FIFO_BUFFER_SIZE];	/* "int" means internal */
static uint8_t 	gs_bsp_uart1_rx_fifo_buf_int[D_BSP_UART_RX_FIFO_BUFFER_SIZE];	/* "int" means internal */
static uint8_t 	gs_bsp_uart1_tx_dma_buf[D_BSP_UART_TX_DMA_BUFFER_SIZE];
static uint8_t 	gs_bsp_uart1_rx_dma_buf[D_BSP_UART_RX_DMA_BUFFER_SIZE];


/**************************************************************
**  Interface
**************************************************************/

/**
 * @brief       UART1 initialization
 * @retval      D_BSP_RET_OK
 * @retval      D_BSP_RET_HAL_ERR
 * @retval      D_BSP_RET_UTILITY_ERR
 * @author      chenwei.gu@murata.com
 */
extern int32_t bsp_uart1_init()
{
	int ret = D_BSP_RET_OK;

	do
	{
		/* UART1 HAL initialization */
		UART_HandleTypeDef* huart1 			= 	&(gs_bsp_uart1_handle.huart);

		huart1->Instance					=	USART1;
		huart1->Init.BaudRate 				= 	115200;
		huart1->Init.WordLength 			= 	UART_WORDLENGTH_8B;
		huart1->Init.StopBits 				= 	UART_STOPBITS_1;
		huart1->Init.Parity 				= 	UART_PARITY_NONE;
		huart1->Init.Mode 					= 	UART_MODE_TX_RX;
		huart1->Init.HwFlowCtl 				= 	UART_HWCONTROL_NONE;
		huart1->Init.OverSampling 			= 	UART_OVERSAMPLING_16;
		huart1->Init.OneBitSampling 		= 	UART_ONE_BIT_SAMPLE_DISABLE;
		huart1->Init.ClockPrescaler 		= 	UART_PRESCALER_DIV1;
		huart1->AdvancedInit.AdvFeatureInit	= 	UART_ADVFEATURE_NO_INIT;
	
		if (HAL_OK != HAL_UART_Init(huart1) )
		{
			ret = D_BSP_RET_HAL_ERR;
			break;
		}
		if (HAL_OK != HAL_UARTEx_SetTxFifoThreshold(huart1, UART_TXFIFO_THRESHOLD_1_8) )
		{
			ret = D_BSP_RET_HAL_ERR;
			break;
		}
		if (HAL_OK != HAL_UARTEx_SetRxFifoThreshold(huart1, UART_RXFIFO_THRESHOLD_1_8) )
		{
			ret = D_BSP_RET_HAL_ERR;
			break;
		}
		if (HAL_OK != HAL_UARTEx_DisableFifoMode(huart1) )
		{
			ret = D_BSP_RET_HAL_ERR;
			break;
		}

		/* Configure UART1 buffer */
		if ( (!lwrb_init(&gs_bsp_uart1_tx_fifo_buf, gs_bsp_uart1_tx_fifo_buf_int, D_BSP_UART_TX_FIFO_BUFFER_SIZE) ) ||
			 (!lwrb_init(&gs_bsp_uart1_rx_fifo_buf, gs_bsp_uart1_rx_fifo_buf_int, D_BSP_UART_RX_FIFO_BUFFER_SIZE) ) )
		{
			ret = D_BSP_RET_RB_INIT_ERR;
			break;
		}
		gs_bsp_uart1_handle.p_tx_fifo_buf 	=	&gs_bsp_uart1_tx_fifo_buf;
		gs_bsp_uart1_handle.p_rx_fifo_buf 	=	&gs_bsp_uart1_rx_fifo_buf;
		gs_bsp_uart1_handle.p_tx_dma_buf	=	gs_bsp_uart1_tx_dma_buf;
		gs_bsp_uart1_handle.p_rx_dma_buf	=	gs_bsp_uart1_rx_dma_buf;

		/* Initialize UART TX DMA status */
		gs_bsp_uart1_handle.tx_dma_status	=	E_BSP_UART_TX_DMA_STATUS_READY;

		ret = D_BSP_RET_OK;
	} while (0);

	return ret;
}

/**
 * @brief       UART1 deInitialization
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_uart1_deInit()
{

}

extern int32_t bsp_uart1_tx_send_ch(const char ch)
{
	int32_t 			ret		=	D_BSP_RET_OK;
	HAL_StatusTypeDef 	hal_ret =	HAL_OK;

	do
	{
		hal_ret = HAL_UART_Transmit(&(gs_bsp_uart1_handle.huart), (uint8_t*)&ch, 1, 1000);
		if (HAL_OK != hal_ret)
		{
			if (HAL_ERROR == hal_ret)
			{
				ret = D_BSP_RET_HAL_ERR;
			}
			else if (HAL_BUSY == hal_ret)
			{
				ret = D_BSP_RET_HAL_BUSY;
			}
			else if (HAL_TIMEOUT == hal_ret)
			{
				ret = D_BSP_RET_HAL_TIMEOUT;
			}

			break;
		}

		ret = D_BSP_RET_OK;
	} while(0);

	return ret;	
}

extern int32_t bsp_uart1_tx_send_str(const char* p_str)
{
	int32_t 			ret		=	D_BSP_RET_OK;
	HAL_StatusTypeDef 	hal_ret =	HAL_OK;

	do
	{
		hal_ret = HAL_UART_Transmit(&(gs_bsp_uart1_handle.huart), (const uint8_t*)p_str, strlen(p_str), 1000);
		if (HAL_OK != hal_ret)
		{
			if (HAL_ERROR == hal_ret)
			{
				ret = D_BSP_RET_HAL_ERR;
			}
			else if (HAL_BUSY == hal_ret)
			{
				ret = D_BSP_RET_HAL_BUSY;
			}
			else if (HAL_TIMEOUT == hal_ret)
			{
				ret = D_BSP_RET_HAL_TIMEOUT;
			}

			break;
		}

		ret = D_BSP_RET_OK;
	} while (0);
	
	return ret;
}

/**
 * @brief       UART1 data transmission through DMA
 * @retval		D_BSP_RET_OK
 * @retval		D_BSP_RET_UART_TX_DMA_BUSY
 * @retval		D_BSP_RET_HAL_ERROR
 * @retval		D_BSP_RET_HAL_BUSY
 * @retval		D_BSP_RET_HAL_TIMEOUT
 * @author      chenwei.gu@murata.com
 */
extern int32_t bsp_uart1_tx_send_dma()
{
	int 				ret 	=	D_BSP_RET_OK;
	HAL_StatusTypeDef 	hal_ret =	HAL_OK;

	lwrb_sz_t read_len = 0;

	do
	{
		if (E_BSP_UART_TX_DMA_STATUS_READY != gs_bsp_uart1_handle.tx_dma_status)
		{
			/* UART TX DMA status busy */
			ret = D_BSP_RET_UART_TX_DMA_BUSY;
			break;
		}

		/* Update UART1 TX DMA status */
		gs_bsp_uart1_handle.tx_dma_status = E_BSP_UART_TX_DMA_STATUS_BUSY;

		/* Get data from UART1 TX FIFO buffer to UART TX DMA buffer */
		read_len = lwrb_read(gs_bsp_uart1_handle.p_tx_fifo_buf, gs_bsp_uart1_handle.p_tx_dma_buf, D_BSP_UART_TX_DMA_BUFFER_SIZE);
		if (read_len)
		{
			/* Transmit data through DMA */
			hal_ret = HAL_UART_Transmit_DMA(&(gs_bsp_uart1_handle.huart), gs_bsp_uart1_handle.p_tx_dma_buf, read_len);
			if (HAL_OK!= hal_ret)
			{
				
				/* Reset UART1 TX DMA status */
				gs_bsp_uart1_handle.tx_dma_status = E_BSP_UART_TX_DMA_STATUS_READY;
				
				if (HAL_ERROR == hal_ret)
				{
					ret = D_BSP_RET_HAL_ERR;
					break;
				}
				else if (HAL_BUSY == hal_ret)
				{
					ret = D_BSP_RET_HAL_BUSY;
					break;
				}
				else if (HAL_TIMEOUT == hal_ret)
				{
					ret = D_BSP_RET_HAL_TIMEOUT;
					break;
				}

			}
		}
		else
		{
			/* Reset UART1 TX DMA status */
			gs_bsp_uart1_handle.tx_dma_status = E_BSP_UART_TX_DMA_STATUS_READY;
		}

		ret = D_BSP_RET_OK;
	} while (0);

	return ret;
}

/**
 * @brief       UART reception enable
 * @retval		D_BSP_RET_OK      
 * @retval		D_BSP_RET_HAL_ERROR
 * @retval		D_BSP_RET_HAL_BUSY
 * @retval		D_BSP_RET_HAL_TIMEOUT
 * @author      chenwei.gu@murata.com
 */
extern int32_t bsp_uart1_rx_enable()
{
	int 				ret 	=	D_BSP_RET_OK;
	HAL_StatusTypeDef 	hal_ret	=	HAL_OK;
	
	do
	{
		/* Enable UAER reception */
		hal_ret = HAL_UARTEx_ReceiveToIdle_DMA(&(gs_bsp_uart1_handle.huart), gs_bsp_uart1_handle.p_rx_dma_buf, D_BSP_UART_RX_DMA_BUFFER_SIZE);
		if (HAL_OK != hal_ret) 
		{
			if (HAL_ERROR == hal_ret) 
			{
				ret = D_BSP_RET_HAL_ERR;
				break;
			} 
			else if (HAL_BUSY == hal_ret) 
			{
				ret = D_BSP_RET_HAL_BUSY;
				break;
			} 
			else if (HAL_TIMEOUT == hal_ret) 
			{
				ret = D_BSP_RET_HAL_TIMEOUT;
				break;
			}
		}

		ret = D_BSP_RET_OK;
	} while (0);

	return ret;
}

/**
 * @brief       UART1 TX FIFO buffer write operation
 * @param		buf			Data to be written
 * @param		size		Expected size of data to be written
 * @retval		Actual size of data to be read
 * @retval		D_BSP_RET_INPUT_ERROR
 * @author      chenwei.gu@murata.com
 */
extern int32_t bsp_uart1_tx_fifo_write(const uint8_t* buf, uint8_t size)
{
	if (!buf)
	{
		return D_BSP_RET_INPUT_ERR;
	}

	/* Write data to buffer */
	return lwrb_write(gs_bsp_uart1_handle.p_tx_fifo_buf, buf, (lwrb_sz_t)size);
}

/**
 * @brief       UART1 RX FIFO buffer read operation
 * @param		buf			Data to be read
 * @param		size 		Expected size of data to be read
 * @retval		Actual size of data to be read
 * @retval		D_BSP_RET_INPUT_ERROR
 * @author      chenwei.gu@murata.com
 */
extern int32_t bsp_uart1_rx_fifo_read(uint8_t* buf, uint8_t size)
{
	if (!buf)
	{
		return D_BSP_RET_INPUT_ERR;
	}

	/* Read data from buffer */
	return lwrb_read(gs_bsp_uart1_handle.p_rx_fifo_buf, buf, (lwrb_sz_t)size);	
}

/**
 * @brief 		UART MSP initialization (This function configures the hardware resources)
 * @param 		huart		UART handle
 * @retval 		None
 * @author      chenwei.gu@murata.com
 */
extern void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (huart->Instance == USART1)
	{
		 /* USART1 clock enable */
	    __HAL_RCC_USART1_CLK_ENABLE();
	    __HAL_RCC_GPIOB_CLK_ENABLE();

	    /**
	     * USART1 GPIO configuration
	     * PB6     ------> USART1_TX
	     * PB7     ------> USART1_RX
	     */
	    GPIO_InitStruct.Pin 		= GPIO_PIN_6|GPIO_PIN_7;
	    GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull 		= GPIO_PULLUP;
	    GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate 	= GPIO_AF7_USART1;
	    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USART1 interrupt initialization */
	    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(USART1_IRQn);

		/* DMA controller clock enable */
  		__HAL_RCC_DMAMUX1_CLK_ENABLE();
  		__HAL_RCC_DMA1_CLK_ENABLE();

		/* USART1 DMA TX initialization */
		DMA_HandleTypeDef* hdma_uart1_tx 		= 	&(gs_bsp_uart1_handle.hdma_usart_tx);

		hdma_uart1_tx->Instance 				=	DMA1_Channel1;
		hdma_uart1_tx->Init.Request 			=	DMA_REQUEST_USART1_TX;
		hdma_uart1_tx->Init.Direction 			=	DMA_MEMORY_TO_PERIPH;
		hdma_uart1_tx->Init.PeriphInc 			=	DMA_PINC_DISABLE;
		hdma_uart1_tx->Init.MemInc 				=	DMA_MINC_ENABLE;
		hdma_uart1_tx->Init.PeriphDataAlignment =	DMA_PDATAALIGN_BYTE;
		hdma_uart1_tx->Init.MemDataAlignment 	=	DMA_MDATAALIGN_BYTE;
		hdma_uart1_tx->Init.Mode 				=	DMA_NORMAL;
		hdma_uart1_tx->Init.Priority 			=	DMA_PRIORITY_LOW;
		
		if (HAL_DMA_Init(hdma_uart1_tx) != HAL_OK)
		{
			/* TBD */
		}

		__HAL_LINKDMA(huart, hdmatx, (*hdma_uart1_tx) );

		/* USART1 DMA RX initialization */
		DMA_HandleTypeDef* hdma_uart1_rx 		= 	&(gs_bsp_uart1_handle.hdma_usart_rx);

		hdma_uart1_rx->Instance 				=	DMA1_Channel2;
		hdma_uart1_rx->Init.Request 			=	DMA_REQUEST_USART1_RX;
		hdma_uart1_rx->Init.Direction 			=	DMA_PERIPH_TO_MEMORY;
		hdma_uart1_rx->Init.PeriphInc 			=	DMA_PINC_DISABLE;
		hdma_uart1_rx->Init.MemInc 				= 	DMA_MINC_ENABLE;
		hdma_uart1_rx->Init.PeriphDataAlignment	= 	DMA_PDATAALIGN_BYTE;
		hdma_uart1_rx->Init.MemDataAlignment 	= 	DMA_MDATAALIGN_BYTE;
		hdma_uart1_rx->Init.Mode 				= 	DMA_CIRCULAR;
		hdma_uart1_rx->Init.Priority 			= 	DMA_PRIORITY_HIGH;
		
		if (HAL_DMA_Init(hdma_uart1_rx) != HAL_OK)
		{
			/* TBD */
		}

		__HAL_LINKDMA(huart, hdmarx, (*hdma_uart1_rx) );

		/* DMA1_Channel1_IRQn interrupt initialization  */
  		HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  		HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

  		/* DMA1_Channel2_IRQn interrupt initialization  */
 	 	HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  		HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	}
}

/**
 * @brief 		UART MSP deInitialization (This function configures the hardware resources)
 * @param 		huart		UART handle
 * @retval 		None
 * @author      chenwei.gu@murata.com
 */
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

		/* USART1 DMA DeInit */
    	HAL_DMA_DeInit(huart->hdmarx);

		/* USART1 interrupt DeInit */
	    HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
}

/**
 * @brief 		HAL UART transmission transfer completed callback.
 * @param 		huart		UART handle
 * @retval 		None
 * @author 		chenwei.gu@murata.com
 */
extern void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Set TX DMA status to ready */
	gs_bsp_uart1_handle.tx_dma_status = E_BSP_UART_TX_DMA_STATUS_READY;
}

/**
 * @brief  		HAL UART reception transfer completed callback
 * @param  		huart		UART handle
 * @author 		chenwei.gu@murata.com
 */
extern void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{	
	UNUSED(huart);
}

/**
 * @brief		HAL UART reception event callback
 * @param  		huart 		UART handle
 * @param  		Size  		Number of data available in application reception buffer
 * 			 				(indicates a position in reception buffer until which, data are available)
 * @retval 		None
 * @author 		chenwei.gu@murata.com
 */
extern void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
{
	if (USART1 == huart->Instance)
	{
		static uint16_t	dma_buf_last_size	=	0;
		uint16_t 		dma_buf_curr_size	=	0;
		__IO uint16_t	proc_char_size		=	0;

		/* Get buffer current using size. */
		dma_buf_curr_size = Size;

		/* Check if number of received data in reception buffer has changed */
		if (dma_buf_last_size != dma_buf_curr_size)
		{
			if (dma_buf_last_size < dma_buf_curr_size)
			{
				/* Continue getting data from the DMA buffer */
				proc_char_size = dma_buf_curr_size - dma_buf_last_size;
				lwrb_write(gs_bsp_uart1_handle.p_rx_fifo_buf, &(gs_bsp_uart1_handle.p_rx_dma_buf[dma_buf_last_size]), proc_char_size);
			}
			else
			{
				/* Restart getting data from the DMA buffer */
				proc_char_size = dma_buf_curr_size;
				lwrb_write(gs_bsp_uart1_handle.p_rx_fifo_buf, &(gs_bsp_uart1_handle.p_rx_dma_buf[0]), proc_char_size);
			}

			dma_buf_last_size = dma_buf_curr_size;
		}
	}
}

/**
 * @brief		DMA1 Channel1 interrupt handler function
 * @retval 		None
 * @author 		chenwei.gu@murata.com
 */
extern void DMA1_Channel1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&(gs_bsp_uart1_handle.hdma_usart_tx) );
}

/**
 * @brief		DMA1 Channel2 interrupt handler function
 * @retval 		None
 * @author 		chenwei.gu@murata.com
 */
extern void DMA1_Channel2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&(gs_bsp_uart1_handle.hdma_usart_rx) );
}

/**
 * @brief		USART1 interrupt handler function
 * @retval 		None
 * @author 		chenwei.gu@murata.com
 */
extern void USART1_IRQHandler()
{
	HAL_UART_IRQHandler(&(gs_bsp_uart1_handle.huart) );
}
