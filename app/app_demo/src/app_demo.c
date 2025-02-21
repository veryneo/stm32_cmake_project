#include "app_demo.h"

#include "bsp.h"

#include "stdio.h"

#include "lib_NDEF_AAR.h"
#include "lib_NDEF_URI.h"
#include "tagtype5_wrapper.h"

#include "stm32_wpan_common.h"
#include "tl.h"
#include "shci.h"
#include "shci_tl.h"
#include "hci_tl.h"
#include "ble_defs.h"
#include "ble_types.h"
#include "ble_gap_aci.h"
#include "ble_gatt_aci.h"
#include "ble_vs_codes.h"
#include "ble_legacy.h"
#include "ble_std.h"
#include "ble_hci_le.h"
#include "ble_hal_aci.h"
#include "svc_ctl.h"
#include "app_conf.h"
#include "otp.h"

#define PLACE_IN_SECTION( __x__ )                               __attribute__((section (__x__)))
#define ALIGN(n)                                                __attribute__((aligned(n)))

#define CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE 255   /**< Set to 255 with the memory manager and the mailbox */

#define TL_BLE_EVENT_FRAME_SIZE ( TL_EVT_HDR_SIZE + CFG_TLBLE_MOST_EVENT_PAYLOAD_SIZE )

#define D_APP_DEMO_TL_MM_BLE_EVT_QUEUE_LEN                      (5)
#define D_APP_DEMO_TL_MM_POOL_SIZE                              (D_APP_DEMO_TL_MM_BLE_EVT_QUEUE_LEN*4U*DIVC((sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE ), 4U))
#define D_APP_DEMO_BLE_BDADDR_SIZE                              (6)
#define D_APP_DEMO_BLE_GAP_DEVICE_NAME_LEN                      (sizeof(gs_app_demo_ble_gap_device_name))

#define D_APP_DEMO_BLE_CUSTOM_SERVICE_UUID                      (0xFE40)
#define D_APP_DEMO_BLE_CUSTOM_WRITE_CHAR_UUID                   (0xFE41)
#define D_APP_DEMO_BLE_CUSTOM_NOTIFY_CHARA_UUID                 (0xFE42)

#define D_APP_DEMO_COPY_BLE_CUSTOM_SERVICE_UUID(uuid)           COPY_UUID_128(uuid,0x00,0x00,0xfe,0x40,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define D_APP_DEMO_COPY_BLE_CUSTOM_WRITE_CHAR_UUID(uuid)        COPY_UUID_128(uuid,0x00,0x00,0xfe,0x41,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define D_APP_DEMO_COPY_BLE_CUSTOM_NOTIFY_CHAR_UUID(uuid)       COPY_UUID_128(uuid,0x00,0x00,0xfe,0x42,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)

#define D_APP_DEMO_BLE_CUSTOM_SERVICE_MAX_ATT_RECORDS           (8)
#define D_APP_DEMO_BLE_CUSTOM_SERVICE_WRTIE_CHAR_VALUE_LEN      (2)
#define D_APP_DEMO_BLE_CUSTOM_SERVICE_NOTIFY_CHAR_VALUE_LEN     (2)
#define D_APP_DEMO_BLE_CUSTOM_SERVICE_CHAR_VALUE_ATTR_OFFSET    (1)

#define CFG_ADV_BD_ADDRESS                                      (0x7257acd87a6c)

#define D_APP_DEMO_BLE_FAST_CONN_ADV_INTERVAL_MIN               (0x00C8)  /**< 250ms */
#define D_APP_DEMO_BLE_FAST_CONN_ADV_INTERVAL_MAX               (0x00C8)  /**< 250ms */
#define D_APP_DEMO_BLE_LP_CONN_ADV_INTERVAL_MIN                 (0x0640)  /**< 1s */
#define D_APP_DEMO_BLE_LP_CONN_ADV_INTERVAL_MAX                 (0x0FA0)  /**< 2.5s */

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
  uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
    uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
      uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
        uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

#define D_APP_DEMO_NFCTAG_BLE_MAC_ADDR_STR_LEN					(6*2 + 5 + 1) /* 6*2 + 5 + 1. 6表示6个16进制数， 5表示5个冒号，1表示1个结束符号 */


typedef enum 
{
    E_APP_DEMO_CPU2_RUN_STATUS_NONE = 0,
    E_APP_DEMO_CPU2_RUN_STATUS_WIRELESS,
    E_APP_DEMO_CPU2_RUN_STATUS_FUS,
} E_APP_DEMO_CPU2_RUN_STATUS_T;

typedef enum
{
    E_APP_DEMO_SHCI_EVENT_PENDING_FLAG_RESET    =   0,
    E_APP_DEMO_SHCI_EVENT_PENDING_FLAG_SET      =   !E_APP_DEMO_SHCI_EVENT_PENDING_FLAG_RESET,
} E_APP_DEMO_SHCI_EVENT_PENDDING_FLAG_T;

typedef enum
{
    E_APP_DEMO_HCI_EVENT_PENDING_FLAG_RESET    =   0,
    E_APP_DEMO_HCI_EVENT_PENDING_FLAG_SET      =   !E_APP_DEMO_SHCI_EVENT_PENDING_FLAG_RESET,
} E_APP_DEMO_HCI_EVENT_PENDDING_FLAG_T;

typedef enum
{
	E_APP_DEMO_BLE_CONN_FLAG_RESET 				=	0,
	E_APP_DEMO_BLE_CONN_FLAG_SET				=	!E_APP_DEMO_BLE_CONN_FLAG_RESET
} E_APP_DEMO_BLE_CONN_FLAG_T;

typedef struct
{
    uint16_t service_handle;                        /* Service handle */
    uint16_t write_char_handle;                     /* Characteristic Write handle */
    uint16_t notify_char_handle;                    /* Characteristic Notify handle */
} S_APP_DEMO_BLE_CUSTOM_SERVICE_CTX_T;


PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t         gs_app_demo_tl_mm_evtpool[D_APP_DEMO_TL_MM_POOL_SIZE];
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static uint8_t         gs_app_demo_tl_mm_system_spare_evt_buf[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t         gs_app_demo_tl_mm_ble_spare_evt_buf[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t  gs_app_demo_shci_tl_cmd_buf;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t  gs_app_demo_hci_tl_cmd_buffer;

static uint8_t bd_address_udn[D_APP_DEMO_BLE_BDADDR_SIZE];

static const uint8_t a_MBdAddr[D_APP_DEMO_BLE_BDADDR_SIZE] =
{
    (uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000000000FF)),
    (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00000000FF00) >> 8),
    (uint8_t)((CFG_ADV_BD_ADDRESS & 0x000000FF0000) >> 16),
    (uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000FF000000) >> 24),
    (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00FF00000000) >> 32),
    (uint8_t)((CFG_ADV_BD_ADDRESS & 0xFF0000000000) >> 40)
};

static const char       gs_app_demo_ble_gap_device_name[]   =   { 'H', 'E', 'L', 'L', 'O', '!' };
static const uint8_t    gs_app_demo_ble_gap_appearance      =   0x00;
static const char       gs_app_demo_ble_ad_local_name[]     = { AD_TYPE_COMPLETE_LOCAL_NAME, 'H', 'E', 'L', 'L', 'O', '!' };
static uint8_t          gs_app_demo_ble_ad_manufacturer_specific_data[14] = { /* Manufacturer specific data used to get compliant with ST BLE Sensor smart phone apk */
                            sizeof(gs_app_demo_ble_ad_manufacturer_specific_data)-1,
                            AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 
                            0x01, /* BlueST Protocol version */
                            0x83, /* BlueST Device Id: 0x83 - P2PServer1 - for more details please see BLE_p2pServer example project */
                            0x00, /* BlueST Feature Mask bits 24~31 */
                            0x00, /* BlueST Feature Mask bits 16~23 */
                            0x00, /* BlueST Feature Mask bits 8~15 */
                            0x00, /* BlueST Feature Mask bits 0~7 */
                            0x00, /* BlueST Device MAC byte 5 */
                            0x00, /* BlueST Device MAC byte 4 */
                            0x00, /* BlueST Device MAC byte 3 */
                            0x00, /* BlueST Device MAC byte 2 */
                            0x00, /* BlueST Device MAC byte 1 */
                            0x00  /* BlueST Device MAC byte 0 */
};

static E_APP_DEMO_CPU2_RUN_STATUS_T             gs_app_demo_cpu2_run_status         =   E_APP_DEMO_CPU2_RUN_STATUS_NONE;
static E_APP_DEMO_SHCI_EVENT_PENDDING_FLAG_T    gs_app_demo_shci_event_pending_flag =   E_APP_DEMO_SHCI_EVENT_PENDING_FLAG_RESET;
static E_APP_DEMO_HCI_EVENT_PENDDING_FLAG_T     gs_app_demo_hci_event_pending_flag  =   E_APP_DEMO_HCI_EVENT_PENDING_FLAG_RESET;
static E_APP_DEMO_BLE_CONN_FLAG_T				gs_app_demo_ble_conn_flag			=	E_APP_DEMO_BLE_CONN_FLAG_RESET;
static uint8_t gs_app_demo_cpu2_err_flag = 0;

static S_APP_DEMO_BLE_CUSTOM_SERVICE_CTX_T gs_app_demo_ble_custom_service_ctx;

static uint8_t gs_app_demo_nfctag_ble_bd_addr[D_APP_DEMO_NFCTAG_BLE_MAC_ADDR_STR_LEN] = {0};

static void app_demo_cpu2_system_init();
static void app_demo_shci_tl_status_notification_callback(SHCI_TL_CmdStatus_t status);
static void app_demo_shci_user_event_received_callback(void* pData);

static void app_demo_ble_init();
static void app_demo_ble_tl_init();
static void app_demo_ble_gap_gatt_init();
static const uint8_t* app_demo_ble_bdaddress_get();
static void app_demo_hci_tl_status_notification_callback(HCI_TL_CmdStatus_t status);
static void app_demo_hci_user_event_receive_callback(void* pData);

static void app_demo_ble_custom_service_init();
// static tBleStatus app_demo_ble_custom_service_char_value_update(uint16_t uuid, uint16_t new_value_len, uint8_t* p_new_value);
static SVCCTL_EvtAckStatus_t app_demo_ble_custom_service_event_handler(void* event);

static void app_demo_ble_advertising();

