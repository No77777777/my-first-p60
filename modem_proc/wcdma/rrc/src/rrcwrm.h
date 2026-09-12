#ifndef RRCWRM_H
#define RRCWRM_H

/*===========================================================================
            RRC  WCDMA  RESOURCE  MANAGER  HEADER  FILE
DESCRIPTION

   This file contains all prototypes used by RRC WRM procedure
EXTERNALIZED FUNCTIONS



INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2000-2009 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/RRC/vcs/rrcwrm.h_v   1.25   19 Jun 2002 10:01:34   bumatt  $
$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrcwrm.h#2 $    $DateTime: 2020/01/24 03:56:36 $ $ $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/15/17   sp      Made changes to adaptively calculate defer time for unlock required cases
10/20/17   sp      Made changes to buffer multiple donwlink sdu inds when received during TA gap
10/03/17   sp      Changes for LTA timeline optimization
05/23/16   sp      Made changes to stop BCCH modification timer when UE doesn't receive lock to transition out of PCH state
09/23/15   as      Made changes to stop QTA-CCCH timer after CCCH transmission
09/25/15   sr      Changes to make state transition cell selection atomic
09/09/15   sr      Made changes to get default band from RF api for lock request
07/08/15   sr      TRM-U changes for NAS-RRC interface
06/26/15   sp      Fixed compilation errors with QSH feature enabled
05/29/15   gv      Additional code changes to support TRM-U
05/20/15   gv      Increase lock wait timer for split acq to 70 msec
04/03/15   sad     QTA clean up changes
10/09/14   sn      QTA during SIB read changes
08/08/14   rmn     Made changes to request lock on supported band indicated by NAS rather
                   than deriving it from Japan PLMN and freq.
08/04/14   vi      Made changes to avoid compiler warnings
06/19/14   rmn     Made changes for DR-DSDS Feature
05/06/14   as      Moving WRM to WL1TRM.
12/30/13   sr      WRRC support for new requirements for LWG DSDS
09/16/13   sn      G2W TA feature changes
11/16/13   sr      WRRC support for LWG DSDS
08/13/13   gv      Added support for FEATURE_TRIPLE_SIM
08/13/13   gv      Made changes to reduce the lock request time
03/25/13   sn      Changes to replace timer based dog reporting with signal based reporting
10/18/12   gv      Replace FEATURE_DUAL_SIM_WCDMA_TUNEAWAY with FEATURE_DUAL_SIM
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/05/12   rv      Added support for QTA.
12/12/11   mn      Made changes to process ps hard abort request received when waiting for lock
06/21/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.100.50
06/08/11   as      Added changes to return tuneaway status
03/02/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.92.50
02/24/11   kp      Added code to store & precess RLC reset during tuneaway
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
01/04/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.87.50
12/09/10   as      Made changes to reset L2 ack info for DSIM Tuneaway
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
11/30/10   as      Added changes to update tuneaway status to trm
11/09/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.84.50
10/29/10   as      Added changes to handle unlock-required
10/28/10   as      Added changes to check for DL ACK for Tuneaway
10/27/10   as      Added changes to save DL SDU during Tuneaway
09/13/10   as      Added code to support FEATURE_DUAL_SIM
03/29/10   sks     Made changes to acquire lock before waking up L1 during
                   connected mode OOS search.
03/12/10   rm      Added code to support FEATURE_DUAL_SIM
===========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "wcdma_variation.h"
#include "uecomdef.h"
#include "rrccmd_v.h"
#include "wl1trm.h"
#ifndef TEST_FRAMEWORK
#include "wl1multisim.h"
#include "wl1idletamgr.h"
#endif
#ifdef FEATURE_DUAL_SIM
#include "rrclcm.h"

#define RRC_GET_LOCK_REQ_TIME(priority)   ((priority == TRM_TRAFFIC) ? 0xFFFFFFFF : 200)
#define ALLOW_LTA_THOUGH_L2_ACK_PENDING 1

/*==================================================
                 TYPE DEFINITIONS
==================================================*/

