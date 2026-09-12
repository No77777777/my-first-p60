#ifndef TDSRRCRCE_H
#define TDSRRCRCE_H
/*===========================================================================
              R R C R C E  H E A D E R  F I L E

DESCRIPTION
 The RRCRCE module consists of RRC CONNECTION ESTABLISHMENT PROCEDURE 
 implementation. The 3GPP TS 25.331 RRC Protocol specification is used as
 basis for implementation.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrce.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

#include "tdscdma_variation.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcrcei.h"
/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/
/*Currently used Bit mask used for trimming RRC CONNECTION REQUEST message to UL TM RB size*/
/*This Bit mask is used to re-set trim RRC CONNECTION REQUEST variable & also to check whether a bit is set or not */
#define TDSRRCRCE_TRIM_REQ_BIT_MASK_NOT_SET                                         0x00000000
  /*This Bit mask is used to indicate monitored cell info is removed in RCE message*/
#define TDSRRCRCE_REMOVED_MONITORED_CELL_FROM_MEAS_RPT_ON_RACH   0x00000001
  /*This Bit mask is used to indicate Measurment on RACH is removed in RCE message*/
#define TDSRRCRCE_REMOVED_ENTIRE_MEAS_RPT_ON_RACH                               0x00000002
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
  /*This Bit mask is used to indicate some or all cell info for Inter Freq Measurment on RACH 
    is removed in RCE message*/
#define TDSRRCRCE_REMOVED_INTERF_MEAS_RPT_ON_RACH                               0x00000004
#endif
#define TDSRRCTMR_WRM_LOCK_WAIT_TIMER_FOR_EST_REQ_IN_MS 30000

/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB

typedef enum
{
  CSFB_INVALID, /* RRC Connection is not established */
  CSFB_STARTED,
  CSFB_CS_RB_RELEASED /* Set when CS call released for a CSFB Connection */
}tdsrrcrce_csfb_status_e_type;

#endif

typedef enum
{
  TDSRRCRCE_RRC_CONN_NOT_ESTABLISHED, /* RRC Connection is not established */

  TDSRRCRCE_RRC_CONN_ESTABLISHED      /* RRC Connection is established */
}tdsrrcrce_rrc_conn_status_e_type;

typedef enum
{
  TDSRRCRCE_NONE_CALL_PENDING,
  TDSRRCRCE_MO_CALL_PENDING,
  TDSRRCRCE_MT_CALL_PENDING,
  TDSRRCRCE_OTHER_CALL_PENDING
}tdsrrcrce_call_pending_e_type;

/* Used to store the RRC Connection Setup message when UE receives
 * redirection information with freq+cpid+crnti information and UTRAN
 * expects UE to directly enter Connected mode if destination state
 * is CELL_FACH
 */
typedef struct
{
  uint32 dl_sdu_length;   /* Length of downlink sdu in bytes */
  uint8 *dl_sdu;          /* Pointer to the Downlink SDU. */
  tdsrrc_DL_CCCH_Message *decoded_msg; /* Pointer to the decoded msg */
  uint32 dl_sdu_type;     /* type of dl sdu */
}tdsdl_ccch_msg_type;


/*===========================================================================
**                      TYPE DECLARATIONS
**=========================================================================*/

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

extern unsigned short tdsrrc_v_300;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION tdsrrcrce_init_procedure

DESCRIPTION
  This function will initialize the RRC Connection Establishment procedure
  substate to TDSRRCRCE_INITIAL Substate. This will called in the RRCTASK 
  modules along with other Init Procedures.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrcrce_init_procedure
