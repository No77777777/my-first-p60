#ifndef DLNSCHMGR_H
#define DLNSCHMGR_H

/*===========================================================================
                 D L N S C H M G R . H

GENERAL DESCRIPTION
  This file contains the declarations and definitions of dlnschmgr needed
  across modules.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2007 - 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/tlm.c_v   1.153   17 Jul 2002 16:36:16   halbhavi  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/dlnschmgr.h#4 $ $DateTime: 2020/06/16 06:12:20 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ---------------------------------------------------------
06/15/20    rs      Delaying "mode timing command [24]" cmd to FW if WL1 sending it very close to DL frame boundary.
01/10/20    ac      Mini dump framework checkin
08/21/17    ss      Increase the threshold for DPCH setup from 1bpg to 2bpg
03/07/16    npn     Double DCH HHO timing database for W+W
11/16/15    sl      Modify the existing TLM_API to retrieve CELL INFO from TLM_DB
03/09/15    sr      DLCHMGR changes for W+W
01/19/15    sks     Global re-org for W+W
02/05/13    kcm     Changes to trigger TxLM disable when AICH is suspended in EDRX config
06/08/13    kcm     Added API to retrieve old dpch combiner pos
04/01/13    hk      Mainlined FEATURE_WCDMA_EFDPCH
03/29/13    sr      Changes to handle race condition for internal AICH resume op
                    and external AICH op
02/11/13    gsk/sr  EDRX Full Sleep initial check in.
08/14/12    hk      Added cell timing check for use in reverse HHO
07/24/12    geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
06/08/12    gv      Feature HS RACH Initial code check in
02/15/12    hk      Timeline alignment changes for HHO & W2G Failure recovery
08/11/10    hk      Changes to wait for the DSD response before proceeding
                    with AICH setup
04/06/10    ks      Added Enhanced FDPCH support
09/24/09    hk      Porting of AICH resetup feature to Offline
12/10/08    ks      Mainlining FEATURE_WCDMA_DL_ENHANCED
10/03/08    hk      Bringing in 7k Mailine fixes
01/25/08    vsr     Checking in Enhanced DL files onto mainline
01/15/08    vp      Added code comments and Removed static db
12/21/07    vb      removed prototype get aich params API 
11/07/07    ks      Added fdpch enum in static db index
11/06/07    vp      Moved DRX handle from dlchmgr to channel specific managers
                    and added shared channel support in sccpchmgr.
08/20/07    vsr     Moved in dl_get_aich_parms()
02/21/07    vp      Initial version
                    
                    ===========================================================================*/
#include "wcdma_variation.h"
#include "customer.h"
#include "comdef.h"
#include "l1const.h"
#include "dlchmgr.h"
#include "wl1drxmanager.h"

/*(AICH + DPCH)*/
#define DL_NSCH_CFG_DB_SIZE 2

/*1. If DPCH Timing differs by more than 256cx1 during TM-HHO then fail the 
  TM-HHO and send failure message to network. This happens during wrong 
  Tau DPCH configuration by network
  2. If DPCH Timing differs by more than 256cx1 while resuming to W after 
  a failed InterRAT Handover, then fail the resumption to W. This happens 
  if searcher detects a false peak for serving cell. 
  In terms of cx8, if the timing diff is positive, it should not be more than 
  256*8 cx8; if the timing diff is negative (rounded), it should not be less than
  (150-1)*256*8 cx8
  3. If PCRC includes DPCH_frame_offset change (can change at the max by 1bpg) 
  then WL1 doesn't do timing slam, instead it will let the finger time tracking
  to converge to the new DPCH position. Before DPCH timing converges to the new
  position if an LTA happens then there is a chance of old_dpch and new_dpch pos
  differing by more than 1bpg hence failing the resume unnecessarily. Relaxing the 
  check to 2bpgs*/
#define DL_MAX_DPCH_TIMING_DIFF (2*CHIPX8_PER_256_CHIP_UNITS)
#define DL_MIN_DPCH_TIMING_DIFF ((NUM_256_CHIP_UNITS_PER_FRAME-2)*CHIPX8_PER_256_CHIP_UNITS)

#define DL_AICH_RESETUP_FING_FREQ_IN_CFN (5)
#define DL_AICH_RESETUP_MAX_CYCLE_IN_CFN (20)

typedef enum {
DL_NSCH_PHYCHAN_AICH,
DL_NSCH_PHYCHAN_HS_RACH_AICH,
DL_NSCH_PHYCHAN_DPCH,
DL_NSCH_PHYCHAN_FDPCH,
DL_NSCH_PHYCHAN_TOTAL_NUM
} dl_nsch_static_db_idx_enum_type;

