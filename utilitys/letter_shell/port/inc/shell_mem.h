#ifndef __SHELL_MEM_H
#define __SHELL_MEM_H

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************
**  Include
**************************************************************/

#include "stddef.h"


/**************************************************************
**  Interface
**************************************************************/

extern void shellMemPoolInit(void);
extern void* shellMemMalloc(size_t size);
extern void shellMemFree(void *ptr);


#ifdef __cplusplus
}
#endif

#endif
