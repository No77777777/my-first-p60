#ifndef TDSRRCCU_H
#define TDSRRCCU_H
/*===========================================================================
              R R C   C E L L   U P D A T E   H E A D E R   F I L E

DESCRIPTION
  This file contains data types and declarations associated with the
  Cell Update Procedure.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrccu.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/27/12   xp      Add a function call to get the status of Cell update and Ura update.
03/10/11   bj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/18/11   ad      Made changes to remove compilation error
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
11/03/10   rl      Fixed compiler warnings
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
11/10/10   bj       Deleted IE "RNC support for change of UE capability".(3GPP: 8.6.2.3)
11/03/10   zwj     Added code to handle REL4 OTA Message.

===========================================================================*/


/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcrbcommon.h"
/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/

/*Currently used Bit mask used for trimming Cell update message to UL TM RB size*/
/*This Bit mask is used to re-set trim cell update variable & also to check whether a bit is set or not */
#define TDSRRCCU_TRIM_REQ_BIT_MASK_NOT_SET                                         0x00000000
/*This Bit mask is used to indicate monitored cell info is removed in cu message*/
#define TDSRRCCU_REMOVED_MONITORED_CELL_FROM_MEAS_RPT_ON_RACH   0x00000001
/*This Bit mask is used to indicate Measurment on RACH is removed in cu message*/
#define TDSRRCCU_REMOVED_ENTIRE_MEAS_RPT_ON_RACH                               0x00000002

#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
/*This Bit mask is used to indicate some or all cell info for Inter Freq Measurment on RACH 
  is removed in cu message*/
#define TDSRRCCU_REMOVED_INTERF_MEAS_RPT_ON_RACH                               0x00000004
#endif

/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/


typedef enum 
{
  TDSRRCCU_COMPLETED,                          /* This indicates that the 
                                               Cell Update procedure is
                                               completed and UE has valid
                                               C-RNTI */ 
  TDSRRCCU_NOT_COMPLETED                       /* This indicates that the Cell
                                               Update proc is not  completed 
                                               and is in the process getting
                                               valid C-RNTI. */
  
} tdsrrccu_cell_update_complete_status_e_type; 


typedef enum 
{
  TDSRRCCU_STARTED,                            /* This indicates that the 
                                               Cell Update proc is started */ 

  TDSRRCCU_NOT_STARTED                         /* This indicates that the Cell
                                               Update proc is not started */
  
} tdsrrccu_cell_update_started_status_e_type; 

 /*==========================================================================
  Type for defining the substate of Cell Update Procedure.
===========================================================================*/

typedef enum
{
  TDSRRCCU_INITIAL,                     /* CU initial sub state    */
  TDSRRCCU_WAIT_FOR_UE_TO_CAMP_ON,      /* CU waits for UE to camp on */
  TDSRRCCU_WAIT_FOR_RB0_EST_CNF,        /* CU waits for RB0 conf from RRC LLC */
  TDSRRCCU_WAIT_FOR_RB1_RB4_EST_CNF,    /* CU waits for RB1toRB4 conf from RRC LLC */
  TDSRRCCU_WAIT_FOR_CELL_UPDATE_CONFIRM, /* CU waits for Cell Update Confirm Msg */ 
  TDSRRCCU_WAIT_FOR_URA_UPDATE_CONFIRM,
                                   
  TDSRRCCU_WAIT_FOR_RNTI_UPDATE_CNF,    /* CU waits for RNTI update */
  TDSRRCCU_WAIT_FOR_RB_EST_CNF,         /* CU waits for conf from CSP procedure */
  TDSRRCCU_WAIT_FOR_PCCH_RB_EST_CNF,    /* CU waits for PCCH channel establishment */
  TDSRRCCU_WAIT_FOR_L2_ACK,             /* CU waits for L2-ACK for Resp Msg */
  TDSRRCCU_WAIT_FOR_SRB2_RE_EST_CNF,       /* CU waits for conf from CSP procedure */
  TDSRRCCU_WAIT_FOR_RB_REL_CNF,         /* CU waits for RB Release confirm from
                                      LLC */  
  TDSRRCCU_WAIT_FOR_BPLMN_SUSPEND_CNF,    /* CU waits for BPLMN Suspend CNF */                                     
  TDSRRCCU_WAIT_FOR_PENDING_CONFIG       /* CU waits for pending configuration completion */

  , TDSRRCCU_WAIT_FOR_RB_PROC_COMPL_IND

#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH
  , TDSRRCCU_WAIT_FOR_SIB7_CURRENT
#endif /*FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH*/

} tdsrrccu_substate_e_type;

#define TDSRRC_T320_INVALID_VALUE 0xFF
/*===========================================================================
**                  CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
extern unsigned short tdsrrc_v_302;

/*===========================================================================

                      FUNCTION DECLARATIONS

=========================================================================*/


