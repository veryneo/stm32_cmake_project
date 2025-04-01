#ifndef APP_LED_H
#define APP_LED_H

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    E_APP_LED_EVENT_LED_CLR_CHG,
    E_APP_LED_EVENT_LED_BLK_CHD
} E_APP_LED_EVENT_T;


extern void app_led_init();
extern void app_led_run();


#ifdef __cplusplus
}
#endif

#endif