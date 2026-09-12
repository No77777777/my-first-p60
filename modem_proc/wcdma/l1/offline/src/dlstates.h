#ifndef DL_STATES_H
#define DL_STATES_H
/*===========================================================================
                        D L _ S T A T E S . H

GENERAL DESCRIPTION
  This module implements the function that are required for the downlink
  operation. The function included in this module control the operation of
  demodulator front and back end.

EXTERNALIZED FUNCTIONS


INTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000-2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/dlstates.h_v   1.5   29 May 2002 11:31:06   msatyana  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/dlstates.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
01/16/18    nd      F3 reduction
08/16/17    ss      Changes for LTA timeline optimization
10/20/15    sl      QXDM cmd changes for DEMOD
07/02/15    ap      Initial check-in for EFACH+MSIM
03/19/15    sr      W+W changes for dlphch/dltri/dlchmgr init functions
02/28/15    rsr     W + W Phase 2 Checkin.
02/04/15    sl      Avoid performing CM suspend operation twice in connected mode cleanup
01/29/14    pr      WL1M global reorg for WplusW
10/28/14    sr      Initial checkin for 0x187B WCDMA power log pkt
07/15/14    ac      NV reorganization
03/18/14    mit     F3 cleanup, enabling via NV's
02/05/14    pr      Added support for DCHSUPA Finger Switching Ops.
01/27/14    sr      Changes for NV 70188 enhancement
11/27/13    hk      Unification of non-intra module cleanup procedures
05/20/13    sad     Support for Enhanced Layer-1 Manager
02/20/13    hk      Removed FEATURE_INTER_SIB_SLEEP
02/20/13    hk      Mainlined FEATURE_WCDMA_DRX_MANAGER
10/22/12    kcm     Reducing F3 prints in Idle Mode scenarios
08/09/12    hk      Streamlined FACH and DCH module cleanup procedures
03/16/12    ms      Added support for FEATURE_WCDMA_HS_FACH
02/08/12    vs      Feature cleanup.
02/01/12    gnk     Nikel Feature cleanup.
09/12/11    kcm     Changed an existing function to perform CM and DL 
                    cleanup for DCH and non DCH states respectively
09/01/11    yh      Move srch cm cleanup after cm mgr is done.
10/15/10    yh      Added support for W2L Compressed mode
05/03/10    hk      Fixed compiler warnings
05/25/09    hk      Fixed compiler warnings
05/08/09    sup     Changing the includes for the renaming of header files  dec.h, enci.h, l1m.h, srch.h, srchi.h, 
                    srchlog.h to wdec.h, wenci.h, wl1m_v.h, wsrch.h, wsrchi.h, wsrchlog.h
12/10/08    ks      Mainlining FEATURE_WCDMA_DL_ENHANCED
11/11/08    ks      Featurising dl_dem_idle_init, not used for new DL
10/03/08    hk      Bringing in 7k Mailine fixes
01/29/08    scm     Move normalize_sfn() macro from dlstates.h to l1utils.h.
10/24/07    vsr     Mainlining MBMS and Enhanced DL changes 
07/30/07    rgn     Changes to move cm gsm cleanup functions to a separate function
12/13/06    kps     Cleanup so build can compile with FEATURE_UMTS_BMC undefined
10/10/06    ms      Added prototype for dl_dch_cleanup_after_hs_teardown.
08/14/06    ub      Added support for HSUPA.
08/09/06    scm     Add extern to dl_aset_sync_info_update_needed.
05/26/06    kps     Change the prototype of dl_sib_wake_up_pccpch_n() to return
                    the number of channels woken up.
04/12/06    mc      Added DL DCH maintenance event handler.
01/08/06    kps     Change SIB sleep feature name to FEATURE_INTER_SIB_SLEEP.
                    Added several functions to support this feature.
09/25/05    kps     Preliminary SIB sleep optimization support. 
                    Code changes are inside #ifdef FEATURE_SIB_SLEEP_OPTIMIZATION
08/10/05    kps     add dl_bmc_ctch_sccpch_offset() and
                    dl_bmc_get_msec_to_sleep_for_ctch(uint16)
06/22/05    scm     Extern dl_bmc_extra_ctch_debug for extra run-time debug.
06/17/05    kps     CR #65225 - Support for ctch filtering whenver
                    FEATURE_UMTS_BMC is defined fix for normalize_sfn() 
05/25/05    kps     add DL_BMC_INVALIDATE_SFN() for BMC.
05/17/05    kps     added dl_bmc_get_sleep_info() and dl_bmc_next_frame_wanted()
                    to support sleep optimization for BMC.
04/25/05    scm     Add extern to dl_bmc_drop_ctch_sccpch_if_setup().
03/22/05    scm     Support CTCH SCCPCH phychan drop.
03/21/05    scm     dl_bmc_enable_sccpch() now returns boolean.
03/15/05    scm     Externalize some functions for BMC.
03/15/05    kps     Implement non-DRX mode in dl_bmc_frame_is_wanted()
03/03/05    scm     Add CCTrCh index parameter to dl_bmc_cctrch_setup_done().
03/03/05    gs      Externalize variable l1_dch_cleanup_hs_cleanup_pending
02/27/05    scm     Adding more function prototypes for BMC support.
02/27/05    scm     Save config info for SCCPCH carrying CTCH for setup later.
02/22/05    scm     Supporting broadcast message reception on CTCH.
12/05/03    scm     Externalize dl_end_cm_suspended_state().
05/29/02    m       Added prototypes to support LOD
01/24/2001  mdl     consolidated dldemstates.h and dldecstates.h into this
                    file dlstates.h.  Changed return types of all state
                    cleanup routines to boolean

01/17/2001  mdl     changed all state cleanup routines to take the next state
                    as an input parameter

11/02/2001  mdl     first cut
===========================================================================*/
#include "wcdma_variation.h"
#include "wl1m.h"

