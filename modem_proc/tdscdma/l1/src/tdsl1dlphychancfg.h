#ifndef TDSL1_DL_PHYCHAN_CFG_H
#define TDSL1_DL_PHYCHAN_CFG_H
/*============================================================================
                     L 1 _ D L _ P H Y C H A N _ C F G _ H
DESCRIPTION

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000-2006 by Qualcomm Technologies Incorporated.  All Rights Reserved.

============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/l1dlphychancfg.h_v   1.19   19 May 2002 03:21:12   halbhavi  $
$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsl1dlphychancfg.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
01/20/11    cxd     update tdsl1_dl_phycfg_pccpch_parms_struct_type, remove tstd.
11/30/10    zwj/ttl Added tstd_indicator in pccpch_parms_struct_type.
11/04/10    yzh     (3GPP 10.3.6.88 & 10.3.6.21) modify UL common timeslot info struct 
                    when modify Uplink&Downlink DPCH info from FDD to TDD.
10/25/10    zwj     Modified tdsl1_sccpch_parms_struct_type and tdsl1_pich_parms_struct_type according new L1 interface for TDD.
10/30/10    ysh     IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).
10/25/10    zwj     Modified tdsl1_dl_phycfg_sccpch_parms_struct_type and tdsl1_dl_phycfg_pich_parms_struct_type according new L1 interface for TDD.
05/24/10    ks      Corrected fdpch related featurization
04/06/10    ks      Added Enhanced FDPCH support
12/10/08    ks      Mainlining FEATURE_TDSCDMA_DL_ENHANCED
10/27/08    hk      Lint Fixes
10/03/08    hk      Bringing in 7k Mailine fixes
12/12/07    vsr     Removed enhanced DL featurization for pri_cpich_scr_code
                    in tdsl1_dl_phycfg_sccpch_parms_struct_type
10/24/07    vsr     Mainlining MBMS and Enhanced DL changes 
08/06/07    vsr     Moved cm related structures from dldem.h
08/08/07    vp      Added support for reading cell id of gps bch in dch state.
12/22/06    ksr     Rel-6 post-verification, skip sync A changes
10/17/06    ms      Added support for FDPCH.
20/03/06    vip     Lint Fixes
09/25/05    kps     Preliminary SIB sleep optimization support.
                    Code changes are inside #ifdef FEATURE_SIB_SLEEP_OPTIMIZATION
04/29/05    scm     Add ctch_fach_tti to tdsl1dlphycfg_cbs_drx_level_1_sched_struct.
02/24/05    scm     Small mod to tdsl1dlphycfg_cbs_drx_level_1_sched_struct.
02/09/05    scm     Add CBS (Cell Broadcast Services) data on SCCPCH
                    scheduling information to tdsl1_dl_phycfg_sccpch_parms_struct_type.
08/30/04    gs      Added interface definitions for HSDPA
08/31/04    src     Fixed a merge/featurisation issue with CLTD merges.
08/26/04    src     Merged in the latest CLTD changes with featurisation under
                    the feature flag FEATURE_TRANSMIT_DIVERSITY_SUPPORT.
06/25/04    scm     Add pccpch_s_for_cell_id to tdsl1_dl_phycfg_pccpch_parms_struct_type.
03/31/03    m       Added po3 field to dl dpch common info struct
05/08/02    sh      Changed primary scrambling code to scrambling code
                    in tdsl1_dl_phycfg_pccpch_parms_struct_type.
01/02/02    gs      1. Removed the enum type definifion for CM scrambling
                       code change type.
                    2. Changed the scr_code_change in DL DPCH RL info to
                       boolean from enum type.
                    3. Changed the DL common info CM info from structure to
                       tdsl1_cm_info_struct_type structure pointer.
                    4. Moved the TDSL1_MAX_RL macro definition to tdsl1sapcommon.h    
12/11/01    asr     Changed tdsl1_dl_phycfg_pich_parms_struct_type to use uint64 (instead of
                    uint32) for the two "imsi" parameters (IMSIs can be 15 digits long)
12/06/01    src     Added feature FEATURE_REL_99_JUNE_2001 and/or comments
                    around the R'99 2001-06 updates from 12/05/01.
12/05/01    src     Updated the tdsl1_dl_phycfg_pich_parms_struct_type with an additional
                    member that accounts for the IMSI-divided-by-8192 value
                    needed for paging occasion computation per R'99 2001-06.
10/15/2001  gs      defined the timer T312 and T313 for sync detection.
09/24/01    asr     Removed sec_scr_code from tdsl1_dl_phycfg_pich_parms_struct_type (taken
                    out in the December 2000 (v3.5.0) spec)
09/04/2001  sh      updated tdsl1_dl_phycfg_pich_parms_struct_type for DRX.
08/24/2001  mdl     removed scrambilng_code field from aich parameters
06/07/2001  mdl     changed TDSL1DLPHYCFG_MAX_CFN_TARGET_SFN_FRAME_OFFSET from 25 to 255

05/11/2001  mdl     removed pri_scr_code field from
                    tdsl1dl_dpch_info_per_rl_struct_type.  pri_cpich_scr_code
                    is used instead in tdsl1dl_info_per_rl_struct_type

04/27/2001  mdl     added bitfields for optional parameter inclusion indication
                    in various RRC Ordered config database structures.
                    Changed common_info to dpch_common in
                    tdsl1_dl_phycfg_common_info_struct_type

03/14/2001  mdl     added primary scrambling code to PCCPCH structure and
                    to the DPCH per radio link structure.
                    Moved the AICH parameters to this file under the feature
                    FEATURE_AICH_IN_DL_CFG.  This feature has not yet been
                    enabled.

01/04/2001  mdl     added #defines for number of paging indicators per
                    frame, i.e. PICH_PI_PER_FRAME{18, 36, 72, 144}

12/12/2000  mdl     substantially modified DPCH structures in
                    tdsl1_dl_phycfg_phychan_db_struct_type

11/27/2000  mdl     added use_pich to tdsl1_dl_phycfg_phychan_db_struct_type to
                    indicate whether a PICH should be setup when a SCCPCH
                    is being setup.

11/02/2000  mdl     initial cut
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#if 0
#include "comdef.h"
#include "tdsl1sapcommon.h"
#include "tdsl1std.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                       ENUM: TX_DIV_MODE_ENUM_TYPE

This enumerates the types of transmit diveresity and corresponds with
IE 10.3.6.74.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef enum {
  TDSL1DLPHYCFG_TX_DIV_NONE,
  TDSL1DLPHYCFG_TX_DIV_STTD,
  TDSL1DLPHYCFG_TX_DIV_CLOSED_LOOP_1,
  TDSL1DLPHYCFG_TX_DIV_CLOSED_LOOP_2,
  TDSL1DLPHYCFG_NUM_TX_DIV_MODES
} tdsl1dlphycfg_tx_div_mode_enum_type;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                     STRUCTURE:  PICH_PARMS_STRUCT_TYPE

This structure contains the information on the PICH and corresponds to
IE 10.3.6.41.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                         ENUM: DPC_MODE_ENUM_TYPE

This enumerates the types of Downlink Power Control and corresponds to
IE 10.3.6.19.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef enum {
  TDSL1DLPHYCFG_DPC_MODE_SINGLE_TPC,
  TDSL1DLPHYCFG_DPC_MODE_TPC_TRIPLE_IN_SOFT
} tdsl1dlphycfg_dpc_mode_enum_type;
/*- Legacy for TDS UL/DL dev, to be deleted later ******************************/

