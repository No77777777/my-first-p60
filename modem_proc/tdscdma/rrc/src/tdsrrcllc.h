#ifndef TDSRRCLLC_H
#define TDSRRCLLC_H
/*===========================================================================

                    L3 - RRC Lower Layer Controller Interface

DESCRIPTION
  This file contains global declarations and external references
  for the RRC Lower Layer Controller (LLC) interface.
  

  The functions that are to be called from external modules are:
  
  - tdsrrcllc_init(void)

    This function initializes the RRC LLC command queues, the BCH data base,
    the CCTrCH Ids, the Lower Layer confirm/indication pending flag, and the 
    ordered config data base.
  
    This function should only be called when RRC is being initialized,
    i.e. after a power up reset, or during an RRC reset.
  
    There is no return value.
  
    See the comments in the function for more detailed information.

  - tdsrrcllc_process_cmd(tdsrrc_cmd_type *cmd_ptr)

    This function is called whenever there is a command for LLC to 
    process. A list of command types that can be processed by LLC are:
    
    - All *_req and *_rsp types going to L1, MAC, and RLC that are generated
      internally by other RRC procedures.
      
    - All *_cnf and *_ind types coming from L1, MAC, and RLC that are
      required by other procedures.
      
    - The general flow (in psuedo code) of processing is as follows:
    
      // This code in tdsrrcllc_init()
      lower_layer_confirm_is_pending = FALSE;
      
      tdsrrcllc_process_cmd(cmd_ptr)
      {
        if(cmd_ptr == Internal_command)
        {
          llc_cmd_q_ptr->contents = cmd_ptr->contents;
        }

        else if (cmd_ptr->cmd_id == Lower_Layer_cnf_or_ind)
        {
          // process can change lower_layer_confirm_is_pending to FALSE
          // if a *_cnf is received.
          rrcllc_process_*_cnf_or_ind(cmd_ptr);
        }    
        
        while (llc_cmd_q_ptr != NULL && !lower_layer_confirm_is_pending)
        {
            // process can change lower_layer_confirm_is_pending to TRUE
            rrcllc_process_int_cmd();

        } // end while

      } // end tdsrrcllc_process_cmd()

    There is no return value.
    
    See the comments in the function for more detailed information.

   - tdsrrcllc_set_ordered_config(tdsrrc_state_e_type next_state, 
                              uint32 dl_sdu_num, void *msg_ptr)
  
    This function takes a pointer to the ASN.1 decoded message, the next
    state, and the downlink SDU number as parameters. It then fills in the 
    ordered config database. After the order config data base is filled in,
    status is then returned to the calling function as to the success
    or failure to fill in the ordered config database.

    The return type is of type tdsrrcllc_oc_status_e_type. This indicates
    if TDSORDERED_CONFIG is set and if not, what the current failure
    status is. The calling procedure then needs to act accordingly.
    
    Once TDSORDERED_CONFIG is set, rrcllc_channel_config() is called with
    LLC using the newly programed data to configure the lower layers with
    TDSORDERED_CONFIG.
    
    TDSORDERED_CONFIG is created.....

    * Idle-Disconnected to Idle-Connecting

    ************ Procedure calls rrcllc_chan_config_req() *************
    1) Copy SIB5 data to TDSCURRENT_CONFIG (Note SIB5 data is already in
       TDSCURRENT_CONFIG, courtesy the PCH setup), unless a SIB change
       has occurred.
    2) Config Lower Layers (via primitives with CURRENT_CONFIG_PTR),
       note this is for PRACH/RACH/CCCH Uplink only, while enabling
       FACH Transport blocks, i.e. RLC-UL, MAC-UL, and PHYCHAN-PRACH.
    3) Wait for Lower Layer confirms
    4) Send Channel Confirm to RRC Connection Establishment

    * Idle-Connecting to CELL_DCH

    ************ Procedure calls tdsrrcllc_set_ordered_config() **********
    1) TDSORDERED_CONFIG = TDSCURRENT_CONFIG
    2) Extract OTA Msg IEs and overwrite those values included in Msg.
    3) ordered_config_set = TRUE
    ************ Procedure calls rrcllc_chan_config_req() *************
    1) Config Lower Layers (via primitives with ORDERED_CONFIG_PTR)
    2) Wait for Lower Layer confirms
    3) Change State (to CELL_DCH)
    4) Send Channel Confirm to RRC Connection Establishment
    *********** Procedure calls tdsrrcllc_clear_ordered_config() *********
    1) ordered_config_set = FALSE
    2) CURRENT_CONFIG_PTR = ORDERED_CONFIG_PTR
    ************ Receive an RB Setup msg & stay in CELL_DCH ***********
    ************ Procedure calls tdsrrcllc_set_ordered_config() **********
    1) TDSORDERED_CONFIG = TDSCURRENT_CONFIG
    2) Extract OTA Msg IEs and overwrite those values included in Msg.
    3) ordered_config_set = TRUE
    ************ Procedure calls rrcllc_chan_config_req() *************
    1) Config Lower Layers (via primitives with ORDERED_CONFIG_PTR)
    2) Wait for Lower Layer confirms
    3) Change State (to CELL_DCH)
    4) Send Channel Confirm to RB Setup
    *********** Procedure calls tdsrrcllc_clear_ordered_config() *********
    1) ordered_config_set = FALSE
    2) CURRENT_CONFIG_PTR = ORDERED_CONFIG_PTR

    * End CELL_DCH transitions with RB setup.
    
    * Idle-Connecting to CELL_FACH (Not implemented or complete, just
      for reference now).

    ************ Procedure calls tdsrrcllc_set_ordered_config() **********
    1) TDSORDERED_CONFIG = TDSCURRENT_CONFIG
    2) If SIB change Overwrite TDSORDERED_CONFIG with SIB5 updates.
    3) Extract OTA Msg IEs and overwrite those values included in Msg.
    4) ordered_config_set = TRUE
    ************ Procedure calls rrcllc_chan_config_req() *************
    1) Config Lower Layers (via primitives with ORDERED_CONFIG_PTR)
    2) Wait for Lower Layer confirms
    3) Change State (to CELL_FACH)
    4) Send Channel Confirm to RRC Connection Establishment
    *********** Procedure calls tdsrrcllc_clear_ordered_config() *********
    1) ordered_config_set = FALSE
    2) CURRENT_CONFIG_PTR = ORDERED_CONFIG_PTR

  - tdsrrcllc_clear_ordered_config(void)

    This function will clear(reset) the ordered_config_set flag. It also
    swaps the tdsordered_config_ptr and tdscurrent_config_ptr.

    The following steps occur:
    1) ordered_config_set = FALSE;
    2) temp_ptr = tdscurrent_config_ptr
    3) tdscurrent_config_ptr = tdsordered_config_ptr
    4) tdsordered_config_ptr = temp_ptr
   
  Functions that can be called from other rrcllc modules are:

  - rrcllc_init_ordered_config()
    
    This function is called by tdsrrcllc_init(). This function initializes
    the ordered config and current config pointers and data inside 
    the ordered config and current config data bases.

  - tdsrrcllc_channel_config_req(tdsrrc_cmd_type *cmd_ptr)
  
    This function will take a pointer to an RRC command packet, ensure that
    it is an RRC_CHAN_CONFIG_REQ command, and then configure the requested
    radio bearers for the next state.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcllc.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
12/17/12   zy      add the check of m_length and k in DMO
06/05/12   hx      Implement suspend RLC during HO period for SRNS relocation 
10/25/11   ysh     Modify req_mak in cphy_setup_req.
03/14/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.93.50
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
03/09/11   ssg     MAC-ehs support on Apollo.
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00

===========================================================================*/


