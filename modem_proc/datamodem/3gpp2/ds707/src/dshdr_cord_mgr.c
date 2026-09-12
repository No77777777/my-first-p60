/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                       D A T A   S E R V I C E S   C O R D

                                 M A N A G E R

GENERAL DESCRIPTION
  This file contains functions used for implementing FEATURE_DS_CORD.

EXTERNALIZED FUNCTIONS
  DSHDR_PKT_CORD_REORIG_AFTER_PHYS_LINK_DOWN    
    Triggers the bring up of traffic channel and takes care of edge conditions
  
INITIALIZATION AND SEQUENCING REQUIREMENTS
    None

   Copyright (c) 2006-2011 by Qualcomm Technologies Incorporated.  All Rights Reserved.
   
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp2/ds707/src/dshdr_cord_mgr.c#2 $ 
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/15/11   mg      Global variable cleanup
10/13/10   op      Migrated to MSG 2.0 macros
02/15/09   ls      Initialize epzid in ds3g_pkt_call_info_type
09/09/05   ssr     New file: contains code that implements FEATURE_DS_CORD
                   This feature provides for traffic channel origination on 
                   receiving incoming DoS messages. Note that this feature is 
                   valid only for EV-DO.
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "target.h"
#include "customer.h"

#ifdef FEATURE_HDR_DOS
#endif /* FEATURE_HDR_DOS */