/*--------------------------------------------------------------------------
                     ENUM: L1_TIMING_IND_ENUM_TYPE

This enumerates whether or not radio link timing should be initialized
or maintained.
--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------
                  STRUCTURE: L1_DL_DPCH_COMMON_INFO_STRUCT_TYPE

This structure contains the parameters relating the the Downlink DPCH info
common for all Radio Links and corresponds to IE 10.3.6.14.

--------------------------------------------------------------------------*/
/*+ Legacy for TDS UL/DL dev, to be deleted later ******************************/
/*--------------------------------------------------------------------------
                 STRUCTURE: L1_DL_DPCH_INFO_PER_RL_STRUCT_TYPE

This structure contains the parameters relating to the Downlink DPCH info
for each Radio Link and corresponds to IE 10.3.6.17.

--------------------------------------------------------------------------*/
/* zhouya del for 10.3.6.21 begin */
#if 0
typedef struct {
  /* bitmask indicating the absence/presence of optional parameters.  Use
     the above defined bit definitions */
  uint16 opt_parms_present_mask;

  /* 10.3.6.17 Primary CPICH usage for channel estimation */
  //tdsl1dlphycfg_ch_est_cpich_usage_enum_type use_pri_cpich;

/* shanyufei this para should delete, FDD only, 3.2.4 begin */
  /* 10.3.6.17 DPCH frame offset */
  uint16 tdsframe_offset;
/* shanyufei this para should delete, FDD only, 3.2.4 end*/

  /* 10.3.6.17 -> 10.3.6.63 -> 10.3.6.64
     secondary CPICH secondary scrambling code */
  uint16 sec_cpich_sec_scr_code;

  /* 10.3.6.17 -> 10.3.6.63 secondary CPICH channelisation code */
  uint16 sec_cpich_ch_code;

  /* how many DPCH multicode channels are being set up */
  uint16 num_dpch;

  /* array of per DPCH info structures, there will be num_dpch valid
     entries */
  struct
  {
    #define DL_DPCH_CODE_CH_SEC_SCR_CODE_PRESENT 0x01
    #define DL_DPCH_CODE_CH_SCR_CODE_CHG_PRESENT 0x02
    /* optional parameter presence mask */
    uint8 opt_parm_present_mask;

    /* 10.3.6.17 -> 10.3.6.64 DPCH Secondary Scrambling Code */
    uint16 sec_scr_code;

    /* 10.3.6.17 Code Number */
    uint16 code_number;

  } dl_chan_code[TDSL1_DL_MAX_DPCH];

  /* 10.3.6.17 TPC Combination Index */
  uint8 tpc_idx;

  /* 10.3.6.17 Closed loop timing adjustment mode */
  uint8 closed_loop_timing_adj_mode;

} tdsl1dl_dpch_info_per_rl_struct_type;