typedef enum
{
  DL_PCCPCH_QXDM_DBG = 0,
  DL_SRCH_FING_DISCREPANCY_QXDM_DBG = 1,
  DL_TIMING_UL_DL_QXDM_DBG = 2,
  DL_SYNC_FAILURE_QXDM_DBG = 3,
  DL_UE_MTPL_QXDM_DBG = 4,
  DL_CPC_EDRX_QXDM_DBG = 5,
  DL_XO_PARAMS_QXDM_DBG = 6,
  DL_NUM_TYPES_QXDM_DBG
}dl_qxdm_debug_enum_type;

typedef struct
{
  /* PSC on which debug crash condition needs to be evaluated */
  uint16 psc_qxdm_dbg[DL_NUM_TYPES_QXDM_DBG];

  /* Frequency on which debug crash condition needs to be evaluated*/
  uint16 freq_qxdm_dbg[DL_NUM_TYPES_QXDM_DBG];
   
  /*state of Layer 1 during which UE should crash */
  l1m_state_enum_type l1m_state_qxdm_dbg[DL_NUM_TYPES_QXDM_DBG];

  /* Counter which keeps incrementing the number of iterations */
  uint16 cur_dbg_counter[DL_NUM_TYPES_QXDM_DBG];
  
  /* set to TRUE upon receiving QXDM cmd  */
  boolean pccpch_dbg_enable;

  /* PCCPCH debugging Sub-Type */
  uint8 pccpch_dbg_type;

  /* Type of PCCPCH for which UE needs to crash */
  dl_bcch_static_db_idx_enum_type static_db_idx_dbg;

  /* Number of PCCPCH decode attempts after which UE should crash */
  uint8 num_pccph_attempts;

  /* set to TRUE upon receiving QXDM cmd  */
  boolean srch_fing_dbg_enable; 

  /* SRCH-FING debugging Sub-Type */
  uint8 srch_fing_dbg_type;

  /* Energy threshold in Ec/Io */
  uint8 srch_fing_eng_thres;

  /* Number of iterations after which UE should crash */
  uint16 num_srch_fing_iterations;

  /* set to TRUE upon receiving QXDM cmd  */
  boolean ul_dl_timing_dbg_enable;
  
  /* Timing debugging Sub-Type */
  uint8 ul_dl_timing_dbg_type;

  /* Timing delta; Should be in cx1 units */
  uint16 ul_dl_timing_thresh;
  
  /* Number of iterations after which UE should crash */
  uint8 num_ul_dl_timing_iterations;

  /* set to TRUE upon receiving QXDM cmd  */
  boolean sync_fail_dbg_enable;

  /* Carrier Index on which Crash condition needs to be evaluated */
  uint8 carr_idx_dbg;

  /* Sub-Type */
  uint8 sync_fail_dbg_type;

  /* Number of iterations after which UE should crash*/  
  uint16 num_sync_fail_iterations;

  /* set to TRUE upon receiving QXDM cmd  */
  boolean ue_mtpl_dbg_enable;

  /* Sub-Type */
  uint8 mtpl_dbg_type;
  
  /* Number of continuous frames for which UE has to stay in MTPL*/
  uint16 num_mtpl_thresh;

  /* set to TRUE upon receiving QXDM cmd */
  boolean edrx_cpc_dbg_enable;

  /* Sub-Type */
  uint8 edrx_dbg_type;
  
  /* Number of iterations after which UE should crash*/  
  uint16 num_edrx_dbg_thresh;
  
  /*set to TRUE upon receiving QXDM cmd */
  boolean xo_params_freq_common_dbg_enable;

  /*set to TRUE upon receiving QXDM cmd */
  boolean xo_params_rot_dbg_enable;
  
  /* Sub-Type */
  uint8 xo_params_dbg_type;

  /* If Freq_common value exceeds beyond this value then start incrementing the counter to crash */
  int16 xo_params_thresh;

  /* Number of iterations after which UE should crash */
  uint8 num_xo_params_iter;
  
  /* Parameter needs to be set for the 1st time when UE enters DPCH */
  boolean entering_1st_time_dpch;
  
  /* Need to discard the 1st deocded sub-frame */
  boolean discard_first_decode_dbg;

  /* Global variable which maintains the last fired interrupt's Sub-Frame number */
  uint16 global_subfr_num_dbg;
  
}dl_qxdm_debug_struct_type;

