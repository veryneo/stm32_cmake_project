/**************************************************************
**  Include
**************************************************************/

#include "bsp_serialport.h"
#include "bsp_errno.h"

#include "stm32wbxx_hal.h"

#include "lwrb.h"

#include "stdint.h"


/**************************************************************
**  Symbol
**************************************************************/

#define D_BSP_SERIALPORT_UART_INSTANSE					USART1

#define D_BSP_SERIALPORT_UART_CLK_ENABLE()				__HAL_RCC_USART1_CLK_ENABLE()
#define D_BSP_SERIALPORT_UART_CLK_DISABLE()				__HAL_RCC_USART1_CLK_DISABLE()

#define D_BSP_SERIALPORT_UART_TX_GPIO_PORT				GPIOB
#define D_BSP_SERIALPORT_UART_TX_GPIO_PIN				GPIO_PIN_6
#define D_BSP_SERIALPORT_UART_TX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define D_BSP_SERIALPORT_UART_TX_GPIO_CLK_DISABLE()		__HAL_RCC_GPIOB_CLK_DISABLE()
#define D_BSP_SERIALPORT_UART_RX_GPIO_PORT				GPIOB
#define D_BSP_SERIALPORT_UART_RX_GPIO_PIN				GPIO_PIN_7
#define D_BSP_SERIALPORT_UART_RX_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define D_BSP_SERIALPORT_UART_RX_GPIO_CLK_DISABLE()		__HAL_RCC_GPIOB_CLK_DISABLE()

#define D_BSP_SERIALPORT_UART_AF						GPIO_AF7_USART1

#define D_BSP_SERIALPORT_UART_IRQ_NUMBER				USART1_IRQn
#define D_BSP_SERIALPORT_UART_IRQ_PRIORITY				(0)
#define D_BSP_SERIALPORT_UART_IRQ_SUBPRIORITY			(0)

#define D_BSP_SERIALPORT_UART_FORCE_RESET()				__HAL_RCC_USART1_FORCE_RESET()						
#define D_BSP_SERIALPORT_UART_RELEASE_RESET()			__HAL_RCC_USART1_RELEASE_RESET()

#define D_BSP_SERIALPORT_UART_DMAMUX_CLK_ENABLE()		__HAL_RCC_DMAMUX1_CLK_ENABLE()
#define D_BSP_SERIALPORT_UART_TX_DMA_CLK_ENABLE()		__HAL_RCC_DMA1_CLK_ENABLE()
#define D_BSP_SERIALPORT_UART_RX_DMA_CLK_ENABLE()		__HAL_RCC_DMA1_CLK_ENABLE()

#define D_BSP_SERIALPORT_UART_TX_DMA_INSTANCE			DMA1_Channel1
#define D_BSP_SERIALPORT_UART_TX_DMA_REQUEST			DMA_REQUEST_USART1_TX
#define D_BSP_SERIALPORT_UART_RX_DMA_INSTANCE			DMA1_Channel2
#define D_BSP_SERIALPORT_UART_RX_DMA_REQUEST			DMA_REQUEST_USART1_RX

#define D_BSP_SERIALPORT_UART_TX_DMA_IRQ_NUMBER			DMA1_Channel1_IRQn
#define D_BSP_SERIALPORT_UART_TX_DMA_IRQ_PRIORITY		(0)
#define D_BSP_SERIALPORT_UART_TX_DMA_IRQ_SUBPRIORITY	(0)

#define D_BSP_SERIALPORT_UART_RX_DMA_IRQ_NUMBER			DMA1_Channel2_IRQn
#define D_BSP_SERIALPORT_UART_RX_DMA_IRQ_PRIORITY		(0)
#define D_BSP_SERIALPORT_UART_RX_DMA_IRQ_SUBPRIORITY	(0)