typedef enum
{
  RRC_WRM_LOCK_ACQUIRED,
  RRC_WRM_LOCK_NOT_ACQUIRED,
  RRC_WRM_TIMER_EXPIRED,
  RRC_ABORT_FROM_HIGHER_LAYERS,
  RRC_NO_RESOURCE_FROM_L1,
  RRC_ABORT_FROM_RR,
  RRC_WRM_LOCK_MAX
} rrc_out_of_lock_wait_e_type;




/*==================================================
                 FUNCTION DECALARATIONS
==================================================*/


/*===========================================================================

FUNCTION  RRC_RELEASE_LOCK

DESCRIPTION

  This function is called to release lock if RRC is holding the lock.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_release_lock(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION  RRC_WAIT_FOR_WRM_LOCK

DESCRIPTION

  This function waits for the lock grant from WRM
    
DEPENDENCIES

  None.
 
RETURN VALUE

  Lock status.
 
SIDE EFFECTS

  None.

===========================================================================*/
rrc_out_of_lock_wait_e_type rrc_wait_for_wrm_lock(sys_modem_as_id_e_type wrrc_as_id, trm_reason_enum_t priority,uint32 lock_time,  wrm_freq_input_type lock_req_bands);

/*===========================================================================

FUNCTION  RRC_ACQUIRE_WRM_LOCK

DESCRIPTION

  This function requests WRM for lock. It then waits for the lock to be granted.
  In the meantime, it also checks MM, L1 and RR commands posted to RRC.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  Lock status.
 
SIDE EFFECTS

  None.

===========================================================================*/
rrc_out_of_lock_wait_e_type  rrc_acquire_wrm_lock(sys_modem_as_id_e_type wrrc_as_id, trm_reason_enum_t priority,uint32 lock_time,wrm_freq_input_type lock_req_bands);

/*===========================================================================

FUNCTION  RRC_START_LOCK_WAIT_TIMER

DESCRIPTION

  This function starts the timer for which RRC should wait for lock
  to be granted by TRM.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_start_lock_wait_timer (sys_modem_as_id_e_type wrrc_as_id, uint32 lock_wait_time,boolean stop_if_already_running);

/*===========================================================================

FUNCTION  RRC_STOP_LOCK_WAIT_TIMER

DESCRIPTION

  This function stops the LOCK_TIMER.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_stop_lock_wait_timer(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION  RRC_ACQUIRE_LOCK_FOR_EST_REQ

DESCRIPTION

  This function requests WRM for lock with high priority for connection
  establishment.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  SUCCESS: RRC was granted lock.
  FAILURE: RRC failed to get the lock.

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type rrc_acquire_lock_for_est_req
(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_establish_cause_e_type establishment_cause,
  uint32 lock_time,
  uint32 lock_wait_time
);

/*===========================================================================

FUNCTION  RRC_ACQUIRE_LOCK_FOR_PCH_TO_FACH

DESCRIPTION

  This function requests WRM for lock when starting PCH to FACH 
  state transition.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  SUCCESS: RRC was granted lock.
  FAILURE: RRC failed to get the lock.

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type rrc_acquire_lock_for_pch_to_fach(sys_modem_as_id_e_type wrrc_as_id);


/*===========================================================================

FUNCTION  RRC_ACQUIRE_LOCK_FOR_RESUMING_ON_WCDMA

DESCRIPTION

  This function requests WRM for lock when resuming on WCDMA and RRC is in
  DCH or FACH state.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  SUCCESS: RRC was granted lock.
  FAILURE: RRC failed to get the lock.

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type rrc_acquire_lock_for_resuming_on_wcdma(sys_modem_as_id_e_type wrrc_as_id);



/*===========================================================================

FUNCTION        RRC_CHECK_LOCK_CAN_BE_ACQUIRED

DESCRIPTION     This function will check with WRM if lock can be acquired for lock_time

DEPENDENCIES
                None

RETURN VALUE
                SUCCESS if lock can be acquired for lock_time
                FAILURE otherwise

SIDE EFFECTS
                None

===========================================================================*/
uecomdef_status_e_type rrc_check_lock_can_be_acquired(sys_modem_as_id_e_type wrrc_as_id, uint32 lock_time, wrm_activity_enum_type priority);


