/**************************************************************
**  Include
**************************************************************/

#include "app_demo.h"

#include "stm32wbxx_hal.h"

#include "otp.h"


/**************************************************************
**  Function Prototype
**************************************************************/

static void optvErrFlag_clear(void);
static void backup_domain_reset(void);
static void ipcc_reset(void);
static void hseClk_tune(void);
static void systemClock_config(void);
static void periphCommonClock_config(void);


/**************************************************************
**  Main
**************************************************************/

int main(void)
{
    HAL_Init();

    optvErrFlag_clear();

    backup_domain_reset();

    ipcc_reset();

    hseClk_tune();

    systemClock_config();

	periphCommonClock_config();

    app_demo();
}


/**************************************************************
**  Function
**************************************************************/

static void optvErrFlag_clear()
{
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
}

static void backup_domain_reset(void)
{
    /* Reset Backup domain (If reset from PIN other than software) */
    if ( (0 != LL_RCC_IsActiveFlag_PINRST() ) && (0 == LL_RCC_IsActiveFlag_SFTRST() ) )
    {
        HAL_PWR_EnableBkUpAccess(); /**< Enable access to the RTC registers */

        /**
         *  Write twice the value to flush the APB-AHB bridge
         *  This bit shall be written in the register before writing the next one
         */
        HAL_PWR_EnableBkUpAccess();

        __HAL_RCC_BACKUPRESET_FORCE();
        __HAL_RCC_BACKUPRESET_RELEASE();
    }
        
    return;
}

static void ipcc_reset(void)
{
    LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_IPCC);
    
    LL_C1_IPCC_ClearFlag_CHx(
        IPCC,
        LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3   |
        LL_IPCC_CHANNEL_4 | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6   );
        
    LL_C2_IPCC_ClearFlag_CHx(
        IPCC,
        LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3   | 
        LL_IPCC_CHANNEL_4 | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6   );
        
    LL_C1_IPCC_DisableTransmitChannel(
        IPCC,
        LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3   | 
        LL_IPCC_CHANNEL_4 | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6   );
        
    LL_C2_IPCC_DisableTransmitChannel(
        IPCC,
        LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3   | 
        LL_IPCC_CHANNEL_4 | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6   );
        
    LL_C1_IPCC_DisableReceiveChannel(
        IPCC,
        LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3   | 
        LL_IPCC_CHANNEL_4 | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6   );
        
    LL_C2_IPCC_DisableReceiveChannel(
        IPCC,
        LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3   | 
        LL_IPCC_CHANNEL_4 | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6   );
}


static void hseClk_tune(void)
{
    OTP_ID0_t * p_otp;

    p_otp = (OTP_ID0_t *) OTP_Read(0);
    if (p_otp)
    {
      LL_RCC_HSE_SetCapacitorTuning(p_otp->hse_tuning);
    }
    
    return;
}

static void systemClock_config(void)
{
    RCC_OscInitTypeDef rcc_osc_InitStruct = {0};
    RCC_ClkInitTypeDef rcc_clk_InitStruct = {0};

    /** Configure LSE Drive Capability */
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /* Configure the main internal regulator output voltage */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    rcc_osc_InitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_MSI|RCC_OSCILLATORTYPE_LSE;
    rcc_osc_InitStruct.HSIState = RCC_HSI_ON;
    rcc_osc_InitStruct.MSIState = RCC_MSI_ON;
    rcc_osc_InitStruct.LSEState = RCC_LSE_ON;
    rcc_osc_InitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    rcc_osc_InitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    rcc_osc_InitStruct.MSIClockRange = RCC_MSIRANGE_10;
    rcc_osc_InitStruct.PLL.PLLState = RCC_PLL_ON;
    rcc_osc_InitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    rcc_osc_InitStruct.PLL.PLLM = RCC_PLLM_DIV2;
    rcc_osc_InitStruct.PLL.PLLN = 8;
    rcc_osc_InitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    rcc_osc_InitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    rcc_osc_InitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    if (HAL_RCC_OscConfig(&rcc_osc_InitStruct) != HAL_OK)
    {

    }

    /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
    */
    rcc_clk_InitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                                |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    rcc_clk_InitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    rcc_clk_InitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    rcc_clk_InitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    rcc_clk_InitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    rcc_clk_InitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
    rcc_clk_InitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

    if (HAL_RCC_ClockConfig(&rcc_clk_InitStruct, FLASH_LATENCY_3) != HAL_OK)
    {

    }
}

static void periphCommonClock_config(void)
{
	RCC_PeriphCLKInitTypeDef periph_clk_initStruct = {0};

	/** Initializes the peripherals clock
	*/
	periph_clk_initStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS | RCC_PERIPHCLK_RFWAKEUP;
	periph_clk_initStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
	periph_clk_initStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE0;
    periph_clk_initStruct.RFWakeUpClockSelection = RCC_RFWKPCLKSOURCE_LSE;

	if (HAL_RCCEx_PeriphCLKConfig(&periph_clk_initStruct) != HAL_OK)
	{

	}
}