/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsrrccmd_v.h"
#include "tdsuecomdef.h"

/*===================================================================
                        DATA DECLARATIONS
=====================================================================*/


/*===================================================================
                        CONSTANTS
=====================================================================*/
/*-------------------------------------------------------------------
This define is an error condition when no valid RLC PDU size is 
availble for TM UL transmissions.
--------------------------------------------------------------------*/
#define TDSRRCLLC_INVALID_RLC_SIZE     0xFFFFFFFFUL

#define TDSINVALID_DFLOW_ID 8
#define TDSINVALID_QUEUE_ID 8
#define TDSINVALID_PDU_INDEX_ID 8

#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
#define TDSINVALID_LOGICAL_CHANNEL_ID 0xFF
#endif
#ifdef FEATURE_TDSCDMA_TVM_BLOCK
#define TDSRRC_BLOCK_TVM_EXTRA_TIME (3000)
#endif

#define TDSRRC_MAX_SEQ_NUM  0x7F

#ifdef FEATURE_TC
/*-------------------------------------------------------------------
This macro translates the loopback test mode from the RRC defined
values to L1 defined enum values.
--------------------------------------------------------------------*/
#define TDSRRCLLC_XLATE_LBT_MODE(rrc_lbt_mode)             \
  (((rrc_lbt_mode) == TDSLB_TEST_MODE_NONE) ? TDSL1_LOOPBACK_NA :  \
     (((rrc_lbt_mode) == TDSLB_TEST_MODE_1) ?  TDSL1_LOOPBACK_MODE_1 : TDSL1_LOOPBACK_MODE_2))

/*-------------------------------------------------------------------*/
#endif /* FEATURE_TC */

/*------------------------------------------------------------------
This enumerates all possibile use types for TOC
-------------------------------------------------------------------*/
typedef enum
{
  TDSTOC_FOR_DCH_AND_FACH,
  TDSTOC_FOR_DCH,
  TDSTOC_FOR_OOS,
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/* This is applicable for OOS triggered during Frequency redirection 
  * for FACH->FACH and FACH->PCH/URA PCH
  */
  TDSTOC_FOR_OOS_WITH_DCH_INFO, 
  TDSTOC_FOR_OOS_WITHOUT_DCH_INFO,
  /*This is applicable for Frequency redirection for FACH->FACH 
      and FACH->PCH*/
  TDSTOC_FOR_FACH_AND_PCH,
  TDSTOC_FOR_FACH_AND_PCH_WITH_DCH_INFO,
  
#endif/*FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU*/
  TDSTOC_INVALID
}tdsrrcllc_toc_usage_e_type;


/*-------------------------------------------------------------------
ENUM: tdsrrcllc_oc_status_e_type

This enum defines the return status for tdsrrcllc_set_ordered_config().
--------------------------------------------------------------------*/
typedef enum
{
  TDSORDERED_CONFIG_SET,
  TDSORDERED_CONFIG_INCOMPATABLE_SIMULTANEOUS_RECONFIG,
  TDSORDERED_CONFIG_CONFIGURATION_NOT_SUPPORTED,
  TDSORDERED_CONFIG_NOT_SET_OTHER,
  TDSORDERED_CONFIG_CONFIGURATION_INVALID,
  TDSMAX_ORDERED_CONFIG_STATUS_TYPES
} tdsrrcllc_oc_status_e_type;