typedef struct {
boolean in_use;

dlchmgr_cfg_db_idx_type chmgr_cfg_db_idx;

dl_nsch_static_db_idx_enum_type static_db_idx;

drx_entity_handle_type drx_handle;
} dl_nsch_cfg_db_struct_type;

typedef struct
{
  /*cell db idx*/
  uint16 cell_db_idx;
  /*scr code*/
  uint16 scr_code;
  /*freq*/
  uint16 frequency;
  /*t-dpch*/
  uint16 tto;
}dl_nsch_rl_info_struct_type;

/*Structure to hold the HHO Info*/
typedef struct {
/*Old DPCH combiner timing wrt FRC in Cx8*/
int32 old_dpch_comb_timing_wrt_frc_in_cx8;

/*New DPCH Timing wrt FRC in cx8*/
int32 new_dpch_timing_wrt_frc_in_cx8;

  int32 old_ul_dpch_pos;

/*Structure to store the cfn - wrc diff before the drop of dpch during Forward
  HHO*/
tlm_cfn_struct_type old_dpch_cfn_wrc_diff;

tlm_tfn_struct_type old_dpch_tfn_wrc_diff;
  /*dpch rl info*/
  dl_nsch_rl_info_struct_type rl_info[L1_ASET_MAX];

} dl_dch_hho_info_struct_type;

/*Structure to hold static information corresponding to AICH channel*/
typedef struct {
uint16 dl_aich_ack_thresh_adj;

uint16 dl_aich_nak_thresh_adj;

uint16 dl_aich_resetup_get_fing_freq;

uint16 dl_aich_resetup_max_cycle;
} dl_aich_static_info_struct_type;

/*Structure to hold information regarding AICH channel resetup*/
typedef struct {
  boolean dl_aich_in_resetup_mode;

  boolean dl_aich_resetup_phch_op_started;

  boolean dl_aich_internal_op;

  uint16 dl_aich_resetup_get_fing_cnt;

  uint16 dl_aich_resetup_cycle_cnt;

  l1_dl_phychan_enum_type aich_type;
} dl_aich_resetup_info_struct_type;

/*Structure containing internal dlnschmgr parameters*/
typedef struct {
  /*Structure to hold HHO Info*/
  dl_dch_hho_info_struct_type dl_dch_hho_info; 
} dlnschmgr_int_cntrl_params_type;

#define DL_AICH_RESETUP_RESET_ALL_DATA()         \
  do {         \
    dl_aich_resetup_info.dl_aich_in_resetup_mode = FALSE;         \
    dl_aich_resetup_info.dl_aich_resetup_phch_op_started = FALSE;         \
    dl_aich_resetup_info.dl_aich_resetup_get_fing_cnt = 0;         \
    dl_aich_resetup_info.dl_aich_resetup_cycle_cnt = 0;         \
    dl_aich_resetup_info.dl_aich_internal_op = FALSE;         \
  }while(0)

#define DL_AICH_RESETUP_IS_GET_FING_CNT_MAX()  \
  (dl_aich_resetup_info.dl_aich_resetup_get_fing_cnt >= dl_aich_static_info.dl_aich_resetup_get_fing_freq)

#define DL_AICH_RESETUP_GET_CYCLE_CNT()  \
  (dl_aich_resetup_info.dl_aich_resetup_cycle_cnt)

#define DL_AICH_RESETUP_IS_CYCLE_CNT_MAX()  \
  (dl_aich_resetup_info.dl_aich_resetup_cycle_cnt >= dl_aich_static_info.dl_aich_resetup_max_cycle)

#define DL_AICH_RESETUP_INC_GET_FING_CNT()  \
  (dl_aich_resetup_info.dl_aich_resetup_get_fing_cnt++)

#define DL_AICH_RESETUP_INC_CYCLE_CNT()  \
  (dl_aich_resetup_info.dl_aich_resetup_cycle_cnt++)

#define DL_AICH_IS_IN_RESETUP_MODE()  \
  (dl_aich_resetup_info.dl_aich_in_resetup_mode == TRUE)

#define DL_AICH_HAS_RESETUP_PHCH_OP_STARTED()  \
  (dl_aich_resetup_info.dl_aich_resetup_phch_op_started == TRUE)

#define DL_AICH_IS_INTERNAL_OP_IN_PROGRESS()  \
  (dl_aich_resetup_info.dl_aich_internal_op == TRUE)

