#ifndef TDSRRCWRM_H
#define TDSRRCWRM_H

/*===========================================================================
            RRC  WCDMA  RESOURCE  MANAGER  HEADER  FILE
DESCRIPTION

   This file contains all prototypes used by RRC WRM procedure
EXTERNALIZED FUNCTIONS



INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcwrm.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "tdsuecomdef.h"
#include "tdsrrccmd_v.h"
#include "tdstrm.h"
#include "tdsl1trm.h"
#include "tdsl1dsds.h"
#include "tdsl1rrcif.h"
#include "tdsrrclogging.h"

#include "trm.h"
#include "tdsrrclcm.h" 
/* This is introduced to increase the manul plmn success rate as TD gets lock consitently */
#define TDSRRC_WRM_LOCK_WAIT_TIME_FOR_ACQUISTION 100
#define TDSRRC_WRM_LOCK_WAIT_TIME_FOR_SPLIT_ACQ 50
#define TIMETICK_MS_FROM_SCLK(sclk) ((sclk)*1000/TIMETICK_NOMINAL_FREQ_HZ)
extern tdsrrc_tuneaway_block_reason_type ta_block_reason;
extern tdsrrc_tuneaway_type ta_type;
extern tdscdma_dsds_rrc_lta_log_packet_type lta_log_record;
extern uint32  lta_duration;
extern int64   lta_start_time;
extern int64   lta_end_time;
extern tdsrrc_state_e_type ta_state;
extern trm_unlock_event_enum_t tdsrrcwrm_unlock_type;
extern uint16 tdsrrc_QTA_overhead_time;

#define TDSRRC_TRM_EXCHANGE_RX(as_id) \
  (((as_id) == SYS_MODEM_AS_ID_1)? tdrm_exchange(TDRM_CLIENT_RRC,TRM_IRAT):tdrm_exchange(TDRM_CLIENT_RRC,TRM_IRAT2))
#define TDSRRC_TRM_EXCHANGE_TX(as_id) \
  (((as_id) == SYS_MODEM_AS_ID_1)? tdrm_exchange(TDRM_CLIENT_TX,TRM_IRAT_TX):tdrm_exchange(TDRM_CLIENT_TX,TRM_IRAT2_TX))


extern boolean tdsrrc_wrm_wait_for_modify_info;


/*==================================================
                 TYPE DEFINITIONS
==================================================*/

typedef enum
{
  TDSRRC_WRM_LOCK_ACQUIRED,
  TDSRRC_WRM_LOCK_NOT_ACQUIRED,
  TDSRRC_WRM_TIMER_EXPIRED,
  TDSRRC_ABORT_FROM_HIGHER_LAYERS,
  TDSRRC_NO_RESOURCE_FROM_L1,
  TDSRRC_ABORT_FROM_RR,
  TDSRRC_WRM_LOCK_MAX
} tdsrrc_out_of_lock_wait_e_type;


typedef enum
{
  TDSRRC_WRM_TIMER_BY_NONE,
  TDSRRC_WRM_TIMER_BY_UNLOCK_BY,
  TDSRRC_WRM_TIMER_BY_UNLOCK_REQUIRED
}tdsrrc_unlock_by_timer_flag_e_type;



/*==================================================
                 FUNCTION DECALARATIONS
==================================================*/