/*===========================================================================

FUNCTION  RRC_ACQUIRE_LOCK_FOR_IHO_OR_CCO

DESCRIPTION

  This function requests WRM for lock with high priority for inter-RAT
  handover and cell change over.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  Lock status.

SIDE EFFECTS

  None.

===========================================================================*/
rrc_out_of_lock_wait_e_type rrc_acquire_lock_for_IHO_or_CCO
(
  sys_modem_as_id_e_type wrrc_as_id,
  uint32 lock_wait_time,
  wrm_freq_input_type lock_req_bands,
  trm_reason_enum_t priority
);

/*===========================================================================

FUNCTION  RRCWRM_POST_PREEMPTION_INDICATION

DESCRIPTION

  This function posts preemption indication to internal command queue
    
DEPENDENCIES

  None.
 
RETURN VALUE

  Lock status.

SIDE EFFECTS

  None.

===========================================================================*/
void rrcwrm_post_preemption_indication
(
sys_modem_as_id_e_type wrrc_as_id
);
/*===========================================================================

FUNCTION  rrc_acquire_lock_for_waking_up_l1_during_conn_oos

DESCRIPTION

  This function requests WRM for lock when waking L1 out of
  sleep during connected mode OOS search.
    
DEPENDENCIES

  None.

RETURN VALUE

  Lock status.

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type rrc_block_wait_to_acquire_lock_for_time
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32, 
  trm_reason_enum_t, 
  wrm_freq_input_type,
  boolean is_lta
);

/*===========================================================================

FUNCTION  rrcwrm_is_process_required

DESCRIPTION

  This function returns True if the incoming command is required a further
  process . Otherwise, the False is returned.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

boolean rrcwrm_is_process_required(sys_modem_as_id_e_type wrrc_as_id,  rrc_cmd_type *cmd_ptr );
/*===========================================================================

FUNCTION  rrcwrm_post_pending_cmd

DESCRIPTION

  This function will post all cmds which were saved during DSDS Tune away
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

void rrcwrm_post_pending_cmd
( 
sys_modem_as_id_e_type wrrc_as_id
);
/*===========================================================================

FUNCTION  rrcwrm_check_and_trash_pending_cmd

DESCRIPTION

  This function will trash pending  all cmds which were saved during DSDS Tune away
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

void rrcwrm_check_and_trash_pending_cmd
( 
sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION  rrc_set_l2_ack_req_mask

DESCRIPTION

  This function sets the L2 Ack required mask
    
DEPENDENCIES

  None.
 
RETURN VALUE

  void.

SIDE EFFECTS

  None.

===========================================================================*/

void rrc_set_l2_ack_req_mask(sys_modem_as_id_e_type wrrc_as_id,  rrc_proc_e_type rrc_proc_name , rrclcm_mui_type mui);


/*===========================================================================

FUNCTION  rrc_reset_l2_ack_req_mask

DESCRIPTION

  This function resets the L2 Ack required mask
    
DEPENDENCIES

  None.
 
RETURN VALUE

  void.

SIDE EFFECTS

  None.

===========================================================================*/

void rrc_reset_l2_ack_req_mask(sys_modem_as_id_e_type wrrc_as_id,  rrclcm_mui_type mui );
/*===========================================================================

FUNCTION  rrc_check_if_proc_l2_ack_req_mask_is_set

DESCRIPTION

  This function checks if L2 ack req mask is set
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrc_check_if_proc_l2_ack_req_mask_is_set(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION  rrc_check_pending_l2_ack

DESCRIPTION

  This function for any pending L2 ack
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

boolean rrc_check_for_pending_l2_ack(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================

FUNCTION  rrcwrm_reset_l2_ack_req_mask

DESCRIPTION

  This function sets the L2 Ack required mask
    
DEPENDENCIES

  None.
 
RETURN VALUE

  void.

SIDE EFFECTS

  None.

===========================================================================*/

void rrcwrm_reset_l2_ack_req_mask(sys_modem_as_id_e_type wrrc_as_id);


