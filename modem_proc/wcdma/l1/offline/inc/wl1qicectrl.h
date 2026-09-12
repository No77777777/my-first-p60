#ifndef WL1_QICE_CTRL_H
#define WL1_QICE_CTRL_H
/*===========================================================================
                 WCDMA L1 QICE Controller

GENERAL DESCRIPTION
  This file contains the code for controlling the operation of the QICE block

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2008-2019 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1qicectrl.h#4 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when       who    what, where, why
--------   ---    ---------------------------------------------------------
01/10/20   ac      Mini dump framework checkin
09/11/19   rv     Move PLT specific functions under PLT feature flag
06/22/16   svh    Update QICE tables taking into account new search mode..
11/20/15   svh    Update QICE tables taking into account new search mode..
10/02/15   kcm    Changes to not trigger reframe at enter/exit of QDTA gap.
                  Esure MMSE filters are frozen prior to QDTA gap entry
08/03/15   svh    Handle race condition in IRP+MCVS  clock losering scenario
07/21/15   rsr    Update cme mcvs info based sw db out of CPC lite sleep gap
07/16/15   rsr    Added Fix to Ensure that MCVS Picks correct Clock Speed 
                  for QICE operations.
07/09/15   svh    IRPM + MCVS changes for TA 2.0.
07/08/15   rsr    Added IRPM MCVS Feature Changes.
06/15/15   rs     BOLT W+W changes
06/03/15   svh    TA featurisation.
06/02/15   sm     Insert api to trigger reframe with a change in RxD Freeze Flag
                  With fix to trigger only with EQ update.
02/27/15   ap     Initial check-in for WL1 QSH framework 
04/22/15   rsr    Added changes to Ensure that IRPM Enable happens 20SF after 
                  first triage cycle.
01/28/15   sm      Advanced Rcvr W2W Global Reorg
10/28/14   sr     Initial checkin for 0x187B WCDMA power log pkt
08/20/14   rsr    Added Fix to Ensure that we immediately Update the NBR CFG
                  BMSK, instead of Posting Local Command for Updation.
08/06/14   sr/rsr Changes to handle nbr QICE cell drop for cell resel to that cell.
06/10/14   qj     Added cfg_done_ngbr_cell_bmsk to wl1_cme_ctrl_cmd_type
03/17/14   mit    Changes to accommodate dynamic cell_db_idx allocation
04/15/14   rsr    Added Fix to Address Race Condition between CME and RxD.
04/10/14   ar     Added QICE suspend client for cell reselection
11/18/13   rsr    Added QICE Multicarrier Support.
11/04/13   rsr    Provided APi to SRCH to avoid calling CME Cb, in case CME 
                  triage is suspended. 
09/12/13   jd      Searcher redesign
08/28/13   sad    Disable SCH-IC if there are multiple cells in ASET
07/24/13   jd      Changes for Dual band Dual carrier
07/23/13   vs     Added support for antenna switch diversity feature.
07/18/13   rsr    MCVS: added support for SVS+, changed prototype
04/09/13   vs     Added support for wcdma periodic statictics log pkt.
04/12/13   vsr    Disable SCHIC for R99 calls with >1 cell in ASET
03/14/13   sj     Changed the query API for MCVS VPE update
03/08/13   sj     MCVS feature enhancement
02/13/13   sj     Dynamic resizing of max QSET size in DC call
                  Eliminating delay in adding cell to QSET
01/24/13   sj     Added function to drop qice neighbor cell to free up cell id
11/28/12          MCVS feature related changes  
07/24/12   geg    Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/21/12   ms     Added API wl1_cme_is_mcpm_relinquish_pending
05/21/12   sj     Added SRCH_DECONFIG_CELLS_CB as CME cmd sender
04/06/12   hk     Added external triggers to enable/disable CME
02/10/12   sj     Added an unified CME local cmd handler.
01/27/12   vs     Nikel feature cleanup.
12/12/11   sj     Added API to check CME pending flag
11/09/11   sj     Changed QICE resource manager to use alternative table 
                  for VPE cycle stressed scenarios
07/05/11   vsr    Mainlining Genesis CME features
01/13/11   vsr    Added support for QICE ASET cell registration
                  Added support for CME triage suspend/resume
11/23/10   vsr    Added API to be called for CME cleanup
10/20/10   vsr    Add QSET Neighbour Cell Support
05/03/10   rvs    Add extra params in
                  wl1_cme_proc_wplt_qice_oride_config_params_cmd().
04/01/10   rvs    Add wl1_cme_proc_wplt_set_cqi_and_demod_algo_bmsk_cmd().
02/11/10   rvs    Updates to support dual carrier.
12/16/09   rvs    Added wl1_cme_proc_wplt_qice_debug_enable_cmd().
09/24/09   rvs    Split header file in wl1qicectrl.h and wl1qicectrli.h.
09/09/09   rvs    Added support for QICE F/Finv and QSET Update log packets.
07/15/09   rvs    Checked in initial version of file.

===========================================================================*/