/*===========================================================================

FUNCTION  TDSRRCWRM_CHECK_L1_RELEASE_RRC_LOCK_FLAG

DESCRIPTION

  This function returns the value of tdsrrc_wrm_lock_releassed_by_l1 flag.

DEPENDENCIES

  None.

RETURN VALUE

  Boolean

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcwrm_check_l1_release_rrc_lock_flag(void);

/*===========================================================================

FUNCTION  TDSRRCWRM_RESEET_L1_RELEASE_RRC_LOCK_FLAG

DESCRIPTION

  This function clears the history about previous call of tdsrrcwrm_release_rrc_lock_on_l1_req by L1.

DEPENDENCIES

  None.

RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcwrm_reset_l1_release_rrc_lock_flag(void);

/*===========================================================================

FUNCTION  TDSRRCWRM_GET_PRIORITY_REASON_FOR_LOCK_UPDATE

DESCRIPTION

  This function returns the value of global tdsrrc_wrm_priority_for_lock_update.

DEPENDENCIES

  None.

RETURN VALUE

  trm_reason_enum_t

SIDE EFFECTS

  None.

===========================================================================*/
trm_reason_enum_t tdsrrcwrm_get_priority_reason_for_lock_update(void);

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
void tdsrrcwrm_release_lock(void);


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
tdsrrc_out_of_lock_wait_e_type  tdsrrcwrm_acquire_wrm_lock(trm_reason_enum_t priority,uint32 lock_time, boolean force_to_send_out);

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
void tdsrrcwrm_start_lock_wait_timer (uint32 lock_wait_time,boolean stop_if_already_running);

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
void tdsrrcwrm_stop_lock_wait_timer(void);

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
uecomdef_status_e_type tdsrrcwrm_acquire_lock_for_est_req
(
  rrc_cn_domain_identity_e_type est_cn_domain_id,
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
uecomdef_status_e_type tdsrrcwrm_acquire_lock_for_pch_to_fach(void);


/*===========================================================================

FUNCTION  tdsrrcwrm_acquire_lock_for_resuming_on_tds

DESCRIPTION

  This function requests WRM for lock when resuming on TDS and RRC is in
  DCH or FACH state.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  SUCCESS: RRC was granted lock.
  FAILURE: RRC failed to get the lock.

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type tdsrrcwrm_acquire_lock_for_resuming_on_tds(void);



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
uecomdef_status_e_type tdsrrcwrm_check_lock_can_be_acquired(uint32 lock_time, trm_reason_enum_t priority);


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
tdsrrc_out_of_lock_wait_e_type tdsrrcwrm_acquire_lock_for_IHO_or_CCO
(
  uint32 lock_wait_time,
  boolean force_to_send_out
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
void tdsrrcwrm_post_preemption_indication
(
  void
);
/*===========================================================================

FUNCTION  tdsrrcwrm_block_wait_to_acquire_lock_for_time

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
uecomdef_status_e_type tdsrrcwrm_block_wait_to_acquire_lock_for_time(uint32 lock_time, trm_reason_enum_t reason_type, boolean *is_abort);

/*===========================================================================

FUNCTION  tdsrrctdrm_modify_info_callback

DESCRIPTION

  This is a call back function to be used by TRM to indicate that modify info (duration change, priority change, etc.) is done
  This function then sets the signal TDSRRC_WRM_INFO_SIG.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrctdrm_modify_info_callback
(
  /* The client which is being informed of the modifiy info result */
  trm_client_enum_t               client,

  /* The result of modify info request */
  trm_modify_return_enum_type     result

);


/*===========================================================================

FUNCTION  tdsrrc_set_l2_ack_req_mask

DESCRIPTION

  This function sets the L2 Ack required mask
    
DEPENDENCIES

  None.
 
RETURN VALUE

  void.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrc_set_l2_ack_req_mask( tdsrrc_proc_e_type rrc_proc_name , tdsrrclcm_mui_type mui);


/*===========================================================================

FUNCTION  tdsrrc_reset_l2_ack_req_mask

DESCRIPTION

  This function resets the L2 Ack required mask
    
DEPENDENCIES

  None.
 
RETURN VALUE

  void.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrc_reset_l2_ack_req_mask( tdsrrclcm_mui_type mui );

/*===========================================================================

FUNCTION  tdsrrc_check_pending_l2_ack

DESCRIPTION

  This function for any pending L2 ack
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

boolean tdsrrc_check_for_pending_l2_ack(void);
/*===========================================================================

FUNCTION  tdsrrcwrm_reset_l2_ack_req_mask

DESCRIPTION

  This function sets the L2 Ack required mask
    
DEPENDENCIES

  None.
 
RETURN VALUE

  void.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcwrm_reset_l2_ack_req_mask( void );


/*===========================================================================

FUNCTION  tdsrrc_ds_check_for_tuneaway

DESCRIPTION

  This function checks whether is allowd or not
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

boolean tdsrrc_ds_check_for_tuneaway(void);


/*===========================================================================

FUNCTION  tdsrrcwrm_handle_unlock_required

DESCRIPTION

  This function handles unlock-required
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcwrm_handle_unlock_required(void);

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

boolean tdsrrcwrm_is_unlock_required_received(void);


/*===========================================================================

FUNCTION  tdsrrcwrm_get_tuneaway_status

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

boolean tdsrrcwrm_get_tuneaway_status(void);

/*===========================================================================

FUNCTION  tdsrrc_freeze_for_qta

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
void tdsrrc_freeze_for_qta(void);



/*===========================================================================

FUNCTION  tdsrrcwrm_validate_qta_handling

DESCRIPTION

  This function validates if QTA can be handled.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  Boolean.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcwrm_validate_qta_handling(void);


/*===========================================================================

FUNCTION  tdsrrc_start_unlock_by_timer_for_tune_away

DESCRIPTION

  This function start unlock by timer which triggers tunaway from wcdma
    
DEPENDENCIES
  None.
 
RETURN VALUE

 None
 
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_start_unlock_by_timer_for_tune_away
(
  /* Sclk timestamp for TRM_UNLOCK_BY */
  uint32                          unlock_by_sclk
);




