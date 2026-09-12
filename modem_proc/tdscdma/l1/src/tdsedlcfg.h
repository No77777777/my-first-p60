#ifndef TDSEDLCFG_H
#define TDSEDLCFG_H

/*============================================================================
              E N H A N C E D   U P L I N K   H E A D E R   F I L E

DESCRIPTION
This files contains all necessary macros and definitions for maintainaing
downlink physical channel configuration  .

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2006 - 2007 Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsedlcfg.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
01/05/11    weijunz initial edit.                     
===========================================================================*/

/* <EJECT> */

/*===========================================================================

                     DEFINE/UNDEFINE LOCAL FEATURES  

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_TDSCDMA_HSUPA

#include "tdsl1rrcif.h"
#include "tdseul.h"
#include "tdsedllog.h"
#include "tdseulsg.h"
#include "tdsdlphch.h"
#include "mcaltds_edl.h"
//#include "tdsl1m.h"
/*===========================================================================

                     DATA TYPE DEFINITIONS

===========================================================================*/

/*
#define TDSEDLCFG_CONFIG_DB_INVALID_PAGE 0xFF
*/

/* EUL combiner channel is reffered to as PhCh 0 for reference
   finger and time tracking purposes */





//DEBUGGING
#define TDSEDLCFG_DEBUG_EUL_DL_LOGGING_HICH     0x1
#define TDSEDLCFG_DEBUG_EUL_DL_LOGGING_HICH_ALL 0x2
#define TDSEDLCFG_DEBUG_EUL_DL_LOGGING_AGCH     0x4
#define TDSEDLCFG_DEBUG_EUL_DL_LOGGING_AGCH_ALL 0x8
#define TDSEDLCFG_DEBUG_EUL_DL_LOGGING_RGCH     0x10
#define TDSEDLCFG_DEBUG_EUL_DL_LOGGING_RGCH_ALL 0x20
#define TDSEDLCFG_DEBUG_EUL_DL_LOGGING_SG       0x40
#define TDSEDLCFG_DEBUG_EUL_DL_LOGGING_BETA_ED  0x80
#define TDSEDLCFG_DEBUG_EUL_DL_LOGGING_ETFC_PR  0x100
#define TDSEDLCFG_DEBUG_EUL_DL_LOGGING_REL6_MPR 0x200
#define TDSEDLCFG_DEBUG_EUL_DL_LOGGING_CM       0x400
#define DEBUG_EUL_MAC_ETFCI_LOGGING   0x1000000
#define DEBUG_EUL_MAC_SI_LOGGING      0x2000000
#define DEBUG_EUL_MAC_HB_LOGGING      0x4000000
#define DEBUG_EUL_MAC_LOW_LOGGING     0x8000000


extern uint32 TDSDEBUG_EUL_DL_LOGGING;

#define  TDSEDLCFG_INVALID_FINGER            0xFF
#define  TDSEDLCFG_INVALID_PSC_VAL           0xFFFF
#define  INVALID_INDEX             0xFF
#define  TDSEDLCFG_INVALID_PAGE_INDEX        0xFF
#define  TDSEDLCFG_INVALID_CELL_INDEX        0xFF
#define  TDSEDLCFG_INVALID_COMB_CHAN_INDEX   0xFF
#define  TDSEDLCFG_INVALID_FRAME_NUMBER      0x8000


#define TDSEDLCFG_INVALID_RDI_PRESENCE      -1
#define TDSEDLCFG_INVALID_TPC_STEP         0xFF
#define TDSEDLCFG_INVALID_BLER_TARGET      0x1
#define TDSEDLCFG_INVALID_AGCH_NUM         0 
#define TDSEDLCFG_INVALID_ERNTI            0
#define TDSEDLCFG_INVALID_NEHICH           0
#define TDSEDLCFG_INVALID_HICH_NUM         0
#define TDSEDLCFG_INVALID_PARAM_ZERO       0
#define TDSEDLCFG_INVALID_PARAM_NEGATIVE_ONE       -1

/*Only for logging purpose..later remove*/
#define TDSEDLCFG_NUM_FRAMES_E_DL_LOGGING 100


#define TDSEDLCFG_INVALID_SUB_FN_VAL 0

/*Initialize the threshold value to 5000 .. TBD*/

#define TDSEDLCFG_HICH_RGCH_THRESHOLD_MAX_VALUE 5000


/*Maximum number of E-AGCH channels*/
#define TDSEDL_MAX_NUM_EAGCH 4 

/*Maximum number of scheduled E-HICH channels*/
#define TDSEDL_MAX_NUM_SCH_EHICH 4

/*Add for E-DL state definition */
/*HSUPA E-DL module state definition*/
typedef enum 
{
  TDSEDL_STATE_INACTIVE,
  TDSEDL_STATE_ACTIVE,
  TDSEDL_STATE_CONFIG_START,
  TDSEDL_STATE_CONFIG_RECFG,
  TDSEDL_STATE_CONFIG_STOP
} tdsedlcfg_edl_state_enum_type ;