/*-------------------------------------------------------------------
ENUM: tdsrrcllc_oc_set_status_e_type

This enum defines the various status values that OC can be set to.
--------------------------------------------------------------------*/
typedef enum
{
  TDSOC_SET_FOR_CELL_FACH,
  TDSOC_SET_FOR_CELL_DCH,
  TDSOC_SET_FOR_URA_PCH,
  TDSOC_SET_FOR_CELL_PCH,
  TDSOC_SET_FOR_DCH_FACH_TRANS,
  TDSOC_SET_FOR_DCH_CELL_PCH_TRANS,
  TDSOC_SET_FOR_DCH_URA_PCH_TRANS,
  TDSOC_SET_FOR_FACH_CELL_PCH_TRANS,
  TDSOC_SET_FOR_FACH_URA_PCH_TRANS,
  TDSOC_NOT_SET
}tdsrrcllc_oc_set_status_e_type;

/*-------------------------------------------------------------------
ENUM: tdsrrcllc_oc_process_state_e_type

This enum defines the various process states for Ordered Config.
--------------------------------------------------------------------*/
typedef enum
{
  TDSLL_CONFIG_WITH_OC,
  TDSLL_NOT_CONFIG_WITH_OC
}tdsrrcllc_oc_process_state_e_type;

/*-------------------------------------------------------------------
ENUM: tdsrrcllc_init_e_type

This enum defines the possible RRC-LLC initialization types.
--------------------------------------------------------------------*/
typedef enum
{
  TDSRRCLLC_STARTUP_INIT,
  TDSRRCLLC_RE_INIT,
  TDSRRCLLC_RE_INIT_FREE_Q,
  TDSRRCLLC_INIT_PHY_CHL_DATA
} tdsrrcllc_init_e_type;

/*-------------------------------------------------------------------
ENUM: tdsrrcllc_crnti_status_e_type

This enum defines the possible status for C_RNTI
--------------------------------------------------------------------*/
typedef enum
{
  TDSC_RNTI_VALID,
  TDSC_RNTI_NOT_VALID
}tdsrrcllc_crnti_status_e_type;

/*-------------------------------------------------------------------
ENUM: tdsrrcllc_urnti_status_e_type

This enum defines the possible status for U_RNTI
--------------------------------------------------------------------*/
typedef enum
{
  TDSU_RNTI_PRESENT,
  TDSU_RNTI_NOT_PRESENT
}tdsrrcllc_urnti_status_e_type;

/*-------------------------------------------------------------------
This is an enumerator that lists all possible states of LLC state
machine.
--------------------------------------------------------------------*/
typedef enum
{
  /* LLC Idle state - accepts all RRC internal commands */
  TDSLLC_IDLE,
  TDSLLC_WAIT_CPHY_SETUP_CNF,
  TDSLLC_WAIT_CPHY_CHANNEL_IND,
  TDSLLC_WAIT_CPHY_IN_SYNC_IND,
  TDSLLC_WAIT_CPHY_IDLE_CNF,
  TDSLLC_WAIT_CPHY_DEACTIVATE_CNF,
  TDSLLC_WAIT_CPHY_ACQ_CNF,
  TDSLLC_WAIT_CPHY_FREQ_SCAN_CNF,
  TDSLLC_WAIT_CPHY_CAPABILITY_CNF,
  TDSLLC_WAIT_CPHY_CELL_SELECTION_CNF,
  TDSLLC_WAIT_CPHY_CELL_TRANSITION_CNF,
  TDSLLC_WAIT_CPHY_IMMEDIATE_MEAS_CNF,
  TDSLLC_WAIT_CPHY_INTEGRITY_KEY_MAUC_CNF,
  TDSLLC_WAIT_CPHY_IMMEDIATE_RESELECTION_CNF,
  TDSLLC_WAIT_MAC_UL_HFN_CNF,
  TDSLLC_WAIT_MAC_DL_HFN_CNF,
  TDSLLC_WAIT_CPHY_MEAS_CTRL_CNF,
  /* Waiting for CRLC_CONFIG_TM_UL_CNF */
  TDSLLC_WAIT_RLC_TM_UL_CNF,
  TDSLLC_WAIT_RLC_TM_DL_CNF,
  TDSLLC_WAIT_RLC_UM_UL_CNF,
  TDSLLC_WAIT_RLC_UM_DL_CNF,
  TDSLLC_WAIT_RLC_AM_CNF,
  TDSLLC_WAIT_RLC_UL_HFN_CNF,
  TDSLLC_WAIT_RLC_DL_HFN_CNF,
  TDSLLC_WAIT_RLC_REL_ALL_CNF,
  TDSLLC_WAIT_NEW_CELL_IND,

  /* Waiting for CPHY_STOP_TDS_MODE_CNF */
  LLC_WAIT_CPHY_STOP_TDS_MODE_CNF,
  /* Waiting for RRC_CPHY_SUSPEND_TDS_MODE_CNF */
  LLC_WAIT_CPHY_SUSPEND_TDS_MODE_CNF,
  /* Waiting for TDSRRC_CPHY_RESUME_TDS_MODE_CNF */
  LLC_WAIT_CPHY_RESUME_TDS_MODE_CNF,

  TDSLLC_WAIT_FOR_STATE_CHG_IND,
  /* New states for Ciphering */
  TDSLLC_WAIT_RRC_RESUME_RLC_RB_REQ,
  TDSLLC_WAIT_RRC_CRLC_UL_RESUME_CNF,
  TDSLLC_WAIT_CPHY_DRX_CNF,
  /* Waiting for CPHY_START_TDS_MODE_CNF */
  LLC_WAIT_CPHY_START_TDS_MODE_CNF,
  TDSLLC_WAIT_CPHY_ACT_TIME_CANCEL_CNF ,
  TDSLLC_STATES_MAX
}tdsrrcllc_substate_e_type;

