#include "app_button.h"

#include "lwbtn.h"

#include "log.h"

#include "tx_api.h"

#include "bsp.h"

#include "stdint.h"

typedef enum 
{
    E_APP_BUTTON_FUNC_LED_CLR_CHG,
    E_APP_BUTTON_FUNC_LED_BLK_CHG,
} E_APP_BUTTON_FUNC_T;

typedef struct 
{
    E_APP_BUTTON_FUNC_T btn_func;
    E_BSP_BUTTON_ID_T btn_id;
} S_APP_BUTTON_CONF_T;


static const S_APP_BUTTON_CONF_T gs_app_button_conf_list[] = 
{
    {.btn_func = E_APP_BUTTON_FUNC_LED_CLR_CHG, .btn_id = E_BSP_BUTTON_ID_2}, 
    {.btn_func = E_APP_BUTTON_FUNC_LED_BLK_CHG, .btn_id = E_BSP_BUTTON_ID_3}
};

static lwbtn_btn_t gs_app_button_lwbtn_btn_list[sizeof(gs_app_button_conf_list)/sizeof(S_APP_BUTTON_CONF_T)];


static uint8_t app_button_state_get(lwbtn_t*, lwbtn_btn_t*);
static void app_button_event_process(lwbtn_t*, lwbtn_btn_t*, lwbtn_evt_t);

extern void app_button_init()
{
    for (uint8_t i = 0; i < sizeof(gs_app_button_conf_list)/sizeof(S_APP_BUTTON_CONF_T); ++i) 
    {
        bsp_button_init(gs_app_button_conf_list[i].btn_id);

        gs_app_button_lwbtn_btn_list[i].arg = (void*)&gs_app_button_conf_list[i]; 
    }

    lwbtn_init(gs_app_button_lwbtn_btn_list, sizeof(gs_app_button_lwbtn_btn_list) / sizeof(lwbtn_btn_t), app_button_state_get, app_button_event_process);
}

extern void app_button_run()
{
    while(1)
    {
        lwbtn_process(tx_time_get());
        tx_thread_sleep(20);
    }
}

static uint8_t app_button_state_get(lwbtn_t* lwbtn, lwbtn_btn_t* lwbtn_btn)
{
    (void)lwbtn;

    int bsp_ret = D_BSP_RET_OK;

    E_BSP_BUTTON_ID_T       btn_id      =   ( (S_APP_BUTTON_CONF_T*)lwbtn_btn->arg)->btn_id;
    E_BSP_BUTTON_STATE_T    btn_state   =   E_BSP_BUTTON_STATE_RELEASED;

    bsp_ret = bsp_button_state_get(btn_id, &btn_state);
    if (bsp_ret != D_BSP_RET_OK)
    {
        logError("bsp_ret = %d\r\n", bsp_ret);
        return 0;
    }

    return (btn_state == E_BSP_BUTTON_STATE_PRESSED);
}

static void app_button_event_process(lwbtn_t* lwbtn, lwbtn_btn_t* lwbtn_btn, lwbtn_evt_t lwbtn_evt)
{
    const S_APP_BUTTON_CONF_T* p_btn_conf = (S_APP_BUTTON_CONF_T*)lwbtn_btn->arg;

    switch(lwbtn_evt)
    {
        case LWBTN_EVT_ONPRESS:
            logDebug("Button %d EVT ONPRESS\r\n", p_btn_conf->btn_id);
            break;  
        case LWBTN_EVT_ONRELEASE:
            logDebug("Button %d EVT ONRELEASE\r\n", p_btn_conf->btn_id);
            break;        
        case LWBTN_EVT_ONCLICK:
            logDebug("Button %d EVT ONCLICK, click cnt = %d\r\n", p_btn_conf->btn_id, lwbtn_btn->click.cnt);
            switch(p_btn_conf->btn_func)
            {
                case E_APP_BUTTON_FUNC_LED_CLR_CHG:
                    logDebug("LED color change\r\n");
                    break;
                case E_APP_BUTTON_FUNC_LED_BLK_CHG:
                    logDebug("LED blink change\r\n");
                    break;
                default:
                    break;
            }
            break;
        case LWBTN_EVT_KEEPALIVE:
            logDebug("Button %d EVT KEEPALIVE, keep cnt = %d\r\n", p_btn_conf->btn_id, lwbtn_btn->keepalive.cnt);
            break;
        default:
            break;
    }
}
