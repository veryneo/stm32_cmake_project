#ifndef __BSP_ERRNO_H
#define __BSP_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************
**  Symbol
**************************************************************/

/**
 * @brief BSP return value definition
 */
#define D_BSP_RET_OK                       		(0)		/* Ok */

#define D_BSP_RET_INPUT_ERR 					(-10)	/* Input parameter error */

#define D_BSP_RET_HAL_ERR	    				(-20)	/* HAL error */

#define D_BSP_RET_RB_INIT_ERR                   (-30)   /* Ring buffer initialization error */
#define D_BSP_RET_RB_READY_ERR                  (-31)   /* Ring buffer is not ready */

#define D_BSP_RET_UART_TX_DMA_BUSY				(-40)	/* UART TX DMA status is busy */

//#define BSP_ERROR_NO_INIT                     -1
//#define BSP_ERROR_WRONG_PARAM                 -2
//#define BSP_ERROR_BUSY                        -3
//#define BSP_ERROR_PERIPH_FAILURE              -4
//#define BSP_ERROR_COMPONENT_FAILURE           -5
//#define BSP_ERROR_UNKNOWN_FAILURE             -6
//#define BSP_ERROR_UNKNOWN_COMPONENT           -7
//#define BSP_ERROR_BUS_FAILURE                 -8
//#define BSP_ERROR_CLOCK_FAILURE               -9
//#define BSP_ERROR_MSP_FAILURE                 -10
//#define BSP_ERROR_FEATURE_NOT_SUPPORTED       -11


/**************************************************************
**  Structure
**************************************************************/



#ifdef __cplusplus
}
#endif

#endif
