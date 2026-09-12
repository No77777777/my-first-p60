#ifndef DS_EPC_TIMER_H
#define DS_EPC_TIMER_H
/*===========================================================================

                      DS_EPC_TIMER. H
                   
DESCRIPTION
  Timer module.
  
  This module handles DS EPC timer processing.
    - EDCT (1x) timer
    - SRAT cleanup timer

EXTERNALIZED FUNCTIONS

 Copyright (c) 2011-2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/epchandoff/inc/ds_epc_timer.h#2 $
  $Author: pwbldsvc $ 

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
08/08/11    fjia    Created Module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_EPC_HANDOFF
#include "rex.h"
#include "ds_epc_pdn_ctl.h"

#ifdef FEATURE_DATA_WLAN_MAPCON
/*===========================================================================
FUNCTION       DS_EPC_HO_GIVEUP_TIMER_CBACK

DESCRIPTION    Function registered when the Timer for a ho giveup timer
               is expires.

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_epc_ho_giveup_timer_cback
( 
  unsigned long parm
);
#endif /*FEATURE_DATA_WLAN_MAPCON*/
/*===========================================================================
FUNCTION       DS_EPC_SRAT_TIMER_CBACK

DESCRIPTION    Function registered when the Timer for a srat timer
               is expires.

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_epc_srat_timer_cback
( 
  unsigned long parm
);
/*===========================================================================
FUNCTION       DS_EPC_TIMER_START

DESCRIPTION    Start an epc timer for specified duration.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_epc_timer_start
(
  ds_epc_timer_type    *epc_timer_ptr,
  rex_timer_cnt_type    duration
);

/*===========================================================================
FUNCTION       DS_EPC_TIMER_STOP

DESCRIPTION    Stop an epc timer.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ds_epc_timer_stop
(
  ds_epc_timer_type    *epc_timer_ptr
);

/*===========================================================================

FUNCTION       DS_EPC_SRAT_TIMER_START

DESCRIPTION    Starts the srat timer for specified duration on a certain 
               TRAT.
 
Parameters     instance_ptr: indicate the EPC instance ptr 
               trat_name:    indicate the SRAT timer was started because
                             HO failed on this trat.
               rat_type:     If trat_name is 0x8004, use rat_type to
                             distinguish UMTS or LTE.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_srat_timer_start
(  
  ds_epc_cb_type                  *instance_ptr,
  ps_iface_name_enum_type          trat_name,
  ds_epc_pdn_rat_type              rat_type
);

/*===========================================================================
FUNCTION       DS_EPC_SRAT_TIMER_STOP

DESCRIPTION    Stop the srat_cleanup Timer.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds_epc_srat_timer_stop
(
  uint32            subs_id,
  ds_epc_cb_type   *instance_ptr
);

/*===========================================================================
FUNCTION       DS_EPC_SRAT_TIMER_CLEAR

DESCRIPTION    Clear the srat_cleanup Timer.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds_epc_srat_timer_clear
(
  uint32            subs_id,
  ds_epc_cb_type   *instance_ptr
);
#ifdef FEATURE_DATA_WLAN_MAPCON
/*===========================================================================
FUNCTION       DS_EPC_HO_GIVEUP_TIMER_START

DESCRIPTION    Starts the ho_giveup timer for specified duration.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_ho_giveup_timer_start(ds_epc_cb_type       *instance_ptr);

/*===========================================================================
FUNCTION       DS_EPC_HO_GIVEUP_TIMER_STOP

DESCRIPTION    Stop the ho_giveup Timer.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds_epc_ho_giveup_timer_stop(ds_epc_cb_type   *instance_ptr);

/*===========================================================================
FUNCTION       DS_EPC_HO_GIVEUP_TIMER_CLEAR

DESCRIPTION    Clear the ho_giveup Timer.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds_epc_ho_giveup_timer_clear(ds_epc_cb_type   *instance_ptr);
#endif /*FEATURE_DATA_WLAN_MAPCON*/
/*===========================================================================

FUNCTION       DS_EPC_RETRY_TIMER_START

DESCRIPTION    Starts the srat timer for specified duration.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_retry_timer_start(ds_epc_cb_type       *instance_ptr);

/*===========================================================================
FUNCTION       DS_EPC_RETRY_TIMER_STOP

DESCRIPTION    Stop the retry Timer.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds_epc_retry_timer_stop
(
  uint32            subs_id,
  ds_epc_cb_type   *instance_ptr
);

/*===========================================================================
FUNCTION       DS_EPC_RETRY_TIMER_CLEAR

DESCRIPTION    Clear the retry Timer.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds_epc_retry_timer_clear(ds_epc_cb_type   *instance_ptr);

/*===========================================================================

FUNCTION       DS_EPC_PENDING_TEARDOWN_TIMER_START

DESCRIPTION    Starts the pending teardown timer for specified duration.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_pending_teardown_timer_start(ds_epc_cb_type       *instance_ptr);

/*===========================================================================
FUNCTION       DS_EPC_PENDING_TEARDOWN_TIMER_STOP

DESCRIPTION    Stop the pending teardown Timer.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds_epc_pending_teardown_timer_stop
(
  uint32            subs_id,
  ds_epc_cb_type   *instance_ptr
);

/*===========================================================================
FUNCTION       DS_EPC_PENDING_TEARDOWN_TIMER_CLEAR

DESCRIPTION    Clear the pending teardown Timer.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void  ds_epc_pending_teardown_timer_clear(ds_epc_cb_type   *instance_ptr);

/*===========================================================================
FUNCTION       DS_EPC_RETRY_TIMER_CBACK

DESCRIPTION    Function registered when the Timer for a retry timer
               expires.

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_epc_retry_timer_cback( unsigned long parm);

/*===========================================================================
FUNCTION       DS_EPC_PENDING_TEARDOWN_TIMER_CBACK

DESCRIPTION    Function registered when the pending tear down timer
               expires.

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_epc_pending_teardown_timer_cback( unsigned long parm);

#ifdef FEATURE_DATA_WLAN_MAPCON
/*===========================================================================
FUNCTION       DS_EPC_HO_THROTTLE_TIMER_START

DESCRIPTION    Starts HO throttle timer for certain technology

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_ho_throttle_timer_start
(
  uint32                         subs_id,
  ds_epc_throttle_timer_type    *epc_timer_ptr,
  rex_timer_cnt_type             duration
);

/*===========================================================================
FUNCTION       DS_EPC_HO_THROTTLE_TIMER_STOP

DESCRIPTION    Stops HO throttle timer for certain technology

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_ho_throttle_timer_stop
(
  uint32                         subs_id,
  ds_epc_throttle_timer_type    *epc_timer_ptr
);

/*===========================================================================
FUNCTION       DS_EPC_HO_THROTTLE_TIMER_CLEAR

DESCRIPTION    Clears HO throttle timer for certain technology

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_ho_throttle_timer_clear
(
  uint32                         subs_id,
  ds_epc_throttle_timer_type    *epc_timer_ptr
);

/*===========================================================================
FUNCTION      DS_EPC_HO_THROTTLE_TIMER_CBACK

DESCRIPTION   Timer callback for throttle timer. 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_epc_ho_throttle_timer_cback
(
  unsigned long parm
);
#endif /*FEATURE_DATA_WLAN_MAPCON*/

#endif /*FEATURE_EPC_HANDOFF*/
#endif // DS_EPC_TIMER_H
