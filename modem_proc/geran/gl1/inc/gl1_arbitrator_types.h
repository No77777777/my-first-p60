#ifndef GARB_TYPES_H
#define GARB_TYPES_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                   G L 1  A R B I T R A T O R  T Y P E S

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/gl1_arbitrator_types.h#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ---------------------------------------------------------
06/11/15   ab        CR927639 : CXM In X2G QDTA use desense_band_id as freq id to avoid X blanking.
06/11/15   ab        CR935174 : Perform G IDLE Freq after all RX done to avoid X desense the G PCH RX.
06/11/15   ab        CR927639 : CXM In X2G QDTA use desense_band_id as freq id to avoid X blanking.
19/08/15   mc        CR874662 : CXM Freq ID support for X2G startup and X2G preload
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
17/06/15   mc        CR855818 : COEX:MCS_CXM_WWAN_TECH_STATE_IND registrations for X2G IRAT need to indicate CXM_LNK_TYPE_IRAT
26/05/15   ab        CR833968 : GL1 COEX - RRBP_HI gets incorrectly de-priortised over Paging CCCH .
14/05/15   mc        CR.821519 : TH 2.0: WLAN COEX timeline reporting errors for FACCH and SDCCH
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
17/03/15   ab        GL1 COEX : CR804948 Handling of multiple RRBP during conflict resolution
17/03/15   shr       CR808932: COEX support for DTM
13/03/15   mc        CR773788 : COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
27/02/15   am        CR646584 WLAN coex and CR    - PART 1
11/02/15   cws       CR789401 Revert G high priority to tier_10 except for data + MMS
18/12/14   cgc       CR766576 FR23109 Adaptive Power monitor during CS in DSDA G + TDD ( TDS/LTE TDD)
03/12/14   npt       CR.759467 : Fix compiler warnings
01/12/14   mc        CR.725853 : DSDA G[DDS]+G[MMS] : MCS using floored values for T10, T15, T20 hence T5 adaptation is required
10/11/14   pjr       CR.752497 : Increase value of USF Utilization counter from 8 to 16
26/09/14   mc        CR.719955 : COEX : X+G[IDLE] DSDA - IDLE monitor not protected COEX with TIER_10 assignment
08/05/14   ws        CR.661576 GL1 Support for sending MCS_CXM_BAND_AVOID_PWR_IND
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
02/07/14   mc        CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
13/03/14   mc        CR.602368 : COEX : Support for Frequency Hopping over large ARFCN lists
14/02/14   mc        CR.572523 : COEX : Ensure all DL bands are registered with CXM on transition to FIND_BCCH
12/02/14   am        CR611265 RACH timing update to GARB
27/11/13   pjr       CR.579214 : COEX : Prioritise PDTCH over TCH during 2 phase access
21/11/13   mc        CR.572523 : COEX : Ensure all DL bands are registered with CXM on transition to FIND_BCCH
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
09/11/13   mc        CR.572746 : COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
04/11/13   mc        CR.556742 : COEX : G+G : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
11/10/13   pjr       CR.555300 : Changes to correctly register freqid during monitors  and acq
01/10/13   sk        CR552259 CR sync from 1.0/1.1
23/09/13   sk        2.0 syncup from 1.1
15/08/13   sk        CR524039 GBTA bringup changes
29/07/13   sk        CR519663 Partial QBTA bringup changes
05/04/13   mc        CR.470155 : COEX Support - GERAN arbitrator framework - PS integration
25/03/13   mc        CR.464496 : COEX Support - GERAN arbitrator SW CXM integration
13/03/13   mc        CR.459487 : COEX Support - GERAN arbitrator framework
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#ifndef CUSTOMER_H
  #include "customer.h"
#endif
#ifndef COMDEF_H
  #include "comdef.h"
#endif
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_if_g.h"
#include "gmacsigl1.h"
#include "gmacsigl1_g.h"
#include "gl1_hw_arbitration.h"
#include "wwan_coex_mgr.h"
#include "l1_sc.h" /* cell_T */