#define D_BSP_SERIALPORT_UART_TX_RING_BUFFER_SIZE		(512+1)
#define D_BSP_SERIALPORT_UART_RX_RING_BUFFER_SIZE		(256+1)
#define D_BSP_SERIALPORT_UART_TX_DMA_BUFFER_SIZE		(512)	/* DMA buffer size should be equal to FIFO buffer to maximize transmission efficiency*/
#define D_BSP_SERIALPORT_UART_RX_DMA_BUFFER_SIZE		(256)


/**************************************************************
**  Structure
**************************************************************/

typedef enum
{
	E_BSP_SERIALPORT_UART_TX_DMA_STATUS_READY	=	0,
	E_BSP_SERIALPORT_UART_TX_DMA_STATUS_BUSY
} E_BSP_SERIALPORT_UART_TX_DMA_STATUS_T;

/**
 * @brief           BSP Serialport handle
 * @author			chenwei.gu@murata.com
 */
typedef struct
{
	UART_HandleTypeDef 						uart_handle;					/* UART handle */
	DMA_HandleTypeDef 						dma_uart_tx_handle;				/* DMA handle (for UART TX) */
	DMA_HandleTypeDef 						dma_uart_rx_handle;				/* DMA handle (for UART RX) */
	lwrb_t*									p_uart_tx_ring_buf;				/* UART TX ring buffer */
	lwrb_t*									p_uart_rx_ring_buf;				/* UART RX ring buffer */
	uint8_t*								p_uart_tx_dma_buf;				/* UART TX DMA buffer */
	uint8_t*								p_uart_rx_dma_buf;				/* UART RX DMA buffer */
	E_BSP_SERIALPORT_UART_TX_DMA_STATUS_T	uart_tx_dma_status;				/* UART TX DMA status */
	void									(*p_uart_rx_callcack)(void);	/* UART RX callback*/
} S_BSP_SERIALPORT_HANDLE_T;


/**************************************************************
**  Variable
**************************************************************/

static S_BSP_SERIALPORT_HANDLE_T gs_bsp_serialport_handle;
static lwrb_t	gs_bsp_serialport_uart_tx_ring_buf;
static lwrb_t	gs_bsp_serialport_uart_rx_ring_buf;
static uint8_t 	gs_bsp_serialport_uart_tx_ring_internal_buf[D_BSP_SERIALPORT_UART_TX_RING_BUFFER_SIZE];	
static uint8_t 	gs_bsp_serialport_uart_rx_ring_internal_buf[D_BSP_SERIALPORT_UART_RX_RING_BUFFER_SIZE];	
static uint8_t 	gs_bsp_serialport_uart_tx_dma_buf[D_BSP_SERIALPORT_UART_TX_DMA_BUFFER_SIZE];
static uint8_t 	gs_bsp_serialport_uart_rx_dma_buf[D_BSP_SERIALPORT_UART_RX_DMA_BUFFER_SIZE];


/**************************************************************
**  Prototype
**************************************************************/

static int32_t bsp_serialport_uart_init();
//static int32_t bsp_serialport_uart_deInit();


/**************************************************************
**  Interface
**************************************************************/

/**
 * @brief       Serialport initialization
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_serialport_init()
{
	bsp_serialport_uart_init();
}

/**
 * @brief       Serialport deInitialization
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_serialport_deInit()
{
	/* TBD */
}

/**
 * @brief       Serialport data send
 * @retval		D_BSP_RET_OK
 * @retval		D_BSP_RET_UART_TX_DMA_BUSY
 * @retval		D_BSP_RET_HAL_ERROR
 * @author      chenwei.gu@murata.com
 */
