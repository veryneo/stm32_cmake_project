/**************************************************************
**  Include
**************************************************************/

#include "bsp_nfc07a1.h"

#include "stm32wbxx_hal.h"


/**************************************************************
** Symbol
**************************************************************/

#define D_BSP_NFC07A1_I2C_INSTANCE                          I2C1

#define D_BSP_NFC07A1_I2C_CLK_ENABLE()                      __HAL_RCC_I2C1_CLK_ENABLE()
#define D_BSP_NFC07A1_I2C_CLK_DISABLE()                     __HAL_RCC_I2C1_CLK_DISABLE()

#define D_BSP_NFC07A1_I2C_SCL_GPIO_PORT                     GPIOB
#define D_BSP_NFC07A1_I2C_SCL_GPIO_PIN                      GPIO_PIN_8
#define D_BSP_NFC07A1_I2C_SCL_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define D_BSP_NFC07A1_I2C_SCL_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()
#define D_BSP_NFC07A1_I2C_SCL_GPIO_AF                       GPIO_AF4_I2C1

#define D_BSP_NFC07A1_I2C_SCA_GPIO_PORT                     GPIOB
#define D_BSP_NFC07A1_I2C_SCA_GPIO_PIN                      GPIO_PIN_9
#define D_BSP_NFC07A1_I2C_SCA_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define D_BSP_NFC07A1_I2C_SCA_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()
#define D_BSP_NFC07A1_I2C_SCA_GPIO_AF                       GPIO_AF4_I2C1


#define D_BSP_NFC07A1_LED_NUM                               (3)

#define D_BSP_NFC07A1_LED1_GPIO_PORT                        GPIOA
#define D_BSP_NFC07A1_LED1_PIN                              GPIO_PIN_15
#define D_BSP_NFC07A1_LED1_GPIO_CLK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE()
#define D_BSP_NFC07A1_LED1_GPIO_CLK_DISABLE()               __HAL_RCC_GPIOA_CLK_DISABLE()

#define D_BSP_NFC07A1_LED2_GPIO_PORT                        GPIOC
#define D_BSP_NFC07A1_LED2_PIN                              GPIO_PIN_10
#define D_BSP_NFC07A1_LED2_GPIO_CLK_ENABLE()                __HAL_RCC_GPIOC_CLK_ENABLE()
#define D_BSP_NFC07A1_LED2_GPIO_CLK_DISABLE()               __HAL_RCC_GPIOC_CLK_DISABLE()

#define D_BSP_NFC07A1_LED3_GPIO_PORT                        GPIOC
#define D_BSP_NFC07A1_LED3_PIN                              GPIO_PIN_6
#define D_BSP_NFC07A1_LED3_GPIO_CLK_ENABLE()                __HAL_RCC_GPIOC_CLK_ENABLE()
#define D_BSP_NFC07A1_LED3_GPIO_CLK_DISABLE()               __HAL_RCC_GPIOC_CLK_DISABLE()

#define D_BSP_NFC07A1_LEDx_GPIO_CLK_ENABLE(__LED__)         do                                                                      \
                                                            {                                                                       \
                                                                if ( (__LED__) == E_BSP_NFC07A1_LED1) D_BSP_NFC07A1_LED1_GPIO_CLK_ENABLE();         \
                                                                else if ( (__LED__) == E_BSP_NFC07A1_LED2) D_BSP_NFC07A1_LED2_GPIO_CLK_ENABLE();    \
                                                                else if ( (__LED__) == E_BSP_NFC07A1_LED3) D_BSP_NFC07A1_LED3_GPIO_CLK_ENABLE();    \
                                                            } while (0)


#define D_BSP_NFC07A1_GPO_GPIO_PORT                         GPIOA
#define D_BSP_NFC07A1_GPO_GPIO_PIN                          GPIO_PIN_6
#define D_BSP_NFC07A1_GPO_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOA_CLK_ENABLE()
#define D_BSP_NFC07A1_GPO_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOA_CLK_DISABLE()
#define D_BSP_NFC07A1_GPO_EXTI_NUM                          EXTI9_5_IRQn
#define D_BSP_NFC07A1_GPO_EXTI_PRIORITY                     (0)