#define FIRST_BYTE_READ 0xFF
#define SECOND_BYTE_READ 0xFF00
#define THIRD_BYTE_READ 0xFF0000
#define FOURTH_BYTE_READ 0xFF000000

#define FIRST_AND_SECOND_BYTE_READ 0xFFFF
#define SECOND_AND_THIRD_BYTE_READ 0xFFFF00
#define THIRD_AND_FOURTH_BYTE_READ 0xFFFF0000

#define FIRST_BYTE_RSHIFT 0
#define SECOND_BYTE_RSHIFT 8
#define THIRD_BYTE_RSHIFT 16
#define FOURTH_BYTE_RSHIFT 24

#define DL_PCCPCH_QXDM_DBG_ENABLED()                (dlstates_ext_cntrl_params[wsub_id].dl_debug_thru_qxdm_content.pccpch_dbg_enable)
#define DL_SRCH_FING_DISCREPANCY_QXDM_DBG_ENABLED() (dlstates_ext_cntrl_params[wsub_id].dl_debug_thru_qxdm_content.srch_fing_dbg_enable)
#define DL_TIMING_UL_DL_QXDM_DBG_ENABLED()          (dlstates_ext_cntrl_params[wsub_id].dl_debug_thru_qxdm_content.ul_dl_timing_dbg_enable)
#define DL_SYNC_FAILURE_QXDM_DBG_ENABLED()          (dlstates_ext_cntrl_params[wsub_id].dl_debug_thru_qxdm_content.sync_fail_dbg_enable)
#define DL_UE_MTPL_QXDM_DBG_ENABLED()               (dlstates_ext_cntrl_params[wsub_id].dl_debug_thru_qxdm_content.ue_mtpl_dbg_enable)
#define DL_EDRX_CPC_QXDM_DBG_ENABLED()              (dlstates_ext_cntrl_params[wsub_id].dl_debug_thru_qxdm_content.edrx_cpc_dbg_enable)
#define DL_XO_PARAMS_QXDM_DBG_ENABLED()             (dlstates_ext_cntrl_params[wsub_id].dl_debug_thru_qxdm_content.xo_params_freq_common_dbg_enable)
#define DL_XO_PARAMS_ROT_QXDM_DBG_ENABLED()         (dlstates_ext_cntrl_params[wsub_id].dl_debug_thru_qxdm_content.xo_params_rot_dbg_enable)

#define QXDM_DEMOD_DBG_PSC_VALID(psc_dbg)             (psc_dbg != 0xFFFF)
#define QXDM_DEMOD_DBG_FREQ_VALID(freq_dbg)           (freq_dbg != 0xFFFF)
#define QXDM_DEMOD_DBG_L1M_STATE_VALID(l1m_state_dbg) (l1m_state_dbg < L1M_NUM_STATES)

#define DL_CONN_STATE_MSG(str, a, b, c) \
if (dl_conn_state_debug_msg_enable) \
{ \
 WL1_MSG3(HIGH,str, a, b, c); \
}\
else \
{ \
  WL1_MSG3(LOW,str, a, b, c); \
}