(    
  void  
);
/*===========================================================================

FUNCTION tdsrrcrce_procedure_event_handler

DESCRIPTION
  This is the entry point for RRC Connection Establishment procedure. All
  events for RCE come to this function from RRC Dispatcher. Depending on the
  UE state of RRC Connection Procedure, it calls the corresponding state 
  handler. These state handlers call substate handlers depending on the 
  the current substate of RRC Connection Establishment procedure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================
FUNCTION tdsrrcrce_get_rrc_connection_status

DESCRIPTION
  This function looks into RRC Connection Establishment provcedure's 
  sub-states and returns the status of RRC Connection.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_rrc_conn_status_e_type: Returns TDSRRCRCE_RRC_CONN_ESTABLISHED if RRC
  Connection Establishment procedure is in TDSRRCRCE_FINAL substate. Otherwise
  it returns TDSRRCRCE_RRC_CONN_NOT_ESTABLISHED.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_rrc_conn_status_e_type tdsrrcrce_get_rrc_connection_status
(
  void                  
);

/*===========================================================================
FUNCTION tdsrrcrce_is_rce_procedure_inactive

DESCRIPTION
  This function looks into RRC Connection Establishment provcedure's 
  sub-states and returns the status of RRC Connection Establishment
  Procedure.

DEPENDENCIES
  None

RETURN VALUE
  boolean: TRUE if RCE is still inactive. (Note - when RCE is waiting for 
           UE to camp, the function will still return TRUE) 
           FALSE when RCE is not in Initial substate and UE is camped on 
           a cell.

SIDE EFFECTS
  To be used only by CSP as the logic is designed for CSP's requirement.
===========================================================================*/
boolean  tdsrrcrce_is_rce_procedure_inactive
(
  void
);

/*===========================================================================
FUNCTION tdsrrcrce_change_substate_to_rrcrce_final

DESCRIPTION
  This function changes the substate of RRC Connection Establishment provcedure's 
  to TDSRRCRCE_FINAL

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  To be used only during G2W Handover and should not be used for any other 
  purpose.
===========================================================================*/
void  tdsrrcrce_change_substate_to_rrcrce_final
(
  void
);
/*===========================================================================
FUNCTION tdsrrcrce_force_initialize

DESCRIPTION
  This function changes the substate of RRC Connection Establishment provcedure's 
  to TDSRRCRCE_INITIAL and sends the required Indication/Response to MM depending
  on RRCRCE substate. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  To be used only during Mode Change and when an Out of Service Area is indicated
  to MM while in IDLE DISCONNECTED state.
  It should not be used for any other purpose.
===========================================================================*/

void tdsrrcrce_force_initialize
(
  boolean initiate_cell_selection
);

/*===========================================================================

FUNCTION tdsrrcrce_verify_ue_identity

DESCRIPTION
  This function checks whether UE identity in the downlink CCCH message is 
  same as UE initial identity.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: This returns TDSRRCRCE_SUCCESS if the match occurs. 
  Otherwise it returns TDSRRCRCE_FAILURE_MINOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_verify_ue_identity
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to the received 
                                                command */
);

/*===========================================================================
FUNCTION RRCRCE_IS_RCE_PROCEDURE_WAITING_FOR_BPLMN

DESCRIPTION
  This function looks into RRC Connection Establishment procedure's 
  sub-states and returns the status of RRC Connection Establishment
  Procedure.

DEPENDENCIES
  None

RETURN VALUE
  boolean: 
           TRUE  - if RCE is waiting for BPLMN search to be suspended/aborted.
           FALSE - otherwise
           
SIDE EFFECTS
  To be used only by CSP for BPLMN search as the logic is designed for CSP's requirement.
===========================================================================*/
boolean tdsrrcrce_is_rce_procedure_waiting_for_bplmn(void);




/*===========================================================================

FUNCTION rrcrce_verify_ue_identity_later_than_r3

DESCRIPTION
  This function checks whether UE identity in the Release 5 downlink CCCH message is 
  same as UE initial identity.

DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrce_status_e_type: This returns TDSRRCRCE_SUCCESS if the match occurs. 
  Otherwise it returns TDSRRCRCE_FAILURE_MINOR.

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrce_status_e_type tdsrrcrce_verify_ue_identity_later_than_r3
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to the received 
                                                command */
);


