
/**************************************************************
**  Include
**************************************************************/

#include "bsp_com_it.h"

#include "stm32wbxx_hal.h"


/**************************************************************
**  Interface
**************************************************************/

/**
  * @brief This function handles Non maskable interrupt.
  */
extern void NMI_Handler(void)
{

}

/**
  * @brief This function handles Hard fault interrupt.
  */
extern void HardFault_Handler(void)
{
	while (1)
	{

	}
}

/**
  * @brief This function handles Memory management fault.
  */
extern void MemManage_Handler(void)
{
	while (1)
	{

	}
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
extern void BusFault_Handler(void)
{
	while (1)
	{

	}
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
extern void UsageFault_Handler(void)
{
	while (1)
	{

	}
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
extern void SVC_Handler(void)
{

}

/**
  * @brief This function handles Debug monitor.
  */
extern void DebugMon_Handler(void)
{

}

/**
  * @brief This function handles Pendable request for system service.
  */
extern void PendSV_Handler(void)
{

}

/**
  * @brief This function handles System tick timer.
  */
extern void SysTick_Handler(void)
{
	HAL_IncTick();
}