/*===========================================================================

FUNCTION tdsrrccu_start_t_305_timer

DESCRIPTION
  This function is called during transition from CELL_DCH to CELL_FACH/
  CELL_PCH/URA_PCH to start the periodic cell update timer T_305
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  tdsrrccu_start_t_305_timer(void);
/*===========================================================================

FUNCTION rrccu_check_if_rabs_need_to_be_released

DESCRIPTION
  This function returns the value of rrccu_rl_failure_info.t314_info.need_to_release_rabs
DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None
===========================================================================*/
boolean  tdsrrccu_get_rabs_need_to_be_released_T314(void);
/*===========================================================================

FUNCTION rrccu_check_if_rabs_need_to_be_released

DESCRIPTION
  This function returns the value of rrccu_rl_failure_info.t314_info.need_to_release_rabs
DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None
===========================================================================*/
boolean  tdsrrccu_get_rabs_need_to_be_released_T315(void);
/*===========================================================================

FUNCTION   RRCCU_IS_SRNS_IN_PROGRESS()

DESCRIPTION
 This function returns if SRNS is in progress with current reconfig procedure


        
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE or FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccu_is_srns_in_progress
(
  void
);
/*===========================================================================

FUNCTION RRCCU_GET_CELL_UPDATE_STARTED_STATUS

DESCRIPTION

  This function checks whether Cell Update procedure is started or not.
  
DEPENDENCIES


RETURN VALUE

  A value of tdsrrccu_cell_update_started_status_e_type indicating the started
  status of the function.

SIDE EFFECTS

  None

===========================================================================*/
tdsrrccu_cell_update_started_status_e_type tdsrrccu_get_cell_update_started_status
(    
  void
);
/*===========================================================================

FUNCTION TDSRRCCU_GET_SUBSTATE

DESCRIPTION

  This function return the current substate of CU
  
DEPENDENCIES


RETURN VALUE

  A value of tdsrrccu_substate_e_type indicating the current substate of CU
SIDE EFFECTS

  None

===========================================================================*/
tdsrrccu_substate_e_type tdsrrccu_get_substate
(
  void
);
/*===========================================================================

FUNCTION RRCCU_GET_CELL_UPDATE_COMPLETE_STATUS

DESCRIPTION

  This function checks whether Cell Update procedure is active or not and 
  returns the status to the procedure.
  
DEPENDENCIES


RETURN VALUE

  A value of tdsrrccu_cell_update_complete_status_e_type indicating the complete
  status of the function.

SIDE EFFECTS

  None

===========================================================================*/
tdsrrccu_cell_update_complete_status_e_type tdsrrccu_get_cell_update_complete_status
(    
  tdsrrc_proc_e_type rrc_proc
);
/*===========================================================================

FUNCTION tdsrrccu_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the Cell Update  procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrccu_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr
);
/*===========================================================================

FUNCTION tdsrrccu_init_procedure

DESCRIPTION
  This function will initialize the Cell Update procedure substate to 
  TDSRRCCU_INITIAL Substate. This will called in the RRCTASK modules 
  along with other Init Procedures.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrccu_init_procedure
(    
  void  
);

/*===========================================================================

FUNCTION RRCCU_REGISTER_FOR_CELL_UPDATE_TO_COMPLETE

DESCRIPTION

  This function registers RRC proceudre to inform when Cell Update is
  completed.
  
DEPENDENCIES


RETURN VALUE

  A value of uecomdef_status_e_type indicating success or failure of 
  registration..

SIDE EFFECTS

  None

===========================================================================*/
uecomdef_status_e_type tdsrrccu_register_for_cell_update_to_complete
(    
  tdsrrc_proc_e_type rrc_proc
);

/*===========================================================================
      
      FUNCTION RRCCU_CLEAR_URA_ID
      
      DESCRIPTION
        
      This function will clear the URA_ID field. Would set it to 0xffff
        
      DEPENDENCIES
        None
      
      RETURN VALUE
        None
        
      SIDE EFFECTS
      
        None
      
===========================================================================*/

void tdsrrccu_clear_ura_id
(
  void
);

/*===========================================================================
      
      FUNCTION RRCCU_UPDATE_URA_ID
      
      DESCRIPTION
        
      This function will convert the oss_bitstring to the unit32 value
	  and then stores the value of in the URA_ID field.
        
      DEPENDENCIES
        None
      
      RETURN VALUE
        None
        
      SIDE EFFECTS
      
        None
      
===========================================================================*/

void tdsrrccu_update_ura_id
(
  ASN1BitStr32 *ura_id
);


