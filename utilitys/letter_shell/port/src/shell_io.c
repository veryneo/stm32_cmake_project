#include "shell_io.h"
#include "shell.h"
#include "log.h"

#include "tx_api.h"

#include "bsp.h"


static TX_MUTEX userShellSendMutex;


static short userShellWrite(char *data, unsigned short len);
static short userShellRead(char *data, unsigned short len);
static int userShellLock(Shell *shell);
static int userShellUnlock(Shell *shell);


extern void shellIOInit(Shell* shell)
{
    UINT tx_ret;

    shell->write    =   userShellWrite;
    shell->read     =   userShellRead;
    shell->lock     =   userShellLock;
    shell->unlock   =   userShellUnlock;

    tx_ret = tx_mutex_create(&userShellSendMutex, "Shell Send Mutex", TX_NO_INHERIT);
    if (tx_ret != TX_SUCCESS)
    {
        while(1)
        {
            /* TBD */
        }
    }
}

static short userShellWrite(char *data, unsigned short len)
{
    int32_t write_len = 0;

    do
    {
        write_len = bsp_serialport_send_cache_data_set((const uint8_t *)data, (uint16_t)len);
        if (write_len <= 0)
        {
            write_len = 0;
            break;
        } 

        bsp_serialport_send();
    } while (0);

    return write_len;
}

static short userShellRead(char *data, unsigned short len)
{
    int32_t read_len = 0;

    do
    {
        read_len = bsp_serialport_receive_cache_data_get((uint8_t *)data, (uint16_t)len);
        if (read_len <= 0)
        {
            read_len = 0;
            break;
        }
    } while (0);

    return read_len;
}

static int userShellLock(Shell *shell)
{
    tx_mutex_get(&userShellSendMutex, TX_WAIT_FOREVER);

    return 0;
}

static int userShellUnlock(Shell *shell)
{
    tx_mutex_put(&userShellSendMutex);

    return 0;
}
