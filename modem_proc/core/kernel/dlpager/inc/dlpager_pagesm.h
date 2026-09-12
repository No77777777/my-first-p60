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
12/09/14   rc,rr,cp     Header file dlpager page state machine 
===========================================================================*/

#pragma once 
#include "dlpager_types.h"
#include <qurt.h>

unsigned int dlpager_pagesm_is_page_evictable( unsigned int );

unsigned int dlpager_pagesm_is_page_dirty( unsigned int );

void dlpager_pagesm_handle_event( dlpager_event_t, unsigned int, qurt_thread_t );

int dlpager_pagesm_init( void );

/**
 * @brief Clean states of pages associated with RX 
 * @warning *MUST BE* invoked from dlpager_reset_rx() only !
 */
extern void dlpager_pagesm_reset_rx (void);