/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "comdef.h"
#include "customer.h"

#include "l1const.h"
#include "l1def.h"
#include "logod.h"

/* -----------------------------------------------------------------------
** Enumerations/Structures/Typedefs
** ----------------------------------------------------------------------- */

/* List of CME local cmd senders */
typedef enum
{
  WL1_CME_HS_STATE_CHANGE_CB_ENABLE,
  WL1_CME_SRCH_DECONFIG_CELLS_CB_RESUME_HS,
  WL1_CME_SRCH_DECONFIG_CELLS_CLIENT_CB,
  WL1_CME_DEMOD_CFG_CMD_DONE_CB,
  WL1_CME_CFN_NOTIFY_CB,
#ifdef FEATURE_WCDMA_ANT_SWITCH_SUPPORT
  WL1_CME_ANT_SWITCH_CB,
#endif
  WL1_CME_CELL_CFG_DONE_CB,
  WL1_CME_EXT_ENABLE,
  WL1_CME_EXT_DISABLE,
  WL1_CME_SCHIC_DISABLE_CB_RESUME_ASET_UPD,
  WL1_CME_DEMOD_CFG_PROC_DONE_CB,
  WL1_CME_RXD_STATE_UPD,
  WL1_CME_IRPM_ENABLE,
 #ifdef FEATURE_WCDMA_VLC
  WL1_CME_VLC_ENABLE,
 #endif
 #ifdef FEATURE_WCDMA_TABASCO_MODEM
  WL1_CME_MCVS_REQ_CB,
  WL1_CME_IRPM_MCVS_UNBLOCK,
 #endif
 #ifdef FEATURE_WCDMA_THOR_MODEM
  WL1_CME_DEMOD_MCVS_BUMP_UP_CB,
  WL1_CME_DEMOD_MCVS_BUMP_DOWN_CB,
 #endif
  WL1_CME_CTRL_CMD_SNDR_MAX
} wl1_cme_ctrl_cmd_sndr_enum_type;

typedef enum
{
  WL1_CME_CARRIER_PRI = 0,
  WL1_CME_CARRIER_SEC,
#ifdef FEATURE_WCDMA_3C_HSDPA
  WL1_CME_CARRIER_TRI,
#endif
  WL1_CME_NUM_CARRIERS
} wl1_cme_carrier_enum_type;

 #ifdef FEATURE_WCDMA_TABASCO_MODEM
/* Enum for SRCH cycle Mode */
typedef enum
{
  WL1_CME_SRCH_CYCLE_0,
  WL1_CME_SRCH_CYCLE_1,
  WL1_CME_SRCH_CYCLE_2,
  WL1_CME_SRCH_CYCLE_3,
  WL1_CME_SRCH_CYCLE_INVALID
} wl1_cme_srch_cycle_factor_type;
#endif

/* -----------------------------------------------------------------------
** Macros
** ----------------------------------------------------------------------- */

#define WL1_CME_SUSPEND_CLIENT_L1M_SETUP                       (0x0002) 
#define WL1_CME_SUSPEND_CLIENT_CME_CLEANUP                     (0x0004) 
#define WL1_CME_SUSPEND_CLIENT_CARRCFG_CLEANUP		       (0x0008)
#define WL1_CME_SUSPEND_CLIENT_CELL_RESEL                      (0x0010)

#define WL1_CME_SRCH_RESULT_NUM_CELLS  (10)
#define WL1_CME_MAX_NUM_PSET_CELLS  (WL1_CME_SRCH_RESULT_NUM_CELLS)

#define WL1_CME_MAX_NUM_NGHBRS  (9)
#define WL1_CME_MAX_NUM_NGHBRS_PER_CARRIER  (2)