extern int32_t bsp_serialport_send()
{
	int 				ret 	=	D_BSP_RET_OK;
	HAL_StatusTypeDef 	hal_ret =	HAL_OK;

	lwrb_sz_t read_len = 0;

	do
	{
		if (gs_bsp_serialport_handle.uart_tx_dma_status != E_BSP_SERIALPORT_UART_TX_DMA_STATUS_READY)
		{
			/* UART TX DMA status busy */
			ret = D_BSP_RET_UART_TX_DMA_BUSY;
			break;
		}

		/* Update UART TX DMA status */
		gs_bsp_serialport_handle.uart_tx_dma_status = E_BSP_SERIALPORT_UART_TX_DMA_STATUS_BUSY;

		/* Get data from UART TX FIFO buffer to UART TX DMA buffer */
		read_len = lwrb_read(gs_bsp_serialport_handle.p_uart_tx_ring_buf, gs_bsp_serialport_handle.p_uart_tx_dma_buf, D_BSP_SERIALPORT_UART_TX_DMA_BUFFER_SIZE);
		if (read_len > 0)
		{
			/* Send data through DMA */
			hal_ret = HAL_UART_Transmit_DMA(&(gs_bsp_serialport_handle.uart_handle), gs_bsp_serialport_handle.p_uart_tx_dma_buf, read_len);
			if (hal_ret != HAL_OK)
			{
				/* Get UART HAL error code */
				uint32_t err_code = 0;
				err_code = HAL_UART_GetError(&(gs_bsp_serialport_handle.uart_handle) );
				(void)err_code;
				
				/* Reset UART TX DMA status */
				gs_bsp_serialport_handle.uart_tx_dma_status = E_BSP_SERIALPORT_UART_TX_DMA_STATUS_READY;

				ret = D_BSP_RET_HAL_ERR;
				break;
			}
		}
		else
		{
			/* No data needs to be sent */ 
			/* Reset UART TX DMA status */
			gs_bsp_serialport_handle.uart_tx_dma_status = E_BSP_SERIALPORT_UART_TX_DMA_STATUS_READY;
		}

		ret = D_BSP_RET_OK;
	} while (0);

	return ret;
}

/**
 * @brief       Serialport reception enable
 * @retval		D_BSP_RET_OK      
 * @retval		D_BSP_RET_HAL_ERROR
 * @author      chenwei.gu@murata.com
 */
extern int32_t bsp_serialport_receive_enable()
{
	int 				ret 	=	D_BSP_RET_OK;
	HAL_StatusTypeDef 	hal_ret	=	HAL_OK;
	
	do
	{
		/* Enable UART reception */
		hal_ret = HAL_UARTEx_ReceiveToIdle_DMA(&(gs_bsp_serialport_handle.uart_handle), gs_bsp_serialport_handle.p_uart_rx_dma_buf, D_BSP_SERIALPORT_UART_RX_DMA_BUFFER_SIZE);
		if (hal_ret != HAL_OK) 
		{
			/* Get UART HAL error code */
			uint32_t err_code = 0;
			err_code = HAL_UART_GetError(&(gs_bsp_serialport_handle.uart_handle) );
			(void)err_code;

			ret = D_BSP_RET_HAL_ERR;
			break;
		}

		ret = D_BSP_RET_OK;
	} while (0);

	return ret;
}

/**
 * @brief       Serialport send cache buffer write operation
 * @param		buf			Data to be written
 * @param		size		Expected size of data to be written
 * @retval		Actual size of data to be written
 * @retval		D_BSP_RET_INPUT_ERROR
 * @author      chenwei.gu@murata.com
 */
extern int32_t bsp_serialport_send_cache_data_set(const uint8_t* buf, uint16_t size)
{
	if (!buf)
	{
		return D_BSP_RET_INPUT_ERR;
	}

	/* Write data to ring buffer */
	return lwrb_write(gs_bsp_serialport_handle.p_uart_tx_ring_buf, buf, (lwrb_sz_t)size);
}

/**
 * @brief		Serialport send cache buffer read operation
 * @param		buf			Data to be read
 * @param		size		Expected size of data to be read
 * @retval		Actual size of data to be read
 * @retval		D_BSP_RET_INPUT_ERROR
 * @author      chenwei.gu@murata.com
 */
