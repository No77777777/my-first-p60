#ifndef HDRSRCHSYNC_H
#define HDRSRCHSYNC_H

/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     H D R    S R C H    S Y N C    S T A T E

                          HDR Search Sync Header File

GENERAL DESCRIPTION
  This file contains prototypes and declarations regional to the HDR SRCH
  sync state.

EXTERNALIZED FUNCTIONS
  
  hdrsrchsync_thresh_init() - Sync State thresh related initialization
  
REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2005 - 2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchsync.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------
08/08/2013   vke     Added handling of sync substate callback
08/18/2005   ljl     Created the file
==========================================================================*/

/* EJECT */
/*==========================================================================

                    FUNCTION PROTOTYPES USED IN THE MODULE

This section contains function prototypes for the functions used in this
module.

==========================================================================*/

/*===========================================================================

FUNCTION HDRSRCHSYNC_SAVE_SYNC_SUBSTATE_CALLBACK

DESCRIPTION
  Save sync substate callback

DEPENDENCIES
  None

PARAMETERS
  substate_cb : call back for sync substate

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsync_save_sync_substate_callback
( 
  hdrsrch_sync_substate_cb_type substate_cb 
);

/*===========================================================================

FUNCTION HDRSRCHSYNC_THRESH_INIT

DESCRIPTION
  Sync State thresh related initialization

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsync_thresh_init( void );

#endif /* HDRSRCHSYNC_H */