#define D_BSP_I2C_POLL_TIMEOUT                              (0x1000)


/**************************************************************
** Sturcture
**************************************************************/

typedef struct
{
    ST25DVxxKC_Object_t     nfctag_obj;
    ST25DVxxKC_Drv_t*       p_nfctag_drv;
    I2C_HandleTypeDef       i2c_handle;
    uint8_t                 gpo_activeFlag;
} S_BSP_NFC07A1_HANDLE_T;


/**************************************************************
** Variable
**************************************************************/

static GPIO_TypeDef*    gs_bsp_nfc07a1_led_port[D_BSP_NFC07A1_LED_NUM] = {D_BSP_NFC07A1_LED1_GPIO_PORT, D_BSP_NFC07A1_LED2_GPIO_PORT, D_BSP_NFC07A1_LED3_GPIO_PORT};
static const uint16_t   gs_bsp_nfc07a1_led_pin[D_BSP_NFC07A1_LED_NUM]  = {D_BSP_NFC07A1_LED1_PIN, D_BSP_NFC07A1_LED2_PIN, D_BSP_NFC07A1_LED3_PIN};

static S_BSP_NFC07A1_HANDLE_T gs_bsp_nfc07a1_handle = {0};


/**************************************************************
** Function Prototype
**************************************************************/

static int32_t bsp_nfc07a1_nfctag_i2C_init(void);
static int32_t bsp_nfc07a1_nfctag_i2c_deInit(void);
static int32_t bsp_nfc07a1_nfctag_i2c_isReady(uint16_t dev_addr, uint32_t trials);
// static int32_t bsp_nfc07a1_nfctag_i2c_reg_write(uint16_t dev_addr, uint16_t reg, uint8_t *p_data, uint16_t length);
// static int32_t bsp_nfc07a1_nfctag_i2c_reg_read(uint16_t dev_addr, uint16_t reg, uint8_t *p_data, uint16_t length);
static int32_t bsp_nfc07a1_nfctag_i2c_reg16_write(uint16_t dev_addr, uint16_t reg, uint8_t *p_data, uint16_t length);
static int32_t bsp_nfc07a1_nfctag_i2c_reg16_read(uint16_t dev_addr, uint16_t reg, uint8_t *p_data, uint16_t length);
// static int32_t bsp_nfc07a1_nfctag_i2c_transmit(uint16_t dev_addr, uint8_t *p_data, uint16_t length);
// static int32_t bsp_nfc07a1_nfctag_i2c_receive(uint16_t dev_addr, uint8_t *p_data, uint16_t length);
static int32_t bsp_nfc07a1_nfctag_tick_get(void);


/**************************************************************
** Interface
**************************************************************/

extern int32_t bsp_nfc07a1_nfctag_init(void)
{
    ST25DVxxKC_IO_t io = {0};

    io.Init     =   (ST25DVxxKC_Init_Func)bsp_nfc07a1_nfctag_i2C_init;   
    io.DeInit   =   (ST25DVxxKC_DeInit_Func)bsp_nfc07a1_nfctag_i2c_deInit;
    io.IsReady  =   (ST25DVxxKC_IsReady_Func)bsp_nfc07a1_nfctag_i2c_isReady;
    io.Read     =   (ST25DVxxKC_Read_Func)bsp_nfc07a1_nfctag_i2c_reg16_read;
    io.Write    =   (ST25DVxxKC_Write_Func)bsp_nfc07a1_nfctag_i2c_reg16_write;
    io.GetTick  =   (ST25DVxxKC_GetTick_Func)bsp_nfc07a1_nfctag_tick_get;

    if (ST25DVxxKC_RegisterBusIO(&(gs_bsp_nfc07a1_handle.nfctag_obj) , &io) != NFCTAG_OK)
    {
        return D_BSP_RET_NFCTAG_ERR;
    }

    gs_bsp_nfc07a1_handle.p_nfctag_drv = (ST25DVxxKC_Drv_t*)(void*)&St25Dvxxkc_Drv;
    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->Init != NULL)
    {
        if (gs_bsp_nfc07a1_handle.p_nfctag_drv->Init(&(gs_bsp_nfc07a1_handle.nfctag_obj) ) != NFCTAG_OK)
        {
            gs_bsp_nfc07a1_handle.p_nfctag_drv = NULL;
            return D_BSP_RET_NFCTAG_ERR;
        }
        else
        {
            return D_BSP_RET_OK;
        }
    }
    else 
    {
        gs_bsp_nfc07a1_handle.p_nfctag_drv = NULL;
        return D_BSP_RET_NFCTAG_ERR;
    }
}

