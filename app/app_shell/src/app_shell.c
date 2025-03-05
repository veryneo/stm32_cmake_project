/**************************************************************
**  Include
**************************************************************/

#include "app_shell.h"

#include "shell.h"
#include "log.h"

#include "tx_api.h"

#include "bsp.h"


/**************************************************************
**  Symbol
**************************************************************/

#define D_APP_SHELL_PARSER_BUFFER_SIZE  (256)


/**************************************************************
**  Variable
**************************************************************/

static Shell        gs_app_shell_handle;
static Log          gs_app_shell_log_handle;
static TX_MUTEX     gs_app_shell_send_mutex;
static TX_SEMAPHORE gs_app_shell_receive_semaphore;
static uint8_t      gs_app_shell_parser_buffer[D_APP_SHELL_PARSER_BUFFER_SIZE];


/**************************************************************
**  Prototype
**************************************************************/

static void app_shell_receive_callback(void);

static short app_shell_io_write(char *data, unsigned short len);
static short app_shell_io_read(char *data, unsigned short len);
static int app_shell_io_lock(Shell *shell);
static int app_shell_io_unlock(Shell *shell);

static void app_shell_log_write(char *buffer, short len);


/**************************************************************
**  Include
**************************************************************/

extern void app_shell_init(void)
{
    bsp_serialport_init();
    bsp_serialport_receive_callback_set(app_shell_receive_callback);

    gs_app_shell_handle.write   =   app_shell_io_write;
    gs_app_shell_handle.read    =   app_shell_io_read;
    gs_app_shell_handle.lock    =   app_shell_io_lock;
    gs_app_shell_handle.unlock  =   app_shell_io_unlock;
    shellInit(&gs_app_shell_handle, (char*)gs_app_shell_parser_buffer, D_APP_SHELL_PARSER_BUFFER_SIZE);
    shellMemPoolInit();

    gs_app_shell_log_handle.active  =   1;
    gs_app_shell_log_handle.level   =   LOG_DEBUG;
    gs_app_shell_log_handle.write   =   app_shell_log_write;
    logRegister(&gs_app_shell_log_handle, &gs_app_shell_handle);

    tx_semaphore_create(&gs_app_shell_receive_semaphore, "Shell Receive Semaphore", 0);
    tx_mutex_create(&gs_app_shell_send_mutex, "Shell Send Mutex", TX_NO_INHERIT);
}

extern void app_shell_run(void)
{
    UINT tx_ret = TX_SUCCESS;

    bsp_serialport_receive_enable();

    while(1)
    {   
        tx_ret = tx_semaphore_get(&gs_app_shell_receive_semaphore, TX_WAIT_FOREVER);
        if (tx_ret != TX_SUCCESS)
        {
            while(1)
            {
                /* TBD */
            }
        }

        char data;
        while (gs_app_shell_handle.read && gs_app_shell_handle.read(&data, 1) == 1)
        {
            shellHandler(&gs_app_shell_handle, data);
        }
    }
}


/**************************************************************
**  Funtion
**************************************************************/

static void app_shell_receive_callback(void)
{
    tx_semaphore_put(&gs_app_shell_receive_semaphore);
}

static short app_shell_io_write(char *data, unsigned short len)
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

static short app_shell_io_read(char *data, unsigned short len)
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

static int app_shell_io_lock(Shell *shell)
{
    tx_mutex_get(&gs_app_shell_send_mutex, TX_WAIT_FOREVER);

    return 0;
}

static int app_shell_io_unlock(Shell *shell)
{
    tx_mutex_put(&gs_app_shell_send_mutex);

    return 0;
}

static void app_shell_log_write(char *buffer, short len)
{
    if (gs_app_shell_log_handle.shell)
    {
        shellWriteEndLine(gs_app_shell_log_handle.shell, buffer, len);
    }
}
