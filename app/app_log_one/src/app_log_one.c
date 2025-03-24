#include "app_log_one.h"

#include "log.h"

#include "tx_api.h"

extern void app_log_one_init()
{

}

extern void app_log_one_run()
{
    while(1)
    {
    	logError("Log one output\r\n");
        tx_thread_sleep(1000);
    }
}