/*+ Legacy for TDS UL/DL dev, to be deleted later ******************************/
/*--------------------------------------------------------------------------
               STRUCT: L1_DL_INFO_PER_RL_STRUCT_TYPE

This structure contains downlink channel information that is per radio link
--------------------------------------------------------------------------*/
/* zhouya del for 10.3.6.21 begin */

typedef struct {
/* shanyufei del for 8.6.6.4 begin */
  /* primary scrambling code of the CPICH */
  uint16 pri_cpich_scr_code;
/* shanyufei del for 8.6.6.4 end */

//#ifdef PDSCH_SUPPORTED
//  l1_pdsch_sho_dch_info_struct_type pdsch_sho_info;
//  l1_pdsch_code_map_struct_type pdsch_code_map;
//#endif

  tdsl1dl_dpch_info_per_rl_struct_type dpch_per_rl;

} tdsl1dl_info_per_rl_struct_type;
/* zhouya del for 10.3.6.21 end */
#endif
/*- Legacy for TDS UL/DL dev, to be deleted later ******************************/
#if 0

/*+ Legacy for TDS UL/DL dev, to be deleted later ******************************/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                   ENUM: PHYCHAN_DB_CFG_TYPE_ENUM_TYPE

This enumerates the two pages of the physical channel database, the ordered
config page, and the current config page.  Parameters are stored in the
ordered config page until the action time of the associated message and
then are transferred to the current config page as the configuration is
changed.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef enum {
  TDSL1DLPHYCFG_ORDERED_CFG,
  TDSL1DLPHYCFG_CURRENT_CFG,
  TDSL1DLPHYCFG_NUM_PHYCHAN_DB_CFG_TYPES
} tdsl1dlphycfg_phychan_db_cfg_page_enum_type;
#endif
/* --------------------------------------------- */
/* HSDPA DL physical channel related information */
/* --------------------------------------------- */


/* Number of HS-SCCH/HS-SICh pairs */
#define TDSL1DLPHYCFG_HS_SCCH_SICH_NUM_MAX 4

/* This structure defines the DL HS DPSCH SCCH information */
typedef struct
{
  /* indicate if secondary scrambling code exists for HS SCCH channel */
  boolean sec_scr_code_valid;
  /* secondary scrambling code value.
     Valid only if sec_scr_code_valid is set to TRUE.
     If not valid then primary CPICH scrambling code is used */
  uint8 sec_scr_code;

  /* number of channelisation codes to SCCH */
  uint8 num_ch_code;
  /* SCCH channelisation codes */
  uint8 ch_code[TDSL1DLPHYCFG_HS_SCCH_SICH_NUM_MAX];
} tdsl1dlphycfg_hs_scch_info_struct_type;

/*- Legacy for TDS UL/DL dev, to be deleted later ******************************/
#endif
#endif /* L1_DL_PHYCHAN_CFG_H */

