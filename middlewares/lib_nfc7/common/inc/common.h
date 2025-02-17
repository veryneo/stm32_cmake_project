/**
  ******************************************************************************
  * @file    common_h.ftl
  * @author  SRA Application Team
  * @version 1.0.1
  * @date    27-Apr-2023
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/
typedef uint8_t boolean;

/**
 * @brief  GPO status information structure definition
 */
typedef struct
{
  uint8_t WritenEEPROM;
  uint8_t RfBusy;
  uint8_t FieldOn;
  uint8_t FieldOff;
  uint8_t MsgInMailbox;
  uint8_t MailboxMsgRead;
  uint8_t RfInterrupt;
  uint8_t Rfuser;
} IT_GPO_STATUS;

/* Exported macro ------------------------------------------------------------*/
#define NFCTAG_4K_SIZE            ((uint32_t) 0x200)
#define NFCTAG_16K_SIZE           ((uint32_t) 0x800)
#define NFCTAG_64K_SIZE           ((uint32_t) 0x2000)

#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#undef FAIL
#define FAIL 0

#undef PASS
#define PASS !FAIL

/* Exported constants --------------------------------------------------------*/
#define MAX_NDEF_MEM                    0x200
#define ST25DVXXKC_MAX_SIZE             NFCTAG_4K_SIZE
#define ST25DVXXKC_NDEF_MAX_SIZE        MIN(ST25DVXXKC_MAX_SIZE,MAX_NDEF_MEM)
#define NFC_DEVICE_MAX_NDEFMEMORY       ST25DVXXKC_NDEF_MAX_SIZE

#define RESULTOK                     0x00
#define ERRORCODE_GENERIC            1

/* Exported functions ------------------------------------------------------- */

#endif /* __COMMON_H */