/* This structure holds the reponse functions for channel type: AICH*/
extern dlchmgr_resp_func_cb_struct_type dl_nsch_aich_resp_func_cb;

/* This structure holds the reponse functions for channel type: DPCH*/
extern dlchmgr_resp_func_cb_struct_type dl_nsch_dpch_resp_func_cb;

/* This structure holds the index management functions for all non-scheduled
   channels (nsch) like dpch/aich*/
extern dlchmgr_idx_mgmt_func_struct_type dl_nsch_idx_mgmt_func;

/* This structure holds the pre and post operation callbacks
   for non-scheduled (nsch) channels like dpch/aich etc*/
extern dlchmgr_op_cb_func_struct_type dl_nsch_aich_op_cb_func;

extern dlchmgr_op_cb_func_struct_type dl_nsch_dpch_op_cb_func;

extern const channel_cb_struct_type dl_nsch_drx_cb_functions;

extern dl_nsch_cfg_db_struct_type dl_nsch_cfg_db[DL_NSCH_CFG_DB_SIZE];

extern dl_aich_static_info_struct_type dl_aich_static_info;

extern dl_aich_resetup_info_struct_type dl_aich_resetup_info;

/*===========================================================================
FUNCTION        dl_nsch_alloc_cfg_db_idx

DESCRIPTION     This function finds a free entry in the bcch manager cfg 
                db and returns it to the caller.

DEPENDENCIES    None

RETURN VALUE
                cfg-db-idx: The allotted index in bcch manager cfg db.

SIDE EFFECTS    None.
===========================================================================*/
extern dl_ch_spec_cfg_db_idx_type dl_nsch_alloc_cfg_db_idx(dlchmgr_cfg_db_idx_type chmgr_cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_dealloc_cfg_db_idx

DESCRIPTION     This function frees entry in the nsch manager cfg db.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void dl_nsch_dealloc_cfg_db_idx(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_per_cfg_init

DESCRIPTION     This function initializes the nsch db for perticular cfg db idx

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void dl_nsch_per_cfg_init(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION        dl_nsch_pre_add_cb

DESCRIPTION     This function does any processing needed before the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-add operations were successful.
                FALSE: At least one pre-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_nsch_pre_add_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx,
  l1_dl_phychan_db_struct_type *phychan_db, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_post_add_cb

DESCRIPTION     This function does any processing needed after the addition 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-add operations were successful.
                FALSE: At least one post-add operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_nsch_post_add_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_next_frame_wanted_cb

DESCRIPTION     This function returns the next frame wanted to DRX. As nsch 
                channels have no schedule, it always returns ALWAYS_ACTIVE.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
extern boolean dl_nsch_next_frame_wanted_cb(drx_entity_handle_type          entity_handle,
  drx_manager_sleep_struct_type  *sleep_info_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_pre_recfg_cb

DESCRIPTION     This function does any processing needed before the re-cfg 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-recfg operations were successful.
                FALSE: At least one pre-recfg operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_nsch_pre_recfg_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_post_recfg_cb

DESCRIPTION     This function does any processing needed after the re-cfg 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-recfg operations were successful.
                FALSE: At least one post-recfg operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_nsch_post_recfg_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_pre_drop_cb

DESCRIPTION     This function does any processing needed before the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-drop operations were successful.
                FALSE: At least one pre-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_nsch_pre_drop_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_post_drop_cb

DESCRIPTION     This function does any processing needed after the drop 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-drop operations were successful.
                FALSE: At least one post-drop operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_nsch_post_drop_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        dl_nsch_get_add_state

DESCRIPTION     This function finds the add state for AICH channel.

DEPENDENCIES    None

RETURN VALUE
                ACTIVE: If the channel has to be added in ACTIVE state (combiner is setup)
                SUSPEND: If the channel has to be added in SUSPEND state (combiner is not setup)

SIDE EFFECTS    None
===========================================================================*/
extern dl_phch_state_enum_type dl_nsch_get_add_state(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_cfn_upd_cb

DESCRIPTION     This function is called from Sequencer every pccpch frame boundary
                in interrupt context. This function posts a local command so that
                AICH resetup processing can take place from task context.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void dl_nsch_cfn_upd_cb(uint8 nsch_cfn, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_aich_reset_resetup_mode

DESCRIPTION     This function resets the resetup related data.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void dl_aich_reset_resetup_mode(void);

/*===========================================================================
FUNCTION        dl_nsch_proc_cfn_upd_cmd

DESCRIPTION     This function does the demod status dump for resuming the AICH setup
                Call back will be provided to DSD upon which ch setup can be resumed

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void dl_nsch_proc_cfn_upd_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_pre_resume_cb

DESCRIPTION     This function does any processing needed before the resumption 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All pre-resume operations were successful.
                FALSE: At least one pre-resume operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_nsch_pre_resume_cb(drx_entity_handle_type entity_handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_post_resume_cb

DESCRIPTION     This function does any processing needed after the resumption 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-resume operations were successful.
                FALSE: At least one post-resume operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_nsch_post_resume_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_post_susp_cb

DESCRIPTION     This function does any processing needed after the suspend 
                of the channel.

DEPENDENCIES    None

RETURN VALUE
                TRUE: All post-suspend operations were successful.
                FALSE: At least one post-suspend operation failed.

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_nsch_post_susp_cb(dl_ch_spec_cfg_db_idx_type cfg_db_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_NSCH_IS_CFN_CALC_FROM_WRC_NEEDED

DESCRIPTION     This function returns the cfn_wrc_diff based on the HHO type

DEPENDENCIES    None

RETURN VALUE
                TRUE: if the L1 is in one of the following HHO types
                1.Forward or Reverse Timing Maintained HHO
                2.Reverse Timing Initialized HHO
                And in these cases returns the stored cfn_wrc_diff

SIDE EFFECTS    None
===========================================================================*/
extern boolean dl_nsch_is_cfn_calc_from_wrc_needed(
  tlm_cfn_struct_type *cfn_wrc_diff,
  tlm_tfn_struct_type *tfn_wrc_diff, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_get_tx_timing

DESCRIPTION     This function calculates the UL DPCH position for a particular channel type

DEPENDENCIES    None

RETURN VALUE
                ACTIVE: Returns the Tx position

SIDE EFFECTS    None
===========================================================================*/
extern uint32 dl_nsch_get_tx_timing(l1_dl_phychan_enum_type phychan_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_get_rx_timing

DESCRIPTION     This function calculates the Rx position for a particular channel type

DEPENDENCIES    None

RETURN VALUE
                ACTIVE: Returns the Tx position

SIDE EFFECTS    None
===========================================================================*/

extern void dl_nsch_get_rx_timing(l1_dl_phychan_enum_type phychan_type,tlm_cfn_struct_type *chan_cfn, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_nsch_check_ref_cell_pos_out_of_range

DESCRIPTION     This function checks if the ref position for a cell that is passed in 
                fits the current dpch channnel timing. Should be used only in cases
                of resume back to old config, will return false in other cases

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern boolean dl_nsch_check_ref_cell_pos_out_of_range(srch_cell_struct_type *cell_ptr, 
                                                       uint32 limit, uint32 *expected_ref_pos, wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_HS_FACH_DRX
/*===========================================================================
FUNCTION        dl_nsch_register_aich_for_edrx

DESCRIPTION     This function register/deregisters AICH and registers EDRX

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
extern void dl_nsch_register_aich_for_edrx(boolean register_aich, wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_HS_FACH_DRX */

/*===========================================================================
FUNCTION        dl_nsch_set_aich_internal_op

DESCRIPTION     This function sets the dl_aich_internal_op to TRUE for any internal
                          AICH operation

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
void dl_nsch_set_aich_internal_op(void);

/*===========================================================================
FUNCTION        dlnschmgr_retreive_old_dpch_comb_pos

DESCRIPTION     This function returns the old dpch combiner position.

DEPENDENCIES    None

RETURN VALUE    int32 combiner pos

SIDE EFFECTS    None.
===========================================================================*/
extern int32 dlnschmgr_retreive_old_dpch_comb_pos(wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION        dlnschmgr_qsh_mdump_collect

DESCRIPTION     Collect Mdump parameters

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None.
===========================================================================*/
QSH_MDUMP_FN_ATTR void dlnschmgr_qsh_mdump_collect(void);

QSH_MDUMP_FN_ATTR void dlnschmgr_qsh_mdump_collect_sub(wsub_id_e_type wsub_id);

#endif

/*===========================================================================
FUNCTION        dl_nsch_get_new_dpch_timing_wrt_frc_in_cx8

DESCRIPTION     returns new_dpch_timing_wrt_frc_in_cx8

DEPENDENCIES    None

RETURN VALUE    new_dpch_timing_wrt_frc_in_cx8

SIDE EFFECTS    None.

===========================================================================*/
extern uint32 dl_nsch_get_new_dpch_timing_wrt_frc_in_cx8(wsub_id_e_type wsub_id);

#endif /* DLNSCHMGR_H */

