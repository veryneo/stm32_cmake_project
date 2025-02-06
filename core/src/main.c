/**************************************************************
**  Include
**************************************************************/

#include "app_demo.h"

#include "stm32wbxx_hal.h"


/**************************************************************
**  Function Prototype
**************************************************************/

static void systemClock_config(void);
static void periphCommonClock_config(void);

/**************************************************************
**  Main
**************************************************************/

int main(void)
{
    HAL_Init();

	systemClock_config();

	periphCommonClock_config();

    app_demo();
}


/**************************************************************
**  Function
**************************************************************/

/**
 *  @brief 		System Clock Configuration
 *  @retval		None
 *  @author		chenwei.gu@murata.com
 */
static void systemClock_config(void)
{
    RCC_OscInitTypeDef rcc_osc_InitStruct = {0};
    RCC_ClkInitTypeDef rcc_clk_InitStruct = {0};

    /* Configure the main internal regulator output voltage */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    rcc_osc_InitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_MSI;
    rcc_osc_InitStruct.HSIState = RCC_HSI_ON;
    rcc_osc_InitStruct.MSIState = RCC_MSI_ON;
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
	periph_clk_initStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS;
	periph_clk_initStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
	periph_clk_initStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE0;

	if (HAL_RCCEx_PeriphCLKConfig(&periph_clk_initStruct) != HAL_OK)
	{

	}
}
