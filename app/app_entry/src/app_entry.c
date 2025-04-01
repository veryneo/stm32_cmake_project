#include "app_entry.h"
#include "app_shell.h"
#include "app_led.h"
#include "app_log_one.h"
#include "app_log_two.h"
#include "app_button.h"

#include "tx_api.h"

#include "stdint.h"


#define ALIGN(n) __attribute__((aligned(n)))

#define D_APP_SHELL_THREAD_STACK_SIZE               (8*128)
#define D_APP_SHELL_THREAD_PRIORITY                 (5)
#define D_APP_SHELL_THREAD_PREEMPT_THRESHOLD        (5)
#define D_APP_SHELL_THREAD_TIME_SLICE               (0)

#define D_APP_LED_THREAD_STACK_SIZE                 (8*128)
#define D_APP_LED_THREAD_PRIORITY                   (10)
#define D_APP_LED_THREAD_PREEMPT_THRESHOLD          (10)
#define D_APP_LED_THREAD_TIME_SLICE                 (0)

#define D_APP_LOG_ONE_THREAD_STACK_SIZE             (8*128)
#define D_APP_LOG_ONE_THREAD_PRIORITY               (10)
#define D_APP_LOG_ONE_THREAD_PREEMPT_THRESHOLD      (10)
#define D_APP_LOG_ONE_THREAD_TIME_SLICE             (0)

#define D_APP_LOG_TWO_THREAD_STACK_SIZE             (8*128)
#define D_APP_LOG_TWO_THREAD_PRIORITY               (10)
#define D_APP_LOG_TWO_THREAD_PREEMPT_THRESHOLD      (10)
#define D_APP_LOG_TWO_THREAD_TIME_SLICE             (0)

#define D_APP_BUTTON_THREAD_STACK_SIZE              (8*128)
#define D_APP_BUTTON_THREAD_PRIORITY                (8)
#define D_APP_BUTTON_THREAD_PREEMPT_THRESHOLD       (8)
#define D_APP_BUTTON_THREAD_TIME_SLICE              (0)  


ALIGN(8) static uint8_t gs_app_shell_thread_stack[D_APP_SHELL_THREAD_STACK_SIZE];
ALIGN(8) static uint8_t gs_app_led_thread_stack[D_APP_LED_THREAD_STACK_SIZE];
ALIGN(8) static uint8_t gs_app_log_one_thread_stack[D_APP_LOG_ONE_THREAD_STACK_SIZE];
ALIGN(8) static uint8_t gs_app_log_two_thread_stack[D_APP_LOG_TWO_THREAD_STACK_SIZE];
ALIGN(8) static uint8_t gs_app_button_thread_stack[D_APP_BUTTON_THREAD_STACK_SIZE];

TX_THREAD app_shell_thread;
TX_THREAD app_led_thread;
TX_THREAD app_log_one_thread;
TX_THREAD app_log_two_thread;
TX_THREAD app_button_thread;


extern void app_entry()
{
    app_shell_init();
    app_led_init();
    app_log_one_init();
    app_log_two_init();
    app_button_init();

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
							  D_APP_SHELL_THREAD_TIME_SLICE,
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
							  D_APP_LED_THREAD_TIME_SLICE,
                              TX_AUTO_START);
    if (tx_ret != TX_SUCCESS)
    {
        while(1)
        {
            /* TBD */
        }
    }

    tx_ret = tx_thread_create(&app_log_one_thread,
                              "APP Log One Thread",
                              app_log_one_run,
                              0,
                              gs_app_log_one_thread_stack,
                              D_APP_LOG_ONE_THREAD_STACK_SIZE, 
                              D_APP_LOG_ONE_THREAD_PRIORITY, 
                              D_APP_LOG_ONE_THREAD_PREEMPT_THRESHOLD, 
							  D_APP_LOG_ONE_THREAD_TIME_SLICE,
                              TX_AUTO_START);
    if (tx_ret != TX_SUCCESS)
    {
        while(1)
        {   
            /* TBD */
        }
    }

    tx_ret = tx_thread_create(&app_log_two_thread,
                              "APP Log Two Thread",
                              app_log_two_run,
                              0,
                              gs_app_log_two_thread_stack,
                              D_APP_LOG_TWO_THREAD_STACK_SIZE, 
                              D_APP_LOG_TWO_THREAD_PRIORITY, 
                              D_APP_LOG_TWO_THREAD_PREEMPT_THRESHOLD, 
							  D_APP_LOG_TWO_THREAD_TIME_SLICE,
                              TX_AUTO_START);
    if (tx_ret != TX_SUCCESS)
    {
        while(1)
        {   
            /* TBD */
        }
    }

    tx_ret = tx_thread_create(&app_button_thread,
                              "APP Button Thread",
                              app_button_run,
                              0,
                              gs_app_button_thread_stack,
                              D_APP_BUTTON_THREAD_STACK_SIZE, 
                              D_APP_BUTTON_THREAD_PRIORITY, 
                              D_APP_BUTTON_THREAD_PREEMPT_THRESHOLD, 
                              D_APP_BUTTON_THREAD_TIME_SLICE,
                              TX_AUTO_START);
    if (tx_ret != TX_SUCCESS)
    {
        while(1)
        {   
            /* TBD */
        }
    }
}