/*HSUPA Non scheduled grant state definition*/
typedef enum
{
  TDSEDL_NON_SCHED_GRANT_INACTIVE,
  TDSEDL_NON_SCHED_GRANT_ACTIVE,
  TDSEDL_NON_SCHED_GRANT_RECFG
} tdsedlcfg_non_sched_grant_state_enum_type;

/* Num pages per channel in EUL DL CFG */
typedef enum {
  TDSEDLCFG_CONFIG_DB_PAGE_ONE,
  TDSEDLCFG_CONFIG_DB_PAGE_TWO,
  TDSEDLCFG_CONFIG_DB_MAX_PAGES,
  TDSEDLCFG_CONFIG_DB_INVALID_PAGE = 0xFF
} tdsedlcfg_config_db_page_enum_type;


#define TDSEDLCFG_CHAN_DISABLE    MCALTDS_EDL_CHAN_DISABLE
#define TDSEDLCFG_CHAN_ENABLE     MCALTDS_EDL_CHAN_ENABLE
#define TDSEDLCFG_CHAN_RECONFIG   MCALTDS_EDL_CHAN_RECONFIG
#define TDSEDLCFG_CHAN_NO_ACTION  MCALTDS_EDL_CHAN_NO_ACTION
#define TDSEDLCFG_MAX_E_DL_CELL_DB_INDEX 7

typedef enum
{
  TDSEDLCFG_AGCH_DROPPED,
  TDSEDLCFG_AGCH_TO_BE_ADDED,
  TDSEDLCFG_AGCH_NOOP
}tdsedlcfg_serial_add_drop_enum_type;


/* The Serving Cell Change (SCC) type when EDL is active
** This will be used to fill information in the EUL_SERVING_CELL_CHANGE event*/
typedef enum
{
  TDSEDLCFG_SCC_TYPE_TIMING_INIT_HHO = 0,
  TDSEDLCFG_SCC_TYPE_TIMING_MAINT_HHO = 1,
  TDSEDLCFG_SCC_TYPE_NOT_IN_HHO = 2
}tdsedlcfg_scc_during_hho_enum_type;

/* This ENUM will be used to fill in information about the action related to
** a serving cell change. The HHO has a global that needs to set in l1msetup.c
** and hence not a part of this ENUM*/
typedef enum
{
  TDSEDLCFG_SERV_CELL_NO_CHANGE = 0,
  TDSEDLCFG_SERV_CELL_CHANGE_RECFG_SOFT = 1,
  TDSEDLCFG_SERV_CELL_CHANGE_RECFG_SOFTER = 2
}tdsedlcfg_serving_cell_change_type_enum_type;

#define TDSEDLCFG_GET_FIRST_BYTE(val) (val & 0xFF) /* Gets first byte of 4 byte val*/
#define TDSEDLCFG_GET_SECOND_BYTE(val) ((val & 0xFF00) >> 8) /* Gets 2nd byte of val*/

/*E-AGCH info page structure*/
typedef struct 
{
  boolean                    new_cfg;
  mcaltds_edl_chan_action_enum_type action;
  /*RDI present indicator*/
  boolean rdi_present ;
  /*TPC step size*/
  tdsl1_edl_tpc_step_size_enum_type tpc_step ;
  /*ERNTI*/
  uint16 ernti ;
  /*E-AGCH Bler target*/
  int8 bler_target ;
  /*start sub frame number. This isn't needed as WCDMA already has this variable*/
  uint16 start_sub_frame_num ;
  /*number of E-AGCH*/
  uint8 num_eagch ;
  tdsl1_eagch_set_info_struct_type eagch_set_info[TDSEDL_MAX_NUM_EAGCH] ;
} tdsedlcfg_agch_info_page_type;


/*AGCH info maintained in DL config db*/
typedef struct 
{
  uint8 cur_page;
  uint8 ord_page;
  tdsedlcfg_agch_info_page_type agch_info_page[TDSEDLCFG_CONFIG_DB_MAX_PAGES];
} tdsedlcfg_agch_info_struct_type;


typedef struct 
{
  boolean new_cfg;
  mcaltds_edl_chan_action_enum_type action;
  uint16                     start_sub_fn;
  /*Number of minimum time slot between  E-PUCH and E-HICH. Should be 4~15*/
  uint8 n_ehich ;
  /*Number of E-HICH channel to monitor. not more than 4*/
  uint8 num_ehich ;
  tdsl1_sch_ehich_set_info_struct_type sch_ehich_set_info[TDSEDL_MAX_NUM_SCH_EHICH] ;
} tdsedlcfg_sched_hich_info_page_type;

/*Scheduled E-HICH info maintained in DL config db*/
typedef struct 
{
  uint8 cur_page;
  uint8 ord_page;
  tdsedlcfg_sched_hich_info_page_type hich_info_page[TDSEDLCFG_CONFIG_DB_MAX_PAGES];
} tdsedlcfg_sched_hich_info_struct_type;


