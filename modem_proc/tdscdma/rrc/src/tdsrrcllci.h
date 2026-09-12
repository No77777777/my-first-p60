#ifndef TDSRRCLLCI_H
#define TDSRRCLLCI_H
/*===========================================================================

                    L3 - RRC Lower Layer Controller Interface
                    For use only by rrcllc functions.

DESCRIPTION
  This file contains function prototypes and local RRC LLC databases.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcllci.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
11/09/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.84.50
10/04/10   prk     Merging latest e-fach fixes.
09/20/10   prk/kp  Added code to support FEATURE_WCDMA_HS_FACH
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
#include "sys_plmn_selection.h"

#ifdef FEATURE_UMTS_PDCP
#ifdef FEATURE_CMI
#include "pdcp.h"
#else/*FEATURE_CMI*/
#include "pdcprrcif.h"
#endif/*FEATURE_CMI*/
#endif /* FEATURE_UMTS_PDCP*/

#define  TDSLLC_MAX_L1_CMDS_PER_RRC_INTERN_CMD  6

#define  TDSLLC_INTER_SRNS_HO_CFN_OFFSET 20

/*-------------------------------------------------------------------
This is to define locally (since no one else should care) the CCTrCH
ID error condition.
--------------------------------------------------------------------*/
#define  TDSRRCLLC_INVALID_CCTRCH_ID            0xFF

#define  TDSRRCTMR_L1_DEADLOCK_DETECT_TIMER_IN_MS 6000UL

/* If ERR_FATAL recovery has been enabled change the delay timer to 1 second */
   #define  TDSRRCTMR_DELAY_ERR_FATAL_TIMER_IN_MS 6000UL
/*------------------------------------------------------------------
This enumerates all possibilities of actions on Physical channels.
-------------------------------------------------------------------*/
typedef enum
{
  TDSLLC_PHYCHAN_NO_OP,
  TDSLLC_PHYCHAN_SETUP,
  TDSLLC_PHYCHAN_RELEASE,
  TDSLLC_PHYCHAN_RECONFIG,
  TDSLLC_PHYCHAN_TRCH_EN_DIS,
  TDSLLC_PHYCHAN_DROP_AND_ADD,
  TDSLLC_PHYCHAN_SET_LBT_MODE
}tdsrrcllc_phy_chan_op_type;

/*------------------------------------------------------------------
This enumerates all possibile LLC command engines.
-------------------------------------------------------------------*/
typedef enum
{
  /* Command engine for a Channel Config command that does not
   * result in a Release all channel scenario.
   */
  TDSLLC_CHAN_CONFIG_ENGINE,
  /* Command engine for a Release all Channel scenario. This
   * could happen as a result of a Channel Config Req.
   */
  TDSLLC_RELEASE_ALL_CHAN_ENGINE,

  /* Command engine for lower layer deactivation */
  TDSLLC_DEACT_ENGINE,

  /* Command engine for cell reselection */
  TDSLLC_CELL_RESELECTION_ENGINE,

  /* Command engine for TDS Mode deactivation */
  LLC_STOP_TDS_MODE_ENGINE,

  /* Command engine for releasing only Physical and Transport
   * channels
   */
  TDSLLC_RELEASE_TR_PHY_CHAN_ENGINE,

  /* Command engine for re-establishing RLC connections */
  TDSLLC_RE_EST_RLC_ENGINE,
  TDSLLC_BYPASS_ENGINE,
  /* Indicates that no command engine is active at present */
  TDSLLC_NO_ENGINE
}tdsrrcllc_cmd_engine_e_type;


/*-------------------------------------------------------------------
ENUM: rrcllc_mac_operation_e_type

This enum defines the possible operations on MAC UL
--------------------------------------------------------------------*/
typedef enum
{
  TDSONLY_SWAP_PTR,
  TDSRECONFIG_MAC
}tdsrrcllc_mac_ul_operation_e_type;


/*-------------------------------------------------------------------
ENUM: rrcllc_mac_operation_e_type

This enum defines the possible synch options between MAC and L1
--------------------------------------------------------------------*/
typedef enum
{
  TDSSYNC_WITH_L1,
  TDSNO_SYNC_WITH_L1
}tdsrrcllc_mac_sync_with_l1_e_type;