extern int32_t bsp_serialport_send_cache_data_get(uint8_t* buf, uint16_t size)
{
	if (!buf)
	{
		return D_BSP_RET_INPUT_ERR;
	}

	/* Read data from FIFO buffer */
	return lwrb_read(gs_bsp_serialport_handle.p_uart_tx_ring_buf, buf, (lwrb_sz_t)size);
}

extern int32_t bsp_serialport_send_cache_used_size_get()
{
	if (lwrb_is_ready(gs_bsp_serialport_handle.p_uart_tx_ring_buf) == 0)
	{
		/* */
		return D_BSP_RET_RB_READY_ERR;
	}
	else
	{
		return lwrb_get_full(gs_bsp_serialport_handle.p_uart_tx_ring_buf);
	}
}

/**
 * @brief       Serialport receive cache buffer read operation
 * @param		buf			Data to be read
 * @param		size 		Expected size of data to be read
 * @retval		Actual size of data to be read
 * @retval		D_BSP_RET_INPUT_ERROR
 * @author      chenwei.gu@murata.com
 */
extern int32_t bsp_serialport_receive_cache_data_get(uint8_t* buf, uint16_t size)
{
	if (!buf)
	{
		return D_BSP_RET_INPUT_ERR;
	}

	/* Read data from buffer */
	return lwrb_read(gs_bsp_serialport_handle.p_uart_rx_ring_buf, buf, (lwrb_sz_t)size);	
}

/**
 * @brief       Serialport receive callback function set
 * @param		p_uart_rx_callcack 		Pointer to UART RX callback function
 * @retval		D_BSP_RET_OK
 * @retval		D_BSP_RET_INPUT_ERROR
 * @author      chenwei.gu@murata.com
 */