extern int32_t bsp_nfc07a1_nfctag_deInit(void)
{
    if (gs_bsp_nfc07a1_handle.p_nfctag_drv != NULL)
    {
        gs_bsp_nfc07a1_handle.p_nfctag_drv = NULL;
        gs_bsp_nfc07a1_handle.nfctag_obj.IsInitialized = 0;
        return D_BSP_RET_OK;
    }
    else
    {
        return D_BSP_RET_NFCTAG_ERR;
    }
}

extern void bsp_nfc07a1_nfctag_isInitialized(uint32_t* const p_isInitialized)
{
    *p_isInitialized = gs_bsp_nfc07a1_handle.nfctag_obj.IsInitialized;
}

extern int32_t bsp_nfc07a1_nfctag_id_read(uint8_t* const wai_id)
{
    if (gs_bsp_nfc07a1_handle.p_nfctag_drv == NULL)
    {
        return D_BSP_RET_NFCTAG_ERR;
    }

    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->ReadID(&(gs_bsp_nfc07a1_handle.nfctag_obj), wai_id) == NFCTAG_OK)
    {
        return D_BSP_RET_OK;
    }
    else
    {
        return D_BSP_RET_NFCTAG_ERR;
    }
}

extern int32_t bsp_nfc07a1_nfctag_it_configure(const uint16_t it_conf)
{
    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->ConfigIT == NULL )
    {
        return D_BSP_RET_NFCTAG_ERR;
    }

    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->ConfigIT(&(gs_bsp_nfc07a1_handle.nfctag_obj), it_conf) != NFCTAG_OK)
    {
        return D_BSP_RET_NFCTAG_ERR;
    }
    else
    {
        return D_BSP_RET_OK;
    }
}

extern int32_t bsp_nfc07a1_nfctag_it_status_get(uint16_t* const it_status)
{
    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->GetITStatus == NULL )
    {
        return D_BSP_RET_NFCTAG_ERR;
    }

    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->GetITStatus(&(gs_bsp_nfc07a1_handle.nfctag_obj), it_status) != NFCTAG_OK)
    {
        return D_BSP_RET_NFCTAG_ERR;
    }
    else
    {
        return D_BSP_RET_OK;
    }
}

extern int32_t bsp_nfc07a1_nfctag_data_read(uint8_t* const p_data, const uint16_t tar_addr, const uint16_t size)
{
    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->ReadData == NULL )
    {
        return D_BSP_RET_NFCTAG_ERR;
    }

    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->ReadData(&(gs_bsp_nfc07a1_handle.nfctag_obj), p_data, tar_addr, size) != NFCTAG_OK)
    {
        return D_BSP_RET_NFCTAG_ERR;
    }
    else
    {
        return D_BSP_RET_OK;
    }
}

extern int32_t bsp_nfc07a1_nfctag_data_write(const uint8_t* const p_data, const uint16_t tar_addr, const uint16_t size)
{
    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->WriteData == NULL )
    {
        return D_BSP_RET_NFCTAG_ERR;
    }

    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->WriteData(&(gs_bsp_nfc07a1_handle.nfctag_obj), p_data,tar_addr, size) != NFCTAG_OK)
    {
        return D_BSP_RET_NFCTAG_ERR;
    }
    else
    {        
        return D_BSP_RET_OK;
    }
}