typedef struct 
{
  /*header*/
  l1_local_cmd_hdr_type hdr;
  /*sender of the local command*/
  wl1_cme_ctrl_cmd_sndr_enum_type sender;
  /* RxD State */
  boolean rxd_state;
} wl1_cme_ctrl_cmd_type;

/* CME Srch Cell Info  */
typedef struct
{
  uint8                   num_cells[WL1_CME_NUM_CARRIERS];
  srch_cell_struct_type * cell_ptr[WL1_CME_NUM_CARRIERS][WL1_CME_SRCH_RESULT_NUM_CELLS];
} wl1_cme_srch_info_struct_type;

#if defined(FEATURE_WCDMA_THOR_MODEM) || defined(FEATURE_WCDMA_TABASCO_MODEM)
typedef struct
{
  uint8 cme_mcvs_txd_status;
  uint8 cme_mcvs_qset_size_status;
  uint8 cme_mcvs_qset_iteration_status[WL1_CME_NUM_CARRIERS];
  uint8 cme_mcvs_rxd_status;
 #ifdef FEATURE_WCDMA_TABASCO_MODEM
  uint8 cme_mcvs_srch_cycle;
  uint8 cme_mcvs_num_cells[WL1_CME_NUM_CARRIERS];
  uint8 cme_mcvs_max_qset_iteration_status[WL1_CME_NUM_CARRIERS];
 #endif
}wl1_cme_mcvs_struct_type;
#endif

typedef struct
{
  /* Mutex for all CME logging. */
  rex_crit_sect_type wl1_cme_logging_mutex;
  /* Mutex for using local_wl1qicectrl_cntrl->wl1_cme_db.carrier. */
  rex_crit_sect_type wl1_cme_carrier_idx_mutex;
  /* Mutex for using CME MCVS with IRPM */
  rex_crit_sect_type wl1_cme_mcvs_crit_sect;
 #ifdef FEATURE_WCDMA_THOR_MODEM
  /* Boolean Override for IRPM MCVS Algortihm*/
  boolean wl1_cme_post_lcmd_for_mcvs;
 #endif
}wl1qicectrl_ext_cntrl_params_type;

typedef void L1M_NEXT_CMD_HANDLER_CB_FUNC_TYPE(wsub_id_e_type wsub_id);
typedef void WL1_CME_FREE_CELL_ID_CB_FUNC_TYPE(wsub_id_e_type wsub_id);
typedef void CME_RXD_TRANS_REFRAME_CB_FUNC_TYPE(wsub_id_e_type wsub_id);
typedef void SRCH_CME_CELL_INFO_CB_FUNC_TYPE(wl1_cme_srch_info_struct_type* srch_info,wsub_id_e_type wsub_id);
typedef void SRCH_CME_TXD_CHANGE_CB_FUNC_TYPE(uint8 cell_db_idx, boolean cpich_td_state,wsub_id_e_type wsub_id);
typedef void CME_PRI_CELL_REFRAME_RXD_CB_FUNC_TYPE(boolean, wsub_id_e_type wsub_id);

extern wl1qicectrl_ext_cntrl_params_type wl1qicectrl_ext_cntrl_params;


extern void wl1qicectrl_int_cntrl_params_init( wsub_id_e_type wsub_id);

#define WL1_CME_LOCK_MCVS_MUTEX()    REX_ISR_LOCK(&wl1qicectrl_ext_cntrl_params.wl1_cme_mcvs_crit_sect)
#define WL1_CME_UNLOCK_MCVS_MUTEX()  REX_ISR_UNLOCK(&wl1qicectrl_ext_cntrl_params.wl1_cme_mcvs_crit_sect)

/*===========================================================================
FUNCTION wl1_cme_init

DESCRIPTION
This function initializes all the one-time and control parameters.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_nv_read

DESCRIPTION
Reads the CME NV value and sets the overrides appropriately.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_nv_read( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_cell_drop

DESCRIPTION
Notifies CME that a cell has been dropped, causing CME to drop the cell from
the P/QSET if it is present.  If the cell is in either set, the SRCH callback 
is deregistered.  It is registered again when the cell drop done handler is
called.  

DEPENDENCIES
None

RETURN VALUE
TRUE

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_cell_drop(
  /* The cell db index of the cell being dropped. */
  uint16 cell_idx,
  /* Client cb after cell is deconfigured */
  WL1_CME_FREE_CELL_ID_CB_FUNC_TYPE *free_cell_id_cb, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_free_cell_db_idx