/*===========================================================================

FUNCTION  rrc_ds_check_for_tuneaway

DESCRIPTION

  This function checks whether is allowd or not
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

boolean rrc_ds_check_for_tuneaway(sys_modem_as_id_e_type wrrc_as_id);


/*===========================================================================

FUNCTION  rrcwrm_handle_unlock_required

DESCRIPTION

  This function handles unlock-required
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

void rrcwrm_handle_unlock_required(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION  rrcwrm_is_unlock_required_received

DESCRIPTION

  This function checks whehter the unlock type received is unlock required or not
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

boolean rrcwrm_is_unlock_required_received(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION  rrcwrm_get_tuneaway_status

DESCRIPTION
s
  This function returns tune away status
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

boolean rrcwrm_get_tuneaway_status(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================

FUNCTION  rrcwrm_get_unlock_type

DESCRIPTION
s
  This function returns unlock type
    
DEPENDENCIES

  None.
 
RETURN VALUE

  trm_unlock_event_enum_t.

SIDE EFFECTS

  None.

===========================================================================*/
__attribute__((section(".uncompressible.text")))
trm_unlock_event_enum_t rrcwrm_get_unlock_type(sys_modem_as_id_e_type wrrc_as_id);


#ifdef FEATURE_QTA
/*===========================================================================

FUNCTION  rrc_freeze_for_qta

DESCRIPTION

  This function sends START_QTA_REQ to WL1 and freezes RRC till
  it gets START_QTA_CNF.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_freeze_for_qta(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION  rrcwrm_validate_qta_handling

DESCRIPTION

  This function validates if QTA can be handled.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  Boolean.

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrcwrm_validate_qta_handling(sys_modem_as_id_e_type wrrc_as_id);
#endif

#ifdef FEATURE_TRIPLE_SIM
/*===========================================================================

FUNCTION  rrcwrm_release_lock_if_current_tuneaway_is_lta

DESCRIPTION
   This function confirms whether the current tuneaway type is still LTA or not
    before releasing the lock
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrcwrm_release_lock_if_current_tuneaway_is_lta(sys_modem_as_id_e_type wrrc_as_id);
#endif

/*===========================================================================

FUNCTION  rrc_check_and_perform_trm_exchange

DESCRIPTION

  This function checks if TRM exchange needs to be done.
  If yes, TRM is exchanged with IRAT client.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_check_and_perform_trm_exchange(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION   RRC_PROCESS_TRM_EXCHANGE

DESCRIPTION

  This function processes the necessary actions to perform a TRM exchange

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_perform_trm_exchange(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION  rrc_wrm_change_priority

DESCRIPTION

  This function requests WRM for lock with new priority
    
DEPENDENCIES

  None.
 
RETURN VALUE

  SUCCESS: RRC was granted lock.
  FAILURE: RRC failed to get the lock.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_wrm_change_priority(sys_modem_as_id_e_type wrrc_as_id, trm_reason_enum_t priority);
/*===========================================================================

FUNCTION  rrc_acquire_lock_on_redirected_freq

DESCRIPTION

  This function requests lock on redirected frequency
    
DEPENDENCIES

  None.
 
RETURN VALUE

SIDE EFFECTS

  None.

===========================================================================*/
rrc_out_of_lock_wait_e_type rrc_wrm_acquire_lock_on_redirected_freq(sys_modem_as_id_e_type wrrc_as_id, rrc_freq_type freq, trm_reason_enum_t priority, uint32 lock_wait_time);
/*===========================================================================

FUNCTION  rrc_wrm_populate_bands_for_lock_req

DESCRIPTION

  This function will populate TRM bands fon which lock is needed
    
DEPENDENCIES

  None.
 
RETURN VALUE

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_wrm_populate_bands_for_lock_req(sys_modem_as_id_e_type wrrc_as_id, wrm_freq_input_type *lock_req_bands, rrc_freq_type freq);

#ifdef FEATURE_QTA
/*===========================================================================

FUNCTION  rrcwrm_handle_qta_during_sib_read

DESCRIPTION

  reponsible for handling QTA request during SIB read
    
DEPENDENCIES

  None.
 
RETURN VALUE

SIDE EFFECTS

  None.

===========================================================================*/
void rrcwrm_handle_qta_during_sib_read(sys_modem_as_id_e_type wrrc_as_id, uint8 duration);
#endif