extern int app_demo()
{
	 uint32_t ret = D_BSP_RET_OK;
     uint16_t ndef_ret = NDEF_OK;

	bsp_uart1_init();

    bsp_nfc07a1_led_init(E_BSP_NFC07A1_LED_GREEN);
    bsp_nfc07a1_led_init(E_BSP_NFC07A1_LED_YELLOW);
    bsp_nfc07a1_led_init(E_BSP_NFC07A1_LED_BLUE);

    bsp_nfc07a1_gpo_init();

    printf("\r\n");
    printf("---------------------------------------------\r\n");
    printf("Demo start ~~~\r\n");
    printf("---------------------------------------------\r\n");
    printf("\r\n");

     uint8_t lowpower_standby_flag = 0;
     ret = bsp_lowpower_mode_standby_resume_check(&lowpower_standby_flag);
     if (ret != D_BSP_RET_OK)
     {
         printf("Error of bsp_lowpower_mode_standby_resume_check !!!\r\n");
     }
     else
     {
         if (lowpower_standby_flag == 1)
         {
             printf("Resume from standby mode\r\n");
         }
         else
         {
        	 printf("Power on\r\n");
         }
     }

     bsp_nfc07a1_led_toggle(E_BSP_NFC07A1_LED_GREEN);

     app_demo_cpu2_system_init();

    while (gs_app_demo_cpu2_run_status != E_APP_DEMO_CPU2_RUN_STATUS_WIRELESS)
    {
        if (gs_app_demo_shci_event_pending_flag == E_APP_DEMO_SHCI_EVENT_PENDING_FLAG_SET)
        {
        	gs_app_demo_shci_event_pending_flag = E_APP_DEMO_SHCI_EVENT_PENDING_FLAG_RESET;
            shci_user_evt_proc();

        }
    }

    app_demo_ble_init();

    while (D_BSP_RET_OK != bsp_nfc07a1_nfctag_init() )
    {
        bsp_nfc07a1_led_toggle(E_BSP_NFC07A1_LED_YELLOW);
        bsp_nfc07a1_led_delay_ms(500);
    }

     NfcTag_SelectProtocol(NFCTAG_TYPE5);

     if (NfcType5_NDEFDetection() != NDEF_OK)
     {
         CCFileStruct.MagicNumber    =   NFCT5_MAGICNUMBER_E1_CCFILE;
         CCFileStruct.Version        =   NFCT5_VERSION_V1_0;
         CCFileStruct.MemorySize     =   (ST25DVXXKC_MAX_SIZE / 8) & 0xFF; // 0x200 = 512 (decimal)
         CCFileStruct.TT5Tag         =   0x05;
         /* Init of the Type Tag 5 component (M24LR) */
         while (NfcType5_TT5Init() != NDEF_OK);
     }

     uint16_t ndef_size = 0;
	 ndef_ret = NDEF_getNDEFSize(&ndef_size);
	 if (ndef_ret != NDEF_OK)
	 {
	 	printf("Error of NDEF_getNDEFSize, ret =  %u!!!\r\n", ndef_ret);
	 }
	 if (ndef_size == 0)
	 {
	 	sURI_Info record_info_uri = {0};
	 	strcpy(record_info_uri.URI_Message, (char*)gs_app_demo_nfctag_ble_bd_addr);
	 	strcpy(record_info_uri.Information,"\0" );
	 	ndef_ret = NDEF_WriteURI(&record_info_uri);
	 	if (ndef_ret != NDEF_OK)
	 	{
	 		printf("Error of NDEF_WriteURI !!!\r\n");
	 	}

	 	sAARInfo record_info_aar = {0};
	 	strcpy(record_info_aar.PackageName, "com.sychip.ble.ai.camera");
	 	ndef_ret = NDEF_AddAAR(&record_info_aar);
	 	if (ndef_ret != NDEF_OK)
	 	{
	 		printf("Error of NDEF_AddAAR !!!\r\n");
	 	}
	 }

	 uint32_t nfctag_password_msb = 000;
	 uint32_t nfctag_password_lsb = 000;
	 ret = bsp_nfc07a1_nfctag_i2cPassword_present(nfctag_password_msb, nfctag_password_lsb);
	 if (D_BSP_RET_OK != ret)
	 {
	 	printf("Error of bsp_nfc07a1_nfctag_i2cPassword_present !!!\r\n");
	 }

	 ret = bsp_nfc07a1_nfctag_it_configure(D_BSP_NFC07A1_NFCTAG_GPO1_ENABLE_MASK | D_BSP_NFC07A1_NFCTAG_GPO1_FILEDCHQNGE_MASK);
	 if (D_BSP_RET_OK != ret)
	 {
	 	printf("ERROR of bsp_nfc07a1_nfctag_it_configure !!!");
	 }

    app_demo_ble_custom_service_init();

    app_demo_ble_advertising();

    uint32_t old_tick = HAL_GetTick();
    uint32_t new_tick = 0;
    while(1)
    {
    	if (gs_app_demo_shci_event_pending_flag == E_APP_DEMO_SHCI_EVENT_PENDING_FLAG_SET)
        {
        	gs_app_demo_shci_event_pending_flag = E_APP_DEMO_SHCI_EVENT_PENDING_FLAG_RESET;
            shci_user_evt_proc();
        }

    	if (gs_app_demo_hci_event_pending_flag == E_APP_DEMO_HCI_EVENT_PENDING_FLAG_SET)
    	{
    		gs_app_demo_hci_event_pending_flag = E_APP_DEMO_HCI_EVENT_PENDING_FLAG_RESET;
    		hci_user_evt_proc();
    	}

    	if (gs_app_demo_ble_conn_flag == E_APP_DEMO_BLE_CONN_FLAG_SET)
    	{
    		bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
    	}

    	new_tick = HAL_GetTick();
    	if (new_tick - old_tick > 30000)
    	{
    		bsp_lowpower_mode_standby_enter();
    	}
    }




    // bsp_delay(5000);

    // bsp_lowpower_mode_standby_enter();



//	ndef_ret = NDEF_ReadNDEF(ndef_buffer);
//	if (ndef_ret != NDEF_OK)
//	{
//		printf("Error of NDEF_ReadNDEF, ret =  %u!!!\r\n", ndef_ret);
//	}
//
//	ndef_ret = NDEF_IdentifyBuffer(&record_info, ndef_buffer);
//	if (ndef_ret != NDEF_OK)
//	{
//		printf("Error of NDEF_IdentifyBuffer, ret =  %u!!!\r\n", ndef_ret);
//	}


//    printf("Check NFC tag I2C ss status\r\n");
//    E_BSP_NFC07A1_NFCTAG_I2C_SS_STATUS_T i2c_ss_status = E_BSP_NFC07A1_NFCTAG_I2C_SS_STATUS_CLOSED;
//    ret = bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get(&i2c_ss_status);
//    if (D_BSP_RET_OK != ret)
//    {
//		printf("Error of bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get !!!\r\n");
//    }
//    else
//    {
//        printf("NFC tag I2C SS status = %u\r\n", i2c_ss_status);
//    }
//
//    printf("\r\n");
//
//    printf("Present wrong password\r\n");
//	uint32_t nfctag_password_msb = 123;
//	uint32_t nfctag_password_lsb = 123;
//	ret = bsp_nfc07a1_nfctag_i2cPassword_present(nfctag_password_msb, nfctag_password_lsb);
//	if (D_BSP_RET_OK != ret)
//	{
//		printf("Error of bsp_nfc07a1_nfctag_i2cPassword_present !!!\r\n");
//	}
//	printf("Check NFC tag I2C ss status\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get(&i2c_ss_status);
//	if (D_BSP_RET_OK != ret)
//    {
//		printf("Error of bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get !!!\r\n");
//    }
//    else
//    {
//        printf("NFC tag I2C SS status = %u\r\n", i2c_ss_status);
//    }
//
//    printf("\r\n");
//
//    printf("Present correct password\r\n");
//	nfctag_password_msb = 0;
//	nfctag_password_lsb = 0;
//	ret = bsp_nfc07a1_nfctag_i2cPassword_present(nfctag_password_msb, nfctag_password_lsb);
//	if (D_BSP_RET_OK != ret)
//	{static void c2_init();
//		printf("Error of bsp_nfc07a1_nfctag_i2cPassword_present !!!\r\n");
//	}
//	printf("Check NFC tag I2C ss status\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get(&i2c_ss_status);
//	if (D_BSP_RET_OK != ret)
//    {
//		printf("Error of bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get !!!\r\n");
//    }
//    else
//    {
//        printf("NFC tag I2C SS status = %u\r\n", i2c_ss_status);
//    }
//
//    printf("\r\n");
//
//    printf("Get User Memory size\r\n");
//    uint32_t blk_num	=	0;
//    uint32_t blk_size	=	0;
//    uint32_t mem_size	=	0;
//    ret = bsp_nfc07a1_nfctag_i2cUsrMemBlockInfo_get(&blk_num, &blk_size);
//    if (D_BSP_RET_OK != ret)
//    {
//    	printf("Error of bsp_nfc07a1_nfctag_i2cUsrMemSize_get !!!\r\n");
//    }
//    else
//    {
//        mem_size = (blk_num + 1) * (blk_size + 1);
//        printf("NFC tag block number = %lu, block size = %lu, memory size = %lu\r\n", blk_num, blk_size, mem_size);
//    }
//
//    ret = bsp_nfc07a1_nfctag_i2cUsrZone_create( (mem_size/4), (mem_size/4), (mem_size/4), (mem_size/4) );
//    if (D_BSP_RET_OK != ret)
//    {
//    	printf("Error of bsp_nfc07a1_nfctag_i2cUsrZone_create !!!\r\n");
//    }
//
//    uint8_t zone_end_add = 0;
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get(E_BSP_NFC07A1_NFCTAG_ZONE_END_1, &zone_end_add);
//	if (D_BSP_RET_OK != ret)
//	{
//		printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get !!!\r\n");
//	}
//	else
//	{
//		printf("Zone 1 end address = %u\r\n", zone_end_add);
//	}
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get(E_BSP_NFC07A1_NFCTAG_ZONE_END_2, &zone_end_add);
//	if (D_BSP_RET_OK != ret)
//	{
//		printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get !!!\r\n");
//	}
//	else
//	{
//		printf("Zone 2 end address = %u\r\n", zone_end_add);
//	}
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get(E_BSP_NFC07A1_NFCTAG_ZONE_END_3, &zone_end_add);
//	if (D_BSP_RET_OK != ret)
//	{
//		printf("ERROR of bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get !!!\r\n");
//	}
//	else
//	{
//		printf("Zone 3 end address = %u\r\n", zone_end_add);
//	}
//
//	ret = bsp_nfc07a1_nfctag_it_configure(D_BSP_NFC07A1_NFCTAG_GPO1_ENABLE_MASK | D_BSP_NFC07A1_NFCTAG_GPO1_FILEDCHQNGE_MASK);
//	if (D_BSP_RET_OK != ret)
//	{
//		printf("ERROR of bsp_nfc07a1_nfctag_it_configure !!!");
//	}
//
//    printf("\r\n");
//
//    printf("Zone 1 is not protected\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_1, E_BSP_NFC07a1_NFCTAG_PROT_CONF_NO_PROT);
//    if (D_BSP_RET_OK != ret)
//    {
//        printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set !!!\r\n");
//    }
//
//    printf("Zone 2 is write protected\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_2, E_BSP_NFC07A1_NFCTAG_PROT_CONF_WRITE_PROT);
//    if (D_BSP_RET_OK != ret)
//    {
//        printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set !!!\r\n");
//    }
//
//    printf("Zone 3 is read protected\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_3, E_BSP_NFC07A1_NFCTAG_PROT_CONF_READ_PROT);
//    if (D_BSP_RET_OK != ret)
//    {
//        printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set !!!\r\n");
//    }
//
//    printf("Zone 4 is read/write protected\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_4, E_BSP_NFC07A1_NFCTAG_PROT_CONF_READWRITE_PROT);
//    if (D_BSP_RET_OK != ret)
//    {
//        printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set !!!\r\n");
//    }
//
//    printf("\r\n");
//
//    printf("Read Zone 1 protect configuration\r\n");
//    E_BSP_NFC07A1_NFCTAG_PROT_CONF_T prot_conf = E_BSP_NFC07A1_NFCTAG_PROT_CONF_ERROR;
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_get(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_1, &prot_conf);
//    if (D_BSP_RET_OK != ret)
//    {
//    	printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_get !!!\r\n");
//    }
//    else
//    {
//    	printf("Protect configuration = %u\r\n", prot_conf);
//    }
//
//    printf("Read Zone 2 protect configuration\r\n");
//    prot_conf = E_BSP_NFC07A1_NFCTAG_PROT_CONF_ERROR;
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_get(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_2, &prot_conf);
//    if (D_BSP_RET_OK != ret)
//    {
//    	printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_get !!!\r\n");
//    }
//    else
//    {
//    	printf("Protect configuration = %u\r\n", prot_conf);
//    }
//
//    printf("Read Zone 3 protect configuration\r\n");
//    prot_conf = E_BSP_NFC07A1_NFCTAG_PROT_CONF_ERROR;
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_get(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_3, &prot_conf);
//    if (D_BSP_RET_OK != ret)
//    {
//    	printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_get !!!\r\n");
//    }
//    else
//    {
//    	printf("Protect configuration = %u\r\n", prot_conf);
//    }
//
//    printf("Read Zone 4 protect configuration\r\n");
//    prot_conf = E_BSP_NFC07A1_NFCTAG_PROT_CONF_ERROR;
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_get(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_4, &prot_conf);
//    if (D_BSP_RET_OK != ret)
//    {
//    	printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_get !!!\r\n");
//    }
//    else
//    {
//    	printf("Protect configuration = %u\r\n", prot_conf);
//    }
//
//    printf("\r\n");
//
//#if 0
//    printf("Present wrong password\r\n");
//	nfctag_password_msb = 123;
//	nfctag_password_lsb = 123;
//	ret = bsp_nfc07a1_nfctag_i2cPassword_present(nfctag_password_msb, nfctag_password_lsb);
//	if (D_BSP_RET_OK != ret)
//	{
//		printf("Error of bsp_nfc07a1_nfctag_i2cPassword_present !!!\r\n");
//	}
//	printf("Check NFC tag I2C ss status\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get(&i2c_ss_status);
//	if (D_BSP_RET_OK != ret)
//    {
//		printf("Error of bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get !!!\r\n");
//    }
//    else
//    {
//        printf("NFC tag I2C SS status = %u\r\n", i2c_ss_status);
//    }
//#else
//    printf("Present correct password\r\n");
//	nfctag_password_msb = 0;
//	nfctag_password_lsb = 0;
//	ret = bsp_nfc07a1_nfctag_i2cPassword_present(nfctag_password_msb, nfctag_password_lsb);
//	if (D_BSP_RET_OK != ret)
//	{
//		printf("Error of bsp_nfc07a1_nfctag_i2cPassword_present !!!\r\n");
//	}
//	printf("Check NFC tag I2C ss status\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get(&i2c_ss_status);
//	if (D_BSP_RET_OK != ret)
//    {
//		printf("Error of bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get !!!\r\n");
//    }
//    else
//    {
//        printf("NFC tag I2C SS status = %u\r\n", i2c_ss_status);
//    }
//#endif
//
//    printf("\r\n");
//
//    uint32_t    mem_index       =   0;
//    uint8_t     mem_writeData   =   0xBB;
//    uint8_t     mem_readData    =   0;
//    for(uint8_t cnt = 1; cnt < 5; cnt++)
//    {
//        /* Compute memory index for each zone */
////        mem_index = (((mem_size / 4) + 1) * (cnt - 1));
//    	mem_index = ( (mem_size) / 4 * (cnt - 1) ) + 100;
//
//		printf("Start writing Zone %u, address %lu\r\n", cnt, mem_index);
//		ret = bsp_nfc07a1_nfctag_data_write(&mem_writeData, mem_index, 1);
//		if (D_BSP_RET_OK != ret)
//		{
//			printf("Error of bsp_nfc07a1_nfctag_data_write !!!\r\n");
//		}
//		else
//		{
//			printf("Write Zone %u done\r\n", cnt);
//		}
//
//		printf("Start reading Zone %u\r\n", cnt);
//		ret = bsp_nfc07a1_nfctag_data_read(&mem_readData, mem_index, 1);
//		if (D_BSP_RET_OK != ret)
//		{
//			printf("Error of bsp_nfc07a1_nfctag_data_read !!!\r\n");
//			printf("Memory read data = %u\r\n", mem_readData);
//		}
//		else
//		{
//			printf("Read Zone %u done, read memory data = %d\r\n", cnt, mem_readData);
//		}
//    }

    // uint8_t nfctag_gpo_activeFlag = 0;

    // while(1)
    //  {
    //     bsp_nfc07a1_gpo_activeFlag_check(&nfctag_gpo_activeFlag);

    //     if (nfctag_gpo_activeFlag)
    //     {
    //         bsp_nfc07a1_led_toggle(E_BSP_NFC07A1_LED_GREEN);
    //         bsp_nfc07a1_gpo_activeFlag_reset();
    //     }
    //  }

    return 0;
}

