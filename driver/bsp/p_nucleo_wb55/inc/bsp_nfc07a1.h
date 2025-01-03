#ifndef __BSP_NFCTAG_H
#define __BSP_NFCTAG_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************
**  Includes
**************************************************************/

#include "bsp_errno.h"

#include "stdint.h"

#include "st25dvxxkc.h"
#include "st25dvxxkc_reg.h"


/**************************************************************
**  Symbol
**************************************************************/

#define D_BSP_NFC07A1_NFCTAG_GPO1_ENABLE_MASK            (ST25DVXXKC_GPO1_ENABLE_MASK)
#define D_BSP_NFC07A1_NFCTAG_GPO1_FILEDCHQNGE_MASK       (ST25DVXXKC_GPO1_FIELDCHANGE_MASK)


/**************************************************************
**  Enumerations
**************************************************************/

typedef enum 
{
    E_BSP_NFC07A1_LED1  =   0,
    E_BSP_NFC07A1_LED2  =   1,
    E_BSP_NFC07A1_LED3  =   2,
    /* Color LED aliases */
    E_BSP_NFC07A1_LED_GREEN  = E_BSP_NFC07A1_LED1,
    E_BSP_NFC07A1_LED_BLUE   = E_BSP_NFC07A1_LED2,
    E_BSP_NFC07A1_LED_YELLOW = E_BSP_NFC07A1_LED3
} E_BSP_NFC07A1_LED;


/**************************************************************
**  Interface
**************************************************************/

extern int32_t bsp_nfc07a1_nfctag_init(void);
extern int32_t bsp_nfc07a1_nfctag_deInit(void);
extern void bsp_nfc07a1_nfctag_isInitialized(uint32_t* const p_isInitialized);
extern int32_t bsp_nfc07a1_nfctag_id_read(uint8_t* const wai_id);
extern int32_t bsp_nfc07a1_nfctag_it_configure(const uint16_t it_conf);
extern int32_t bsp_nfc07a1_nfctag_it_status_get(uint16_t* const it_status);
extern int32_t bsp_nfc07a1_nfctag_data_read(uint8_t* const p_data, const uint16_t tar_addr, const uint16_t size);
extern int32_t bsp_nfc07a1_nfctag_data_write(const uint8_t* const p_data, const uint16_t tar_addr, const uint16_t size);
extern int32_t bsp_nfc07a1_nfctag_reg_read(uint8_t* const p_data, const uint16_t tar_addr, const uint16_t size);
extern int32_t bsp_nfc07a1_nfctag_reg_write(const uint8_t* const p_data, const uint16_t tar_addr, const uint16_t size);
extern int32_t bsp_nfc07a1_nfctag_device_isReady(const uint32_t trials);
extern int32_t bsp_nfc07a1_nfctag_i2cPassword_present(const uint32_t password_msb, const uint32_t password_lsb);

extern void bsp_nfc07a1_led_init(E_BSP_NFC07A1_LED led);
extern void bsp_nfc07a1_led_deInit(E_BSP_NFC07A1_LED led);
extern void bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED led);
extern void bsp_nfc07a1_led_off(E_BSP_NFC07A1_LED led);
extern void bsp_nfc07a1_led_toggle(E_BSP_NFC07A1_LED led);

extern void bsp_nfc07a1_gpo_init(void);
extern void bsp_nfc07a1_gpo_deInit(void);
extern void bsp_nfc07a1_gpo_activeFlag_reset(void);
extern void bsp_nfc07a1_gpo_activeFlag_check(uint8_t *p_flag);

#ifdef __cplusplus
}
#endif

#endif