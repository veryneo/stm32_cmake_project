#ifndef BSP_BUTTON_H
#define BSP_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stdint.h"


typedef enum 
{
    E_BSP_BUTTON_ID_2,
    E_BSP_BUTTON_ID_3,
    E_BSP_BUTTON_ID_COUNT
} E_BSP_BUTTON_ID_T;

typedef enum 
{
    E_BSP_BUTTON_STATE_RELEASED,
    E_BSP_BUTTON_STATE_PRESSED
} E_BSP_BUTTON_STATE_T;


extern int32_t bsp_button_init(E_BSP_BUTTON_ID_T);
extern int32_t bsp_button_deinit(E_BSP_BUTTON_ID_T);
extern int32_t bsp_button_state_get(E_BSP_BUTTON_ID_T, E_BSP_BUTTON_STATE_T*);


#ifdef __cplusplus
}
#endif

#endif