extern void shci_notify_asynch_evt(void* pdata)
{
    gs_app_demo_shci_event_pending_flag = E_APP_DEMO_SHCI_EVENT_PENDING_FLAG_SET;
    return;
}

extern void hci_notify_asynch_evt(void* pdata)
{
    gs_app_demo_hci_event_pending_flag = E_APP_DEMO_HCI_EVENT_PENDING_FLAG_SET;
    return;
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification(void *pckt)
{
    hci_event_pckt *event_pckt;
    // evt_blecore_aci *blecore_evt;
    evt_le_meta_event *le_meta_evt;
    
    event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;

    switch (event_pckt->evt)
    {
        case HCI_LE_META_EVT_CODE:
            le_meta_evt = (evt_le_meta_event *)(event_pckt->data);
            switch (le_meta_evt->subevent)
            {
                case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
                	gs_app_demo_ble_conn_flag = E_APP_DEMO_BLE_CONN_FLAG_SET;
                    printf("BLE connection establish\r\n");
                    break; /* HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE */
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return (SVCCTL_UserEvtFlowEnable);
}

extern int __io_putchar(int ch)
{
    bsp_uart1_tx_send_ch(ch);
    return ch;
}

static void app_demo_cpu2_system_init()
{
    TL_MM_Config_t          tl_mm_conf          =   {0};
    SHCI_TL_HciInitConf_t   shci_tl_init_conf   =   {0};

    /* Initialization of the reference table shared with the CPU2 in SRAM2*/
    TL_Init();

    /* System channel initialization */
    shci_tl_init_conf.p_cmdbuffer       = (uint8_t*)&gs_app_demo_shci_tl_cmd_buf;
    shci_tl_init_conf.StatusNotCallBack = app_demo_shci_tl_status_notification_callback;	
    shci_init(app_demo_shci_user_event_received_callback, (void*)&shci_tl_init_conf);
	  
    /*  Memory Manager channel initialization */
    tl_mm_conf.p_AsynchEvtPool          =   gs_app_demo_tl_mm_evtpool;
    tl_mm_conf.p_BleSpareEvtBuffer      =   gs_app_demo_tl_mm_system_spare_evt_buf;       /* UNUSED, but kept for future compatibility */
    tl_mm_conf.p_SystemSpareEvtBuffer   =   gs_app_demo_tl_mm_ble_spare_evt_buf;    /* UNUSED, but kept for future compatibility, but used by FUS today only */
    tl_mm_conf.AsynchEvtPoolSize        =   D_APP_DEMO_TL_MM_POOL_SIZE;
    TL_MM_Init(&tl_mm_conf);
	  
    /**< Release the CPU2 */
    TL_Enable();
	  
    return;
}

static void app_demo_shci_tl_status_notification_callback(SHCI_TL_CmdStatus_t status)
{
    /* Callback not implemented - code flow under control of the developer */
    (void)status;
    return;
}

static void app_demo_shci_user_event_received_callback(void* pData)
{
    TL_AsynchEvt_t*         p_sys_event;
    SHCI_C2_Ready_Evt_t*    p_sys_ready_event;
    // SCHI_SystemErrCode_t*   p_sys_error_code;
  
    p_sys_event = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pData)->pckt->evtserial.evt.payload);
    
    switch(p_sys_event->subevtcode)
    {
        case SHCI_SUB_EVT_CODE_READY:
            p_sys_ready_event = (SHCI_C2_Ready_Evt_t*)p_sys_event->payload;
            if (p_sys_ready_event->sysevt_ready_rsp == WIRELESS_FW_RUNNING)
            {
                printf("CPU2 run wireless stack\r\n");
                gs_app_demo_cpu2_run_status = E_APP_DEMO_CPU2_RUN_STATUS_WIRELESS;
            }
            else if (p_sys_ready_event->sysevt_ready_rsp == FUS_FW_RUNNING)
            {
                printf("CPU2 run fus\r\n");
                gs_app_demo_cpu2_run_status = E_APP_DEMO_CPU2_RUN_STATUS_FUS;
            }
            break; /* SHCI_SUB_EVT_CODE_READY */
        case SHCI_SUB_EVT_ERROR_NOTIF:
            printf("CPU2 system error\r\n");
            gs_app_demo_cpu2_err_flag = 1;
            break; /* SHCI_SUB_EVT_ERROR_NOTIF */
        default:
            break;
    }
    
    // ((tSHCI_UserEvtRxParam *)pData)->status = SHCI_TL_UserEventFlow_Disable;
    
    return;
}

static void app_demo_ble_init()
{
    SHCI_CmdStatus_t ret;
  
    SHCI_C2_Ble_Init_Cmd_Packet_t ble_init_cmd_packet =
    {
        {{0,0,0}},                          /**< Header unused */
        {0,                                 /** pBleBufferAddress not used */
        0,                                  /** BleBufferSize not used */
        CFG_BLE_NUM_GATT_ATTRIBUTES,
        CFG_BLE_NUM_GATT_SERVICES,
        CFG_BLE_ATT_VALUE_ARRAY_SIZE,
        CFG_BLE_NUM_LINK,
        CFG_BLE_DATA_LENGTH_EXTENSION,
        CFG_BLE_PREPARE_WRITE_LIST_SIZE,
        CFG_BLE_MBLOCK_COUNT,
        CFG_BLE_MAX_ATT_MTU,
        CFG_BLE_PERIPHERAL_SCA,
        CFG_BLE_CENTRAL_SCA,
        CFG_BLE_LS_SOURCE,
        CFG_BLE_MAX_CONN_EVENT_LENGTH,
        CFG_BLE_HSE_STARTUP_TIME,
        CFG_BLE_VITERBI_MODE,
        CFG_BLE_OPTIONS,
        0,
        CFG_BLE_MAX_COC_INITIATOR_NBR,
        CFG_BLE_MIN_TX_POWER,
        CFG_BLE_MAX_TX_POWER,
        CFG_BLE_RX_MODEL_CONFIG,
        CFG_BLE_MAX_ADV_SET_NBR, 
        CFG_BLE_MAX_ADV_DATA_LEN,
        CFG_BLE_TX_PATH_COMPENS,
        CFG_BLE_RX_PATH_COMPENS,
        CFG_BLE_CORE_VERSION,
        CFG_BLE_OPTIONS_EXT
        }
    };

    /* BLE channel initialization */
    app_demo_ble_tl_init();

    /* Starts the BLE Stack on CPU2 */
    ret = SHCI_C2_BLE_Init(&ble_init_cmd_packet);
    if (ret != SHCI_Success)
    {
        printf("Start BLE stack error\r\n");
    }

    app_demo_ble_gap_gatt_init();
    
}

static void app_demo_ble_tl_init()
{
    HCI_TL_HciInitConf_t hci_tl_init_conf;

    hci_tl_init_conf.p_cmdbuffer = (uint8_t*)&gs_app_demo_hci_tl_cmd_buffer;
    hci_tl_init_conf.StatusNotCallBack = app_demo_hci_tl_status_notification_callback;
    hci_init(app_demo_hci_user_event_receive_callback, (void*) &hci_tl_init_conf);
}

static void app_demo_ble_gap_gatt_init()
{
    uint16_t gap_service_handle, gap_dev_name_char_handle, gap_appearance_char_handle;
    const uint8_t *bd_address;
    uint32_t srd_bd_address[2];
    tBleStatus ret = BLE_STATUS_SUCCESS;
    
    /* BLE HCI Reset to synchronize BLE Stack */
    hci_reset();

    /* Write the BD Address */
    bd_address = app_demo_ble_bdaddress_get();
    aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, (uint8_t*) bd_address);
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("Set bd address err\r\n");
    }

    /** 
     * Static random Address
     * The two upper bits shall be set to 1
     * The lowest 32bits is read from the UDN to differentiate between devices
     * The RNG may be used to provide a random number on each power on
     */
    srd_bd_address[1]   =   0x0000ED6E;
    srd_bd_address[0]   =   LL_FLASH_GetUDN( );
    ret = aci_hal_write_config_data(CONFIG_DATA_RANDOM_ADDRESS_OFFSET, CONFIG_DATA_RANDOM_ADDRESS_LEN, (uint8_t*)srd_bd_address);
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("Set random bd ddress err\r\n");
    }

    /*  Set TX Power */
    ret = aci_hal_set_tx_power_level(0, CFG_TX_POWER);
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("Set TX power err\r\n");
    }

    /* Set Radio activity event mask */
    ret = aci_hal_set_radio_activity_mask(CFG_RADIO_ACTIVITY_EVENT_MASK);
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("Set radio activity event mask err\r\n");
    }

    /* Initialize GATT */
    ret = aci_gatt_init();
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("GATT init err\r\n");
    }

    /*  Initialize GAP */
    ret = aci_gap_init(
            GAP_PERIPHERAL_ROLE,
            PRIVACY_DISABLED,
            D_APP_DEMO_BLE_GAP_DEVICE_NAME_LEN,
            &gap_service_handle,
            &gap_dev_name_char_handle,
            &gap_appearance_char_handle
        );
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("GAP init err\r\n");
    }

    /* Update GAP Service Device Name characteristic value */
    ret = aci_gatt_update_char_value(
            gap_service_handle,
            gap_dev_name_char_handle,
            0,
            D_APP_DEMO_BLE_GAP_DEVICE_NAME_LEN,
            (uint8_t *)gs_app_demo_ble_gap_device_name
        );
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("GAP device name char value update err\r\n");
    }

    /* Update GAP Service Appearance characteristic value */
    ret = aci_gatt_update_char_value(
            gap_service_handle,
            gap_appearance_char_handle,
            0,
            sizeof(gs_app_demo_ble_gap_appearance),
            (uint8_t *)&gs_app_demo_ble_gap_appearance
        );
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("GAP appearance char value err\r\n");
    }

    return;
}