/* Used to determine if the UARFCN passed to the function tdsrrc_validate_dl_arfcn() is 
 * coming from TDS or GSM.
 */
typedef enum
{
  TDSW_UARFCN_FROM_GSM_NBR_LIST,
  TDSW_UARFCN_NOT_FROM_GSM_NBR_LIST
} tdsw_uarfcn_source_e_type;

/* HRNTI status */
typedef enum
{
  TDSHRNTI_INVALID,
  TDSHRNTI_VALID
} tdsrrc_hsdpa_hrnti_enum_type;

/* Indicates type of RLC size list specification */
typedef enum
{
  TDSRLC_SIZES_ALL,
  TDSRLC_SIZES_CONFIGURED,
  TDSRLC_SIZES_EXPLICIT_LIST,
  TDSRLC_SIZES_UNCONFIGURED
} tdsrrc_rlc_size_restriction_config_e_type;

/*-------------------------------------------------------------------
ENUM: tdsrrcllc_mlength_k_chk_e_type

This enum defines the return status for tdsrrcllc_set_ordered_config().
--------------------------------------------------------------------*/
typedef enum
{
  TDSDMO_mlength_k_chk_SUCCESS,
  TDSDMO_mlength_k_chk_Unsupported_Config,
  TDSDMO_mlength_k_chk_Invalid_Config,
  TDSMAX_mlength_k_chk_STATUS_TYPES
} tdsrrcllc_mlength_k_chk_e_type;

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* Used to store the Message Variables for determining HSDPA Action & Bit Mask */
typedef struct
{
  boolean msg_params_present;
  /* If above present, then only one of the following is present */
  boolean hrnti_present;
  uint16 hrnti;
  boolean hspdsch_info_present;
#ifdef FEATURE_TDSCDMA_CPC_DTX
  boolean rl_link_present;
  uint16 new_rl_link;
#endif
  boolean hsdsch_info_present;
  boolean hs_dpcch_info_present;
  boolean beta_table_present;
  boolean hrnti_changed;
  boolean mac_hs_restore_tsns;
} tdsrrc_hsdpa_msg_struct_type;


/*This is the structure which contains the two DRX coefficients 
  value received in OTA*/
typedef struct{
  uint32 length;
  uint32 length2;
}tdsutran_drx_cycle_length_coef_type;

/*This structure contains the DRX coefficients along with the 
  timer value for DRX CYCLE 2*/
typedef struct {
  tdsutran_drx_cycle_length_coef_type utran_drx_cycle_length;
  uint32 drx_cycle2_timer_value;
}tdsutran_drx_cycle_type;

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

extern uint8 tdsMaxSynUlTrans[4];
extern int8  tdsTddPrachCode_R4[28][2];
extern uint8 tdsMidambleConfiguration[9];

/* this table use to convert from ASN1 to OC
    refer to 3GPP: 10.3.6.91) */
extern uint8 tdsTpcStepSize[4];
/* this table use to convert from ASN1 to OC
    refer to 3GPP: 10.3.6.41 */
extern uint8 tdsMidambleAllocMode[4];

extern uint16 tdsRucchTimer[];
extern uint16 tdsTWaitimer[];
extern uint8 tdsSITimer[];
extern uint16 tdsRetransTimerForSI[];

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */

#ifdef FEATURE_TDSCDMA_ULDL_LOG_SUPPORT
extern void tdsrrc_get_ul_log_message( tdspdu_buf_type encoded_sdu,uint8 channel_type,uint8 message_type,uint8 rb_id,void *msg_ptr,uecomdef_logchan_e_type rrclog_ul_lc);

extern void tdsrrc_get_dl_log_message(tdsrrc_cmd_type *cmd_ptr);
#endif
extern void tdsrrcllc_clear_sib5_change_in_progress_flag(void);

extern boolean tdsrrcrcr_is_proc_active(void);

extern void tdsrrcmeas_trash_and_send_meas_failure(void);

extern boolean tdsrrcllc_get_sib5_change_in_progress_flag(void);

extern uint8 tdsrrcllc_check_if_rbid_present_in_chan_config_within_index( tdsrrc_channel_config_req_type * chan_config_req, tdsrrc_RB_Identity rb_id, uint8 count);

extern uecomdef_status_e_type tdsrrc_transition_to_disconnected_state( tdsrrc_proc_e_type rrc_proc,tdsrrc_tx_to_discon_reason_e_type tx_to_discon_reason);

/*===================================================================
                        DATA STRUCTURES
====================================================================*/

/*===================================================================
*                       FUNCTION PROTOTYPES
====================================================================*/