/*------------------------------------------------------------------
This enumerates the config db to be used for unmapping RB info.
-------------------------------------------------------------------*/
typedef enum
{
  TDSOC_ONLY,
  TDSTOC_ONLY,
  TDSOC_AND_TOC
}tdsrrcllc_unmap_rb_e_type;
/*------------------------------------------------------------------
This enumerates all possibile actions on a Channel Config Request
-------------------------------------------------------------------*/
typedef enum
{
  TDSTRASH_CONFIG_REQ,
  TDSPROCESS_CONFIG_REQ
}tdsrrcllc_action_on_config_req_e_type;


typedef enum
{
 TDSORDERED_CONFIG_ONLY,
 TDSCURRENT_CONFIG_ONLY,
 TDSORDERED_AND_CURRENT_CONFIG
}tdsrrcllc_reestab_config_e_type;

/* Enumerated type of the HO type */
typedef enum
{
  TDSRRCLLC_HHO          = 0,
  TDSRRCLLC_BHO          = 1,
} tdsrrcllc_event_ho_type_e_type;

/* Enumerated type of the HO failure cause */
typedef enum
{
  TDSRRCLLC_HO_FAILURE_CAUSE_Channel_Setup_Failure          = 0,
  TDSRRCIHO_HO_FAILURE_CAUSE_Downlink_Sync_Failure          = 1,
  TDSRRCIHO_HO_FAILURE_CAUSE_MAX                            = 0xFF
} tdsrrcllc_event_ho_failure_cause_e_type;

/*------------------------------------------------------------------
This contains a list of CCTrCH IDs.
-------------------------------------------------------------------*/
typedef struct
{
  byte  num_ids;
  cctrch_id_type  id[TDSUE_MAX_CCTRCH - 1];
}tdsrrcllc_cctrch_id_list_type;


/*-------------------------------------------------------------------
This structure holds a list of possible physical channels and the
corresponding actions on them.
TBD: At this point since there can not be multiple Physical channels
of any one type, there is only one occurance of a physical channel
of each physical channel type. This will have to be modified later to
accomodate multiple physical channels of each type.
--------------------------------------------------------------------*/
typedef struct
{
  tdsrrcllc_phy_chan_op_type  pccpch_action;
  tdsrrcllc_phy_chan_op_type  n_pccpch_action;
  tdsrrcllc_phy_chan_op_type  sccpch_action;
  tdsrrcllc_phy_chan_op_type  prach_action;
  tdsrrcllc_phy_chan_op_type  ul_dpch_action;
  tdsrrcllc_phy_chan_op_type  dl_dpch_action;
  boolean                  aset_update;
}tdsrrcllc_phy_chan_action_list_type;


/*-------------------------------------------------------------------
This structure holds a list of RLC commands for configuring
Uplink and downlink UM/TM and AM logical channel entities.
--------------------------------------------------------------------*/
typedef struct {
 /* UL TM logical channel config request */
 tdsrlc_ul_tm_config_type  ul_tm;
 /* UL TM logical channel config request */
 tdsrlc_dl_tm_config_type  dl_tm;
 /* UL TM logical channel config request */
 tdsrlc_ul_um_config_type  ul_um;
 /* UL TM logical channel config request */
 tdsrlc_dl_um_config_type  dl_um;
 /* UL TM logical channel config request */
 tdsrlc_am_config_req_type am;
} tdsrrcllc_rlc_config_req_list_type;


#ifdef FEATURE_UMTS_PDCP

/*-------------------------------------------------------------------
This structure holds a list of PDCP commands for configuring
PDCP entities.
--------------------------------------------------------------------*/
typedef struct {
  uint32 num_rbs;
  pdcp_cfg_req_data_type pdcp_cfg_req[MAX_RAB_TO_SETUP];
} tdsrrcllc_pdcp_config_req_list_type;

#endif /* FEATURE_UMTS_PDCP */
/*-------------------------------------------------------------------
This structure holds a list of L1 commands. This list stores all L1
commands that need to be sent for any one RRC internal command.
--------------------------------------------------------------------*/
typedef struct
{
  int32 num_l1_req;
  int32 current_l1_req;
  tdsl1_ext_cmd_type  l1_req[TDSLLC_MAX_L1_CMDS_PER_RRC_INTERN_CMD];
}tdsrrcllc_l1_req_list_type;