/*===========================================================================

FUNCTION  rrc_return_default_band_lock_req

DESCRIPTION

  reponsible for returning a valid default band from RF api
    
DEPENDENCIES

  None.
 
RETURN VALUE

SIDE EFFECTS

  None.

===========================================================================*/
sys_band_class_e_type rrc_return_default_band_lock_req(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION  RRCWRM_MAP_PRIORITY_TO_ACTIVITY

DESCRIPTION

  This function maps the TRM priority to right activity which will be passed to WRM
    
DEPENDENCIES

  None.
 
RETURN VALUE

  Activity Type

SIDE EFFECTS

  None.

===========================================================================*/
wrm_activity_enum_type rrcwrm_map_priority_to_activity(sys_modem_as_id_e_type wrrc_as_id, trm_reason_enum_t priority);
/*===========================================================================

FUNCTION  rrcwrm_stop_wl1_qta_ccch_timer

DESCRIPTION
    This function calls WL1 API to stop QTA-CCCH timer so that QTA can be allowed
  
    
DEPENDENCIES

  None.
 
RETURN VALUE

SIDE EFFECTS

  None.

===========================================================================*/
void rrcwrm_stop_wl1_qta_ccch_timer(sys_modem_as_id_e_type wrrc_as_id);

/*===========================================================================

FUNCTION  rrc_return_unlock_by_timer_val

DESCRIPTION

  This function is called by WL1 to get the remaining unlock by timer value
    
DEPENDENCIES

  None.
 
RETURN VALUE

SIDE EFFECTS

  None.

===========================================================================*/
uint64 rrc_return_unlock_by_timer_val(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================

FUNCTION  rrc_check_and_post_unlock_by_tmr_expiry_if_pending

DESCRIPTION

  This function posts unlock by timer expiry indication if it is pended
    
DEPENDENCIES

  None.
 
RETURN VALUE

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_check_and_post_unlock_by_tmr_expiry_if_pending(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================
FUNCTION rrcpg1_check_and_abort_bcch_mod_info_processing()

DESCRIPTION
This function will check & abort processing of BCCH
ModInfo & also it takes care of stopping this timer 
RRC_BCCH_MODIFICATION_TIMER if its running. 

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void rrcpg1_check_and_abort_bcch_mod_info_processing
(
sys_modem_as_id_e_type wrrc_as_id
);


/*===========================================================================
FUNCTION  rrc_return_1x_srlte_capability_ind

DESCRIPTION

  reponsible for returning the 1X SRLTE capability for the sub id passed as arguement
    
DEPENDENCIES

  None.
 
RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrc_return_1x_srlte_capability_ind(sys_modem_as_id_e_type wrrc_as_id);
/*===========================================================================

FUNCTION  rrcwrm_check_and_trash_pending_cmd

DESCRIPTION

  This function will trash pending  all cmds which were saved during DSDS Tune away
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

void rrcwrm_check_and_trash_pending_cmd
( 
  sys_modem_as_id_e_type wrrc_as_id
);

/*===========================================================================

FUNCTION  RRC_PERSISTENT_PAGE_COLLISION_EVENT_CB

DESCRIPTION

  This is a call back function to be called by WRM on receiving persistent collision  event 'TRM_ASYNC_IDLE_COLL_EVENT_INFO' from TRM.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void rrc_persistent_page_collision_event_cb(sys_modem_as_id_e_type wrrc_as_id, trm_idle_coll_event_data idle_coll_event_info);
/*===========================================================================

FUNCTION  rrc_get_unlock_req_timer_val

DESCRIPTION

  This function calculates the unlock required timer value based on the current
  OC status and the associated activation times.

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None.

===========================================================================*/
uint16 rrc_get_unlock_req_timer_val(sys_modem_as_id_e_type wrrc_as_id);
#endif

#endif  /*#ifndef RRCWRM_H*/