/*====================================================================
FUNCTION: tdsrrcllc_init()

DESCRIPTION:
  This function initializes all RRCLLC data including BCCH, PCCH and
  the TDSORDERED_CONFIG databases. This calls a number of other
  initialization functions.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_init(tdsrrcllc_init_e_type  init_type
                 );

/*====================================================================
FUNCTION: tdsrrcllc_process_cmd()

DESCRIPTION:
  This is called for the following conditions:
  
  - when an internal command is received by the RRC dispatcher and
    the commands final destination is L1, MAC or RLC. 

  - when a confirmation or indication from L1, MAC or RLC is received
    by the RRC dispatcher.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_process_cmd
(
  tdsrrc_cmd_type   *cmd_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_set_ordered_config

DESCRIPTION:
  This is called when the TDSORDERED_CONFIG variable needs to be set.
  It checks to see whether or not it is set or not, and returns
  the status of the check.

  It also takes a pointer to the asn1 decoded message stream and
  places it into the tdsordered_config database.

  The procedure name is required so that the message asn1 decoded
  message can be properly typecast so as to populate the 
  TDSORDERED_CONFIG database.

DEPENDENCIES:
  None.

RETURN VALUE:
  tdsrrcllc_oc_status_e_type - indicates whether the TDSORDERED_CONFIG
                            variable could be successfully set or
                            what the failure was.

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type tdsrrcllc_set_ordered_config
(
  /*
  * This variable indicates the PDU number as defined in the ASN.1
  * header file. This information can be used to map the to correct
  * logical channel received on and then down to the message type.
  * The message contents are in msg_ptr, and need to be typecasted.
  */
  tdsrrc_proc_e_type   proc_id,
  tdsrrc_state_e_type  next_state,
  uint32            dl_sdu_num,
  void             *msg_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_swap_oc_and_cc

DESCRIPTION:
  This function swaps tdsordered_config_ptr and current_config_ptrs
  without altering the ordered_config_status.
  This could be called in place of tdsrrcllc_clear_ordered_config
  when the ordered_config_status needs to be preserved.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void  tdsrrcllc_swap_oc_and_cc(void);

/*====================================================================
FUNCTION: tdsrrcllc_clear_ordered_config

DESCRIPTION:
  This is called when the TDSORDERED_CONFIG variable needs to be cleared.

  This function puts control of TDSORDERED_CONFIG in the hands of the 
  individual procedures. Each procedure can then decide whether or not
  they wish to wait for a layer 2 ack before clearing or just clearing
  after reciept of an lower layer configuration message.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_clear_ordered_config
(
  void
);

/*====================================================================
FUNCTION: tdsrrcllc_channel_config_req()

DESCRIPTION:
  This is called when an TDSRRC_CHANNEL_CONFIG_REQ is received.
  This function adds, drops, or and performs an add/drop of idle 
  or connected mode channels.

DEPENDENCIES:
  None.

RETURN VALUE:
  uecomdef_status_e_type - indicates whether or not the config
                           parameters were correctly set.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_channel_config_req
(
  tdsrrc_cmd_type   *cmd_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_reset_l1_req()

DESCRIPTION:
  This is called when an TDSRRC_RESET_L1_REQ is received.
  This function is called when an RRC procedure needs to send Layer
  1 to the Idle state.

DEPENDENCIES:
  None.

RETURN VALUE:
  uecomdef_status_e_type - indicates whether or not the config
                           parameters were correctly set.

SIDE EFFECTS:
  None.
====================================================================*/
uecomdef_status_e_type tdsrrcllc_reset_l1_req
(
  tdsrrc_cmd_type  *cmd_ptr
);

/*====================================================================
FUNCTION: tdsrrcllc_calculate_ul_tm_ccch_rlc_size()

DESCRIPTION:
  This is called when the RLC size for an uplink Transparent Mode
  Channel is required. 
  This function returns the rlc size removing the MAC header size from the tansport block size

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32  - The size of the RLC Transparent Mode buffer.

SIDE EFFECTS:
  Note that currently all logical channels mapped to a Transport
  channel must have the same TrBLK size. Therefore, by extracting
  the RLC size of the first Transport Format, this will apply to
  all Transport Formats for all logical channels and types mapped to
  this TrCH.
====================================================================*/
uint32 tdsrrcllc_calculate_ul_tm_ccch_rlc_size(void);


/*====================================================================
FUNCTION: tdsrrcllc_get_ordered_config_status()

DESCRIPTION:
  This function checks if TDSORDERED_CONFIG is set. If so, it
  returns TRUE. If it's not set it returns FALSE.

DEPENDENCIES:
  None.

RETURN VALUE:
  Returns a value that indicates if Ordered Config is set for
  an RRC state or not set.
  
SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_set_status_e_type tdsrrcllc_get_ordered_config_status( void );

/*====================================================================
FUNCTION: tdsrrcllc_get_toc_usage()

DESCRIPTION:
  This function checks returns the toc_usage status

DEPENDENCIES:
  None.

RETURN VALUE:
  toc_usage status

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_toc_usage_e_type tdsrrcllc_get_toc_usage(void);



/*====================================================================
 FUNCTION: tdsrrcllc_reconfig_in_progress()
 
 DESCRIPTION:
   This function returns info on whether or not a reconfiguration is
   currently in progress.
 
 DEPENDENCIES:
   None.
 
 RETURN VALUE:
   Returns a value that indicates if a reconfiguration is in progress.
 
 SIDE EFFECTS:
   None.
 ====================================================================*/
 boolean tdsrrcllc_reconfig_in_progress( void );



 /*====================================================================
FUNCTION: tdsrrcllc_get_ordered_config_state_and_proc()

DESCRIPTION:
  This function returns oc status. It also indicates which process
  has set the OC and process status.

DEPENDENCIES:
  None.

RETURN VALUE:
  Returns a value that indicates if Ordered Config is set for
  an RRC state or not set.
  
SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_set_status_e_type tdsrrcllc_get_ordered_config_state_and_proc(
                                                   tdsrrc_proc_e_type *proc_ptr,
                                                   tdsrrcllc_oc_process_state_e_type *process_status_ptr
                                                    );


 /*====================================================================
FUNCTION: tdsrrcllc_reset_ordered_config()

DESCRIPTION:
  This function is used to reset the OC

DEPENDENCIES:
  None.

RETURN VALUE:
  Returns a value that indicates if Ordered Config is set for
  an RRC state or not set.
  
SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_set_status_e_type tdsrrcllc_reset_ordered_config(
                                                   tdsrrc_proc_e_type proc_id
                                                   ,boolean reset_toc

                                                    );

/*====================================================================
FUNCTION: tdsrrcllc_get_drx_info()

DESCRIPTION:
  This function returns the shorter of the PS or CS CN DRX Cycle
  Length.

DEPENDENCIES:
  None.

RETURN VALUE:
  uint32 of the DRX Cycle length, per 25.304.

SIDE EFFECTS:
  This function should only be called after the following function
  calls:
  - tdsrrcllc_copy_sib_to_oc() or
  - tdsrrcllc_set_ordered_config().
====================================================================*/
uint32 tdsrrcllc_get_drx_info( void );