typedef struct
{
  /* This falg is used to control the processing of sfn_cfn_diff_cmd request from searcher.
    will be set to TRUE once if the modulator dpch timing is written to f/w. will be set to FALSE
    when demod initiates seq slm to dpch timing
*/
  boolean dl_tx_dpch_mod_timing_set_ready;

/* This flag will enable us to do certain things which we do
   only when we enter DCH for the first time. In other words
   it doesnt consider resumes, doesn't consider reconfigs etc.
   This is useful for doing such things as initializing the
   cell sync info(OFF/Tm) for the active set cells */
  boolean dl_aset_sync_info_update_needed;
}dlstates_ext_cntrl_params_mdump_type;

typedef struct
{
  dl_qxdm_debug_struct_type dl_debug_thru_qxdm_content;

  /* This sub-structure is for minidump purpose */
  dlstates_ext_cntrl_params_mdump_type dlstates_ext_ctrl_params_mdump;
  
}dlstates_ext_cntrl_params_type;

extern dlstates_ext_cntrl_params_type dlstates_ext_cntrl_params[WCDMA_NUM_SUBS];



/* This falg is used to control the processing of sfn_cfn_diff_cmd request from searcher.
    will be set to TRUE once if the modulator dpch timing is written to f/w. will be set to FALSE
    when demod initiates seq slm to dpch timing 
*/

/* 70188 NV support */
extern dl_nv_struct_type wl1_dl_nv;

/* Connected state f3 msg support */
extern boolean dl_conn_state_debug_msg_enable;

#define DL_NV_IS_FING_BUCKSHOT_ENABLED()  wl1_dl_nv.finger_Buckshot_Algo
#define DL_NV_IS_CPC_LITE_SLEEP_ENABLED()  wl1_dl_nv.CPC_Lite_Sleep
#define DL_NV_IS_FDPCH_BIAS_COMP_ENABLED()  wl1_dl_nv.FDPCH_SIRE_Bias_Comp

#define DL_NV_IS_TRIAGE_DBG_F3s_ENABLED()  wl1_dl_nv.enable_Triage_dbg_F3s
#define DL_NV_IS_IDLE_TRIAGE_DBG_F3s_ENABLED()  wl1_dl_nv.enable_Idle_Triage_dbg_F3s
#define DL_NV_IS_DL_CELL_CFG_DBG_F3s_ENABLED()  wl1_dl_nv.enable_DL_Cell_Cfg_F3s
#define DL_NV_IS_DL_PHCH_DBG_F3s_ENABLED()  wl1_dl_nv.enable_DL_PHCH_F3s

#define DL_NV_IS_HS_ONLY_TPC_ENABLED()  wl1_dl_nv.hsonlytpc_nv.hsonlytpc_enable
#define DL_NV_GET_HSONLYTPC_DUP_THRESH_PERCENTAGE ()  wl1_dl_nv.hsonlytpc_nv.hsonlytpc_dup_thresh_percentage
#define DL_NV_GET_HSONLYTPC_TPC_IMBAL_THRESH_PERCENTAGE ()  wl1_dl_nv.hsonlytpc_nv.hsonlytpc_TPC_imbal_thresh_percentage
#define DL_NV_IS_FING_SWITCH_DCUPA_ENABLED()  wl1_dl_nv.dynamic_fing_switch_dcupa

#define DL_NV_IS_DLOLPC_DBG_F3s_ENABLED()  wl1_dl_nv.enable_DL_Cell_Cfg_F3s
#define DL_NV_IS_WL1DEMODDRV_DBG_F3s_ENABLED()  wl1_dl_nv.enable_DL_Cell_Cfg_F3s
#define DL_NV_IS_DLTRCHMEAS_DBG_F3s_ENABLED()  wl1_dl_nv.enable_DL_Cell_Cfg_F3s
#define DL_NV_IS_W_POWER_LOGPKT_F3s_ENABLED()  wl1_dl_nv.enable_DL_Cell_Cfg_F3s
#define DL_NV_IS_RXB_PRIO_OVER_QTA_ENABLED()  wl1_dl_nv.rxb_prio_over_qta
#define DL_NV_IS_LTA_TL_OPTS_ENABLED() wl1_dl_nv.enable_lta_tl_opts

/*===========================================================================
FUNCTION        DL_DEC_CLEANUP

FILE NAME

DESCRIPTION     This is the one time cleanup routine for the demodulator module

DEPENDENCIES

RETURN VALUE    TRUE if the cleanup is complete or FALSE if cleanup has not
                yet finished and it will be indicated by subsequent local cmd.

SIDE EFFECTS
===========================================================================*/
boolean dl_dec_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_IDLE_INIT