#ifdef FEATURE_WLAN_COEX_SW_CXM
#include "wcn_coex_mgr.h"
#endif

/*===========================================================================

                         DATA DECLARATIONS

===========================================================================*/

/* CXM */
#define MAX_CXM_SCELL_ARFCNS 8 /* 8 x ULDL ARFCNs = 16 Freq IDs */
#define MAX_CXM_NCELL_ARFCNS 8 /* 8 x DL ARFCNs = 8 Freq IDs */
#define MAX_CXM_FREQIDS 24 /* 8 for Serving Cell ARFCNs + 8 for monitors or NCELL activities (or 4 x DL bands for FIND_BCCH and IRAT) */

#define FNMOD8_FOR_CXM_REPORT 1
#define FNMOD416_FOR_CXM_REPORT 1
/* QS */
#define QS_SLOT_0 0
#define QS_SLOT_1 (QS_PER_SLOT*1) - 1
#define QS_SLOT_2 (QS_PER_SLOT*2) - 1
#define QS_SLOT_3 (QS_PER_SLOT*3) - 1
#define QS_SLOT_4 (QS_PER_SLOT*4) - 1
#define QS_SLOT_5 (QS_PER_SLOT*5) - 1
#define QS_SLOT_6 (QS_PER_SLOT*6) - 1
#define QS_SLOT_7 (QS_PER_SLOT*7) - 1

/* Timeline */
#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING

#define GARB_TIMELINE_MAX_ENTRIES            40
#define GARB_TIMELINE_MAX_FRAMES             4
#define GARB_TIMELINE_THIRD_FRAME            2
#define GARB_TIMELINE_FOURTH_FRAME           3
#else /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */
#define GARB_TIMELINE_MAX_ENTRIES 10
#define GARB_TIMELINE_MAX_FRAMES  2
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

#define GARB_TIMELINE_THIS_FRAME 0
#define GARB_TIMELINE_NEXT_FRAME 1
#define GARB_TIMELINE_MAX_FRAMES_CONFLICTING 2
#define GARB_TIMELINE_RW_BUFFER  2


#define MAX_XPGDSDA_UL_TIER10_PER_FRAME 2
#define MAX_CPGDSDA_UL_TIER10_PER_FRAME 2
#define MAX_WPGDSDA_UL_TIER10_PER_FRAME 2
#define MAX_LPGDSDA_UL_TIER10_PER_FRAME 2
#define MAX_TPGDSDA_UL_TIER10_PER_FRAME 2

#define MAX_XPGDSDA_DL_TIER10_PER_FRAME 2
#define MAX_CPGDSDA_DL_TIER10_PER_FRAME 2
#define MAX_WPGDSDA_DL_TIER10_PER_FRAME 2
#define MAX_LPGDSDA_DL_TIER10_PER_FRAME 2
#define MAX_TPGDSDA_DL_TIER10_PER_FRAME 2

#define GARB_CXM_IND_UPDATE_PERIOD 13 /* 13 frames, ~60ms */
#define GARB_CXM_IND_UPDATE_NOW 0xFFFFFFFF

#define GARB_X2G_UL_SLOT_RESTRICTION 2
#define GARB_X2G_DL_SLOT_RESTRICTION 1

/* ARFCN */
#define GARB_UNKNOWN_ARFCN 0xFFFF

/* Monitor Reduction */
#define COEX_MONSCAN_REDUCTION_FACTOR 2
#define COEX_MONSCAN_LENGTH_TCH_MF26  24
#define COEX_MONSCAN_LENGTH_PS_MF13  8 /* 11 scheduled per 13 frames, 3 of which are serving cell meas */

/* USF Utilisation */
#define GARB_MAX_NUM_CONT_BLANKED_UL_PDTCH_THRESHOLD  16

/* G + G Conflict Resolution */
#define ACTIVITY_TIER_GPG_HI_PRIO ACTIVITY_TIER_10
#define ACTIVITY_TIER_GPG_LO_PRIO ACTIVITY_TIER_20