#if defined(FEATURE_3GPP_FAST_DORMANCY)
/*====================================================================
FUNCTION: tdsrrcllc_get_drx_info_values()

DESCRIPTION:
  This function returns the shorter of the PS or CS CN DRX Cycle
  Length.

DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
  This function should only be called after the following function
  calls:
  - tdsrrcllc_copy_sib_to_oc() or
  - tdsrrcllc_set_ordered_config().
====================================================================*/
void tdsrrcllc_get_drx_info_values
(
  uint32 *cs_drx_ptr,
  uint32 *ps_drx_ptr,
  uint32 *utran_drx_ptr
);
#endif

/*====================================================================
FUNCTION: tdsrrcllc_set_utran_drx_info()

DESCRIPTION:
  This function sets the utran_drx_coef in to tdsordered_config
  data structure. This api is used when a procedure does not have
  to set tdsordered_config but needs to configure it with utran_drx_info

DEPENDENCIES:
  None.

RETURN VALUE:
  void

SIDE EFFECTS:
====================================================================*/
void tdsrrcllc_set_utran_drx_info( uint32 utran_drx_coef );

/*====================================================================
FUNCTION: tdsrrcllc_get_current_crnti_status()

DESCRIPTION:
  This function is responsible for returning the C-RNTI status as it
  exists in the Current Config. 
  
DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_crnti_status_e_type
  
SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_crnti_status_e_type
tdsrrcllc_get_current_crnti_status(void);

/*====================================================================
FUNCTION: tdsrrcllc_get_current_urnti()

DESCRIPTION:
  This function is responsible for returning the U-RNTI  as it
  exists in the Current Config. 
  
DEPENDENCIES:
  None

RETURN VALUE:
  tdsrrcllc_crnti_status_e_type
  
SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_urnti_status_e_type
tdsrrcllc_get_current_urnti(uint32 *urnti);

#ifdef FEATURE_TC
/*====================================================================
FUNCTION: tdsrrcllc_update_lbt_mode_for_dchs()

DESCRIPTION:
  This function updates the Loopback Test mode in CCTrCH Info for
  L1 in the Ordered Config.
  It updates the mode for those transport channels that have a DTCH
  logical channel mapped. 
  
DEPENDENCIES:
  None.

RETURN VALUE:
  None.
  
SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_lbt_mode_for_dchs(
  tdsl1_loop_back_mode_type_enum_type tdsloopback_test_mode
#ifdef FEATURE_DUMMY_SIGNALING   
  #error code not present
#endif
);

/*====================================================================
FUNCTION: tdsrrcllc_update_lbt_mode_for_fachs()

DESCRIPTION:
  This function updates the Loopback Test mode in CCTrCH Info for
  L1 in the Ordered Config.
  It updates the mode for those transport channels that have a DTCH
  logical channel mapped. 
  
DEPENDENCIES:
  None.

RETURN VALUE:
  None.
  
SIDE EFFECTS:
  None.
====================================================================*/

void tdsrrcllc_update_lbt_mode_for_fachs(
  tdsl1_loop_back_mode_type_enum_type tdsloopback_test_mode
);

#endif /* FEATURE_TC */

/*====================================================================
FUNCTION: tdsrrcllc_update_sib1_data()

DESCRIPTION:
  This function reads SIB1 for the Current Serving Cell and updates
  both Current and Ordered Config databases with DRX cycle values
  for all available CN domains.
  If a domain is not present (CS or PS), the DRX cycle length for
  that domain is initialized to the one for the other domain to
  prevent any wrong calculations elsewhere.
  
DEPENDENCIES:
  The caller of this function has to make sure Cell Reselection is
  not in progress when this is called. Otherwise it may lead to
  storing DRX values of the old cell.

RETURN VALUE:
  Success/Failure to update.

SIDE EFFECTS:
  Look under dependency.
====================================================================*/
uecomdef_status_e_type  tdsrrcllc_update_sib1_data(void);


/*============================================================================
FUNCTION: tdsrrcllc_return_freq_for_fach_pch()

DESCRIPTION:
  This function returns the frequency that UE is directed to when going to 
  either one of Cell_FACH, Cell_PCH or URA_PCH
         
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE
  FAILURE indicates redirection frequency is NOT available.

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_return_freq_for_fach_pch(tdsrrc_freq_type  *freq);

/*============================================================================
FUNCTION: tdsrrcllc_return_psc_for_fach_pch()

DESCRIPTION:
  This function returns the Primary Scvrambling Code that UE is directed to 
  when going from to either one of Cell_FACH, Cell_PCH or URA_PCH
         
DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS/FAILURE
  FAILURE indicates redirection CPID is NOT available.

SIDE EFFECTS:
=============================================================================*/
uecomdef_status_e_type
tdsrrcllc_return_psc_for_fach_pch(tdsrrc_cpid_type  *cpid);

