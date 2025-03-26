#ifndef STM32WBXX_IT_H
#define STM32WBXX_IT_H

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


#endif /* __STM32WBXX_IT_H */