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

typedef enum
{
    E_BSP_NFC07A1_NFCTAG_I2C_SS_STATUS_CLOSED = 0,
    E_BSP_NFC07A1_NFCTAG_I2C_SS_STATUS_OPEN

} E_BSP_NFC07A1_NFCTAG_I2C_SS_STATUS_T;

typedef enum
{
    E_BSP_NFC07A1_NFCTAG_PROT_ZONE_1 = ST25DVXXKC_PROT_ZONE1,
    E_BSP_NFC07A1_NFCTAG_PROT_ZONE_2 = ST25DVXXKC_PROT_ZONE2,
    E_BSP_NFC07A1_NFCTAG_PROT_ZONE_3 = ST25DVXXKC_PROT_ZONE3,
    E_BSP_NFC07A1_NFCTAG_PROT_ZONE_4 = ST25DVXXKC_PROT_ZONE4,
	E_BSP_NFC07A1_NFCTAG_PROT_ZONE_ERROR

} E_BSP_NFC07A1_NFCTAG_PROT_ZONE_T;

typedef enum
{
    E_BSP_NFC07a1_NFCTAG_PROT_CONF_NO_PROT          =   ST25DVXXKC_NO_PROT,
    E_BSP_NFC07A1_NFCTAG_PROT_CONF_WRITE_PROT       =   ST25DVXXKC_WRITE_PROT,
    E_BSP_NFC07A1_NFCTAG_PROT_CONF_READ_PROT        =   ST25DVXXKC_READ_PROT,
    E_BSP_NFC07A1_NFCTAG_PROT_CONF_READWRITE_PROT   =   ST25DVXXKC_READWRITE_PROT,
	E_BSP_NFC07A1_NFCTAG_PROT_CONF_ERROR

} E_BSP_NFC07A1_NFCTAG_PROT_CONF_T;

typedef enum
{
	E_BSP_NFC07A1_NFCTAG_ZONE_END_1 = ST25DVXXKC_ZONE_END1,
	E_BSP_NFC07A1_NFCTAG_ZONE_END_2,
	E_BSP_NFC07A1_NFCTAG_ZONE_END_3,
	E_BSP_NFC07A1_NFCTAG_ZONE_END_ERROR
    
} E_BSP_NFC07A1_NFCTAG_ZONE_END_T;

typedef struct
{
  uint8_t block_size;
  uint16_t block_num;
} S_BSP_NFC07A1_NFCTAG_MEM_SIZE_T;


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
extern int32_t bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get(E_BSP_NFC07A1_NFCTAG_I2C_SS_STATUS_T* const p_ss_status);
extern int32_t bsp_nfc07a1_nfctag_i2cUsrZone_create(const uint16_t zone1_len, const uint16_t zone2_len, const uint16_t zone3_len, const uint16_t zone4_len);
extern int32_t bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set(const E_BSP_NFC07A1_NFCTAG_PROT_ZONE_T zone, const E_BSP_NFC07A1_NFCTAG_PROT_CONF_T prot_conf);
extern int32_t bsp_nfc07a1_nfctag_i2cUsrZoneProtect_get(const E_BSP_NFC07A1_NFCTAG_PROT_ZONE_T zone, E_BSP_NFC07A1_NFCTAG_PROT_CONF_T* const prot_conf);
extern int32_t bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get(const E_BSP_NFC07A1_NFCTAG_ZONE_END_T zone, uint8_t* const p_zone_end);
extern int32_t bsp_nfc07a1_nfctag_i2cUsrMemSize_get(uint32_t* const blk_num, uint32_t* const blk_size);
extern int32_t bsp_nfc07a1_nfctag_mailboxCtrlDyn_reset();
extern int32_t bsp_nfc07a1_nfctag_mailboxCtrlDyn_read(uint8_t* const status);
extern int32_t bsp_nfc07a1_nfctag_lockCCFile_read(uint8_t* const lock0_status, uint8_t* const lock1_status);

extern void bsp_nfc07a1_led_init(E_BSP_NFC07A1_LED led);
extern void bsp_nfc07a1_led_deInit(E_BSP_NFC07A1_LED led);
extern void bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED led);
extern void bsp_nfc07a1_led_off(E_BSP_NFC07A1_LED led);
extern void bsp_nfc07a1_led_toggle(E_BSP_NFC07A1_LED led);
extern void bsp_nfc07a1_led_delay_ms(uint32_t delay);

extern void bsp_nfc07a1_gpo_init(void);
extern void bsp_nfc07a1_gpo_deInit(void);
extern void bsp_nfc07a1_gpo_activeFlag_reset(void);
extern void bsp_nfc07a1_gpo_activeFlag_check(uint8_t *p_flag);

#ifdef __cplusplus
}
#endif

#endif