/*Add for TD-SCDMA HSUPA*/
/*Non Scheduled E-HICH set configuration type*/
typedef struct
{
    /*time slot number (0..6)*/
    uint8 ts_num ;
    /*channelisation code*/
    tdsl1_edl_ch_code_enum_type chan_code ;
    tdsl1_midamble_ss_burst_struct_type ehich_midamble ;
} tdsedl_non_sched_ehich_set_info_struct_type ;

typedef struct 
{
  /*Signature group index for non shceduled E-HICH channel*/
  uint8   sig_grp_idx;
  tdsedl_non_sched_ehich_set_info_struct_type non_sch_ehich_set_info ;
} tdsedlcfg_non_sched_hich_info_page_type;

typedef struct
{
    
    boolean new_cfg;
    mcaltds_edl_chan_action_enum_type action;
    uint16                    start_sub_fn;
    /*Number of E-UCCH instances in one TTI. 1..8*/
    uint8 num_eucch ;
    /*minimum number of time slot between E-PUCH and E-HICH. 4..15*/
    uint8 n_ehich ;
    /*timeslot resource related information, bit[4:0] for TS1~TS5, TS1 is MSB*/
    uint8 trri ;
    /*Power resource related information . Index from 1 to 32*/
    uint8 prri ;
    /*Code resource related information This is index range from 0 to 30*/
    uint8 crri ;
    /*Activation time of non scheduled grant .*/
    /*25.331 10.3.3.1 shows CFN. */
    uint8 activation_time ;
    /*first or second sub frame . 0 or 1 */
    uint8 sub_fn ;
    /*Grant repetition period. 1,2,4,8,16,32,64. 1 means grant is continuous*/
    uint8 rep_period ;
    /*Grant repetition length. This value is ignored if repetition period is 1. */
    uint8 rep_length ;

    /*Non scheduled E-HICH channel info*/
    tdsedlcfg_non_sched_hich_info_page_type non_sch_ehich_info ;

} tdsedlcfg_non_sched_grant_info_page_type ;

  
/*Non Shceduled grant  info */
typedef struct 
{
  uint8 cur_page;
  uint8 ord_page;
  tdsedlcfg_non_sched_grant_info_page_type non_sch_grant_info_page[TDSEDLCFG_CONFIG_DB_MAX_PAGES];
} tdsedlcfg_non_sched_grant_info_struct_type;

/*Non scheduled grant database structure*/
typedef struct
{
    tdsedlcfg_non_sched_grant_state_enum_type current_state ;
    tdsedlcfg_non_sched_grant_state_enum_type next_state ;
    tdsedlcfg_non_sched_grant_info_struct_type non_sch_grant_info ;

} tdsedlcfg_non_sched_grant_db_struct_type ;




/*E-DL config db */
typedef struct 
{
  tdsedlcfg_edl_state_enum_type e_dl_state;
  tdsedlcfg_edl_state_enum_type e_dl_next_state;
  tdsedlcfg_agch_info_struct_type agch_info;
  tdsedlcfg_sched_hich_info_struct_type sch_hich_info ;
  uint16 cached_ernti ;

  tdsdlphch_cfg_handle_type dpch_cfg_handle;
  
  
} tdsedlcfg_edl_config_db_struct_type;



/******************************************************************************
                     COMMAND: TDSDLCHMGR_DL_CPHY_SETUP_LOCAL_CMD_TYPE
******************************************************************************/

/******************************************************************************
                     Defining structures that can be filled to store samples of AGCH/HICH and RGCH
                     and they can be assigned to log packet and flushed when needed.
******************************************************************************/
typedef  struct{
  uint8 new_agch;
  uint8 agch_predec_sym_err;
  uint8 agch_valid;
  uint8 agch_val;
  uint8 ag_scope;
  uint8 pri_sec_ernti;
  uint16 agch_dec_energy;
}tdsedlcfg_agch_per_sample_info_struct_type;


typedef struct{
  boolean new_hich;
  int32 eng;
  uint16 thresh;
  uint32 sc_wts;
  uint8 n_slots;
}tdsedlcfg_hich_per_sample_info_struct_type;

typedef struct{
  uint8 num_hich_samples;
  tdsedlcfg_hich_per_sample_info_struct_type hich_info[TDSEDLLOG_DECODE_LOGGING_NUM_SAMPLES];
}tdsedlcfg_hich_per_rl_info_struct_type;

typedef struct{
  boolean new_rgch;
  int32 eng;
  uint16 thresh;
  uint8 n_slots;
}tdsedlcfg_rgch_per_sample_info_struct_type;

typedef struct{
  tdsedlcfg_rgch_per_sample_info_struct_type rgch_info[TDSEDLLOG_DECODE_LOGGING_NUM_SAMPLES];
  uint8 num_rg_samples;
}tdsedlcfg_rgch_per_rl_info_struct_type;