static const uint8_t* app_demo_ble_bdaddress_get()
{
    uint8_t *p_otp_addr;
    const uint8_t *p_bd_addr;
    uint32_t udn;
    uint32_t company_id;
    uint32_t device_id;
  
    udn = LL_FLASH_GetUDN();
  
    if (udn != 0xFFFFFFFF)
    {
        company_id = LL_FLASH_GetSTCompanyID();
        device_id = LL_FLASH_GetDeviceID();
  
        /**
        * Public Address with the ST company ID
        * bit[47:24] : 24bits (OUI) equal to the company ID
        * bit[23:16] : Device ID.
        * bit[15:0] : The last 16bits from the UDN
        * Note: In order to use the Public Address in a final product, a dedicated
        * 24bits company ID (OUI) shall be bought.
        */
        bd_address_udn[0] = (uint8_t)(udn & 0x000000FF);
        bd_address_udn[1] = (uint8_t)((udn & 0x0000FF00) >> 8);
        bd_address_udn[2] = (uint8_t)device_id;
        bd_address_udn[3] = (uint8_t)(company_id & 0x000000FF);
        bd_address_udn[4] = (uint8_t)((company_id & 0x0000FF00) >> 8);
        bd_address_udn[5] = (uint8_t)((company_id & 0x00FF0000) >> 16);

        (void)gs_app_demo_nfctag_ble_bd_addr;
		char *ptr = (char*)gs_app_demo_nfctag_ble_bd_addr;
		for (int8_t i = 5; i >= 0; --i)
		{
			if (i < 5)
			{
				*ptr++ = ':'; 	// 非首元素前添加冒号
			}
			snprintf(ptr, 3, "%02X", bd_address_udn[i]);
			ptr += 2; 			// 移动指针到下一个位置
		}
		*ptr = '\0';

        p_bd_addr = (const uint8_t *)bd_address_udn;
    }
    else
    {
        p_otp_addr = OTP_Read(0);
        if (p_otp_addr)
        {
            p_bd_addr = ((OTP_ID0_t*)p_otp_addr)->bd_address;
        }
        else
        {
            p_bd_addr = a_MBdAddr;
        }
    }
  
    return p_bd_addr;
}