/*===========================================================================

FUNCTION   tdsrrcrce_get_sib16_ptr

DESCRIPTION

  This function gets the Sib16 ptr either from the actual sib16 or the
  SIB16 version of Default Configuration
  
DEPENDENCIES

  None.
 
RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
struct tdsrrc_SysInfoType16 * tdsrrcrce_get_sib16_ptr(void);

/*===========================================================================

FUNCTION   tdsrrcrce_get_rb_setup_r6_ptr_for_dc

DESCRIPTION

  This function gets the default config ptr from RB setup r6 I.E.
  
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrc_RadioBearerSetup_r6_IEs pointer

SIDE EFFECTS

  None.

===========================================================================*/
struct tdsrrc_RadioBearerSetup_r6_IEs * tdsrrcrce_get_rb_setup_r6_ptr_for_dc
(
  void
);

/*===========================================================================

FUNCTION   tdsrrcrce_is_dc_config_on_hspa

DESCRIPTION

  This function will indicate if DC will configure RB's/SRB's on HSPA channel's
  i.e. EUL in UL & HSDPA on DL. Currently only DC-17 does config SRB's on 
   HSPA channel's.
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE -If DC will configure RB's/SRB's on HSPA channel's
  FASLE - other wise
  
SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcrce_is_dc_config_on_hspa
(
  void
);

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*===========================================================================

FUNCTION   tdsrrcrce_get_rbs_ptr

DESCRIPTION

  This function gets the RBS ptr either from the actual RBS or the
  RBS version of Default Configuration
  
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrc_DL_DCCH_Message pointer

SIDE EFFECTS

  None.

===========================================================================*/
struct tdsrrc_DL_DCCH_Message * tdsrrcrce_get_rbs_ptr
(
  void
);
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

/*===========================================================================
FUNCTION tdsrrcrce_get_rrc_est_cause

DESCRIPTION
  Returns establishment cause

DEPENDENCIES
  None

RETURN VALUE
  Returns establishment cause

SIDE EFFECTS
  None
===========================================================================*/
rrc_establish_cause_e_type tdsrrcrce_get_rrc_est_cause
(
  void
);
#ifdef FEATURE_TDSCDMA_MTPL_THERMAL_BACKOFF 
/*===========================================================================
FUNCTION tdsrrcrce_set_rrc_est_cause_for_emergency_call

DESCRIPTION
  Sets the establishment cause to RRC_EST_EMERGENCY_CALL

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_set_rrc_est_cause_for_emergency_call
(
  void
);
#endif
/*===========================================================================
FUNCTION tdsrrcrce_get_rce_sub_state()

DESCRIPTION
  This function lwill return current RRC Connection Establishment 
  procedure's sub state.
DEPENDENCIES
  None

RETURN VALUE
  return current rce sub state.
SIDE EFFECTS
none
===========================================================================*/
tdsrrcrce_substate_e_type  tdsrrcrce_get_rce_sub_state
(
void
);

/*===========================================================================
FUNCTION tdsrrcrce_check_for_rai_mismatch()

DESCRIPTION
  This function compares the camped on LAI/RAI with the one given in buffered EST_REQ to decide
  on whether to proceed with EST_REQ or reject it.
DEPENDENCIES
  None

RETURN VALUE
  TRUE: Mismatch and reject the buffered EST_REQ.
SIDE EFFECTS
none
===========================================================================*/
boolean tdsrrcrce_check_for_rai_mismatch
(
rrc_plmn_identity_type camped_plmn
);