/*-------------------------------------------------------------------
This structure holds data pertaining to the current RRC internal
command under process.
--------------------------------------------------------------------*/
typedef struct
{
  /* Indicates if data in this structure is valid */
  boolean  valid;

  /* Procedure that sent the currently-being-processed RRC internal command.
   * A procedure ID of TDSMAX_NUM_OF_PROCEDURES indicates the command was
   * internally generated by LLC
   */
  tdsrrc_proc_e_type  procedure;

  /* RRC state to be transitioned to after processing the command. */
  uint32                chan_config_transaction_id;

  /* RRC state to be transitioned to after processing the command. */
  tdsrrc_state_e_type  next_rrc_state;

  /* Currently processed RRC internal command ID. This is valid only if
   * the procedure ID is a valid procedure (other than LLC).
   */
  tdsrrc_cmd_e_type  rrc_current_cmd_id;

  /* This boolean indicates if a "Clear Ordered Config" operation is
   * poending.
   */
  boolean  clear_oc_pending;

  struct
  {
    /*store the reason for channel configuration */
    tdsrrcllc_chan_cfg_reason_e_type       chan_config_reason;
    /* Indicates if Channel Establishment Indication has been received from L1 */
    boolean   chan_est_ind_rcvd;
    /* LLC Command engine active at present */
    tdsrrcllc_cmd_engine_e_type  current_cmd_engine;

    /* List of RLC Configure requests */
    tdsrrcllc_rlc_config_req_list_type  rlc_config_req_list;

    /* List of physical channels and the corresponding actions on them */
    tdsrrcllc_phy_chan_action_list_type  phy_chan_action_list;

    /* Boolean indicates if this is a solo BCH operation */
    boolean       solo_bch_operation;

    /* Loopback mode - applicable only when DL DPCH action is "Set Loopback mode" */
    tdsrrc_lb_test_mode_e_type  lb_test_mode;
    /* Indicates if dummy signalling for loopback mode 2 is enabled */
    boolean       dummy_signaling_enabled;

    /* Frequency information for Inter-frequency HHO */
    boolean       new_freq;
    boolean       hho_ciph_update_required;


  /* HSDPA Active/Inactive Parameters */

    boolean hsdpa_inactive_to_active;
    boolean hsdpa_active_to_inactive;
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    boolean hsdpa_ehs_to_hs;
    boolean hsdpa_hs_to_ehs;
#endif
    /* List of L1 request commands */
    tdsrrcllc_l1_req_list_type  l1_req_list;

    /* List of CCTrCH IDs that need to be released once all CPHY_SETUP_REQs are
     * sent and corresponding CNF received.
     */
    tdsrrcllc_cctrch_id_list_type  cctrch_id_release_list;

    /* Indicates if a Confirm is needed */
    boolean  cnf_needed;

    /* Status of the Channel Config command */
    tdsrrcllc_chan_cfg_status_e_type  chan_cfg_status;

#ifdef FEATURE_UMTS_PDCP
    tdsrrcllc_pdcp_config_req_list_type   pdcp_config_req_list;
#endif /* FEAUTURE_UMTS_PDCP */

  }chan_config;

  /* This structure will store the cell reselection information. It will be updated whenever LLC
   * receives a cell reselection request from CCM */
  struct
  {
    /* Freq. of the cell to be reselected */
    tdsrrc_freq_type  freq;

    /* Scrambling Code of the Cell */
    tdsrrc_cpid_type   cpid;

    /* The PLMN ID  */
    rrc_plmn_identity_type      plmn_id;

    /* Cell ID */
    tdsrrc_int_cell_id_type        cell_id;

  }rrcllc_cell_resel_info;

  tdsl1_stop_cause_enum_type       wcdma_stop_cause;

  sys_stop_mode_reason_e_type   wcdma_deact_reason;

  boolean                       rnti_update_cnf_needed;

  tdsrrc_rlc_re_est_for_srns_rb_e_type rlc_re_est_rbs;
  boolean            update_mac_with_rnti;
  /* boolean used durin RNTI_UPDATE_REQ to 
   * indicate that SCCPCH selection is 
   * required
   */
  boolean            sccpch_selec_reqd;

  /* boolean indicating if the command under process is a Inter-frequency
   * Hard Handover within DCH
   */
  boolean       is_hho_in_dch;

  boolean       err_recovery_in_progress;

    boolean send_reset_l1_cnf;

  /* add a flag, remind revert process for CR354247. */
  boolean       is_revert_process;
}tdsrrcllc_rrc_cmd_process_data_type;
#ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
typedef struct
{
  uint8 rb_id[TDSMAC_DL_MAX_RB_ID];
  uint8 rb_num;
}tdsrrcllc_continue_rlc_data_type;