static void app_demo_hci_tl_status_notification_callback(HCI_TL_CmdStatus_t status)
{
    /* Callback not implemented - code flow under control of the developer */
    (void)status;
    return;
}

static void app_demo_hci_user_event_receive_callback(void * pData)
{
    SVCCTL_UserEvtFlowStatus_t svctl_return_status;
    tHCI_UserEvtRxParam *pParam;
    
    pParam = (tHCI_UserEvtRxParam *)pData; 
    
    svctl_return_status = SVCCTL_UserEvtRx((void *)&(pParam->pckt->evtserial));
    if (svctl_return_status != SVCCTL_UserEvtFlowDisable)
    {
        pParam->status = HCI_TL_UserEventFlow_Enable;
    }
    else
    {
        pParam->status = HCI_TL_UserEventFlow_Disable;
    }
}

static void app_demo_ble_custom_service_init()
{
    tBleStatus ret = BLE_STATUS_SUCCESS;
    Char_UUID_t uuid16;
    
    /* Register the event handler to the BLE controller */
    SVCCTL_RegisterSvcHandler(app_demo_ble_custom_service_event_handler);
    
    /*  Add My Very Own Service */
    D_APP_DEMO_COPY_BLE_CUSTOM_SERVICE_UUID(uuid16.Char_UUID_128);
    ret = aci_gatt_add_service(
            UUID_TYPE_128,
            (Service_UUID_t*)&uuid16,
            PRIMARY_SERVICE,
            D_APP_DEMO_BLE_CUSTOM_SERVICE_MAX_ATT_RECORDS,
            &(gs_app_demo_ble_custom_service_ctx.service_handle)
        );
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("ADD custom service err\r\n");
    }
    
    /* Add My Very Own Write Characteristic */
    D_APP_DEMO_COPY_BLE_CUSTOM_WRITE_CHAR_UUID(uuid16.Char_UUID_128);
    ret = aci_gatt_add_char(
            gs_app_demo_ble_custom_service_ctx.service_handle,
            UUID_TYPE_128, &uuid16,
            D_APP_DEMO_BLE_CUSTOM_SERVICE_WRTIE_CHAR_VALUE_LEN,                                   
            CHAR_PROP_WRITE_WITHOUT_RESP|CHAR_PROP_READ,
            ATTR_PERMISSION_NONE,
            GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
            10, /* encryKeySize */
            1, /* isVariable */
            &(gs_app_demo_ble_custom_service_ctx.write_char_handle)
        );
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("ADD custom service write char err\r\n");
    }
    
    /* Add My Very Own Notify Characteristic */
    D_APP_DEMO_COPY_BLE_CUSTOM_NOTIFY_CHAR_UUID(uuid16.Char_UUID_128);
    ret = aci_gatt_add_char(
            gs_app_demo_ble_custom_service_ctx.service_handle,
            UUID_TYPE_128, &uuid16,
            D_APP_DEMO_BLE_CUSTOM_SERVICE_NOTIFY_CHAR_VALUE_LEN,
            CHAR_PROP_NOTIFY,
            ATTR_PERMISSION_NONE,
            GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
            10, /* encryKeySize */
            1, /* isVariable: 1 */
            &(gs_app_demo_ble_custom_service_ctx.notify_char_handle)
        );
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("ADD custom service notify char err\r\n");
    }
    
    return;
}

