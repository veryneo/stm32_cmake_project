#include "bsp_button.h"
#include "bsp_errno.h"

#include "stm32wbxx_hal.h"

#include "stdint.h"
#include "stdbool.h"

/**************************************************************
**  Symbol
**************************************************************/

#define D_BSP_BUTTON_ENABLE_GPIO_CLOCK(port) \
    do { \
        if(         (port) == GPIOA)    __HAL_RCC_GPIOA_CLK_ENABLE(); \
        else if(    (port) == GPIOB)    __HAL_RCC_GPIOB_CLK_ENABLE(); \
        else if(    (port) == GPIOC)    __HAL_RCC_GPIOC_CLK_ENABLE(); \
        else if(    (port) == GPIOD)    __HAL_RCC_GPIOD_CLK_ENABLE(); \
        else if(    (port) == GPIOE)    __HAL_RCC_GPIOE_CLK_ENABLE(); \
        else if(    (port) == GPIOH)    __HAL_RCC_GPIOH_CLK_ENABLE(); \
    } while(0)  


/**************************************************************
**  Structure
**************************************************************/

typedef struct 
{
    GPIO_TypeDef*   port;
    uint16_t        pin;       
    uint32_t        pull; 
    bool            active_low;
} S_BSP_BUTTON_GPIO_CONF_T;


/**************************************************************
**  Variable
**************************************************************/

static const S_BSP_BUTTON_GPIO_CONF_T gs_bsp_button_gpio_conf[E_BSP_BUTTON_ID_COUNT] = 
{
    [E_BSP_BUTTON_ID_2] = {GPIOD, GPIO_PIN_0, GPIO_PULLUP, true},
    [E_BSP_BUTTON_ID_3] = {GPIOD, GPIO_PIN_1, GPIO_PULLUP, true}
};


/**************************************************************
**  Interface
**************************************************************/

extern int32_t bsp_button_init(E_BSP_BUTTON_ID_T btn_id)
{
    if (btn_id >= E_BSP_BUTTON_ID_COUNT) 
    {
        return D_BSP_RET_INPUT_ERR;
    }
    
    const S_BSP_BUTTON_GPIO_CONF_T* p_btn_gpio_conf = &gs_bsp_button_gpio_conf[btn_id];
    
    D_BSP_BUTTON_ENABLE_GPIO_CLOCK(p_btn_gpio_conf->port);

    GPIO_InitTypeDef gpio_initStruct = {0};
    gpio_initStruct.Pin   = p_btn_gpio_conf->pin;
    gpio_initStruct.Mode  = GPIO_MODE_INPUT;
    gpio_initStruct.Pull  = p_btn_gpio_conf->pull;
    gpio_initStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(p_btn_gpio_conf->port, &gpio_initStruct);

    return D_BSP_RET_OK;
}

extern int32_t bsp_button_deinit(E_BSP_BUTTON_ID_T btn_id)
{
    if (btn_id >= E_BSP_BUTTON_ID_COUNT) 
    {
        return D_BSP_RET_INPUT_ERR;
    }

    const S_BSP_BUTTON_GPIO_CONF_T* p_btn_gpio_conf = &gs_bsp_button_gpio_conf[btn_id];

    HAL_GPIO_DeInit(p_btn_gpio_conf->port, p_btn_gpio_conf->pin);

    return D_BSP_RET_OK;
}

extern int32_t bsp_button_state_get(E_BSP_BUTTON_ID_T btn_id, E_BSP_BUTTON_STATE_T* btn_state)
{
    if (btn_state == NULL || (btn_id >= E_BSP_BUTTON_ID_COUNT) ) 
    {
        return D_BSP_RET_INPUT_ERR;
    }

    const S_BSP_BUTTON_GPIO_CONF_T* p_btn_gpio_conf = &gs_bsp_button_gpio_conf[btn_id];

    *btn_state = (HAL_GPIO_ReadPin(p_btn_gpio_conf->port, p_btn_gpio_conf->pin)
                  == (p_btn_gpio_conf->active_low ? GPIO_PIN_RESET : GPIO_PIN_SET) ) 
                  ? E_BSP_BUTTON_STATE_PRESSED : E_BSP_BUTTON_STATE_RELEASED;

    return D_BSP_RET_OK;
}
