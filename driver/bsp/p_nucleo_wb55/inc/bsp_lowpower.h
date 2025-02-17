#ifndef __BSP_LOWPOWER_H
#define __BSP_LOWPOWER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_errno.h"

#include "stdint.h"

extern void bsp_lowpower_mode_standby_enter();
extern int32_t bsp_lowpower_mode_standby_resume_check(uint8_t* const p_flag);

#ifdef __cplusplus
}
#endif

#endif