/*===========================================================================
FUNCTION    tdsrrcrce_send_conn_rel_cause_event 

DESCRIPTION
  This function will log the event named EVENT_WCDMA_CONN_REL_CAUSE to diag task.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcrce_send_conn_rel_cause_event
(
  uint8 conn_rel_cause
);

/*===========================================================================
FUNCTION    tdsrrcrce_send_conn_req_cause_event 

DESCRIPTION
  This function will log the event named EVENT_WCDMA_CONN_REQ_CAUSE to diag task.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrce_send_conn_req_cause_event
(
  uint8 conn_req_cause
);

/*===========================================================================

FUNCTION tdsrrcrce_trim_rce_msg_ul_tm_tb_size

DESCRIPTION
  This procedure is the event handler for the RRC CONNECTION REQUEST procedure.


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

boolean tdsrrcrce_trim_rce_msg_ul_tm_tb_size
(
  tdsrrc_UL_CCCH_Message *msg_ptr,
  uint32 pdu_length
);

/*===========================================================================

FUNCTION tdsrrcrce_reset_trim_bit_mask

DESCRIPTION
  This procedure Reset RRC CONNECTION REQUEST Trim bit mask.


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcrce_reset_trim_bit_mask
(
void
);

/*===========================================================================

FUNCTION tdsrrcrce_get_rcsc_status

DESCRIPTION
  This function get status whether ACK is received for RRC Connection setup 
  complete .


DEPENDENCIES
  None.

RETURN VALUE
  TRUE: ACK is received for RRC Connection setup complete.

===========================================================================*/
boolean tdsrrcrce_get_rcsc_status
(
void
);

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
/*===========================================================================

FUNCTION tdsrrcrce_post_internal_csfb_abort_cmd

DESCRIPTION
  This function posts internal command to trigger RRC connection.
   abort when the CSFB CS call ends.


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcrce_post_internal_csfb_abort_cmd(void);
#endif



/*===========================================================================

FUNCTION tdsrrcrce_is_call_pending

DESCRIPTION
  This function is called by CSP to check if the RCE is in
  call pending state


DEPENDENCIES
  None.

RETURN VALUE
  TDSRRCRCE_NONE_CALL_PENDING,
  TDSRRCRCE_MO_CALL_PENDING,
  TDSRRCRCE_MT_CALL_PENDING,
  TDSRRCRCE_OTHER_CALL_PENDING

===========================================================================*/
tdsrrcrce_call_pending_e_type tdsrrcrce_is_call_pending(void);

/*===========================================================================

FUNCTION tdsrrcrce_is_cs_call_pending

DESCRIPTION
  This function is called by CSP to check if the RCE is in
  CS call pending state


DEPENDENCIES
  None.

RETURN VALUE
  TRUE: CS call is pending.
  FALSE: CS call is not pending

===========================================================================*/
boolean tdsrrcrce_is_cs_call_pending(void);

#ifdef  FEATURE_TDSCDMA_SPECIFIC_SIB_READING
/*===========================================================================

FUNCTION tdsrrcrce_is_waiting_for_sib7_update

DESCRIPTION
   This function is used to check whether it is waiting for SIB7 udpate.


DEPENDENCIES
  TRUE : RRC is waiting for SIB7

RETURN VALUE
  None.

===========================================================================*/
boolean tdsrrcrce_is_waiting_for_sib7_update
(
void
);
#endif
/*===========================================================================

FUNCTION tdsrrcrce_get_rrc_abort_cnf_status

DESCRIPTION
   This function is used to get the rrc_abort_cnf status.


DEPENDENCIES
  TRUE : rrc need to send the abort_cnf

RETURN VALUE
  None.

===========================================================================*/
boolean tdsrrcrce_get_rrc_abort_cnf_status
(
void
);
/*===========================================================================

FUNCTION tdsrrcrce_get_rce_ta_allow_status

DESCRIPTION
   This function is used to get the tdsrrcrce_allow_ta_flag status.


DEPENDENCIES
  TRUE : rrc allow the TA at connecting state.

RETURN VALUE
  None.

===========================================================================*/
boolean tdsrrcrce_get_rce_ta_allow_status
(
 void
);

/*===========================================================================

FUNCTION tdsrrcrce_is_good_freq_exist_in_mset

DESCRIPTION
  This function check whether exist good frequency in MSET


DEPENDENCIES
  None.

RETURN VALUE
  TURE: exist good freq rscp > bar_cell_rscp_threshold
  FALSE: no freq rscp > bar_cell_rscp_threshold

===========================================================================*/
boolean tdsrrcrce_is_good_freq_exist_in_mset
(
 void
);
#endif /* TDSRRCRCE_H */