/*===========================================================================

FUNCTION  tdsrrc_tuneaway_block_reason_type

DESCRIPTION

  This function start set Tune Away block reason
    
DEPENDENCIES
  None.
 
RETURN VALUE

 None
 
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrc_set_ta_block_reason
(
  tdsrrc_tuneaway_block_reason_type block_reason
);
/*===========================================================================

FUNCTION  tdsrrc_check_and_perform_trm_exchange

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
void tdsrrc_check_and_perform_trm_exchange(trm_reason_enum_t reason);
/*===========================================================================

FUNCTION  tdsrrc_t2x_trm_exchange_needed

DESCRIPTION

  This function checks if TRM exchange is needed for t2x IRAT.
  If yes, TRM will be exchanged with IRAT client.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRM exchange is needed or not for this t2x IRAT.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrc_t2x_trm_exchange_needed
(
  tdsrrc_csp_interrat_rat_e_type rat
);

/*===========================================================================

FUNCTION  tdsrrc_trm_exchange

DESCRIPTION

  This function checks if irat\irat_tx client has lock, then do trm exchange, the status of exchange of irat\irat_tx is returned through
  function parameter
    
DEPENDENCIES

  None.
 
RETURN VALUE
  exchange succeeds with IRAT or IRAT_TX

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrc_trm_exchange
(
  boolean *irat_exchange_suc, 
  boolean *irat_tx_exchange_suc
);
/*===========================================================================

FUNCTION  tdsrrc_trm_exchange_out

DESCRIPTION

  This function is used to do trm exchange for T2X procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE
  None

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_trm_exchange_out(tdsrrc_csp_interrat_rat_e_type rat);

/*===========================================================================

FUNCTION  tdsrrc_wrm_change_priority

DESCRIPTION

  This function requests WRM for lock with new priority
    
DEPENDENCIES

  None.
 
RETURN VALUE

  void

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_wrm_change_priority
(
  trm_reason_enum_t priority, 
  boolean is_irat_procedure
);
/*===========================================================================

FUNCTION  tdsrrcwrm_is_valid_proc

DESCRIPTION

  This function is used to whether it is a valid proc for TDS
    
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean

SIDE EFFECTS



===========================================================================*/
boolean tdsrrcwrm_is_valid_proc
(
  sys_proc_type_e_type proc_type
);

/*===========================================================================

FUNCTION  tdsrrcwrm_proc_map_priority

DESCRIPTION

  This function is used to map proc to priority
    
DEPENDENCIES

  None.
 
RETURN VALUE

trm_sub_reason_enum_t

SIDE EFFECTS

  

===========================================================================*/
trm_reason_enum_t tdsrrcwrm_proc_map_priority
(
  void
);

/*===========================================================================

FUNCTION  tdsrrcwrm_proc_map_priority_wt_timer

DESCRIPTION

  This function is used to map proc to priority when wt_timer is running
    
DEPENDENCIES

  None.
 
RETURN VALUE

trm_sub_reason_enum_t

SIDE EFFECTS

  

===========================================================================*/
trm_reason_enum_t tdsrrcwrm_proc_map_priority_wt_timer
(
  void
);

/*===========================================================================

FUNCTION  tdsrrcwrm_pass_proc_to_trm

DESCRIPTION

  This function is used to pass proc to trm
    
DEPENDENCIES

  None.
 
RETURN VALUE

sys_proc_type_e_type

SIDE EFFECTS

  

===========================================================================*/
sys_proc_type_e_type tdsrrcwrm_pass_proc_to_trm
(
  trm_reason_enum_t priority
);

#endif  /*#ifndef RRCWRM_H*/