extern int32_t bsp_nfc07a1_nfctag_reg_read(uint8_t* const p_data, const uint16_t tar_addr, const uint16_t size)
{
    if (ST25DVxxKC_ReadRegister(&gs_bsp_nfc07a1_handle.nfctag_obj, p_data, tar_addr, size) != NFCTAG_OK)
    { 
        return D_BSP_RET_NFCTAG_ERR;
    }
    else
    {
        return D_BSP_RET_OK;
    }
}

extern int32_t bsp_nfc07a1_nfctag_reg_write(const uint8_t* const p_data, const uint16_t tar_addr, const uint16_t size)
{
    if (ST25DVxxKC_WriteRegister(&gs_bsp_nfc07a1_handle.nfctag_obj, p_data, tar_addr, size) != NFCTAG_OK)
    {
        return D_BSP_RET_NFCTAG_ERR;
    }
    else
    {
        while(gs_bsp_nfc07a1_handle.p_nfctag_drv->IsReady(&gs_bsp_nfc07a1_handle.nfctag_obj, 1) != NFCTAG_OK ) {};
        
        return D_BSP_RET_OK;
    }  
}

extern int32_t bsp_nfc07a1_nfctag_device_isReady(const uint32_t trials)
{   
    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->IsReady == NULL)
    {
        return D_BSP_RET_NFCTAG_ERR;
    }
  
    if (gs_bsp_nfc07a1_handle.p_nfctag_drv->IsReady(&gs_bsp_nfc07a1_handle.nfctag_obj, trials) != NFCTAG_OK)
    {
        return D_BSP_RET_NFCTAG_ERR;
    }
    else
    {
        return D_BSP_RET_OK;
    }
}

extern int32_t bsp_nfc07a1_nfctag_i2cPassword_present(const uint32_t password_msb, const uint32_t password_lsb)
{
    ST25DVxxKC_PASSWD_t password = {0};

    password.MsbPasswd = password_msb;
    password.LsbPasswd = password_lsb;
    
    if (ST25DVxxKC_PresentI2CPassword(&gs_bsp_nfc07a1_handle.nfctag_obj, password) != NFCTAG_OK)
    {
        return D_BSP_RET_NFCTAG_ERR;
    }
    else
    {
        return D_BSP_RET_OK;
    }
}

extern void bsp_nfc07a1_led_init(E_BSP_NFC07A1_LED led)
{
    GPIO_InitTypeDef gpio_initStruct = {0};

    /* Enable the LED GPIO Clock */
    D_BSP_NFC07A1_LEDx_GPIO_CLK_ENABLE(led);

    /* Configure the LED GPIO pin */
    gpio_initStruct.Pin     = gs_bsp_nfc07a1_led_pin[led];
    gpio_initStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    gpio_initStruct.Pull    = GPIO_PULLUP;
    gpio_initStruct.Speed   = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(gs_bsp_nfc07a1_led_port[led], &gpio_initStruct);

    HAL_GPIO_WritePin(gs_bsp_nfc07a1_led_port[led], gs_bsp_nfc07a1_led_pin[led], GPIO_PIN_RESET);   
}

extern void bsp_nfc07a1_led_deinit(E_BSP_NFC07A1_LED led)
{
    /* Turn off LED */
    HAL_GPIO_WritePin(gs_bsp_nfc07a1_led_port[led], gs_bsp_nfc07a1_led_pin[led], GPIO_PIN_RESET);

    /* Deinit the LED GPIO pin */
    HAL_GPIO_DeInit(gs_bsp_nfc07a1_led_port[led], gs_bsp_nfc07a1_led_pin[led]);
}

extern void bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED led)
{
    HAL_GPIO_WritePin(gs_bsp_nfc07a1_led_port[led], gs_bsp_nfc07a1_led_pin[led], GPIO_PIN_SET);
}

extern void bsp_nfc07a1_led_off(E_BSP_NFC07A1_LED led)
{
    HAL_GPIO_WritePin(gs_bsp_nfc07a1_led_port[led], gs_bsp_nfc07a1_led_pin[led], GPIO_PIN_RESET);
}

extern void bsp_nfc07a1_led_toggle(E_BSP_NFC07A1_LED led)
{
    HAL_GPIO_TogglePin(gs_bsp_nfc07a1_led_port[led], gs_bsp_nfc07a1_led_pin[led]);
}