// static tBleStatus app_demo_ble_custom_service_char_value_update(uint16_t uuid, uint16_t new_value_len, uint8_t* p_new_value)
// {
//     tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  
//     switch(uuid)
//     {
//         case D_APP_DEMO_BLE_CUSTOM_NOTIFY_CHARA_UUID:
//             if (new_value_len <= D_APP_DEMO_BLE_CUSTOM_SERVICE_NOTIFY_CHAR_VALUE_LEN)
//             {
//                 ret = aci_gatt_update_char_value(
//                         gs_app_demo_ble_custom_service_ctx.service_handle,
//                         gs_app_demo_ble_custom_service_ctx.notify_char_handle,
//                         0,
//                         new_value_len,
//                         p_new_value
//                     );
//                 // ret = aci_gatt_update_char_value(
//                 //         myVeryOwnServiceContext.MyVeryOwnServiceHandle,
//                 //         myVeryOwnServiceContext.MyVeryOwnNotifyCharacteristicHandle,
//                 //         0, /* charValOffset */
//                 //         newValueLength, /* charValueLen */
//                 //         (uint8_t *) pNewValue
//                 //     );
//             }
//             break;
//         default:
//             break;
//     }
    
//     return ret;
// }

static SVCCTL_EvtAckStatus_t app_demo_ble_custom_service_event_handler(void* event)
{
    SVCCTL_EvtAckStatus_t   return_value;
    hci_event_pckt*         event_pckt;
    evt_blecore_aci*        blecore_evt;
    aci_gatt_attribute_modified_event_rp0 *attribute_modified;
    
    return_value = SVCCTL_EvtNotAck;
    event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)event)->data);
    
    switch(event_pckt->evt)
    {
        case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
        {
            blecore_evt = (evt_blecore_aci*)event_pckt->data;
            switch(blecore_evt->ecode)
            {
                case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
                    attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
                    if(attribute_modified->Attr_Handle == (gs_app_demo_ble_custom_service_ctx.write_char_handle + D_APP_DEMO_BLE_CUSTOM_SERVICE_CHAR_VALUE_ATTR_OFFSET))
                    {
                        if (attribute_modified->Attr_Data[1] == 0x01)
                        {

                        }
                        else 
                        {

                        }
                    }
          
                    break;
                default:
                    break;
            }
        }
        
        break; /* HCI_HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE_SPECIFIC */
    default:
        break;
    }

    return return_value;
}

