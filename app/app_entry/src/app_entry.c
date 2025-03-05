#include "app_entry.h"
#include "app_shell.h"
#include "app_led.h"

#include "tx_api.h"

#include "stdint.h"


#define ALIGN(n) __attribute__((aligned(n)))

#define D_APP_LED_THREAD_STACK_SIZE                 (8*128)
#define D_APP_LED_THREAD_PRIORITY                   (10)
#define D_APP_LED_THREAD_PREEMPT_THRESHOLD          (10)
#define D_APP_LED_THREAD_TIME_SLICE                 (5)

#define D_APP_SHELL_THREAD_STACK_SIZE               (8*128)
#define D_APP_SHELL_THREAD_PRIORITY                 (5)
#define D_APP_SHELL_THREAD_PREEMPT_THRESHOLD        (5)
#define D_APP_SHELL_THREAD_TIME_SLICE               (0)

ALIGN(8) static uint8_t gs_app_led_thread_stack[D_APP_LED_THREAD_STACK_SIZE];
ALIGN(8) static uint8_t gs_app_shell_thread_stack[D_APP_SHELL_THREAD_STACK_SIZE];

TX_THREAD app_led_thread;
TX_THREAD app_shell_thread;


extern void app_entry()
{
    app_shell_init();
    app_led_init();

    tx_kernel_enter();
}

extern VOID tx_application_define(VOID *first_unused_memory)
{
    UINT tx_ret = TX_SUCCESS;

    tx_ret = tx_thread_create(&app_shell_thread,
                              "APP Shell Thread",
                              app_shell_run,
                              0,
                              gs_app_shell_thread_stack,
                              D_APP_SHELL_THREAD_STACK_SIZE, 
                              D_APP_SHELL_THREAD_PRIORITY, 
                              D_APP_SHELL_THREAD_PREEMPT_THRESHOLD, 
                              TX_NO_TIME_SLICE, 
                              TX_AUTO_START);
    if (tx_ret != TX_SUCCESS)
    {
    while(1)
        {
        /* TBD */
        }
    }

    tx_ret = tx_thread_create(&app_led_thread,
                              "APP LED Thread",
                              app_led_run,
                              0,
                              gs_app_led_thread_stack,
                              D_APP_LED_THREAD_STACK_SIZE, 
                              D_APP_LED_THREAD_PRIORITY, 
                              D_APP_LED_THREAD_PREEMPT_THRESHOLD, 
                              TX_NO_TIME_SLICE, 
                              TX_AUTO_START);
    if (tx_ret != TX_SUCCESS)
    {
        while(1)
        {
            /* TBD */
        }
    }
}