extern void bsp_nfc07a1_gpo_init(void)
{
    GPIO_InitTypeDef gpio_initStruct = {0};

    /* Enable the GPO GPIO Clock */
    D_BSP_NFC07A1_GPO_GPIO_CLK_ENABLE();

    /* Configure the GPO GPIO pin */
    gpio_initStruct.Pin     = D_BSP_NFC07A1_GPO_GPIO_PIN;
    gpio_initStruct.Mode    = GPIO_MODE_IT_RISING;
    gpio_initStruct.Pull    = GPIO_NOPULL;
    gpio_initStruct.Speed   = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(D_BSP_NFC07A1_GPO_GPIO_PORT, &gpio_initStruct);

    HAL_NVIC_SetPriority(D_BSP_NFC07A1_GPO_EXTI_NUM, D_BSP_NFC07A1_GPO_EXTI_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(D_BSP_NFC07A1_GPO_EXTI_NUM);
}

extern void bsp_nfc07a1_gpo_activeFlag_reset(void)
{
    gs_bsp_nfc07a1_handle.gpo_activeFlag = 0;
}

extern void bsp_nfc07a1_gpo_activeFlag_check(uint8_t *p_flag)
{
    *p_flag = gs_bsp_nfc07a1_handle.gpo_activeFlag;
}

extern void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    GPIO_InitTypeDef 			gpio_initStruct 		=	{0};
    RCC_PeriphCLKInitTypeDef 	periphClk_initStruct 	=	{0};
    HAL_StatusTypeDef			hal_ret					=	HAL_OK;

    if (hi2c->Instance == gs_bsp_nfc07a1_handle.i2c_handle.Instance)
    {
    	/* Configure I2C1 Clock */
    	periphClk_initStruct.PeriphClockSelection 	=	RCC_PERIPHCLK_I2C1;
    	periphClk_initStruct.I2c1ClockSelection 	=	RCC_I2C1CLKSOURCE_SYSCLK;
        hal_ret = HAL_RCCEx_PeriphCLKConfig(&periphClk_initStruct);
    	if (hal_ret != HAL_OK)
        {
        	(void)hal_ret;
        }

        /* GPIO Clock enable */
        D_BSP_NFC07A1_I2C_SCL_GPIO_CLK_ENABLE();
        D_BSP_NFC07A1_I2C_SCA_GPIO_CLK_ENABLE(); 
        
        /**
         * NFC Tag I2C GPIO Configuration 
         * PB8 ------> I2C1_SCL
         * PB9 ------> I2C1_SDA   
         */
        gpio_initStruct.Pin = D_BSP_NFC07A1_I2C_SCL_GPIO_PIN;
        gpio_initStruct.Mode = GPIO_MODE_AF_OD;
        gpio_initStruct.Pull = GPIO_PULLUP;
        gpio_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_initStruct.Alternate = D_BSP_NFC07A1_I2C_SCL_GPIO_AF;
        HAL_GPIO_Init(D_BSP_NFC07A1_I2C_SCL_GPIO_PORT, &gpio_initStruct); 

        gpio_initStruct.Pin = D_BSP_NFC07A1_I2C_SCA_GPIO_PIN;
        gpio_initStruct.Mode = GPIO_MODE_AF_OD;
        gpio_initStruct.Pull = GPIO_PULLUP;
        gpio_initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_initStruct.Alternate = D_BSP_NFC07A1_I2C_SCA_GPIO_AF;
        HAL_GPIO_Init(D_BSP_NFC07A1_I2C_SCA_GPIO_PORT, &gpio_initStruct);

        /* I2C1 Clock enable */
    	D_BSP_NFC07A1_I2C_CLK_ENABLE();
    }
}

