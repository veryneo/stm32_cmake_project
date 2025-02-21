
/**************************************************************
**  Include
**************************************************************/

#include "stm32wbxx_it.h"

#include "stm32wbxx_hal.h"

#include "app_conf.h"


/**************************************************************
**  Interface
**************************************************************/

/**************************************************************
**  Cortex Processor Interruption and Exception Handlers
**************************************************************/

/**
 *  @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{

}

/**
 *  @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    while (1)
    {

    }
}

/**
 *  @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
    while (1)
    {

    }
}

/**
 *  @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{

}

/**
 *  @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{

}

/**
 *  @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{

}

/**
 *  @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

void IPCC_C1_TX_IRQHandler(void)
{
	HW_IPCC_Tx_Handler();
  	  return;
}

void IPCC_C1_RX_IRQHandler(void)
{
	HW_IPCC_Rx_Handler();
	return;
}
