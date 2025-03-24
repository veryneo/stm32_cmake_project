
/**************************************************************
**  Include
**************************************************************/

#include "shell_mem.h"

#include "tx_api.h"

#include "stdint.h"


/**************************************************************
**  Symbol
**************************************************************/

#define ALIGN(n) __attribute__( (aligned(n) ) )

#define SHELL_MEMPOOL_SIZE (8 * 50)


/**************************************************************
**  Variable
**************************************************************/

ALIGN(8) static uint8_t shellMemPoolBuf[SHELL_MEMPOOL_SIZE];

static TX_BYTE_POOL userShellMemPool;


/**************************************************************
**  Inferface
**************************************************************/

extern void shellMemPoolInit(void)
{
    UINT tx_ret = TX_SUCCESS;

    tx_ret = tx_byte_pool_create(&userShellMemPool, "Shell Mem Pool", shellMemPoolBuf, SHELL_MEMPOOL_SIZE);
    if (tx_ret != TX_SUCCESS)
    {
        while(1)
        {
            /* TBD */
        }
    }
}

extern void* shellMemMalloc(size_t size)
{
    UINT tx_ret = TX_SUCCESS;

    void *ptr = NULL;

    tx_ret = tx_byte_allocate(&userShellMemPool, &ptr, size, TX_NO_WAIT);
    if (tx_ret != TX_SUCCESS)
    {
        ptr = NULL;
    }

    return ptr;
}

extern void shellMemFree(void *ptr)
{
    tx_byte_release(ptr);
}