/*For G+G Data + MMS the high pri needs to be T5 because all other TIER
would be floored on the Data Sub */
#define ACTIVITY_TIER_GPG_HI_PRIO_DUAL_DATA ACTIVITY_TIER_5
#define ACTIVITY_TIER_GPG_LO_PRIO_DUAL_DATA ACTIVITY_TIER_20


/*===========================================================================

                        PUBLIC DECLARATIONS

===========================================================================*/

typedef enum
{
   /* Unknown */
   GARB_CHAN_UNKNOWN      = 0,

   /* Both */
   GARB_CHAN_ULDL_TCH     = 1,
   GARB_CHAN_ULDL_SACCH   = 2,
   GARB_CHAN_ULDL_SDCCH   = 3,
   GARB_CHAN_ULDL_FACCH   = 4,
   GARB_CHAN_ULDL_PTCCH   = 5,
   GARB_CHAN_ULDL_PDTCH   = 6,
   GARB_CHAN_ULDL_RATSCCH = 7,

   /* Downlink */
   GARB_CHAN_DL_CCCH      = 8,

   /* Uplink */
   GARB_CHAN_UL_RACH      = 9,
   GARB_CHAN_UL_PACCH     = 10,
   GARB_CHAN_UL_RRBP_HI   = 11,
   GARB_CHAN_UL_RRBP_LO   = 12,

   GARB_CHAN_NCELL_HI     = 13,
   GARB_CHAN_NCELL_LO     = 14,

   GARB_CHAN_MAX          = 15

} garb_channel_t;

typedef enum
{
   GARB_RX,
   GARB_TX,
   GARB_RXTX,
   GARB_RXTX_NONE
} garb_rxtx_t;

typedef enum
{
   TYPE_RX,
   TYPE_RX_MON,
   TYPE_RX_INVALID
} garb_rx_mon_type_t;

typedef enum
{
   GARB_NULL       = 0,
   GARB_FIND_BCCH  = 1,
   GARB_IDLE       = 2,
   GARB_ACCESS     = 3,
   GARB_2P_ACCESS  = 4,
   GARB_CS         = 5,
   GARB_CS_SDCCH   = 6,
   GARB_PS         = 7,
   GARB_CS_PS      = 8,  //Used for DTM
   GARB_NUM_STATES = 9
} garb_state_t;

typedef enum
{
   GARB_MODE_GPG_DSDS,
   GARB_MODE_GPG_DSDA,
   GARB_MODE_XPG_DSDS,
   GARB_MODE_XPG_DSDA,
   GARB_MODE_CPG_DSDA,
   GARB_MODE_WPG_DSDA,
   GARB_MODE_TPG_DSDA,
   GARB_MODE_LPG_DSDA,
#ifdef FEATURE_TRIPLE_SIM
   GARB_MODE_TSTS,
#endif
   GARB_MODE_INVALID
} garb_multi_sim_mode_t;

typedef enum 
{
  NO_FREQ_ID_UPDATE,
  FREQ_ID_UPDATE
}idle_freq_id_update_t;

#define GARB_FIND_BCCH_FREQID_REGISTRATION_THRESH  12

/* Look-ahead for ARFCNs */
typedef void (*garb_get_arfcns_t)(int16, uint16, uint16, ARFCN_T*, gas_id_t);

/* Tx Request (FACCH, RATCCH delay &c ) */
typedef boolean (*garb_request_tx_t)(gl1_msg_chan_type, gas_id_t);

/* COEX protected vs non-COEX protected measurement based on FN */
typedef boolean (*garb_is_coex_protected_mon_t)(uint8 FNModMF, gas_id_t gas_id);

/* Channel Type Validation */
#define IS_VALID_CHANNEL(channel) ( (channel < GARB_CHAN_MAX) && (channel != GARB_CHAN_UNKNOWN) )