extern void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
    if (hi2c->Instance == gs_bsp_nfc07a1_handle.i2c_handle.Instance)
    {
        /* Peripheral Clock disable */
        D_BSP_NFC07A1_I2C_CLK_DISABLE();
        D_BSP_NFC07A1_I2C_SCL_GPIO_CLK_DISABLE();
        D_BSP_NFC07A1_I2C_SCA_GPIO_CLK_DISABLE(); 

        /**
         * NFC Tag I2C GPIO Configuration 
         * PB8 ------> I2C1_SCL
         * PB9 ------> I2C1_SDA   
         */
        HAL_GPIO_DeInit(D_BSP_NFC07A1_I2C_SCL_GPIO_PORT, D_BSP_NFC07A1_I2C_SCL_GPIO_PIN);
        HAL_GPIO_DeInit(D_BSP_NFC07A1_I2C_SCA_GPIO_PORT, D_BSP_NFC07A1_I2C_SCA_GPIO_PIN);
    }
}

extern void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(D_BSP_NFC07A1_GPO_GPIO_PIN);
}

extern void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (D_BSP_NFC07A1_GPO_GPIO_PIN == GPIO_Pin)
    {
        gs_bsp_nfc07a1_handle.gpo_activeFlag = 1;
    }
}


/**************************************************************
** Function
**************************************************************/

static int32_t bsp_nfc07a1_nfctag_i2C_init(void)
{
    int32_t ret = D_BSP_RET_OK;

    do
    {
        I2C_HandleTypeDef* hi2c = &(gs_bsp_nfc07a1_handle.i2c_handle);

        /* I2C Initialization */
        hi2c->Instance               = D_BSP_NFC07A1_I2C_INSTANCE;                 
        hi2c->Init.Timing            = 0x10B07CB4;
        hi2c->Init.OwnAddress1       = 0;                
        hi2c->Init.AddressingMode    = I2C_ADDRESSINGMODE_7BIT;
        hi2c->Init.DualAddressMode   = I2C_DUALADDRESS_DISABLE;
        hi2c->Init.OwnAddress2       = 0;
        hi2c->Init.OwnAddress2Masks  = I2C_OA2_NOMASK;
        hi2c->Init.GeneralCallMode   = I2C_GENERALCALL_DISABLE;
        hi2c->Init.NoStretchMode     = I2C_NOSTRETCH_DISABLE;
        if (HAL_OK != HAL_I2C_Init(hi2c) )
        {
            ret = D_BSP_RET_HAL_ERR;
            break;
        }

        /* Configure Analogue Filter */
        if (HAL_OK != HAL_I2CEx_ConfigAnalogFilter(hi2c, I2C_ANALOGFILTER_ENABLE) )
        {
            ret = D_BSP_RET_HAL_BUSY;
            break;
        }

        /* Configure Digital Filter */
        if (HAL_OK != HAL_I2CEx_ConfigDigitalFilter(hi2c, 2) )
        {
            ret = D_BSP_RET_HAL_BUSY;
            break;
        }

        ret = D_BSP_RET_OK;
    } while (0);    

    return ret;
}

static int32_t bsp_nfc07a1_nfctag_i2c_deInit(void)
{
    int32_t ret = D_BSP_RET_OK;

    do
    {
        I2C_HandleTypeDef* hi2c = &(gs_bsp_nfc07a1_handle.i2c_handle);

        /* I2C DeInitialization */
        if (HAL_OK != HAL_I2C_DeInit(hi2c) )
        {
            ret = D_BSP_RET_HAL_ERR;
            break;
        }

        ret = D_BSP_RET_OK;
    } while (0);
    
    return ret;
}

static int32_t bsp_nfc07a1_nfctag_i2c_isReady(uint16_t dev_addr, uint32_t trials)
{
    int32_t ret = D_BSP_RET_OK;

    HAL_StatusTypeDef hal_ret = HAL_OK;
    
    do
    {
        I2C_HandleTypeDef* hi2c = &(gs_bsp_nfc07a1_handle.i2c_handle);

        hal_ret = HAL_I2C_IsDeviceReady(hi2c, dev_addr, trials, D_BSP_I2C_POLL_TIMEOUT);
        if (HAL_OK != hal_ret)
        {
            if (HAL_BUSY == hal_ret)
            {
                ret = D_BSP_RET_HAL_BUSY;
            }
            else if (HAL_ERROR == hal_ret)
            {
                ret = D_BSP_RET_HAL_ERR;
            }
            else{}
        }

        ret = D_BSP_RET_OK;
    } while (0);

    return ret;
}