FILE NAME

DESCRIPTION     This is the IDLE state initializer for the demodulator module

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dec_idle_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_IDLE_CLEANUP

FILE NAME

DESCRIPTION     This is the IDLE state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if the cleanup is complete or FALSE if cleanup has not
                yet finished and it will be indicated by subsequent local cmd.

SIDE EFFECTS
===========================================================================*/
boolean dl_dec_idle_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_ACQ_INIT

FILE NAME

DESCRIPTION     This is the ACQ state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dec_acq_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_ACQ_CLEANUP

FILE NAME

DESCRIPTION     This is the ACQ state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if the cleanup is complete or FALSE if cleanup has not
                yet finished and it will be indicated by subsequent local cmd.

SIDE EFFECTS
===========================================================================*/
boolean dl_dec_acq_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_BCH_INIT

FILE NAME

DESCRIPTION     This is the BCH state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dec_bch_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_BCH_CLEANUP

FILE NAME

DESCRIPTION     This is the BCH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if the cleanup is complete or FALSE if cleanup has not
                yet finished and it will be indicated by subsequent local cmd.

SIDE EFFECTS
===========================================================================*/
boolean dl_dec_bch_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_PCH_INIT

FILE NAME

DESCRIPTION     This is the PCH state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dec_pch_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_PCH_CLEANUP

FILE NAME

DESCRIPTION     This is the PCH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if the cleanup is complete or FALSE if cleanup has not
                yet finished and it will be indicated by subsequent local cmd.

SIDE EFFECTS
===========================================================================*/
boolean dl_dec_pch_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_FACH_INIT

FILE NAME

DESCRIPTION     This is the FACH state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dec_fach_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_FACH_CLEANUP

FILE NAME

DESCRIPTION     This is the FACH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if the cleanup is complete or FALSE if cleanup has not
                yet finished and it will be indicated by subsequent local cmd.

SIDE EFFECTS
===========================================================================*/
boolean dl_dec_fach_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_DCH_INIT

FILE NAME

DESCRIPTION     This is the DCH state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dec_dch_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEC_DCH_CLEANUP

FILE NAME

DESCRIPTION     This is the DCH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if the cleanup is complete or FALSE if cleanup has not
                yet finished and it will be indicated by subsequent local cmd.

SIDE EFFECTS
===========================================================================*/
boolean dl_dec_dch_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_INIT

FILE NAME

DESCRIPTION     This is the one time initialization function for the
                demodulator module.
DEPENDENCIES

RETURN VALUE    None

SIDE EFFECTS
===========================================================================*/
void dl_dem_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_GENERAL_CLEANUP

FILE NAME       dlstates.c

DESCRIPTION     This function performs the cleanup routines common to all
                states.  It begins the operation of tearing down all of
                the active physical channels.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    All of the active physical channels will be started to be
                torn down.
===========================================================================*/
boolean dl_dem_general_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_CLEANUP

FILE NAME

DESCRIPTION     This is the one time cleanup routine for the demodulator module

DEPENDENCIES

RETURN VALUE    TRUE if the cleanup is complete or FALSE if cleanup has not
                yet finished and it will be indicated by subsequent local cmd.

SIDE EFFECTS
===========================================================================*/
boolean dl_dem_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_IDLE_CLEANUP

FILE NAME

DESCRIPTION     This is the IDLE state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    FALSE indicating that cleanup is complete at the exit
                of this function

SIDE EFFECTS
===========================================================================*/
boolean dl_dem_idle_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_ACQ_INIT

FILE NAME

DESCRIPTION     This is the ACQ state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dem_acq_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_ACQ_CLEANUP

FILE NAME

DESCRIPTION     This is the ACQ state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    FALSE if channels have to be torn down, TRUE otherwise

SIDE EFFECTS
===========================================================================*/
boolean dl_dem_acq_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_BCH_INIT

FILE NAME

DESCRIPTION     This is the BCH state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dem_bch_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_BCH_CLEANUP

FILE NAME

DESCRIPTION     This is the BCH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
boolean dl_dem_bch_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_PCH_INIT

FILE NAME

DESCRIPTION     This is the PCH state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dem_pch_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_PCH_CLEANUP

FILE NAME

DESCRIPTION     This is the PCH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
boolean dl_dem_pch_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_FACH_INIT