typedef struct
{
   ARFCN_T                arfcn;   // specific_bcch/reselect_req arfcn
   /* used in decode bcch_list_process */
   uint16                 freq_reg_threshold_counter;  // threashold to re-register arfcn toget freqid
   uint16                 length;  // length of bcch decode list from RR  
   frequency_list_T       frequency_list;
   boolean                bplmn_bcch_acq_list;  // if BPLMN
   dBx16_T                rx_power_dbm10[MAX_CXM_FREQIDS];  // for reporting pwr for reg freq
   
   boolean               *bcch_decoded_list;// -> l1_BCCH_List_ptr->bcch_decoded[0]
   ARFCN_T               *arfcn_list;  // -> l1_BCCH_List_ptr->arfcn[0]
   dBx16_T               *rx_power_instant_list; // l1_BCCH_List_ptr->rx_power_instant[0]   
} garb_params_find_bcch_t;

typedef struct
{
   boolean                ccch_sdcch_comb;
   frequency_list_T       frequency_list;
   idle_freq_id_update_t  idle_freq_id_update;
   sys_band_T             interband;
   paging_mode_T          paging_mode;
   uint32                 paging_period;
   uint32                 paging_position;
   l1_fm_next_activity_T  next_activity;
   dBx16_T                rx_power_dbm10[7]; // 1+SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL
} garb_params_idle_t;

typedef struct
{
   ARFCN_T                arfcn;
   int16                  txlev;
   uint32                 rach_fn;
   int16                  rach_delay;
} garb_params_access_t;

// This tracks the channel types over the look-ahead window. If frames remaining are non-zero then the
// specified UL and DL channels will override the default (for e.g. UL=FACCH DL=TCH overrides TCH if
// a FACCH Tx is in progress). The frames remaining count tracks how many frames of the active block (e.g. FACCH)
// have yet to traverse through each frame of the look-ahead window.
typedef struct
{
   garb_channel_t         ul_channel;
   garb_channel_t         dl_channel;
   uint8                  frames_remaining;
} garb_channels_t;

typedef struct
{
   uint8                  TN;
   frequency_list_T       frequency_list;
   boolean                is_fr;
   uint8                  subchannel;
   uint8                  sacch_frame;
   uint8                  idle_frame;
   uint8                  num_ncell_arfcn;
   garb_channels_t        channels[GARB_TIMELINE_MAX_FRAMES]; // The channel types in use across the look-ahead window
   ARFCN_T                ncell_arfcn[26];
   ARFCN_T                scell_arfcn[26];
} garb_params_cs_t;

typedef struct
{
   frequency_list_T       frequency_list;
   boolean                is_sdcch_8;
   uint8                  subchannel;
   garb_channels_t        channels[GARB_TIMELINE_MAX_FRAMES]; // The channel types in use across the look-ahead window
   uint8                  num_ncell_arfcn;
   ARFCN_T                ncell_arfcn[102];
} garb_params_sdcch_t;

typedef struct
{
  rrbp_msg_t             rrbp_msg[GL1_DEFS_SLOTS_IN_FRAME];
  pcr_format_t           rrbp_burst[GL1_DEFS_SLOTS_IN_FRAME];
  uint8                  rrbp_ts[GL1_DEFS_SLOTS_IN_FRAME];
  uint32                 rrbp_fn;
}garb_params_ps_rrbp_t;

typedef struct
{
   frequency_list_T       frequency_list;
   uint8                  num_ul_slots;
   uint8                  num_dl_slots;
   garb_params_ps_rrbp_t  rrbp_params;
   uint8                  num_ncell_arfcn;
   ARFCN_T                ncell_arfcn[13];
   ARFCN_T                scell_arfcn[13];
   boolean                ps_ul_alloc[GL1_DEFS_SLOTS_IN_FRAME];  //contains slammed ul slot allocation for DTM and transfer mode
   boolean                ps_dl_alloc[GL1_DEFS_SLOTS_IN_FRAME];  //contains slammed dl slot allocation for DTM and transfer mode
} garb_params_ps_t;

typedef struct
{
   frequency_list_T       frequency_list;
}  garb_params_twophase_t;