#endif


/*===========================================================================

FUNCTION: tdsrrcllc_update_hsdpa_mac_action_l1_ops_for_old_config

DESCRIPTION:
  This  function updates the HS and EHS operations for L1 and MAC in ordered config

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None
===========================================================================*/
void tdsrrcllc_update_hsdpa_mac_action_l1_ops_for_old_config
(
  tdsl1_hsdpa_setup_ops_enum_type hsdpa_l1_ops
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  ,tdsmac_hs_transition_e_type hs_transition
#endif

);

/*===========================================================================

FUNCTION: tdsrrcllc_get_log_chan_action_for_rb_action

DESCRIPTION:
  This  function returns the RLC action based on RB Action

DEPENDENCIES:
  None.

RETURN VALUE:
  RLC Action

SIDE EFFECTS:
  None
===========================================================================*/
tdsrlc_channel_action_e_type  tdsrrcllc_get_log_chan_action_for_rb_action
(
  tdsrrc_rb_config_type * rb_config
    , uint16 *ul_cfg_mask,
     uint16 * dl_cfg_mask

);

#ifdef FEATURE_TDSCDMA_HSUPA
/*====================================================================
FUNCTION: tdsrrcllc_check_if_config_mismatch_for_non_schedule()

DESCRIPTION:
  This function will check only non-schedule is start, OTA only reconfig 
  schedule elements, RRC will not notify the elements to L1.

DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE : RRC not notify L1 reconfig HSUPA
  FALSE  RRC notify L1 reconfig HSUPA.
  
SIDE EFFECTS:
  None.

=====================================================================*/
boolean tdsrrcllc_check_if_config_mismatch_for_non_schedule
(
  void
);

#endif /*FEATURE_TDSCDMA_HSUPA*/

/*===========================================================================
FUNCTION   tdsrrcllc_cmac_ul_remove_invalid_tfc

DESCRIPTION
   RRC remove the invalid TFC,which TFCI length exceed the TFCI coding

DEPENDENCIES
  None.

RETURN VALUE


SIDE EFFECTS
  None.
===========================================================================*/
void tdsrrcllc_cmac_ul_remove_invalid_tfc
(
  tdsmac_ul_tfcs_type *tfcs_ptr, 
  uint8 tfci_coding,
  boolean *flag_tfc_remove
);

#ifdef FEATURE_TDSCDMA_UL_RLC_SIZE_CHANGE
/*====================================================================
FUNCTION: tdsrrcllc_get_start_reest_rb_from_nv()

DESCRIPTION:
  This function is used to get the start reestablish RB from NV.

DEPENDENCIES:
  None.

RETURN VALUE:
  DCCH_UM_RADIO_BEARER_ID
  DCCH_DT_LOW_PRI_RADIO_BEARER_ID
  
SIDE EFFECTS:
  None.

=====================================================================*/
uint8 tdsrrcllc_get_start_reest_rb_from_nv
(
void
);
#endif
#ifdef FEATURE_TDSCDMA_ONE_SIDE_REEST
/*====================================================================
FUNCTION: tdsrrcllc_check_and_stop_rlc_entity_due_to_reest()

DESCRIPTION:
  This function is used to stop the RLC entity.

DEPENDENCIES:
  None.

RETURN VALUE:

  
SIDE EFFECTS:
  None.

=====================================================================*/
void tdsrrcllc_check_and_stop_rlc_entity_due_to_reest
(
tdsrlc_am_config_req_type * am_rb_config
);

#endif
#endif /* TDSRRCLLCI_H */