DESCRIPTION

DEPENDENCIES
None

RETURN VALUE
TRUE

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_free_cell_db_idx(uint16 num_cells, WL1_CME_FREE_CELL_ID_CB_FUNC_TYPE *free_cell_id_cb, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_hs_ops_needed

DESCRIPTION
API called by HS to see if CME cfg is required for transition to new state

DEPENDENCIES
None

RETURN VALUE
TRUE is CME cfg is required; Else FALSE

SIDE EFFECTS
Return value of TRUE indicates need for break in context
===========================================================================*/
boolean wl1_cme_hs_ops_needed(boolean hs_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_cleanup

DESCRIPTION
API to kickoff the cleanup check and sending cleanup confirmation

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
Cleanup confirmation will be sent once CME cleanup is completed
===========================================================================*/
void wl1_cme_cleanup( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_suspend_cme_triage

DESCRIPTION
Suspend CME triage

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_suspend_cme_triage(uint16 client_id, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_resume_cme_triage

DESCRIPTION
Resume CME triage

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_resume_cme_triage(uint16 client_id, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_srch_config_cells_cb

DESCRIPTION
Callback called by searcher when requested cells have been configured

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_srch_config_cells_cb(uint16 cell_bmsk, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_is_any_non_primary_cells_configured

DESCRIPTION
checks to see if any non primary configured cells idx present in the 
cell_bmask passed as argument

DEPENDENCIES
None

RETURN VALUE
TRUE : if any non primary cells present in bmask
FALSE: Otherwise

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_cme_is_any_non_primary_cells_configured(uint16 cell_bmsk, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_srch_deconfig_cells_cb

DESCRIPTION
Callback called by searcher when requested cells have been deconfigured

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_srch_deconfig_cells_cb(uint16 cell_bmsk, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_get_qset_neighbours

DESCRIPTION
Function returns a bitmask with cells that have been configured by CME 

DEPENDENCIES
None

RETURN VALUE
Cell Bitmask 

SIDE EFFECTS
None
===========================================================================*/
uint16 wl1_cme_get_qset_neighbours( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================
FUNCTION wl1_cme_mini_dump_cb

DESCRIPTION
This function Records Mini Dump Information for Debug

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_mini_dump_cb(void* debug_dump_ptr);
#endif

/*===========================================================================
FUNCTION wl1_cme_disable_schic_for_sho

DESCRIPTION
This function is called during an ASU if the ASET count is >1. It will disable
SCHIC if HS is not enabled.

DEPENDENCIES
None

RETURN VALUE
TRUE if ASU can proceed; FALSE if ASU needs to wait for SCHIC disable

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_cme_disable_schic_for_sho(
  L1M_NEXT_CMD_HANDLER_CB_FUNC_TYPE *l1m_next_cmd_handler_cb, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_is_mcpm_relinquish_pending

DESCRIPTION
Query if CME is pending on QICE MCPM relinquish

DEPENDENCIES
None

RETURN VALUE
TRUE if QICE MCPM relinquish, FALSE otherwise

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_cme_is_mcpm_relinquish_pending( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_is_cme_pending

DESCRIPTION
Gets the value of cme pending flag.

DEPENDENCIES
None

RETURN VALUE
TRUE if CME pending flag is set, FALSE otherwise

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_cme_is_cme_pending( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_THOR_MODEM
/*===========================================================================
FUNCTION wl1_cme_handle_irpm_bump_down

DESCRIPTION
Called to notify CME of potential IRPM Clock Bump Down

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_handle_irpm_bump_down(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_handle_irpm_bump_up

DESCRIPTION
Called to notify CME of potential IRPM Clock Bump Up

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_handle_irpm_bump_up(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_update_mcvs_info_from_sw_db

DESCRIPTION
Called to notify CME to update mcvs info based on sw database

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_update_mcvs_info_from_sw_db(wsub_id_e_type wsub_id);
#endif

#if defined(FEATURE_WCDMA_PLT) || defined(FEATURE_WCDMA_PLT_MODE)
#error code not present
#endif /* defined(FEATURE_WCDMA_PLT) || defined(FEATURE_WCDMA_PLT_MODE) */
/*===========================================================================
FUNCTION wl1_cme_ctrl_cmd_handler

DESCRIPTION
Handler for local cmd - L1_CME_CTRL_CMD

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_ctrl_cmd_handler(wl1_cme_ctrl_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_trigger_enable

DESCRIPTION
This function posts the control command to enable CME.
  Is useful to switch the execution to task context for the cases where the CME enable 
  can only be triggered from interrupt context.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_trigger_enable( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_trigger_disable

DESCRIPTION
This function posts the control command to disable CME.
  Is useful to switch the execution to task context for the cases where the CME disable 
  can only be triggered from interrupt context.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_trigger_disable( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_enable

DESCRIPTION
Enables the CME, registers callbacks.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void wl1_cme_enable(
  /* Used to identify the caller of this function. */
  uint8 calling_client, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_disable

DESCRIPTION
Disables the CME, clearing out the P/QSET and deregistering all callbacks.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_disable(
  /* Used to identify the caller of this function. */
  uint8 calling_client, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_is_cme_enabled

DESCRIPTION
Gets the state of CME.  

DEPENDENCIES
None

RETURN VALUE
TRUE if CME is enabled, FALSE otherwise.

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_cme_is_cme_enabled( wsub_id_e_type wsub_id);

boolean wl1_is_qice_enabled_on_c1( wsub_id_e_type wsub_id);
boolean wl1_is_qice_enabled_on_c2( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_is_qice_cfg_vpe_high

DESCRIPTION
Get the current QICE request for MCVS VPE update

DEPENDENCIES
None

RETURN VALUE
Boolean

SIDE EFFECTS
None
===========================================================================*/
extern uint8 wl1_cme_is_qice_cfg_vpe_high(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_is_triage_suspended

DESCRIPTION
Returns the Current CME traige Status 

DEPENDENCIES
None

RETURN VALUE
Boolean

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_cme_is_triage_suspended( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_rxd_freeze_flag_upd

DESCRIPTION
Callback called when receive diversity changes.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
extern void wl1_cme_rxd_freeze_flag_upd(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_get_qice_info_for_logging

DESCRIPTION
gives the qice state info per carrier ie whether it is qice off,full qice or qpc.
        00: QICE OFF
        01: FULL QICE ON
        10: invalid
        11: QPC/LEQ ON


DEPENDENCIES
None

RETURN VALUE
uint8 

SIDE EFFECTS
None
===========================================================================*/
uint8 wl1_cme_get_qice_info_for_logging(uint8 carrier, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION wl1_cme_get_qice_reason_for_logging

DESCRIPTION
gives the qice reason bitmask per carrier.

DEPENDENCIES
None

RETURN VALUE
uint8

SIDE EFFECTS
None
===========================================================================*/
uint8 wl1_cme_get_qice_reason_for_logging(uint8 carrier, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_get_power_log_packet_info

DESCRIPTION
Returns QICE info for W power log pkt.

DEPENDENCIES
None

RETURN VALUE
uint8

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_get_power_log_packet_info(wl1_power_logpkt_qice_info_struct_type *qice_info, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION cme_register_rxd_trans_reframe_cb

DESCRIPTION
This Function registers the RxD Callback to Drop MCVS Clocks.

DEPENDENCIES
None

RETURN VALUE


SIDE EFFECTS
None
===========================================================================*/
extern void cme_register_rxd_trans_reframe_cb(CME_RXD_TRANS_REFRAME_CB_FUNC_TYPE *rxd_trans_cb, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_is_qice_rx_enabled

DESCRIPTION
Returns the boolean QICE Rx (On either carrier) enabled state.

DEPENDENCIES
None

RETURN VALUE
TRUE is there is at least one cell in the QSET and QICE is selected in the
demod path.

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_cme_is_qice_rx_enabled( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_is_hs_rx_update_pending

DESCRIPTION
Returns the boolean HS Rx update pending state.

DEPENDENCIES
None

RETURN VALUE
TRUE if a Rx update cmd is pending for HS receiver

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_cme_is_hs_rx_update_pending( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_is_txd_enabled_in_qset

DESCRIPTION
Check if any cell in the QSET has TxD enabled

DEPENDENCIES
None

RETURN VALUE
TRUE if any cell in the QSET has TxD enabled

SIDE EFFECTS
None
===========================================================================*/
boolean wl1_cme_is_txd_enabled_in_qset( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_register_primary_cell_reframe_rxd_cb_func

DESCRIPTION
Register rxd callback func for QICE primary cell reframe

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_register_primary_cell_reframe_rxd_cb_func(CME_PRI_CELL_REFRAME_RXD_CB_FUNC_TYPE *rxd_cb, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_get_ta_log_eq_info_for_cell

DESCRIPTION
Gets the value for eq_info in the per_cell_info_struct_type structure of the
WCDMA_FINGER_TA_INFO_LOG_PACKET_type structure.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
uint8 wl1_cme_get_ta_log_eq_info_for_cell(
  /* Cell db index of cell to get TA log info for. */
  uint8 cell_idx, wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_THOR_MODEM) || defined(FEATURE_WCDMA_TABASCO_MODEM)
/*===========================================================================
FUNCTION     wl1_cme_get_mcvs_info

DESCRIPTION
  This function reads the structure from the CME module that has details on the CME TxD status and QSET size across all carriers.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void wl1_cme_get_mcvs_info(wl1_cme_mcvs_struct_type *wmcvsdrv_cme_struct, wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_WCDMA_TABASCO_MODEM
/*===============================================================================
FUNCTION wl1_cme_update_srch_utilzation_param

DESCRIPTION
Updates the SRCH Utilization Factor for QICE operation

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===================================================================================*/
void wl1_cme_update_srch_cycle_param(uint8 cycle_factor, wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION wl1_cme_flush_logging

DESCRIPTION
Submits any EQ/SCHIC/QICE log packets that might be partially full.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_flush_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_update_eq_logging

DESCRIPTION
Updates the current EQ log packet by getting data from the FW/SW interface.
Prepares to submit the log packet if it fills up or if EQ logging is stopped.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_update_eq_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_submit_eq_log

DESCRIPTION
Called when the local command is processed, submits the EQ log packet.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_submit_eq_log( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION wl1_cme_update_schic_logging

DESCRIPTION
Updates the current SCHIC log packet by getting data from the FW/SW
interface.  Prepares to submit the log packet if it fills up or if SCHIC
logging is stopped.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_update_schic_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_submit_schic_log

DESCRIPTION
Called when the local command is processed, submits the SCHIC log packet.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_submit_schic_log( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION wl1_cme_update_qice_debug_logging

DESCRIPTION
Updates the current QICE DEBUG log packet by getting data from the FW/SW
interface.  Prepares to submit the log packet if it fills up or if QICE
logging is stopped.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_update_qice_debug_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_submit_qice_debug_log

DESCRIPTION
Called when the local command is processed, submits the QICE DEBUG log packet.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_submit_qice_debug_log( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_qdta_ops

DESCRIPTION
This API would set the WL1_CME_CLIENT_BMSK_RXD_QDTA bmsk and kickstart 
cme controller for QDTA related operations
DEPENDENCIES
None

RETURN VALUE
Boolean

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_qdta_ops(wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_CME_QICE_SFC_LOGGING
/*===========================================================================
FUNCTION wl1_cme_update_qice_sfc_logging

DESCRIPTION
Updates the current QICE SFC log packet by getting data from the FW/SW
interface.  Prepares to submit the log packet if it fills up or if QICE
logging is stopped.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_update_qice_sfc_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_submit_qice_sfc_log

DESCRIPTION
Called when the local command is processed, submits the QICE SFC log packet.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_submit_qice_sfc_log( wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_CME_QICE_SFC_LOGGING */

#ifdef FEATURE_WCDMA_CME_QICE_FFINV_LOGGING
/*===========================================================================
FUNCTION wl1_cme_update_qice_ffinv_logging

DESCRIPTION
Updates the current QICE FFINV log packet by getting data from the FW/SW
interface.  Prepares to submit the log packet if it fills up or if QICE
logging is stopped.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_update_qice_ffinv_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_cme_submit_qice_ffinv_log

DESCRIPTION
Called when the local command is processed, submits the QICE SFC log packet.

DEPENDENCIES
None

RETURN VALUE
None

SIDE EFFECTS
None
===========================================================================*/
void wl1_cme_submit_qice_ffinv_log( wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_CME_FFINV_LOGGING */

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION  wl1_qsh_mdump_collect_qice

DESCRIPTION
  Collect QICE variables.

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_qice(wsub_id_e_type wsub_id);
#endif


#ifdef WL1_CME_UNIT_TEST
#error code not present
#endif /* WL1_CME_UNIT_TEST */

#endif /* WL1_QICE_CTRL_H */
