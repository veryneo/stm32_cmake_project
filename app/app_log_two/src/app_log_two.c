#include "app_log_two.h"

#include "log.h"

#include "tx_api.h"

extern void app_log_two_init()
{

}

extern void app_log_two_run()
{
    while(1)
    {
    	logInfo("Log Two Output\r\n");
        tx_thread_sleep(1000);
    }
}