typedef struct{
  /* This is the sub fn of the first sample of this log packet*/
  uint16 curr_sub_fn;
  /* Using the curr_sub_fn value the delta of the first sample of each 
  ** of the channels on a per RL basis is stored. This is for the parser to
  ** append the sub fn where the result is printed appropriately*/
  uint8 agch_delta;
  uint8 hich_delta[TDSEUL_MAX_RL];
  uint8 rgch_delta[TDSEUL_MAX_RL];
  uint8 num_agch_samples;
  tdsedlcfg_agch_per_sample_info_struct_type agch_info[TDSEDLLOG_DECODE_LOGGING_NUM_SAMPLES];
  tdsedlcfg_hich_per_rl_info_struct_type hich_per_rl_info[TDSEUL_MAX_RL];
  tdsedlcfg_rgch_per_rl_info_struct_type rgch_per_rl_info[TDSEUL_MAX_RL];
  /* This flag is used to know if we are MRC combining or equal weight combining*/
  boolean mrc_info;
}tdsedlcfg_dec_log_pkt_per_sample_info_struct_type;



/*===========================================================================

                     DEFINE/UNDEFINE LOCAL FEATURES  

===========================================================================*/

extern tdsedlcfg_dec_log_pkt_per_sample_info_struct_type *tdsedl_dec_log_pkt_per_sample_ptr;
extern tdsedlcfg_edl_config_db_struct_type  tdsedl_config_db;
extern tdsedlcfg_non_sched_grant_db_struct_type tdsedl_non_sched_grant_config_db ;



/*Only for testing purposes for Tau DPCH change Later will be removed*/
extern tdsl1_e_dl_info_struct_type temp_e_dl_info_ptr;


/* This variable is used to store the index of the buffer in which we store the 
** AGCH results that is used for logging later*/
#define E_DL_MAX_AGCH_RES_BUFFS       2
extern uint8 tdsedl_ag_res_buff_idx[E_DL_MAX_AGCH_RES_BUFFS];


/* The Serving Cell Change (SCC) type when EDL is active
** This will be used to fill information in the EUL_SERVING_CELL_CHANGE event*/
extern tdsedlcfg_scc_during_hho_enum_type tdsedl_scc_during_hho;

/* Boolean that keeps track of a forced sync A when EUL is active*/
extern boolean tdsedl_active_during_forced_sync_a;

/* This flag is an indication to MAC that there is a serving EDCH RLS change */
extern boolean tdsedl_edch_serv_cell_and_rls_change_ind;

/* Flag to keep track of simultaneous UL and DL drop for HHO*/
extern boolean tdsedl_simultaneous_ul_dl_drop;

/* Flag to indicate that the e_dl_module should call the L1m CPHY_SETUP_CNF to RRC
** after the frame boundary command handler, before exiting the EUL DL config procedure*/
extern boolean tdsedl_call_l1m_cphy_setup_cnf_to_rrc;

/* =======================MACROS ============================*/

#define TDSEDLCFG_IS_EUL_ACTIVE() (tdsedl_config_db.e_dl_state == TDSEDL_STATE_ACTIVE)
#define TDSEDLCFG_IS_EUL_INACTIVE() (tdsedl_config_db.e_dl_state == TDSEDL_STATE_INACTIVE)

#define TDSEDLCFG_IS_NON_SCHED_GRANT_ACTIVE() (tdsedl_non_sched_grant_config_db.current_state == TDSEDL_NON_SCHED_GRANT_ACTIVE)
#define TDSEDLCFG_IS_NON_SCHED_GRANT_INACTIVE() (tdsedl_non_sched_grant_config_db.current_state == TDSEDL_NON_SCHED_GRANT_INACTIVE)



/* If the E_DL_STATE is not active and if it is not inactive also, then EUL DL should be in config state*/
#define TDSEDLCFG_IS_EUL_IN_CONFIG() (tdsedl_config_db.e_dl_state != TDSEDL_STATE_INACTIVE)&&(tdsedl_config_db.e_dl_state != TDSEDL_STATE_ACTIVE)
#define TDSEDLCFG_GET_EUL_DL_STATE() tdsedl_config_db.e_dl_state
/* Macros to set and get the flag used by EUL DL module to call the tdsl1msetup_send_cphy_setup_cnf_to_rrc() in the
** frame boundary command handler.*/
#define TDSEDLCFG_SET_EDL_TRIGGER_CPHY_SETUP_CNF_FLAG(val) tdsedl_call_l1m_cphy_setup_cnf_to_rrc = val
#define TDSEDLCFG_GET_EDL_TRIGGER_CPHY_SETUP_CNF_FLAG()    tdsedl_call_l1m_cphy_setup_cnf_to_rrc



/* Macros to get/set current/ordered channel page and to allocate 
 * ordered channel page
 */
#define  TDSEDLCFG_GET_AGCH_CUR_PAGE()  (tdsedl_config_db.agch_info.cur_page)
#define  TDSEDLCFG_GET_AGCH_ORD_PAGE()  (tdsedl_config_db.agch_info.ord_page)
#define  TDSEDLCFG_ALLOCATE_AGCH_ORD_PAGE()  \
           (tdsedl_config_db.agch_info.ord_page = (\
            (TDSEDLCFG_GET_AGCH_CUR_PAGE() == TDSEDLCFG_CONFIG_DB_PAGE_TWO) ? \
             TDSEDLCFG_CONFIG_DB_PAGE_ONE : TDSEDLCFG_CONFIG_DB_PAGE_TWO))
