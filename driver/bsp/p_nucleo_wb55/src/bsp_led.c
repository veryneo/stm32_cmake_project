/**************************************************************
**  Include
**************************************************************/

#include "bsp_led.h"

#include "stm32wbxx_hal.h"


/**************************************************************
**  Symbol
**************************************************************/

/**
 * @defgroup LED pin configuration
 */
#define D_BSP_LED_NUM                       3

#define D_BSP_LED1_PIN                      GPIO_PIN_5
#define D_BSP_LED1_GPIO_PORT                GPIOB
#define D_BSP_LED1_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define D_BSP_LED1_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOB_CLK_DISABLE()

#define D_BSP_LED2_PIN                      GPIO_PIN_0
#define D_BSP_LED2_GPIO_PORT                GPIOB
#define D_BSP_LED2_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define D_BSP_LED2_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOB_CLK_DISABLE()

#define D_BSP_LED3_PIN                      GPIO_PIN_1
#define D_BSP_LED3_GPIO_PORT                GPIOB
#define D_BSP_LED3_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define D_BSP_LED3_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOB_CLK_DISABLE()

#define D_BSP_LEDx_GPIO_CLK_ENABLE(__LED__) do                                                                      \
                                            {                                                                       \
                                                if ( (__LED__) == E_BSP_LED1) D_BSP_LED1_GPIO_CLK_ENABLE();         \
                                                else if ( (__LED__) == E_BSP_LED2) D_BSP_LED2_GPIO_CLK_ENABLE();    \
                                                else if ( (__LED__) == E_BSP_LED3) D_BSP_LED3_GPIO_CLK_ENABLE();    \
                                            } while (0)


/**************************************************************
**  Variable
**************************************************************/

static GPIO_TypeDef*   gs_gpio_port[D_BSP_LED_NUM]  = {D_BSP_LED1_GPIO_PORT, D_BSP_LED2_GPIO_PORT, D_BSP_LED3_GPIO_PORT};
static const uint16_t  gs_gpio_pin[D_BSP_LED_NUM]   = {D_BSP_LED1_PIN, D_BSP_LED2_PIN, D_BSP_LED3_PIN};


/**************************************************************
**  Interface
**************************************************************/

/**
 * @brief       Initialization of LED
 * @param[in]   led: Specifies the Led to be initialized
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_led_init(E_BSP_LED led)
{
    GPIO_InitTypeDef gpio_initStruct = {0};

    /* Enable the LED GPIO Clock */
    D_BSP_LEDx_GPIO_CLK_ENABLE(led);

    /* Configure the LED GPIO pin */
    gpio_initStruct.Pin     = gs_gpio_pin[led];
    gpio_initStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    gpio_initStruct.Pull    = GPIO_NOPULL;
    gpio_initStruct.Speed   = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(gs_gpio_port[led], &gpio_initStruct);

    HAL_GPIO_WritePin(gs_gpio_port[led], gs_gpio_pin[led], GPIO_PIN_RESET);
}

/**
 * @brief       Deinitialization of LED
 * @param[in]   led: Specifies the Led to be deinitialized
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_led_deinit(E_BSP_LED led)
{
    /* Turn off LED */
    HAL_GPIO_WritePin(gs_gpio_port[led], gs_gpio_pin[led], GPIO_PIN_RESET);

    /* Deinit the LED GPIO pin */
    HAL_GPIO_DeInit(gs_gpio_port[led], gs_gpio_pin[led]);
}

/**
 * @brief       Turn on selected LED
 * @param[in]   led: Specifies the Led to be set on
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_led_on(E_BSP_LED led)
{
    HAL_GPIO_WritePin(gs_gpio_port[led], gs_gpio_pin[led], GPIO_PIN_SET);
}

/**
 * @brief       Turn off selected LED
 * @param[in]   led: Specifies the Led to be set off
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_led_off(E_BSP_LED led)
{
    HAL_GPIO_WritePin(gs_gpio_port[led], gs_gpio_pin[led], GPIO_PIN_RESET);
}

/**
 * @brief       Toggle selected LED
 * @param[in]   led: Specifies the Led to be toggled
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_led_toggle(E_BSP_LED led)
{
    HAL_GPIO_TogglePin(gs_gpio_port[led], gs_gpio_pin[led]);
}

/**
 * @brief       Time delay (in illiseconds)
 * @param[in]   delay_ms: Specifies the delay time length, in milliseconds
 * @retval      None
 * @author      chenwei.gu@murata.com
 */
extern void bsp_led_delay(uint32_t delay_ms)
{
    HAL_Delay(delay_ms);
}