typedef struct
{
   cell_T**               ba_list_pptr;
   uint8                  ba_list_size;
} garb_params_monscan_t;

typedef struct
{
   boolean                configured;
   ARFCN_T                arfcn;
   cxm_tech_link_type     link_type;
   garb_channel_t         channel_type;
   uint32                 s_time;
   uint32                 e_time;
   cxm_activity_type      cxm_tier;
   uint32                 cxm_freqid;
   uint32                 cxm_priority;
   int16                  backoff_desense;
   int16                  backoff_vbatt;
   uint8                  burst_num;
   geran_pdu_priority_t   priority;
} garb_slot_t;

typedef struct
{
   boolean                configured;
   uint32                 s_time;
   uint32                 e_time;
} garb_monitor_nozone_t;

typedef struct
{
   boolean                configured;
   uint16                 arfcn;
   uint16                 alpha;
   uint16                 search_width;
   garb_slot_t            slots[GL1_DEFS_SLOTS_IN_FRAME];
   uint8                  num_slots;
   uint8                  num_protected_slots;
   uint32                 cxm_freqid;
} garb_rx_frame_t;

typedef struct
{
   boolean                configured;
   uint16                 arfcn;
   uint16                 alpha;
   uint16                 timing_advance;
   garb_slot_t            slots[GL1_DEFS_SLOTS_IN_FRAME];
   int16                  power;
   uint8                  num_slots;
   uint8                  num_protected_slots;
   uint32                 cxm_freqid;
} garb_tx_frame_t;

typedef struct
{
   boolean                configured;
   uint8                  num_mons;
   garb_slot_t            mon[10];
} garb_mons_t;

typedef struct
{
   uint32                 fn;
   uint32                 FnMod26;
   uint32                 UstmrRefStart;
   uint32                 UstmrRefEnd;
   garb_rx_frame_t        rx;
   garb_tx_frame_t        tx;
   garb_mons_t            mons;
   garb_monitor_nozone_t  monitor_nozone[MAX_MONITOR_NOZONES];
} garb_frame_t;

typedef struct
{
   uint32                 fn;
   ARFCN_T                arfcn;
   cxm_tech_link_type     link_type;
   garb_rxtx_t            rxtx;
   garb_channel_t         channel_type;
   uint8                  burst_num;
   uint8                  frame_ref_count; // 0 for FN N , 1 for FN N+1
   uint16                 power;
   uint32                 s_time;
   uint32                 e_time;
   cxm_activity_type      cxm_tier;
} garb_activity_t;

typedef struct
{
   uint32                 arfcn;
   uint32                 cxm_freqid;
} garb_arfcn_table_entry_t;

typedef struct
{
   garb_arfcn_table_entry_t arfcns[MAX_CXM_FREQIDS];
   uint8                    num_arfcns;
} garb_arfcn_table_t;

typedef struct
{
   cxm_tech_link_direction  indication;
   uint32                   last_ind_fn;
} garb_cxm_time_ind_t;

typedef struct
{
   frequency_list_T         frequency_list;
   garb_get_arfcns_t        get_arfcns;
} garb_cxm_freq_ind_t;

typedef struct
{
   uint32                   configured;
   uint32                   s_time;
   uint32                   e_time;
} garb_timeline_rxtx_t;

#ifdef FEATURE_WLAN_COEX_SW_CXM
typedef struct
{
  uint32                    alpha;
  cxm_action_e              action;
  cxm_read_req_s            read_params;
} garb_metrics_t;
#endif