#define  TDSEDLCFG_SET_AGCH_CUR_PAGE(page)  (tdsedl_config_db.agch_info.cur_page = page)
#define  TDSEDLCFG_SET_AGCH_ORD_PAGE(page)  (tdsedl_config_db.agch_info.ord_page = page)

#define  TDSEDLCFG_GET_HICH_CUR_PAGE()  (tdsedl_config_db.sch_hich_info.cur_page)
#define  TDSEDLCFG_GET_HICH_ORD_PAGE()  (tdsedl_config_db.sch_hich_info.ord_page)
#define  TDSEDLCFG_ALLOCATE_HICH_ORD_PAGE()  \
           (tdsedl_config_db.sch_hich_info.ord_page = (\
            (TDSEDLCFG_GET_HICH_CUR_PAGE() == TDSEDLCFG_CONFIG_DB_PAGE_TWO) ? \
             TDSEDLCFG_CONFIG_DB_PAGE_ONE : TDSEDLCFG_CONFIG_DB_PAGE_TWO))
#define  TDSEDLCFG_SET_HICH_CUR_PAGE(page)  (tdsedl_config_db.sch_hich_info.cur_page = page)
#define  TDSEDLCFG_SET_HICH_ORD_PAGE(page)  (tdsedl_config_db.sch_hich_info.ord_page = page)

#define  TDSEDLCFG_GET_NON_SCHED_GRANT_CUR_PAGE()  (tdsedl_non_sched_grant_config_db.non_sch_grant_info.cur_page)
#define  TDSEDLCFG_GET_NON_SCHED_GRANT_ORD_PAGE()  (tdsedl_non_sched_grant_config_db.non_sch_grant_info.ord_page)
#define  TDSEDLCFG_ALLOCATE_NON_SCHED_GRANT_ORD_PAGE()  \
           (tdsedl_non_sched_grant_config_db.non_sch_grant_info.ord_page = (\
            (TDSEDLCFG_GET_NON_SCHED_GRANT_CUR_PAGE() == TDSEDLCFG_CONFIG_DB_PAGE_TWO) ? \
             TDSEDLCFG_CONFIG_DB_PAGE_ONE : TDSEDLCFG_CONFIG_DB_PAGE_TWO))
#define  TDSEDLCFG_SET_NON_SCHED_GRANT_CUR_PAGE(page)  (tdsedl_non_sched_grant_config_db.non_sch_grant_info.cur_page = page)
#define  TDSEDLCFG_SET_NON_SCHED_GRANT_ORD_PAGE(page)  (tdsedl_non_sched_grant_config_db.non_sch_grant_info.ord_page = page)

/* MACRO to check whether EDL state is in config start, config reconfig*/
#define TDSEDLCFG_IS_EDL_STATE_CONFIG_START()  (tdsedl_config_db.e_dl_state == TDSEDL_STATE_CONFIG_START)
#define TDSEDLCFG_IS_EDL_STATE_CONFIG_RECFG()  (tdsedl_config_db.e_dl_state == TDSEDL_STATE_CONFIG_RECFG)




  /*Macro to set the SERVING CELL CHANGE IND to MAC as FALSE*/
#define TDSEDLCFG_SET_SERV_RLS_CHANGE_FLAG(value) \
      tdsedl_edch_serv_cell_and_rls_change_ind = value

  /* MACRO to set the FORCED SYNC A flag when EDL is active*/
#define TDSEDLCFG_SET_FORCED_SYNC_FLAG(value) \
    tdsedl_active_during_forced_sync_a = value

  /* MACRO to set the EDL SCC information during HHO*/
#define TDSEDLCFG_SET_SCC_TYPE_DURING_HHO(scc_type) \
    tdsedl_scc_during_hho = scc_type

  /* Macro for reading the flag for serving RLS change that is indicated to MAC for
  ** SI trigger.*/
 #define TDSEDLCFG_GET_SERV_RLS_CHANGE_FLAG() tdsedl_edch_serv_cell_and_rls_change_ind

  /* Macro to read the field valid_ref_fing and this is used to decide whether the results can be obtained
  ** on EUL DL channels on this RL can be used or not*/
 #define TDSEDLCFG_GET_RL_VALID_REF_FING(e_dl_rl_idx) 0


  /*These #defines are used to fill the action and type field of the EUL serving cell
  ** change event SCC -> Serving Cell Change*/
#define TDSEDLCFG_SCC_EVENT_ACTION_HARD 0
 
 
#define TDSEDLCFG_SCC_EVENT_TYPE_TIMING_MAINT 1

#define TDSEDLCFG_IS_CONFIG_DB_PAGE_VALID(page)  (page != TDSEDLCFG_CONFIG_DB_INVALID_PAGE)

#define TDSEDLCFG_IS_CONFIG_DB_PAGE_INVALID(page)  (page == TDSEDLCFG_CONFIG_DB_INVALID_PAGE)