/*====================================================================
FUNCTION: tdsrrcllc_can_oc_be_cleared_now

DESCRIPTION:
  This returns a boolean depending on if the LLC state permits
  a call to clear_ordered_config_now or not.
  If a valid LLC operation is under process, OC can not be
  cleared and this function returns a FALSE. Before returning
  a FALSE, it registers a clear_ordered_config operation so 
  that it is called immeditaley after a Command Engine is 
  finished.

DEPENDENCIES:
  None

RETURN VALUE:
  TRUE = OC can be cleared now
  FALSE = OC can not be cleared.

SIDE EFFECTS:
  A clear ordered config event is registered.
====================================================================*/
boolean  tdsrrcllc_can_oc_be_cleared_now(void);


/*====================================================================
FUNCTION: tdsrrcllc_update_ciphering_config_info_for_first_tm_rb()

DESCRIPTION:
  This function updates the ciphering information in MAC.

DEPENDENCIES:
  None

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_ciphering_config_info_for_first_tm_rb(void);

/*====================================================================
FUNCTION: tdsrrcllc_update_ciphering_config_info_for_handover()

DESCRIPTION:
  This function updates the ciphering information in RLC and MAC.

DEPENDENCIES:
  None
           
RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrcllc_update_ciphering_config_info_for_handover(void);


/*====================================================================
FUNCTION: tdsrrcllc_set_ordered_config_for_rl_failure

DESCRIPTION:
  This is called when the TDSORDERED_CONFIG variable needs to be set.
  It checks to see whether or not it is set or not, and returns
  the status of the check.

  It also takes a pointer to the asn1 decoded message stream and
  places it into the tdsordered_config database.

  The procedure name is required so that the message asn1 decoded
  message can be properly typecast so as to populate the 
  TDSORDERED_CONFIG database.

DEPENDENCIES:
  CELL_DCH is the only supported state.

RETURN VALUE:
  tdsrrcllc_oc_status_e_type - indicates whether the TDSORDERED_CONFIG
                            variable could be successfully set or
                            what the failure was.

SIDE EFFECTS:
  None.
====================================================================*/
tdsrrcllc_oc_status_e_type tdsrrcllc_set_ordered_config_for_rl_failure
(
  /*
  * This variable indicates the PDU number as defined in the ASN.1
  * header file. This information can be used to map the to correct
  * logical channel received on and then down to the message type.
  * The message contents are in msg_ptr, and need to be typecasted.
  */
  tdsrrc_proc_e_type  proc_id
);



 /*====================================================================
FUNCTION: tdsrrcllc_return_current_substate()

DESCRIPTION:
  This function returns the current llc substate.

DEPENDENCIES:
  None.

RETURN VALUE:
  current llc substate.

SIDE EFFECTS:
  None.

=====================================================================*/
tdsrrcllc_substate_e_type tdsrrcllc_return_current_substate(void);


/*====================================================================
FUNCTION: tdsrrcllc_check_going_to_old_config()

DESCRIPTION:
  This function checks if going back to old config is ongoing.

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE   going back to old confing in progress.
  FALSE  going back to old confing is not in progress

SIDE EFFECTS:
  None.

=====================================================================*/
boolean tdsrrcllc_check_going_to_old_config(void);

/*====================================================================
FUNCTION: tdsrrcllc_check_sufficient_act_time()

DESCRIPTION:
  This function checks if there is sufficient activation time to kill
  the pending configuration.

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE   Sufficient activation time .
  FALSE  Insufficient activation time.

SIDE EFFECTS:
  None.

=====================================================================*/
boolean tdsrrcllc_check_sufficient_act_time(void);

/*====================================================================
FUNCTION: tdsrrcllc_get_n_bcch_setup_status()

DESCRIPTION:
  This function will return N-BCCH set up status.
  It will return TRUE if N-BCCH is set up FALSE other wise

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE : If N-BCCH is set up
  FALSE otherwise
SIDE EFFECTS:
  None.

=====================================================================*/
boolean tdsrrcllc_get_n_bcch_setup_status
(
void
);
/*====================================================================
FUNCTION: tdsrrcllc_get_s_bcch_setup_status()

DESCRIPTION:
  This function will return S-BCCH set up status.
  It will return TRUE if S-BCCH is set up FALSE other wise

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE : If S-BCCH is set up
  FALSE otherwise
SIDE EFFECTS:
  None.

=====================================================================*/
boolean tdsrrcllc_get_s_bcch_setup_status
(
void
);

/*====================================================================
FUNCTION: tdsrrcllc_get_rach_cctrch_ptr()

DESCRIPTION:
  This function returns rach cctrch ptr from current config

DEPENDENCIES:
  None.

RETURN VALUE:
  This function returns rach cctrch ptr from current config
SIDE EFFECTS:
  None.

=====================================================================*/
tdsmac_ul_rach_cctrch_config_type* tdsrrcllc_get_rach_cctrch_ptr
(
  void
);

/*====================================================================
FUNCTION: tdsrrcllc_check_if_chan_config_in_progress_for_sib5_change()

DESCRIPTION:
  This function will return TRUE if channels are being configured for sib5 change(Requested by CSP)
  It will return FALSE other wise

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE : if channels are being configured for sib change
  FALSE otherwise
SIDE EFFECTS:
  None.

=====================================================================*/
boolean tdsrrcllc_check_if_chan_config_in_progress_for_sib5_change
(
  void
);

