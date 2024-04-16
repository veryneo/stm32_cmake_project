#ifndef __BSP_DEF_H
#define __BSP_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************
**  Include
**************************************************************/

/* Common include file */
#include "stdint.h"
#include "string.h"


/**************************************************************
**  Symbol
**************************************************************/

/**
 * @brief BSP error number definition
 */
#define D_BSP_RET_OK                       		(0)
#define D_BSP_RET_INPUT_ERROR					(-1)
#define D_BSP_RET_HAL_ERROR						(-2)
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


#ifdef __cplusplus
}
#endif

#endif