#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
/*===========================================================================
      
      FUNCTION RRCCU_GET_URA_ID
      
      DESCRIPTION
        
      This function will populate the URA_ID in the arguments passed
        
      DEPENDENCIES
        None
      
      RETURN VALUE
        None
        
      SIDE EFFECTS
      
        None
      
===========================================================================*/

void tdsrrccu_get_ura_id
(
  uint32 *ura_id
);
#endif

/*===========================================================================

FUNCTION tdsrrccu_check_rb_in_estab_rab

DESCRIPTION
  This function will check whether the rb is in established rabs, if so it returns
  true, else it returns false and clears cu state

DEPENDENCIES
  None

RETURN VALUE
  boolean
  TRUE  : rb is in established rabs
  FALSE : rb is not in established rabs 

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrrccu_check_rb_in_estab_rab
(
 void
);
 /*===========================================================================

FUNCTION RRCCU_RETURN_CU_SUBSTATE

DESCRIPTION
  This function returns the cu subtate

  
DEPENDENCIES
  None

RETURN VALUE
  cu substate
  
SIDE EFFECTS

  None

===========================================================================*/
tdsrrccu_substate_e_type tdsrrccu_return_cu_substate
(
  void
);
/*===========================================================================

FUNCTION tdsrrccu_reset_trim_bit_mask

DESCRIPTION
  This procedure Reset Cell Update Trim bit mask.


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccu_reset_trim_bit_mask
(
void
);
/*===========================================================================

FUNCTION tdsrrccu_trim_cu_msg_ul_tm_tb_size

DESCRIPTION
  This procedure is the event handler for the Cell Update procedure.


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

boolean tdsrrccu_trim_cu_msg_ul_tm_tb_size
(
  tdsrrc_UL_CCCH_Message *msg_ptr,
  uint32 pdu_length
);

/*===========================================================================

FUNCTION tdsrrccu_rlc_ul_data_ind

DESCRIPTION

  This function is used as call back function from rlc when uplink data 
  presence has to be indicated in cell_pch or ura_pch state.
  
DEPENDENCIES


RETURN VALUE
           viod


SIDE EFFECTS

  None

===========================================================================*/
#ifdef FEATURE_DSM_WM_CB
void tdsrrccu_rlc_ul_data_ind (struct dsm_watermark_type_s *, void *);
#else /*FEATURE_DSM_WM_CB*/
void tdsrrccu_rlc_ul_data_ind (void);
#endif /*FEATURE_DSM_WM_CB*/

/*===========================================================================
FUNCTION tdsrrccu_send_cell_update_initiate_req

DESCRIPTION
  This function posts a cell update request to rrccu itself
  in internal command queue. This function is used to avoid the race conditions. 

DEPENDENCIES
  None.

RETURN VALUE
  None.
===========================================================================*/
void tdsrrccu_send_cell_update_initiate_req
(
  tdsrrc_cell_update_cause_e_type cause
);

/*===========================================================================
FUNCTION       tdsrrccu_init_last_cu_cnf_info

DESCRIPTION
   Initialises all the variables, which was required to send a response to the previous CU Cnf
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_init_last_cu_cnf_info
(
  void
);

/*===========================================================================
FUNCTION RRCCU_CLEAR_PROCEDURE

DESCRIPTION
  This function will clear the global variables.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_clear_procedure
(
  void
);

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION tdsrrccu_check_if_cell_selection_in_progress

DESCRIPTION
   Checks if Cell Selection was triggerred to due to the directed cell info present 
   in CU Cnf msg.
   This is used by CCM to to send NEW_CELL_IND to CU in  this scenario
DEPENDENCIES
  None

RETURN VALUE
  TRUE : If the CELL Selection was triggered due to directed cell info in CU CNF
  else
  FALSE
SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrccu_check_if_cell_selection_in_progress
(
  void 
);

#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/

/*===========================================================================
FUNCTION   tdsrrccu_map_cu_cause

DESCRIPTION
  This function maps the OTA format CU cause type to RRC internal format CU cause.
        
DEPENDENCIES
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
tdsrrc_cell_update_cause_e_type tdsrrccu_map_cu_cause
(
  tdsrrc_CellUpdateCause rrccu_cell_update_cause
);

/*===========================================================================
FUNCTION tdsrrccu_get_cu_cause_for_wait_timer

DESCRIPTION
  Returns the cause for which CU was requested when the wait timer was running. Basically it is the pending CU cause.
  
DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
tdsrrc_cell_update_cause_e_type tdsrrccu_get_cu_cause_for_wait_timer
(
  void
);

/*===========================================================================
FUNCTION tdsrrccu_reset_cu_cause_for_wait_timer

DESCRIPTION
  Resets cu cause for wait timer stored in CU internal DB.

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_reset_cu_cause_for_wait_timer
(
  void
);

/*===========================================================================
FUNCTION tdsrrccu_get_cu_wait_timer_value

DESCRIPTION
  Returns the value of the wait timer that was given by N\W in PCH state.
  
DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
uint8 tdsrrccu_get_cu_wait_timer_value
(
  void
);

/*===========================================================================
FUNCTION tdsrrccu_reset_cu_wait_timer_value

DESCRIPTION
  Resets wait timer value in CU internal DB.
  
DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrccu_reset_cu_wait_timer_value
(
  void
);


/*===========================================================================

FUNCTION RRCCU_PROCESS_CELL_UPDATE_CONFIRM_R4

DESCRIPTION
  This function processes the REl 5 CELL UPDATE CONFIRM message received in response to
  Cell Update message.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_process_cell_update_confirm_r4
(    
  void *msg_ptr,                              /* Pointer to Downlink Msg */
  uint32 dl_sdu_type                          /* Downlink SDU type */
);