/*====================================================================
FUNCTION: tdsrrcllc_process_chan_config_request()

DESCRIPTION:
  This function will process the channel config request

DEPENDENCIES:
  None.

RETURN VALUE:
  Next LLC State
  
SIDE EFFECTS:
  None.

=====================================================================*/
tdsrrcllc_substate_e_type tdsrrcllc_process_chan_config_request
(
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================
FUNCTION   tdsrrcllc_cphy_ciphering_key_req

DESCRIPTION
  This function sends a TDSL1_CPHY_CIPHERING_KEY_REQ to L1. It gets a L1 command buffer,
  fills it up with the data, pointer to which is passed as a parameter to
  this function. It then sends the command over to L1.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS or FAILURE

SIDE EFFECTS
  None.
===========================================================================*/
uecomdef_status_e_type tdsrrcllc_cphy_ciphering_key_req
(
void
);

/*===========================================================================
FUNCTION RRCLLC_QUEUE_INIT

DESCRIPTION
  This function initializes the RRC LLC queues. This function needs
  to be called once at the time of the RRC task start-up.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsrrcllc_queue_init
(
  void 
);

/*===========================================================================
FUNCTION: tdsrrcllc_curr_cmd_rel_all_req

DESCRIPTION:
  This  function returns if the current cmd is CRLC REL all req. 
  In that state L2 ack can be ignored.

DEPENDENCIES:
  None.

RETURN VALUE:
TRUE/FALSE

SIDE EFFECTS:
  None
===========================================================================*/
boolean tdsrrcllc_curr_cmd_rel_all_req
(
  void
);
/*====================================================================
FUNCTION: tdsrrcllc_reset_toc_usage_change_oos_to_dch()

DESCRIPTION:
  This function resets the value of toc_usage_change_oos_to_dch

DEPENDENCIES:
  None.

RETURN VALUE:
 boolean

SIDE EFFECTS:
  None.

=====================================================================*/
void tdsrrcllc_reset_toc_usage_change_oos_to_dch
(
  void
);
/*====================================================================
FUNCTION: tdsrrcllc_get_toc_usage_change_oos_to_dch()

DESCRIPTION:
  This function returns the value of toc_usage_change_oos_to_dch

DEPENDENCIES:
  None.

RETURN VALUE:
 boolean

SIDE EFFECTS:
  None.

=====================================================================*/
boolean tdsrrcllc_get_toc_usage_change_oos_to_dch
(
  void
);
/*===========================================================================
FUNCTION: tdsrrcllc_phy_channels_up

DESCRIPTION:
  This  function returns true if both pccpch and sccpch are up for DL

DEPENDENCIES:
  None.

RETURN VALUE:
TRUE/FALSE

SIDE EFFECTS:
  None
===========================================================================*/
boolean tdsrrcllc_phy_channels_up
(
  void
);

/*==========================================================
FUNCTION  : tdsrrcllc_rx_window_size()

DESCRIPTION : 
  Return the Window Size for receiving.

DEPENDENCIES: 
  None.

RETURN VALUE:
  Window size for receiving for RLC.

SIDE EFFECTS:
  None.
============================================================*/
uint16 tdsrrcllc_rx_window_size
(
  tdsrrc_ReceivingWindowSize rrc_rx_window_size
);

/*========================================================== 
FUNCTION  : tdsrrcllc_check_if_rlc_stopped_by_llc() 
 
DESCRIPTION :  
Return the if RLC is stopped by LLC  . 
  
DEPENDENCIES:  
None. 
 
RETURN VALUE: 
    TRUE: RLC is stopped by LLC. 
    FALSE: Otherwise 
 
SIDE EFFECTS: 
None. 
============================================================*/ 
boolean tdsrrcllc_check_if_rlc_stopped_by_llc 
( 
   void 
); 
#ifdef FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT
/*===========================================================================

FUNCTION   TDSRRCLLC_INIT_DEFAULT_CONFIGURATION

DESCRIPTION

  This function initializes the default configurations.  We see a lot of
  hard-coded values in the foll. functions.  All the values are copied
  straight from 13.7 of 25.331.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcllc_init_default_configuration
(
  void
);
#endif

#ifdef FEATURE_TDSCDMA_TVM_BLOCK
/*===========================================================================

FUNCTION  tdsrrc_calculate_block_tvm_time

DESCRIPTION
  This function is used to calculate the tvm block time after HO

DEPENDENCIES


RETURN VALUE
  TMV block time interval

SIDE EFFECTS
  None
===========================================================================*/
uint16 tdsrrc_calculate_block_tvm_time
(
  void
);

/*===========================================================================

FUNCTION          tdsrrctvm_send_block_tvm_cmd

DESCRIPTION 

    This function sends TDSCMAC_UL_TRAFFIC_VOLUME_BLOCK_REQ to MAC.

DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None.

===========================================================================*/

extern void tdsrrctvm_send_block_tvm_cmd
(
  uint16 block_time
);

/*===========================================================================

FUNCTION   tdsrrctvm_tvm_identity_exist

DESCRIPTION 

    Check if TVM measurement ID exist or not

DEPENDENCIES

    None

RETURN VALUE

    TRUE   -  Exist
    FALSE - Non-exist

SIDE EFFECTS

    None
===========================================================================*/

extern boolean tdsrrctvm_tvm_identity_exist
(
  void
);
#endif /*FEATURE_TDSCDMA_TVM_BLOCK*/

#endif /* TDSRRCLLC_H */
