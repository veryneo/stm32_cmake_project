#include "app_demo.h"

#include "bsp.h"

#include "stdio.h"

#include "lib_NDEF_AAR.h"
#include "lib_NDEF_URI.h"
#include "tagtype5_wrapper.h"


extern int app_demo()
{
	uint32_t ret = D_BSP_RET_OK;
    uint16_t ndef_ret = NDEF_OK;

	bsp_uart1_init();

    bsp_led_init(E_BSP_LED_GREEN);

    bsp_nfc07a1_led_init(E_BSP_NFC07A1_LED_GREEN);
    bsp_nfc07a1_led_init(E_BSP_NFC07A1_LED_YELLOW);
    bsp_nfc07a1_led_init(E_BSP_NFC07A1_LED_BLUE);

    bsp_nfc07a1_gpo_init();

    printf("\r\n");
    printf("---------------------------------------------\r\n");
    printf("Start Test\r\n");
    printf("---------------------------------------------\r\n");
    printf("\r\n");

    uint8_t lowpower_standby_flag = 0;

    ret = bsp_lowpower_mode_standby_resume_check(&lowpower_standby_flag);
    if (ret != D_BSP_RET_OK)
    {
        printf("Error of bsp_lowpower_mode_standby_resume_check !!!\r\n");
        bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
    }
    else
    {
        if (lowpower_standby_flag == 1)
        {
            printf("Resume from standby mode\r\n");
            bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_GREEN);
        }
        else
        {
            bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_YELLOW);
        }
    }

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
//    uint8_t ndef_buffer[256] = {0};
//    sRecordInfo_t record_info = {0};
    
	ndef_ret = NDEF_getNDEFSize(&ndef_size);
	if (ndef_ret != NDEF_OK)
	{
		printf("Error of NDEF_getNDEFSize, ret =  %u!!!\r\n", ndef_ret);
		bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
	}
	if (ndef_size == 0)
	{
		sURI_Info record_info_uri = {0};
		strcpy(record_info_uri.URI_Message,"AA:BB:CC:DD:EE:FF" );
		strcpy(record_info_uri.Information,"\0" );
		ndef_ret = NDEF_WriteURI(&record_info_uri);
		if (ndef_ret != NDEF_OK)
		{
			printf("Error of NDEF_WriteURI !!!\r\n");
			bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
		}

		sAARInfo record_info_aar = {0};
		strcpy(record_info_aar.PackageName, "com.sychip.ble.ai.camera");
		ndef_ret = NDEF_AddAAR(&record_info_aar);
		if (ndef_ret != NDEF_OK)
		{
			printf("Error of NDEF_AddAAR !!!\r\n");
			bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
		}
	}

	uint32_t nfctag_password_msb = 000;
	uint32_t nfctag_password_lsb = 000;
	ret = bsp_nfc07a1_nfctag_i2cPassword_present(nfctag_password_msb, nfctag_password_lsb);
	if (D_BSP_RET_OK != ret)
	{
		printf("Error of bsp_nfc07a1_nfctag_i2cPassword_present !!!\r\n");
		bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
	}

	ret = bsp_nfc07a1_nfctag_it_configure(D_BSP_NFC07A1_NFCTAG_GPO1_ENABLE_MASK | D_BSP_NFC07A1_NFCTAG_GPO1_FILEDCHQNGE_MASK);
	if (D_BSP_RET_OK != ret)
	{
		printf("ERROR of bsp_nfc07a1_nfctag_it_configure !!!");
		bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
	}

    bsp_delay(5000);

    bsp_lowpower_mode_standby_enter();



//	ndef_ret = NDEF_ReadNDEF(ndef_buffer);
//	if (ndef_ret != NDEF_OK)
//	{
//		printf("Error of NDEF_ReadNDEF, ret =  %u!!!\r\n", ndef_ret);
//		bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//	}
//
//	ndef_ret = NDEF_IdentifyBuffer(&record_info, ndef_buffer);
//	if (ndef_ret != NDEF_OK)
//	{
//		printf("Error of NDEF_IdentifyBuffer, ret =  %u!!!\r\n", ndef_ret);
//		bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//	}


