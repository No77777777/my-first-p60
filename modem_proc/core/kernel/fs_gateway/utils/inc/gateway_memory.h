/*=============================================================================
  Copyright (c) 2008 Qualcomm Technologies Incorporated.
           All Rights Reserved.
 * gateway_memory.h
=============================================================================*/

#ifndef __GATEWAY_MEMORY_H__
#define __GATEWAY_MEMORY_H__

#include <stdio.h>
#include <stdlib.h>
#include "qurt_memory.h"


/*************************************************************************
* Global variables, Defines and macros
*************************************************************************/
#define GATEWAY_MAP_FAILED 				((void*)-1)
#define GATEWAY_MAP_PROT_NONE   		0x00    /* no permissions */
#define GATEWAY_MAP_PROT_READ   		0x01    /* pages can be read */
#define GATEWAY_MAP_PROT_WRITE  		0x02    /* pages can be written */
#define GATEWAY_MAP_PROT_EXEC   		0x04    /* pages can be executed */

/*************************************************************************
 * Map vaddr space of given size from gateway pool.
 *************************************************************************/
void* gateway_memory_map(size_t len);

/*************************************************************************
 * Un-map vaddr space of given size from gateway pool.
 *************************************************************************/
void gateway_memory_unmap(void *vaddr);

/*************************************************************************
* Change protections on the given address space.
**************************************************************************/
int gateway_memory_change_prot(void *vaddr, size_t len, int prot);


#endif //end of __GATEWAY_MEMORY_H__