FILE NAME

DESCRIPTION     This is the FACH state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dem_fach_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_FACH_CLEANUP

FILE NAME

DESCRIPTION     This is the FACH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if no channels have to be torn down, FALSE otherwise.

SIDE EFFECTS
===========================================================================*/
boolean dl_dem_fach_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_DCH_INIT

FILE NAME

DESCRIPTION     This is the DCH state initializer for the demodulator module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dem_dch_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DEM_DCH_CLEANUP

FILE NAME

DESCRIPTION     This is the DCH state cleanup routine for the demodulator
                module.

DEPENDENCIES

RETURN VALUE    TRUE if no channels have to be torn down, FALSE otherwise.

SIDE EFFECTS
===========================================================================*/
boolean dl_dem_dch_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_INIT

FILE NAME

DESCRIPTION     This is the one time initialization function for the downlink
                controller.  It just calls the associated decoder and
                demodulator initializers.

DEPENDENCIES

RETURN VALUE    None

SIDE EFFECTS
===========================================================================*/
void dl_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_ONE_TIME_INIT

FILE NAME

DESCRIPTION     

DEPENDENCIES

RETURN VALUE    None

SIDE EFFECTS
===========================================================================*/
extern void dl_one_time_init(void);

/*===========================================================================
FUNCTION        DL_CLEANUP

FILE NAME

DESCRIPTION     This is the one time cleanup routine for the downlink
                controller.  It simply calls the associated decoder and
                demodulator cleanup routines.

DEPENDENCIES

RETURN VALUE    The logical AND of the two dldec and dldem cleanup routines.

SIDE EFFECTS
===========================================================================*/
boolean dl_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_IDLE_INIT

FILE NAME

DESCRIPTION     This is the IDLE state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_idle_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_IDLE_CLEANUP

FILE NAME

DESCRIPTION     This is the IDLE state cleanup routine for the downlink
                controller.

DEPENDENCIES    None

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS    L1M may have to wait for a further local command to
                complete the exit from the IDLE state.
===========================================================================*/
boolean dl_idle_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_ACQ_INIT

FILE NAME

DESCRIPTION     This is the ACQ state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_acq_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_ACQ_CLEANUP

FILE NAME

DESCRIPTION     This is the ACQ state cleanup routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
===========================================================================*/
boolean dl_acq_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_BCH_INIT

FILE NAME

DESCRIPTION     This is the BCH state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_bch_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_BCH_CLEANUP

FILE NAME

DESCRIPTION     This is the BCH state cleanup routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
===========================================================================*/
boolean dl_bch_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_PCH_INIT

FILE NAME

DESCRIPTION     This is the PCH state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_pch_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_PCH_CLEANUP

FILE NAME

DESCRIPTION     This is the PCH state cleanup routine for the downlink
                controller.

DEPENDENCIES

RETURN VALUE    The logical AND of the dldem and dldec IDLE cleanup calls

SIDE EFFECTS
===========================================================================*/
boolean dl_pch_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_FACH_INIT

FILE NAME

DESCRIPTION     This is the FACH state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_fach_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_DCH_INIT

FILE NAME

DESCRIPTION     This is the DCH state initializer for the downlink controller.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_dch_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  dl_maint_event_handler

DESCRIPTION     
      This function is the maintenance handler for the DL DCH channel.

DEPENDENCIES
      None
      
RETURN VALUE
      None
      
SIDE EFFECTS
      None