// static int32_t bsp_nfc07a1_nfctag_i2c_reg_write(uint16_t dev_addr, uint16_t reg, uint8_t *p_data, uint16_t length)
// {
//     int32_t ret = D_BSP_RET_OK;

//     HAL_StatusTypeDef   hal_ret         = HAL_OK;
//     int32_t             hal_i2c_errCode = HAL_I2C_ERROR_NONE;

//     do
//     {
//         I2C_HandleTypeDef* hi2c = &(gs_bsp_nfc07a1_handle.i2c_handle);

//         hal_ret = HAL_I2C_Mem_Write(hi2c, dev_addr, reg, I2C_MEMADD_SIZE_8BIT, p_data, length, D_BSP_I2C_POLL_TIMEOUT);
//         if (HAL_OK != hal_ret)
//         {
//             if (HAL_ERROR == hal_ret)
//             {
//                 /* Get the error code */
//                 hal_i2c_errCode = HAL_I2C_GetError(hi2c);
//                 (void)hal_i2c_errCode;
                
//                 ret = D_BSP_RET_HAL_ERR;
//                 break;
//             }
//             else if (HAL_BUSY == hal_ret)
//             {
//                 ret = D_BSP_RET_HAL_BUSY;
//                 break;
//             }
//             else{}
//         }

//         ret = D_BSP_RET_OK;
//     } while (0);

//     return ret;
// }

// static int32_t bsp_nfc07a1_nfctag_i2c_reg_read(uint16_t dev_addr, uint16_t reg, uint8_t *p_data, uint16_t length)
// {
//     int32_t ret = D_BSP_RET_OK;

//     HAL_StatusTypeDef   hal_ret         = HAL_OK;
//     int32_t             hal_i2c_errCode = HAL_I2C_ERROR_NONE;

//     do
//     {
//         I2C_HandleTypeDef* hi2c = &(gs_bsp_nfc07a1_handle.i2c_handle);

//         hal_ret = HAL_I2C_Mem_Read(hi2c, dev_addr, reg, I2C_MEMADD_SIZE_8BIT, p_data, length, D_BSP_I2C_POLL_TIMEOUT);
//         if (HAL_OK != hal_ret)
//         {
//             if (HAL_ERROR == hal_ret)
//             {
//                 /* Get the error code */
//                 hal_i2c_errCode = HAL_I2C_GetError(hi2c);
//                 (void)hal_i2c_errCode;
                
//                 ret = D_BSP_RET_HAL_ERR;
//                 break;
//             }
//             else if (HAL_BUSY == hal_ret)
//             {
//                 ret = D_BSP_RET_HAL_BUSY;
//                 break;
//             }
//             else{}
//         }

//         ret = D_BSP_RET_OK;
//     } while (0);

//     return ret;
// }

static int32_t bsp_nfc07a1_nfctag_i2c_reg16_write(uint16_t dev_addr, uint16_t reg, uint8_t *p_data, uint16_t length)
{
    int32_t ret = D_BSP_RET_OK;

    HAL_StatusTypeDef   hal_ret         = HAL_OK;
    int32_t             hal_i2c_errCode = HAL_I2C_ERROR_NONE;

    do
    {
        I2C_HandleTypeDef* hi2c = &(gs_bsp_nfc07a1_handle.i2c_handle);

        hal_ret = HAL_I2C_Mem_Write(hi2c, dev_addr, reg, I2C_MEMADD_SIZE_16BIT, p_data, length, D_BSP_I2C_POLL_TIMEOUT);
        if (HAL_OK != hal_ret)
        {
            if (HAL_ERROR == hal_ret)
            {
                /* Get the error code */
                hal_i2c_errCode = HAL_I2C_GetError(hi2c);
                (void)hal_i2c_errCode;
                
                ret = D_BSP_RET_HAL_ERR;
                break;
            }
            else if (HAL_BUSY == hal_ret)
            {
                ret = D_BSP_RET_HAL_BUSY;
                break;
            }
            else{}
        }

        ret = D_BSP_RET_OK;
    } while (0);

    return ret;
}