extern int32_t bsp_serialport_receive_callback_set(void (*p_uart_rx_callcack)(void) )
{
	if (!p_uart_rx_callcack)
	{
		return D_BSP_RET_INPUT_ERR;
	}

	int32_t ret = D_BSP_RET_OK;

	do
	{
		/* Set UART RX callback function */
		gs_bsp_serialport_handle.p_uart_rx_callcack = p_uart_rx_callcack;

		ret = D_BSP_RET_OK;
	} while (0);
	
	return ret;
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

	if (huart->Instance == gs_bsp_serialport_handle.uart_handle.Instance)
	{
		/* UART clock enable */
		D_BSP_SERIALPORT_UART_CLK_ENABLE();
		D_BSP_SERIALPORT_UART_TX_GPIO_CLK_ENABLE();

	    /**
	     * UART GPIO configuration
	     * PB6     ------> USART1_TX
	     * PB7     ------> USART1_RX
	     */
	    GPIO_InitStruct.Pin 		= D_BSP_SERIALPORT_UART_TX_GPIO_PIN;
	    GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull 		= GPIO_PULLUP;
	    GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	    GPIO_InitStruct.Alternate 	= D_BSP_SERIALPORT_UART_AF;
	    HAL_GPIO_Init(D_BSP_SERIALPORT_UART_TX_GPIO_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin 		= D_BSP_SERIALPORT_UART_RX_GPIO_PIN;
	    GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull 		= GPIO_PULLUP;
	    GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	    GPIO_InitStruct.Alternate 	= D_BSP_SERIALPORT_UART_AF;
	    HAL_GPIO_Init(D_BSP_SERIALPORT_UART_TX_GPIO_PORT, &GPIO_InitStruct);

		/* UART interrupt initialization */
	    HAL_NVIC_SetPriority(D_BSP_SERIALPORT_UART_IRQ_NUMBER, D_BSP_SERIALPORT_UART_IRQ_PRIORITY, D_BSP_SERIALPORT_UART_IRQ_SUBPRIORITY);
	    HAL_NVIC_EnableIRQ(D_BSP_SERIALPORT_UART_IRQ_NUMBER);

		/* DMA controller clock enable */
		D_BSP_SERIALPORT_UART_DMAMUX_CLK_ENABLE();
		D_BSP_SERIALPORT_UART_TX_DMA_CLK_ENABLE();
		D_BSP_SERIALPORT_UART_RX_DMA_CLK_ENABLE();

		/* UART DMA TX initialization */
		DMA_HandleTypeDef* dma_uart_tx_handle 			= 	&(gs_bsp_serialport_handle.dma_uart_tx_handle);

		dma_uart_tx_handle->Instance 					=	D_BSP_SERIALPORT_UART_TX_DMA_INSTANCE;
		dma_uart_tx_handle->Init.Request 				=	D_BSP_SERIALPORT_UART_TX_DMA_REQUEST;
		dma_uart_tx_handle->Init.Direction 				=	DMA_MEMORY_TO_PERIPH;
		dma_uart_tx_handle->Init.PeriphInc 				=	DMA_PINC_DISABLE;
		dma_uart_tx_handle->Init.MemInc 				=	DMA_MINC_ENABLE;
		dma_uart_tx_handle->Init.PeriphDataAlignment 	=	DMA_PDATAALIGN_BYTE;
		dma_uart_tx_handle->Init.MemDataAlignment 		=	DMA_MDATAALIGN_BYTE;
		dma_uart_tx_handle->Init.Mode 					=	DMA_NORMAL;
		dma_uart_tx_handle->Init.Priority 				=	DMA_PRIORITY_LOW;
		
		if (HAL_DMA_Init(dma_uart_tx_handle) != HAL_OK)
		{
			/* TBD */
		}

		__HAL_LINKDMA(huart, hdmatx, (*dma_uart_tx_handle) );

		/* UART DMA RX initialization */
		DMA_HandleTypeDef* dma_uart_rx_handle 			= 	&(gs_bsp_serialport_handle.dma_uart_rx_handle);

		dma_uart_rx_handle->Instance 					=	D_BSP_SERIALPORT_UART_RX_DMA_INSTANCE;
		dma_uart_rx_handle->Init.Request 				=	D_BSP_SERIALPORT_UART_RX_DMA_REQUEST;
		dma_uart_rx_handle->Init.Direction 				=	DMA_PERIPH_TO_MEMORY;
		dma_uart_rx_handle->Init.PeriphInc 				=	DMA_PINC_DISABLE;
		dma_uart_rx_handle->Init.MemInc 				= 	DMA_MINC_ENABLE;
		dma_uart_rx_handle->Init.PeriphDataAlignment	= 	DMA_PDATAALIGN_BYTE;
		dma_uart_rx_handle->Init.MemDataAlignment 		= 	DMA_MDATAALIGN_BYTE;
		dma_uart_rx_handle->Init.Mode 					= 	DMA_CIRCULAR;
		dma_uart_rx_handle->Init.Priority 				= 	DMA_PRIORITY_HIGH;
		
		if (HAL_DMA_Init(dma_uart_rx_handle) != HAL_OK)
		{
			/* TBD */
		}

		__HAL_LINKDMA(huart, hdmarx, (*dma_uart_rx_handle) );

		/* DMA1_Channel1_IRQn interrupt initialization  */
  		HAL_NVIC_SetPriority(D_BSP_SERIALPORT_UART_TX_DMA_IRQ_NUMBER, D_BSP_SERIALPORT_UART_TX_DMA_IRQ_PRIORITY, D_BSP_SERIALPORT_UART_TX_DMA_IRQ_SUBPRIORITY);
  		HAL_NVIC_EnableIRQ(D_BSP_SERIALPORT_UART_TX_DMA_IRQ_NUMBER);

  		/* DMA1_Channel2_IRQn interrupt initialization  */
 	 	HAL_NVIC_SetPriority(D_BSP_SERIALPORT_UART_RX_DMA_IRQ_NUMBER, D_BSP_SERIALPORT_UART_RX_DMA_IRQ_PRIORITY, D_BSP_SERIALPORT_UART_RX_DMA_IRQ_SUBPRIORITY);
  		HAL_NVIC_EnableIRQ(D_BSP_SERIALPORT_UART_RX_DMA_IRQ_NUMBER);
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
	if (huart->Instance == gs_bsp_serialport_handle.uart_handle.Instance)
	{
		/* Reset UART */
		D_BSP_SERIALPORT_UART_FORCE_RESET();
		D_BSP_SERIALPORT_UART_RELEASE_RESET();

		/* UART clock disable */
		D_BSP_SERIALPORT_UART_CLK_DISABLE();

	    /**
	     * UART1 GPIO configuration
	     * PB6     ------> UART1_TX
	     * PB7     ------> UART1_RX
	     */
	    HAL_GPIO_DeInit(D_BSP_SERIALPORT_UART_TX_GPIO_PORT, D_BSP_SERIALPORT_UART_TX_GPIO_PIN);
	    HAL_GPIO_DeInit(D_BSP_SERIALPORT_UART_RX_GPIO_PORT, D_BSP_SERIALPORT_UART_RX_GPIO_PIN);

		/* UART DMA deInit */
    	HAL_DMA_DeInit(huart->hdmarx);

		/* UART interrupt deInit */
	    HAL_NVIC_DisableIRQ(D_BSP_SERIALPORT_UART_IRQ_NUMBER);
	}
}

/**
 * @brief 		UART transmission transfer completed callback.
 * @param 		huart		UART handle
 * @retval 		None
 * @author 		chenwei.gu@murata.com
 */
extern void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == gs_bsp_serialport_handle.uart_handle.Instance)
	{
		/* Set UART TX DMA status to ready */
		gs_bsp_serialport_handle.uart_tx_dma_status = E_BSP_SERIALPORT_UART_TX_DMA_STATUS_READY;

		/* Check for remaining data to send */
		if (lwrb_get_full(gs_bsp_serialport_handle.p_uart_tx_ring_buf) >0)
		{
			/* Continue sending data */
			bsp_serialport_send();
		}
	}
}

