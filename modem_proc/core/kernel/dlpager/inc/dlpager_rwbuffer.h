
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               DL PAGER TOP LEVEL HEADER FILE

GENERAL DESCRIPTION

  Copyright (c) 2010 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/09/14   cp      Header file for APIs related rw buffer management
===========================================================================*/

#pragma once
#include <dlpager_types.h>

void  dlpager_rwbuffer_init(void);      // must be called before alloc and free
void *dlpager_rwbuffer_alloc(dlpager_priority_t);     // returns a pointer to a rw buffer
void  dlpager_rwbuffer_free(void *, dlpager_priority_t);    // should only be passed with a void * that was returned from a previous alloc