/*===========================================================================

FUNCTION tdsrrccu_validate_rel_rb_exist_in_reconfig_list_r4

DESCRIPTION
  This function checks if the Cu Cnf has the same RB ID in the RB Release list
   and also in RB Reconfig/RB Affected List
  
DEPENDENCIES
  None

RETURN VALUE
  True, if same RB ID is present in RB Release list and also in RB Reconfig/RB Affected List
  else
  FALSE
  
SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrccu_validate_rel_rb_exist_in_reconfig_list_r4
(
  tdsrrc_CellUpdateConfirm_r4_IEs * cu_confirm_ptr
);

/*===========================================================================

FUNCTION tdsrrccu_get_cu_uu_status

DESCRIPTION
  This function checks if cell update or ura update is started for filter 
  CCCH OTA message

  
DEPENDENCIES
  None

RETURN VALUE
  True, ether cell update started or ura update started.
  else
  FALSE
  
SIDE EFFECTS

  None

===========================================================================*/
extern boolean tdsrrccu_get_cu_uu_status
(
  void
);
#ifdef FEATURE_TDSCDMA_RCR_CANCEL_RECFG
/*===========================================================================

FUNCTION RRCCU_CHECK_PROC_WAITING_FOR_CHAN_CONFIG_CNF

DESCRIPTION
  This function will check the status of the procedure that has set the
  oc  

DEPENDENCIES
  None

RETURN VALUE
  boolean
  TRUE  :  procedure waiting for ch config cnf 
  FALSE :  procedure in some other state. 

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrrccu_check_proc_waiting_for_chan_config_cnf 
(
tdsrrc_proc_e_type proc_id
);
#endif
/*===========================================================================

FUNCTION tdsrrccu_clear_pending_cu_procedure

DESCRIPTION
  This function clear the CU state, variable.
  
DEPENDENCIES


RETURN VALUE
  void


SIDE EFFECTS

  None

===========================================================================*/
void tdsrrccu_clear_pending_cu_procedure
(
void
);
/*===========================================================================

FUNCTION tdsrrccu_get_reestablish_timer

DESCRIPTION
  This function get the re-establish timer length.
  
DEPENDENCIES


RETURN VALUE
  void


SIDE EFFECTS

  None

===========================================================================*/
uint32 tdsrrccu_get_reestablish_timer
(
void
);


/*===========================================================================

FUNCTION tdsrrccu_check_cu_active_before_lock_rel

DESCRIPTION 
  This procedure is used to query if CU is active for UL Data Transmission,
  Periodic Cell Update, Paging Response 

DEPENDENCIES
    None.

RETURN VALUE
   NONE
successful.

===========================================================================*/

boolean tdsrrccu_check_cu_active_before_lock_rel
(
  void
);

/*===========================================================================
      
      FUNCTION tdsrrccu_post_buffered_commands
      
      DESCRIPTION
        
      This function will post any commands buffered when.
      UE was in standby mode
        
      DEPENDENCIES
        None
      
      RETURN VALUE
        None
        
      SIDE EFFECTS
      
        None
      
===========================================================================*/
void tdsrrccu_post_buffered_commands
(
  void
);

/*===========================================================================

FUNCTION tdsrrccu_check_and_set_v302_when_T302_expired_during_acquire_lock

DESCRIPTION 
  This procedure is used to check and set v302,if it exeed n302,set tdsrrc_reestablish_timer_out to TRUE

DEPENDENCIES
    None.

RETURN VALUE
   NONE
successful.

===========================================================================*/

void tdsrrccu_check_and_set_v302_when_T302_expired_during_acquire_lock(void);

/*===========================================================================

FUNCTION RRCCU_CONVERT_T302_TO_MS

DESCRIPTION
  This function converts the tdsrrc_T_302 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsrrccu_convert_t302_to_ms
(
  tdsrrc_T_302 rrccu_t_302                    /* enum for Timer T302 */
);


#endif /* TDSRRCCU_H */