===========================================================================*/
void dl_dch_maint_event_handler( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION        DL_DCH_CM_DCH_GSM_CLEANUP_DONE
                DL_DCH_CM_DCH_INTERF_CLEANUP_DONE
                DL_DCH_CM_DCH_LTE_CLEANUP_DONE
                DL_DCH_CM_STATE_MACHINE_SUSPEND_DONE

FILE NAME

DESCRIPTION     This is the DCH state CM cleanup done routine. It send a
                local command for module cleanup done for CM module and
                deregisters GSTMR tick handler.

DEPENDENCIES

RETURN VALUE    

SIDE EFFECTS
===========================================================================*/
extern void dl_dch_cm_state_machine_suspend_done( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        DL_CONN_STATE_CLEANUP

FILE NAME

DESCRIPTION     This is the connected (FACH/DCH) state cleanup routine for demodulator
                and decoder modules.

DEPENDENCIES

RETURN VALUE    

SIDE EFFECTS
===========================================================================*/
extern boolean dl_conn_state_cleanup(l1m_state_enum_type next_state, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        dl_perform_idle_cm_module_cleanup

FILE NAME

DESCRIPTION     This function cleans up CM module if the current state is DCH.   
                In DCH/NON-DCH case it sends DL module cleanup done command. 

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
extern void dl_perform_idle_cm_module_cleanup(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION dl_end_cm_suspended_state

DESCRIPTION
  This function does 3 things:
  - Call l1_cm_end_compressed_mode() to initialize CM manager data.
  - Disable the compressed mode update event on the general timeline.
  - Call l1cmsrch_transition_from_cell_dch() to clear CM measurement data.

  Normally these things are done when transitioning out of DCH with the
  l1m_dch_cleanup() call.  However, in the case of W2G HO, we still call
  l1m_dch_cleanup(), but it doesn't fully terminate compressed mode.  This is
  so we can RESUME WCDMA if the handover fails.  But if the handover succeeds,
  this function is called, so compressed mode can clean itself up.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void dl_end_cm_suspended_state( wsub_id_e_type wsub_id);

#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION        DL_LOG_INIT

FILE NAME

DESCRIPTION
	Associates log on demand handlers with the log codes

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void dl_log_init ( wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION      DL_DEM_ENABLE_DBG_MSGS

DESCRIPTION
  This function enables/disables dbg msgs from demod side

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
===========================================================================*/
extern void dl_dem_enable_dbg_msgs(boolean enable, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      DL_DEM_ENABLE_CONN_STATE_MSGS

DESCRIPTION
  This function enables/disables the debug f3s from Demod side

PARAMETERS
  None.

DEPENDENCIES
  None
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
===========================================================================*/
extern void dl_dem_enable_conn_state_msgs(boolean enable, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION wl1_dl_nv_read

DESCRIPTION
  Reads the DL NV value and sets the overrides appropriately.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
 ===========================================================================*/
void wl1_dl_nv_read( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION:    DL_DEBUG_PCCPCH_QXDM_CMD

DESCRIPTION: This function evaluates if UE needs to crash upon PCCPCH deocde.
             Case1: Generic Crash condition
             Case2: If the strongest finger RSSI is better than -12db
             Case3: If the decoded PCCPCH energy is zero
             Case4: (Case2 && Case3)

DEPENDENCIES:
             QXDM command has to be sent prior inorder to evaluate the below function

INPUTS:
             psc:  Primary Scrambling Code of the cell on which PCCPCH decode is attempted
             freq: Frequency of the cell on which PCCPCH decode is attempted
             static_db_idx: Type of PCCPCH being setup
             current_pccpch_attempt: Number of PCCPCH decode being attempted
             eng: Decoded PCCPCH energy

RETURN VALUE:
             None

SIDE EFFECTS:
             None
  
===========================================================================*/
extern void dl_dbg_qxdm_evaluate_pccpch(uint16 psc, uint16 freq, dl_bcch_static_db_idx_enum_type static_db_idx,uint16 current_pccpch_attempt,uint32 eng,boolean crc_status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DL_DEBUG_EVALUATE_SRCH_FING_DISCREPANCY_CMD

DESCRIPTION: This function evaluates if UE needs to crash upon discrepancy 
             b/w SRCHR and Finger parameters.
             Case1: if SRCHR path energies and Finger energies vary by more than a threshold for threshold durations
             Case2: If SRCHR paths are real (non-virtual paths) but the finger assoicated with the path is not in LOCK
             Case3: If Finger energy drops below a threshold
             Case4: If the difference in PN position of the finger's assigned pos and current pos is more than 512 cx1

DEPENDENCIES:
             QXDM command has to be sent prior inorder to evaluate the below function

INPUTS:
             path_ptr: Path pointer
             path_idx: Path index on which the crash condition needs is evaluated

RETURN VALUE
             None

SIDE EFFECTS
             None
  
===========================================================================*/
extern void dl_debug_evaluate_srch_fing_discrepancy(dl_tri_path_struct_type *path_ptr,uint8 path_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DL_DEBUG_EVALUATE_UL_DL_TIMING

DESCRIPTION: This function evaluates if UE needs to crash upon discrepancy b/w UL/DL Timing
             Case1: if Tx-Rx position is more than a threshold 
             Case2: No crash- Print an F3 which would log the current DL pos, UL pos and UL-DL diff


DEPENDENCIES:
             QXDM command has to be sent prior inorder to evaluate the below function

INPUTS:
             wrc_count: Current WRC counter in cx8
             combiner_count: Aligned combiner count in cx1
             tx_sys_count: Current Tx System Counter in cx8

RETURN VALUE
             None

SIDE EFFECTS
             None
===========================================================================*/
extern void dl_debug_evaluate_ul_dl_timing(uint32 wrc_count, uint32 combiner_count, uint32 tx_sys_count, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DL_DEBUG_EVALUATE_DL_SYNC_FAILURE

DESCRIPTION: This function evaluates if UE needs to crash upon PC SYNC FAILURE
             Case1: Crash if RLF/PhyChan Failure happens
             Case2: Crash UE upon DP Pilot Amp saturation
             Case3: Crash upon SIRE fluctuations

DEPENDENCIES:
             QXDM command has to be sent prior inorder to evaluate the below function

INPUTS:
             carr_idx: Carrier on which crash condition needs to be evaluated
             cur_pilot_amp: Current DP Pilot Amp; Valid only for Case2

RETURN VALUE
             None

SIDE EFFECTS
             None
===========================================================================*/
extern void dl_debug_evaluate_dl_sync_failure(uint8 carr_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DL_DEBUG_EVALUATE_UE_MTPL

DESCRIPTION: This function evaluates if UE needs to crash upon transmitting at MTPL
             If UE stays in MTPL for threshold frames then crash UE

DEPENDENCIES
             QXDM command has to be sent prior inorder to evaluate the below function

INPUTS:
             is_ue_at_mtpl: Boolean which tells if UE is in MTPL or not

RETURN VALUE
             None

SIDE EFFECTS
             None
===========================================================================*/
extern void dl_debug_evaluate_ue_mtpl(uint8 fw_pc_bmsk, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DL_DEBUG_EVALUATE_EDRX_CPC

DESCRIPTION: This function evaluates if UE needs to crash when WFW fails to 
             fire decode done ISRs during EDRX/CPC call


DEPENDENCIES: 
             QXDM command has to be sent prior inorder to evaluate the below function

INPUTS:
             scch_dec_subfr: Current Sub-Frame number

RETURN VALUE
              None

SIDE EFFECTS
              None
===========================================================================*/
#if defined( FEATURE_WCDMA_CPC_DRX) || defined(FEATURE_WCDMA_HS_FACH_DRX)
extern void dl_debug_evaluate_edrx_cpc(uint16 scch_dec_subfr, uint8 scchDemodEnabled, wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_CPC_DRX || FEATURE_WCDMA_HS_FACH_DRX */
/*===========================================================================
FUNCTION     DL_DEBUG_EVALUATE_XO_PARAMS

DESCRIPTION: This function evaluates if UE needs to crash when Freq_common is stuck/drops/jumps/exceeds a threshold


DEPENDENCIES: 
             QXDM command has to be sent prior inorder to evaluate the below function

INPUTS:
             freq_common: Freq_common at current frame

RETURN VALUE
              None

SIDE EFFECTS
              None
===========================================================================*/
extern void dl_debug_evaluate_xo_params(int16 freq_common, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DL_DEBUG_THROUGH_QXDM

DESCRIPTION: This function sets the demod global variables as received
             through diag command. This function is a part of the callback function for 
             diag command dispatch table.
  
DEPENDENCIES: 
             This function is called when "send_data 75 4 57 0 <val>" is issued from cmd 
             line interface from QXDM

RETURN VALUE
             None

SIDE EFFECTS
             None
===========================================================================*/
extern void dl_dbg_qxdm_demod_internal( uint16 psc, uint16 freq, uint8 l1m_state_to_crash, uint8 debug_switch_type, uint32 val, wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION     DL_DEBUG_QXDM_SET_DISCARD_FIRST_DECODE_DBG

DESCRIPTION: This function sets/resets the value of the global variable.
  
DEPENDENCIES: 

RETURN VALUE
             None

SIDE EFFECTS
             None
===========================================================================*/
extern void dl_dbg_qxdm_set_discard_first_decode_dbg(boolean val,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DL_DEBUG_QXDM_SET_ENTERING_1ST_TIME_DPCH

DESCRIPTION: This function sets/resets the value of the global variable.
  
DEPENDENCIES: 

RETURN VALUE
             None

SIDE EFFECTS
             None
===========================================================================*/
extern void dl_dbg_qxdm_set_entering_1st_time_dpch(boolean val,wsub_id_e_type wsub_id);
#endif
