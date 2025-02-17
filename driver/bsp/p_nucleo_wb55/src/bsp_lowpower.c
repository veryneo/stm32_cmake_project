#include "bsp_lowpower.h"

#include "stm32wbxx_hal.h"


#define D_BSP_LOWPOWER_WAKEUP_PIN       PWR_WAKEUP_PIN1


extern void bsp_lowpower_mode_standby_enter()
{
    /* Wakeup Pin PA0 */
    HAL_PWR_EnableWakeUpPin(D_BSP_LOWPOWER_WAKEUP_PIN);

    /* Set low-power mode of CPU2 */
    LL_C2_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);

    /* Clear all related wakeup flags*/
    if( 	(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)		&&
    		(__HAL_PWR_GET_FLAG(PWR_FLAG_C2SB) != RESET)	)
    {
    	/* Clear Standby flag */
    	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_C2SB);
    }

    /* Clear all related wakeup flags */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    if( 	(LL_PWR_IsActiveFlag_C1SB() == 0)	||
    		(LL_PWR_IsActiveFlag_C2SB() == 0)	)
    {
      /* Set the lowest low-power mode for CPU2: shutdown mode */
      LL_C2_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);
    }

    HAL_PWR_EnterSTANDBYMode();
}

extern int32_t bsp_lowpower_mode_standby_resume_check(uint8_t* const p_flag)
{
    if (p_flag == NULL)
    {
        return D_BSP_RET_INPUT_ERR;
    }

    int32_t ret = D_BSP_RET_OK;
    
    do
    {
        if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
        {
            /* Clear Standby flag */
            __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    
            /* Check and Clear the Wakeup flag */
            if (__HAL_PWR_GET_FLAG(PWR_FLAG_WU) != RESET)
            {
                __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
            }

            *p_flag = 1;
        }
        else
        {
            *p_flag = 0;
        }

        ret = D_BSP_RET_OK;
    } while (0);
    

    return ret;
}