/**
 * @brief  		UART reception transfer completed callback
 * @param  		huart		UART handle
 * @author 		chenwei.gu@murata.com
 */
extern void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{	
	UNUSED(huart);
}

/**
 * @brief		UART reception event callback
 * @param  		huart 		UART handle
 * @param  		Size  		Number of data available in application reception buffer
 * 			 				(indicates a position in reception buffer until which, data are available)
 * @retval 		None
 * @author 		chenwei.gu@murata.com
 */
extern void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
{
	if (huart->Instance == gs_bsp_serialport_handle.uart_handle.Instance)
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
				lwrb_write(gs_bsp_serialport_handle.p_uart_rx_ring_buf, &(gs_bsp_serialport_handle.p_uart_rx_dma_buf[dma_buf_last_size]), proc_char_size);
			}
			else
			{
				/* Restart getting data from the DMA buffer */
				proc_char_size = dma_buf_curr_size;
				lwrb_write(gs_bsp_serialport_handle.p_uart_rx_ring_buf, &(gs_bsp_serialport_handle.p_uart_rx_dma_buf[0]), proc_char_size);
			}

			dma_buf_last_size = dma_buf_curr_size;

			if (gs_bsp_serialport_handle.p_uart_rx_callcack != NULL)
			{
				gs_bsp_serialport_handle.p_uart_rx_callcack();
			}
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
	HAL_DMA_IRQHandler(&(gs_bsp_serialport_handle.dma_uart_tx_handle) );
}

/**
 * @brief		DMA1 Channel2 interrupt handler function
 * @retval 		None
 * @author 		chenwei.gu@murata.com
 */
extern void DMA1_Channel2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&(gs_bsp_serialport_handle.dma_uart_rx_handle) );
}

/**
 * @brief		USART1 interrupt handler function
 * @retval 		None
 * @author 		chenwei.gu@murata.com
 */
extern void USART1_IRQHandler()
{
	HAL_UART_IRQHandler(&(gs_bsp_serialport_handle.uart_handle) );
}