//    printf("Check NFC tag I2C ss status\r\n");
//    E_BSP_NFC07A1_NFCTAG_I2C_SS_STATUS_T i2c_ss_status = E_BSP_NFC07A1_NFCTAG_I2C_SS_STATUS_CLOSED;
//    ret = bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get(&i2c_ss_status);
//    if (D_BSP_RET_OK != ret)
//    {
//		printf("Error of bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get !!!\r\n");
//	    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//	    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//	}
//	printf("Check NFC tag I2C ss status\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get(&i2c_ss_status);
//	if (D_BSP_RET_OK != ret)
//    {
//		printf("Error of bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get !!!\r\n");
//	    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//	{
//		printf("Error of bsp_nfc07a1_nfctag_i2cPassword_present !!!\r\n");
//	    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//	}
//	printf("Check NFC tag I2C ss status\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get(&i2c_ss_status);
//	if (D_BSP_RET_OK != ret)
//    {
//		printf("Error of bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get !!!\r\n");
//	    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//        bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//        bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//    }
//
//    uint8_t zone_end_add = 0;
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get(E_BSP_NFC07A1_NFCTAG_ZONE_END_1, &zone_end_add);
//	if (D_BSP_RET_OK != ret)
//	{
//		printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get !!!\r\n");
//	    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//	}
//	else
//	{
//		printf("Zone 1 end address = %u\r\n", zone_end_add);
//	}
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get(E_BSP_NFC07A1_NFCTAG_ZONE_END_2, &zone_end_add);
//	if (D_BSP_RET_OK != ret)
//	{
//		printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get !!!\r\n");
//	    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//	}
//	else
//	{
//		printf("Zone 2 end address = %u\r\n", zone_end_add);
//	}
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get(E_BSP_NFC07A1_NFCTAG_ZONE_END_3, &zone_end_add);
//	if (D_BSP_RET_OK != ret)
//	{
//		printf("ERROR of bsp_nfc07a1_nfctag_i2cUsrZoneEnd_get !!!\r\n");
//	    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//	    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//	}
//
//    printf("\r\n");
//
//    printf("Zone 1 is not protected\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_1, E_BSP_NFC07a1_NFCTAG_PROT_CONF_NO_PROT);
//    if (D_BSP_RET_OK != ret)
//    {
//        printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set !!!\r\n");
//        bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//    }
//
//    printf("Zone 2 is write protected\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_2, E_BSP_NFC07A1_NFCTAG_PROT_CONF_WRITE_PROT);
//    if (D_BSP_RET_OK != ret)
//    {
//        printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set !!!\r\n");
//        bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//    }
//
//    printf("Zone 3 is read protected\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_3, E_BSP_NFC07A1_NFCTAG_PROT_CONF_READ_PROT);
//    if (D_BSP_RET_OK != ret)
//    {
//        printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set !!!\r\n");
//        bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//    }
//
//    printf("Zone 4 is read/write protected\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set(E_BSP_NFC07A1_NFCTAG_PROT_ZONE_4, E_BSP_NFC07A1_NFCTAG_PROT_CONF_READWRITE_PROT);
//    if (D_BSP_RET_OK != ret)
//    {
//        printf("Error of bsp_nfc07a1_nfctag_i2cUsrZoneProtect_set !!!\r\n");
//        bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//        bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//        bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//        bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//        bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//	    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
//	}
//	printf("Check NFC tag I2C ss status\r\n");
//    ret = bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get(&i2c_ss_status);
//	if (D_BSP_RET_OK != ret)
//    {
//		printf("Error of bsp_nfc07a1_nfctag_i2cSecuritySessionDyn_get !!!\r\n");
//	    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//		    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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
//		    bsp_nfc07a1_led_on(E_BSP_NFC07A1_LED_BLUE);
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

extern int __io_putchar(int ch)
{
    bsp_uart1_tx_send_ch(ch);

    return ch;
}