#define HSUPA_EUL_CBFUNC_PENDING (1)
#define HSUPA_EDL_CBFUNC_PENDING (1<<1)
#define HSUPA_NONSCHED_EDL_CBFUNC_PENDING (1<<2)

/******************************************************************************/

/*===========================================================================

FUNCTION        EUL_VALIDATE_CONFIG

DESCRIPTION     This function validates the EUL DL and UL config as received
                in the TDSL1_CPHY_SETUP_REQ command from RRC

DEPENDENCIES    Unknown 

RETURN VALUE    None

SIDE EFFECTS    EUL DL config database updated

===========================================================================*/
extern boolean tdsedl_eul_validate_config(tdsl1_setup_cmd_type* setup);

/*===========================================================================

FUNCTION        TDSE_DL_PHYCHAN_CFG_VALIDATE

DESCRIPTION     This function validates the EUL DL config as received
                in the TDSL1_CPHY_SETUP_REQ command from RRC

DEPENDENCIES    Unknown 

RETURN VALUE    None

SIDE EFFECTS    EUL DL config database updated

===========================================================================*/
extern boolean tdsedl_phychan_cfg_validate(tdsl1_setup_cmd_type *setup);

/*===========================================================================

FUNCTION       TDSEDL_NON_SCHED_GRANT_CFG_VALIDATE

DESCRIPTION     This function validates the EUL Non scheduled grant  config as received
                in the TDSL1_CPHY_SETUP_REQ command from RRC

DEPENDENCIES    Unknown

RETURN VALUE    None

SIDE EFFECTS    EUL non scheduled grant  config database updated

===========================================================================*/
boolean tdsedl_non_sched_grant_cfg_validate(tdsl1_setup_cmd_type *setup) ;







//boolean tdsedl_dch_cleanup(uint8 next_state);


/*===========================================================================

FUNCTION        TDSE_DL_INIT

DESCRIPTION     This function initializes EUL DL data structures on power up

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    EUL DL config database and other globals initialized

===========================================================================*/
extern void tdsedl_init(void);

/*===========================================================================

FUNCTION        TDSEDL_NON_SCHED_GRANT_INIT

DESCRIPTION     This function initializes EUL Non sched grant data structures on power up

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    EUL non shced grant config database and other globals initialized

===========================================================================*/
void tdsedl_non_sched_grant_init(void);

/*===========================================================================

FUNCTION        TDSEDL__NON_SCHED_GRANT_CONFIG

DESCRIPTION     This function configures the EUL Non scheduled grant  info into ordered config
                EUL data base and set up the MCAL data base and send message to FW.

DEPENDENCIES    Unknown

RETURN VALUE    None

SIDE EFFECTS    EUL DL config database updated

===========================================================================*/
void tdsedl_non_sched_grant_config(uint8 e_req_mask, tdsl1_non_sched_grant_info_struct_type *cmd_grant_info_ptr) ;

/*===========================================================================

FUNCTION        TDSEDL_CONFIG

DESCRIPTION     This function configures the EUL DL info into ordered config
                EUL data base and sets up the EUL COMB CHAN CFG EVT to proceed 
                further 

DEPENDENCIES    Unknown 

RETURN VALUE    None

SIDE EFFECTS    EUL DL config database updated

===========================================================================*/
extern void tdsedl_config(uint8 e_req_mask, tdsl1_e_dl_info_struct_type *e_dl_info_ptr);


/*===========================================================================

FUNCTION        TDSEDL_CONFIG_START

DESCRIPTION     This function handles EUL DL start request

DEPENDENCIES    Unknown 

RETURN VALUE    None

SIDE EFFECTS    EUL DL config database updated

===========================================================================*/
void tdsedl_config_start(tdsl1_e_dl_info_struct_type *e_dl_info_ptr);

/*===========================================================================

FUNCTION        TDSEDL_CONFIG_RECFG

DESCRIPTION     This function handles EUL DL recfg request

DEPENDENCIES    Unknown 

RETURN VALUE    None

SIDE EFFECTS    EUL DL config database updated

===========================================================================*/
void tdsedl_config_recfg(tdsl1_e_dl_info_struct_type *e_dl_info_ptr);

/*===========================================================================

FUNCTION        TDSEDL_CONFIG_STOP

DESCRIPTION     This function handles EUL DL stop request

DEPENDENCIES    Unknown 

RETURN VALUE    None

SIDE EFFECTS    EUL DL config database updated

===========================================================================*/
void tdsedl_config_stop(void);



/*===========================================================================

FUNCTION        TDSEDL_CONFIG_SUB_FRM_BDRY_CB

DESCRIPTION     This function is called at sub frame boundary. It switches E-DL 
                 db current page to ordered page and update E-DL state machine.
 
DEPENDENCIES    Unknown 

RETURN VALUE    None

SIDE EFFECTS    Unknown

===========================================================================*/
extern void tdsedl_config_sub_frm_bdry_cb(uint16 sub_fn,uint32 reg_id);