static void app_demo_ble_advertising()
{
    tBleStatus ret = BLE_STATUS_SUCCESS;

    ret = aci_gap_set_discoverable(
        ADV_IND,                                        /*< Advertise as connectable, undirected. */
        D_APP_DEMO_BLE_FAST_CONN_ADV_INTERVAL_MIN,      /*< Set the advertising interval min value. */
        D_APP_DEMO_BLE_FAST_CONN_ADV_INTERVAL_MAX,      /*< Set the advertising interval max value. */
        GAP_PUBLIC_ADDR,                                /*< Use the public address. */
        NO_WHITE_LIST_USE,                             /*< No white list. */
        sizeof(gs_app_demo_ble_ad_local_name), (uint8_t*)gs_app_demo_ble_ad_local_name,/*< Use a local name. */
        0, NULL,                                       /*< Do not include the service UUID list. (no adopted services) */
        0x0000, 0x0000);                               /*< NaN, do not put in advertising data. */
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("Set BLE discoverable err\r\n");
    }

    ret = aci_gap_update_adv_data(sizeof(gs_app_demo_ble_ad_manufacturer_specific_data), (uint8_t*)gs_app_demo_ble_ad_manufacturer_specific_data);
    if (ret != BLE_STATUS_SUCCESS)
    {
        printf("Update adv data err\r\n");
    }
}