typedef struct
{
   gas_id_t                 gas_id;

   garb_state_t             state;
   garb_multi_sim_mode_t    multi_sim_mode;

   garb_params_find_bcch_t  params_find_bcch;
   garb_params_idle_t       params_idle;
   garb_params_access_t     params_access;
   garb_params_cs_t         params_cs;
   garb_params_sdcch_t      params_sdcch;
   garb_params_ps_t         params_ps;
   garb_params_monscan_t    params_monscan;
   garb_params_twophase_t   params_two_phase; 


  /* DL band tables map a DL band to a CXM frequency ID
   *
   * In the FIND_BCCH and X2G IRAT states frequencies are registered at ARFCN
   * level granularity.
   */
   uint32                   dl_band_table[INVALID_BAND];

  /* UL and DL ARFCN tables map a UL or DL ARFCN to a CXM frequency ID
   *
   * Serving Cell frequencies are registered at the ARFCN level. In addition to
   * any identified Serving Cell frequencies, 5 x GSM DL bands are registered with
   * CXM. These band level Freq IDs used during cell acquisition and NCELL
   * activities.
   *
   * The CXM supports a maximum of 24 registered ARFCNs at any one time. Note
   * that seperate registrations are required for both UL and DL. The allocation
   * of 24 x UL or DL ARFCS is segmented as follows:
   *
   * 10 x UL ARFCN (SCELL)
   * 10 x DL ARFCN (SCELL)
   *  4 x DL Bands (NCELL)
   *
   * If there are more than 10 ARFCNs in the frequency hopping or BA list then we
   * are required to periodically update CXM with a new list of registered ARFCNs.
   *
   *
   */
   garb_arfcn_table_t       ul_arfcn_table;
   garb_arfcn_table_t       dl_arfcn_table;

#ifdef FEATURE_GSM_COEX_SW_CXM
   garb_cxm_time_ind_t      cxm_time_ind;
   garb_cxm_freq_ind_t      cxm_freq_ind;

   cxm_activity_table_s     cxm_activity_table[MAX_ACTIVITY_TIERS];
#endif /* FEATURE_GSM_COEX_SW_CXM */

#ifdef FEATURE_WLAN_COEX_SW_CXM
   boolean                  cxm_policy_updated;
   cxm_active_config_v01_s  cxm_policy;
   garb_metrics_t           cxm_metrics;
#endif /* FEATURE_WLAN_COEX_SW_CXM */

   garb_frame_t             frames[GARB_TIMELINE_MAX_FRAMES];

#ifdef FEATURE_GARB_QTF /* GARB TODO : Implement USTMR simulation for QTF */
   uint32                   qtf_ustmr_offset;
#endif /* FEATURE_GARB_QTF */

   /* Indicates that this GAS is X2G IRAT */
   boolean                  irat;

   /* !!! CRITICAL SECTION !!! - THE BELOW DATA IS ACCESSED BY MULTIPLE CONTEXTS */
   uint16                   cxm_peer_tech; /* Peer TECH in DUAL ACTIVE configurations */

#ifdef FEATURE_GSM_COEX
   sys_band_T               desense_band;
   boolean                  gas_is_coex_agressor; /* UL may be YIELD'ed due to DL activity on peer TECH */
   boolean                  gas_is_coex_victim;   /* DL may be YIELD'ed due to UL activity on peer TECH */
   boolean                  peer_x_tech_is_tdd;   /* PEER X TECH is TDS/LTE TDD or FDD */

   garb_activity_t          timeline[GARB_TIMELINE_RW_BUFFER][GARB_TIMELINE_MAX_ENTRIES];
   uint8                    timeline_num_entries[GARB_TIMELINE_RW_BUFFER];

   garb_timeline_rxtx_t     timeline_rx[GARB_TIMELINE_RW_BUFFER][GARB_TIMELINE_MAX_FRAMES];
   garb_timeline_rxtx_t     timeline_tx[GARB_TIMELINE_RW_BUFFER][GARB_TIMELINE_MAX_FRAMES];

   /* used for buffering timeline entries */
   uint8                    read_t_index;
   uint8                    write_t_index;
   
   /* !!! CRITICAL SECTION !!! THE ABOVE DATA IS ACCESSED BY MULTIPLE CONTEXTS */
#endif /*  FEATURE_GSM_COEX */

   gl1_hw_arbitration_t*    arbitrated_frame;

} garb_arbitrated_gas_t;

#endif /*GARB_TYPES_H*/