/*===========================================================================

FUNCTION        TDSEDL_CONFIG_SUB_FRM_BDRY_CB

DESCRIPTION     This function is called at sub frame boundary. It switches Non sched 
                grant DB current page to ordered page and updates non sched grant
                state machine.
 
DEPENDENCIES    Unknown 

RETURN VALUE    None

SIDE EFFECTS    Unknown

===========================================================================*/
extern void tdsedl_non_sched_grant_config_sub_frm_bdry_cb(uint16 sub_fn,uint32 reg_id);





/*===========================================================================
FUNCTION         E_DL_CHECK_STTD_RECONFIG

DESCRIPTION      This function is called when there is a DL DPCH reconfiguration
                 in TDSL1_CPHY_SETUP_REQ command. The function checks if STTD setup of 
                 any of the existing RLs in EDCH active set needs to be reconfigured
                 due to DPCH tx diversity reconfig in RLs of R99 active set. If STTD 
                 needs to be reconfigured for any of the existing EDCH active set RLs,
                 it issues an implicit reconfig by setting req_mask, e_req_mask, and 
                 per-RL channel masks in TDSL1_CPHY_SETUP_REQ command approriately. 

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     The Higher level req_mask and e_req_mask are updated to
                 trigger a STTD reconfiguration.

===========================================================================*/
extern void tdsedl_check_sttd_reconfig(void);

/*===========================================================================
FUNCTION         E_DL_ADD_RL

DESCRIPTION      This function adds a new RL to the EDCH RL list.
          

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     None

===========================================================================*/
extern void tdsedl_add_rl(tdsl1_e_dl_info_struct_type *e_dl_info_ptr);







#ifdef FEATURE_TDSCDMA_L1_LOGGING_ENABLED
/*===========================================================================
FUNCTION         TDSEDL_SUBMIT_DL_CHAN_INFO_LOG_PKT

DESCRIPTION      This function commits the DL CHAN INFO log packet 
                           This will be called from the function that is posted after the 
                           frame boundary command has been handled.

DEPENDENCIES     None

RETURN VALUE     Boolean

SIDE EFFECTS     None

===========================================================================*/

extern void tdsedl_submit_dl_chan_info_log_pkt (void);

/*===========================================================================
FUNCTION         TDSEDL_SUBMIT_DL_DEC_INFO_LOG_PKT

DESCRIPTION      This function fills in the log pointer allocated for this log packet from the
                           global structure that has accumulated the information on a per sample basis.

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     None

===========================================================================*/

extern void tdsedl_submit_dl_dec_info_log_pkt(void);

#endif  /* FEATURE_TDSCDMA_L1_LOGGING_ENABLED */

/*===========================================================================
FUNCTION         TDSEDL_UPDATE_RLS_RG_COMB_IDX_PER_RL

DESCRIPTION      This function updates the information about the RLS idx and the 
                           rg_comb_idx  for each of the RLs and also specifically the serv_rls_idx 
                           and serv_rg_comb_idx.

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     None

===========================================================================*/

extern void e_dl_update_rls_rg_comb_idx_per_rl(void);



/*===========================================================================
FUNCTION         TDSEDL_INIT_DEC_LOG_PKT

DESCRIPTION      This function initializes important fields of the dl_ded log packet 
                           per sample structure.

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     None

===========================================================================*/
extern void tdsedl_init_dec_log_pkt(void);





#ifdef FEATURE_L1_LOG_ON_DEMAND
/*===========================================================================
FUNCTION         TDSEDL_LOG_INIT

DESCRIPTION      This function registers with the log on demand and also
                 sets the global flag to indicate whether valid log packet info 
                 present or not. It is initialized to FALSE.

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     None

===========================================================================*/
void tdsedl_log_init(void);

/*===========================================================================
FUNCTION         TDSEDL_CHAN_INFO_LOG_ON_DEMAND

DESCRIPTION      This function checks whether the valid flag is TRUE.
                 If it is TRUE and EUL is active, then commit the log packet 


DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     None

===========================================================================*/
extern void tdsedl_chan_info_log_on_demand(void);
#endif


extern boolean tdsedl_debug_enable_sw_wrkarnd_agch_issue_scc;

extern boolean tdsedl_drop_precedes_asu_cmd_cphy_setup;

/*===========================================================================
FUNCTION         TDSEDL_PREPARE_MCAL_DATA

DESCRIPTION      This function prepares the EDL config data for the MCAL
                 so that the configuration procedure can be done within MCAL.

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     An MCAL data structure is updated.

===========================================================================*/
extern void tdsedl_prepare_mcal_data(void);



#if 0
#ifdef FEATURE_WCDMA_HSUPA_AGCH_MD_FIX

/* HACK HACK -This global is used to indicate whether a AGCH reconfiguration has
** been triggered following a N_PCCPCH decode.*/
extern boolean edl_periodic_agch_recfg_state;

extern boolean e_dl_agch_recfg_pending_cphy_setup;

extern uint16 e_dl_agch_recfg_pending_cphy_setup_act_time;

#define TDSEDLCFG_SET_PENDING_CPHY_SETUP_FLAG(val) \
  e_dl_agch_recfg_pending_cphy_setup = val