/**************************************************************
**  Function
**************************************************************/

/**
 * @brief       UART initialization
 * @retval      D_BSP_RET_OK
 * @retval      D_BSP_RET_HAL_ERR
 * @retval      D_BSP_RET_RB_INIT_ERR
 * @author      chenwei.gu@murata.com
 */
static int32_t bsp_serialport_uart_init()
{
	int ret = D_BSP_RET_OK;

	do
	{
		/* Configure UART initialization */
		UART_HandleTypeDef* uart_handle 			= 	&(gs_bsp_serialport_handle.uart_handle);

		uart_handle->Instance						=	D_BSP_SERIALPORT_UART_INSTANSE;
		uart_handle->Init.BaudRate 					= 	115200;
		uart_handle->Init.WordLength 				= 	UART_WORDLENGTH_8B;
		uart_handle->Init.StopBits 					= 	UART_STOPBITS_1;
		uart_handle->Init.Parity 					= 	UART_PARITY_NONE;
		uart_handle->Init.Mode 						= 	UART_MODE_TX_RX;
		uart_handle->Init.HwFlowCtl 				= 	UART_HWCONTROL_NONE;
		uart_handle->Init.OverSampling 				= 	UART_OVERSAMPLING_16;
		uart_handle->Init.OneBitSampling 			= 	UART_ONE_BIT_SAMPLE_DISABLE;
		uart_handle->Init.ClockPrescaler 			= 	UART_PRESCALER_DIV1;
		uart_handle->AdvancedInit.AdvFeatureInit	= 	UART_ADVFEATURE_NO_INIT;
	
		if (HAL_UART_Init(uart_handle) != HAL_OK)
		{
			ret = D_BSP_RET_HAL_ERR;
			break;
		}
		if (HAL_UARTEx_SetTxFifoThreshold(uart_handle, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
		{
			ret = D_BSP_RET_HAL_ERR;
			break;
		}
		if (HAL_UARTEx_SetRxFifoThreshold(uart_handle, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
		{
			ret = D_BSP_RET_HAL_ERR;
			break;
		}
		if (HAL_UARTEx_DisableFifoMode(uart_handle) != HAL_OK)
		{
			ret = D_BSP_RET_HAL_ERR;
			break;
		}

		/* Configure UART buffer */
		if ( (!lwrb_init(&gs_bsp_serialport_uart_tx_ring_buf, gs_bsp_serialport_uart_tx_ring_internal_buf, D_BSP_SERIALPORT_UART_TX_RING_BUFFER_SIZE) ) ||
			 (!lwrb_init(&gs_bsp_serialport_uart_rx_ring_buf, gs_bsp_serialport_uart_rx_ring_internal_buf, D_BSP_SERIALPORT_UART_RX_RING_BUFFER_SIZE) ) )
		{
			ret = D_BSP_RET_RB_INIT_ERR;
			break;
		}
		gs_bsp_serialport_handle.p_uart_tx_ring_buf =	&gs_bsp_serialport_uart_tx_ring_buf;
		gs_bsp_serialport_handle.p_uart_rx_ring_buf =	&gs_bsp_serialport_uart_rx_ring_buf;
		gs_bsp_serialport_handle.p_uart_tx_dma_buf	=	gs_bsp_serialport_uart_tx_dma_buf;
		gs_bsp_serialport_handle.p_uart_rx_dma_buf	=	gs_bsp_serialport_uart_rx_dma_buf;

		/* Initialize UART TX DMA status */
		gs_bsp_serialport_handle.uart_tx_dma_status	=	E_BSP_SERIALPORT_UART_TX_DMA_STATUS_READY;

		ret = D_BSP_RET_OK;
	} while (0);

	return ret;
}

/**
 * @brief       UART deInitialization
 * @retval      D_BSP_RET_OK
 * @retval      ...
 * @author      chenwei.gu@murata.com
 */
//static int32_t bsp_serialport_uart_deInit()
//{
//	/* TBD */
//}
