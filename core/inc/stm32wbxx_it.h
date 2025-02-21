#ifndef __STM32WBXX_IT_H
#define __STM32WBXX_IT_H

#ifdef __cplusplus
 extern "C" {
#endif


/**************************************************************
**  Interface
**************************************************************/

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void IPCC_C1_TX_IRQHandler(void);
void IPCC_C1_RX_IRQHandler(void);


#endif /* __STM32WBXX_IT_H */