#define TDSEDLCFG_IS_PENDING_CPHY_SETUP() \
  e_dl_agch_recfg_pending_cphy_setup

#define TDSEDLCFG_SET_ACT_TIME_CPHY_SETUP(val) \
  e_dl_agch_recfg_pending_cphy_setup_act_time= val

#define TDSEDLCFG_GET_ACT_TIME_CPHY_SETUP() \
  e_dl_agch_recfg_pending_cphy_setup_act_time
  

/*===========================================================================
FUNCTION         E_DL_START_AGCH_RECFG_POST_N_PCCPCH

DESCRIPTION      This function begins the AGCH reconfiguration procedure
                 after the N_PCCPCH decode is done. This will trigger a timing
                 changed reconfiguration of the AGCH channel. This is a HACK to
                 try to solve the AGCH MD issue that is tied to the N_PCCPCH.

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     None

===========================================================================*/
extern void e_dl_start_agch_recfg_post_n_pccpch(boolean recfg_state);


/*===========================================================================
FUNCTION         E_DL_CFG_EVT_HANDLER_AGCH_CFG_POST_N_PCCPCH

DESCRIPTION      This function is the config event handler specifically written
                 for the scenario of performing a reconfiguration of the AGCH Channel
                 after a N_PCCPCH decode is done.

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     None

===========================================================================*/
extern void e_dl_cfg_evt_handler_agch_cfg_post_n_pccpch(void);


/*===========================================================================
FUNCTION         E_DL_CFG_CMD_HANDLER_AGCH_CFG_POST_N_PCCPCH

DESCRIPTION      This function is the config local command handler specifically written
                 for the scenario of performing a reconfiguration of the AGCH Channel
                 after a N_PCCPCH decode is done.

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     None

===========================================================================*/
extern void e_dl_cfg_cmd_handler_agch_cfg_post_n_pccpch(void);


/*===========================================================================
FUNCTION         E_DL_FRM_BDRY_EVT_HANDLER_AGCH_CFG_POST_N_PCCPCH

DESCRIPTION      This function is the frame boundary event handler specifically written
                 for the scenario of performing a reconfiguration of the AGCH Channel
                 after a N_PCCPCH decode is done.

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     None

===========================================================================*/
extern void e_dl_frm_bdry_evt_handler_agch_cfg_post_n_pccpch(void);


/*===========================================================================
FUNCTION         E_DL_FRM_BDRY_CMD_HANDLER_AGCH_CFG_POST_N_PCCPCH

DESCRIPTION      This function is the frame boundary command handler specifically written
                 for the scenario of performing a reconfiguration of the AGCH Channel
                 after a N_PCCPCH decode is done.

DEPENDENCIES     None

RETURN VALUE     None

SIDE EFFECTS     None

===========================================================================*/
extern void e_dl_frm_bdry_cmd_handler_agch_cfg_post_n_pccpch(void);


/*===========================================================================
FUNCTION         IS_E_DL_AGCH_RECFG_POST_N_PCCPCH

DESCRIPTION      This function indicates to the caller whether the AGCH reconfig
                 after an N_PCCPCH decode has been triggered. If so then the CPHY_SETUP
                 requests should be help up till the config is done
                 for the scenario of performing a reconfiguration of the AGCH Channel
                 after a N_PCCPCH decode is done.

DEPENDENCIES     None

RETURN VALUE     Boolean

SIDE EFFECTS     The TDSL1_CPHY_SETUP_REQ is put on hold till the AGCH reconfiguration is done.

===========================================================================*/

extern boolean is_e_dl_agch_recfg_post_n_pccpch(void);


/*===========================================================================
FUNCTION         E_DL_DLEAY_AGCH_RECFG_POST_N_PCCPCH

DESCRIPTION      This function delays the AGCH recfg process if TDSL1_CPHY_SETUP_REQ
                 is in progress

DEPENDENCIES     None

RETURN VALUE     Boolean

SIDE EFFECTS     The TDSL1_CPHY_SETUP_REQ is put on hold till the AGCH reconfiguration is done.

===========================================================================*/

void e_dl_delay_agch_recfg_post_n_pccpch(void);


/*===========================================================================
FUNCTION         E_DL_DLEAY_AGCH_RECFG_POST_N_PCCPCH_CHECK

DESCRIPTION      This function checks if the AGCH recfg has been delayed due
                 to TDSL1_CPHY_SETUP_REQ and if TDSL1_CPHY_SETUP_REQ is no longer in 
                 progress starts the AGCH recfg

DEPENDENCIES     None

RETURN VALUE     Boolean

SIDE EFFECTS     The TDSL1_CPHY_SETUP_REQ is put on hold till the AGCH reconfiguration is done.

===========================================================================*/

void e_dl_delay_agch_recfg_post_n_pccpch_check(void);

#endif /*end of FEATURE_WCDMA_HSUPA_AGCH_MD_FIX*/

#endif

boolean tdsedl_module_drop(void) ;

#endif /*end of FEATURE_TDSCDMA_HSUPA*/
#endif /*end of TDSEDLCFG_H*/