static int32_t bsp_nfc07a1_nfctag_i2c_reg16_read(uint16_t dev_addr, uint16_t reg, uint8_t *p_data, uint16_t length)
{
    int32_t ret = D_BSP_RET_OK;

    HAL_StatusTypeDef   hal_ret         = HAL_OK;
    int32_t             hal_i2c_errCode = HAL_I2C_ERROR_NONE;

    do
    {
        I2C_HandleTypeDef* hi2c = &(gs_bsp_nfc07a1_handle.i2c_handle);

        hal_ret = HAL_I2C_Mem_Read(hi2c, dev_addr, reg, I2C_MEMADD_SIZE_16BIT, p_data, length, D_BSP_I2C_POLL_TIMEOUT);
        if (HAL_OK != hal_ret)
        {
            if (HAL_ERROR == hal_ret)
            {
                /* Get the error code */
                hal_i2c_errCode = HAL_I2C_GetError(hi2c);
                (void)hal_i2c_errCode;
                
                ret = D_BSP_RET_HAL_ERR;
                break;
            }
            else if (HAL_BUSY == hal_ret)
            {
                ret = D_BSP_RET_HAL_BUSY;
                break;
            }
            else{}
        }

        ret = D_BSP_RET_OK;
    } while (0);

    return ret;
}

// static int32_t bsp_nfc07a1_nfctag_i2c_transmit(uint16_t dev_addr, uint8_t *p_data, uint16_t length)
// {
//     int32_t ret = D_BSP_RET_OK;

//     HAL_StatusTypeDef   hal_ret         = HAL_OK;
//     int32_t             hal_i2c_errCode = HAL_I2C_ERROR_NONE;

//     do
//     {
//         I2C_HandleTypeDef* hi2c = &(gs_bsp_nfc07a1_handle.i2c_handle);

//         hal_ret = HAL_I2C_Master_Transmit(hi2c, dev_addr, p_data, length, D_BSP_I2C_POLL_TIMEOUT);
//         if (HAL_OK != hal_ret)
//         {
//             if (HAL_ERROR == hal_ret)
//             {
//                 /* Get the error code */
//                 hal_i2c_errCode = HAL_I2C_GetError(hi2c);
//                 (void)hal_i2c_errCode;

//                 ret = D_BSP_RET_HAL_ERR;
//                 break;
//             }
//             else if (HAL_BUSY == hal_ret)
//             {
//                 ret = D_BSP_RET_HAL_BUSY;
//                 break;
//             }
//             else{}
//         }

//         ret = D_BSP_RET_OK;
//     } while (0);

//     return ret;
// }

// static int32_t bsp_nfc07a1_nfctag_i2c_receive(uint16_t dev_addr, uint8_t *p_data, uint16_t length)
// {
//     int32_t ret = D_BSP_RET_OK;

//     HAL_StatusTypeDef   hal_ret         = HAL_OK;
//     int32_t             hal_i2c_errCode = HAL_I2C_ERROR_NONE;

//     do
//     {
//         I2C_HandleTypeDef* hi2c = &(gs_bsp_nfc07a1_handle.i2c_handle);

//         hal_ret = HAL_I2C_Master_Receive(hi2c, dev_addr, p_data, length, D_BSP_I2C_POLL_TIMEOUT);
//         if (HAL_OK != hal_ret)
//         {
//             if (HAL_ERROR == hal_ret)
//             {
//                 /* Get the error code */
//                 hal_i2c_errCode = HAL_I2C_GetError(hi2c);
//                 (void)hal_i2c_errCode;
                
//                 ret = D_BSP_RET_HAL_ERR;
//                 break;
//             }
//             else if (HAL_BUSY == hal_ret)
//             {
//                 ret = D_BSP_RET_HAL_BUSY;
//                 break;
//             }
//             else{}
//         }

//         ret = D_BSP_RET_OK;
//     } while (0);

//     return ret;
// }

static int32_t bsp_nfc07a1_nfctag_tick_get(void)
{
    int32_t tick = (int32_t)HAL_GetTick();
    return tick;
}
