#ifndef WMCVSDRV_H
#define WMCVSDRV_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                         WCDMA MCVS Driver interface

GENERAL DESCRIPTION

  This file contains the source code for the WCDMA Modem Power Abstraction
  layer implementation.


Copyright (c) 2000-2015 by Qualcomm Technologies, Inc. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/wmcvsdrv.h#10 $
$DateTime: 2020/02/19 23:04:30 $
$Author: pwbldsvc $

when           who      what, where, why
--------       ----     ----------------------------------------------------------
02/20/20       ss       Send END request to Conc MGR when W stack is Relinquished and check WRM LOCK state.
05/12/16       kr       Clear C1 related MCVS param whern C1 is going down
1/2/20         sch     Added a new scenario for HS offset config 1 and config 9
11/04/19      gmr       When Releasing Q6 speed Make sure set proper value so that it will go to default speed
06/17/16       kr       Need Q6A to be at 288 with FBRX in voice call for 8940
03/01/16       kr       Make sure Q6A runs at>=288 MHZ with WTR3925[FBRX enabled] 
02/12/16       kr       Release MCVS with value other than 0 for q6B[Jacala specific]
1/8/16         kr       New clk plan for Jacala
12/18/15       kr       Send Q6B Clk Cfg command in a QTA gap for IDLE mode [max of IDLE / Data SUB]
11/04/15       kr       FR 30623: Searcher MCVS optimization,Support for LOW SVS for srch cycle count
10/27/15       kr       BACKOUT--FR 30623: Searcher MCVS optimization,Support for LOW SVS for srch cycle count
10/27/15       kr       FR 30623: Searcher MCVS optimization,Support for LOW SVS for srch cycle count
09/03/15       rs       Fixing compiler warnings for Atlas
08/03/15       Kr       Handle race condition in IRP+MCVS  clock losering scenario
07/22/15       kr       Reset CME client info once QICE is released
07/17/15       sks      ONly access QICE clks using CME's TxD and RxD status.
07/17/15       rsr      Added New HAndling for MCVS + CME.
07/08/14       rsr      Add current q6 and vpe speed api
07/02/15       bj       Increase the split acq step1+NASTT step lock duration from  80 to 85 ms 
                        and boost the clock level to NOM for NASTT search in ACQ state to avoid 1x page miss.
06/19/15       kr       TA.2.0 Enable LOW SVS for Eldarian
06/15/15       rs       BOLT W+W changes
06/12/15       sks      TH - MCVS Changes for using single VU in DC in DS mode.
06/10/15       kr       send MCVS request for Q6A=384 from L1 whenever QICE is enabled
06/03/15       kr       TA featurization
01/19/15       sks      Global re-org for W+W
12/18/14       sks      CPC LS changes
12/05/14       psr      Thor compilation fixes
12/01/14       stk      FR24685:UMTS MCVS for Thor Modem
10/1/14        stk      Fixed the NV bug for clock bumpup and added support for THOR clk bump up via NV
09/16/14       stk      JO SW-FW interface change to support Q6 clock speeds 
08/08/14       sks      Initial Version. MCPM/MCVS code cleanup and JO MCVS 

 ===========================================================================*/

#include "rex.h"
#include "mcpm_api.h"
#include "l1const.h"
#include "wcdma_common_api.h"
#include "wl1_qsh_ext.h" 
#include "cmgr_api.h"

#ifdef FEATURE_WCDMA_MCVS


/* ******************************************** */
/*                  CONSTANTS                   */
/* ******************************************** */
#define MAX_CARRIERS                         2
#define WMCVS_PERCENT_UTIL_HIGH   85
#define WMCVS_PERCENT_UTIL_LOW    80
#define WMCVS_SRCH_PERIOD360      36
#define WMCVS_SRCH_PERIOD180      18
#define MAX_PRIORITY_SRCH_CLIENT_PERIOD_180  4
#define MAX_PRIORITY_SRCH_CLIENT             6
#define WMCVS_SCENARIO_START 1
#define WMCVS_SCENARIO_STOP 0


/* ******************************************** */
/*       STRUCTURES & ENUM DECLARATIONS         */
/* ******************************************** */
typedef enum
{
   MCVS_STANDALONE,
   MCVS_THROUGH_MCPM
}wmcvsdrv_mcvs_req_type;

/* MCVS clients */
typedef enum
{
  WCDMA_WMCPM_MCVSDRV_VPE_PULLIN_UPDATE,      /* 0 */
  WCDMA_WMCPM_MCVSDRV_VPE_UPDATE,             /* 1 */
  WCDMA_WMCPM_MCVSDRV_Q6CLK_UPDATE,           /* 2 */
  WCDMA_WMCPM_MCVSDRV_SRCHCLK_UPDATE,         /* 3 */
  WCDMA_WMCPM_MCVSDRV_VPE_UPDATE_CME,         /* 4 */
  WCDMA_WMCPM_MCVSDRV_CLIENT_Q6A_BOOST,       /* 5 */
  WCDMA_WMCPM_MCVSDRV_CLIENT_Q6A_MCVS,        /* 6 */
  WCDMA_WMCPM_MCVSDRV_Q6B_NASTT_UPDATE        /* 7 */
}wmcvsdrv_client_type;

/*used for Scalar Q6*/

typedef enum
{
  WMCVS_Q6CLK_NULL = 0, 
  WMCVS_Q6ACLK_LOW_CORNER1 = 288000, /*Non Corner*/
  WMCVS_Q6ACLK_SVS_LOW = 403200, /*LOW SVS*/
  WMCVS_Q6ACLK_SVS = 595200, /* SVS*/
  WMCVS_Q6ACLK_SVS_PLUS = 729600, /* SVS+*/  
  WMCVS_Q6ACLK_NOM = 86400, /* NOM*/
  WMCVS_Q6ACLK_NOM_PLUS = 96000, /* NOM+*/
  WMCVS_Q6ACLK_TURBO =  105600 /* TURBO*/ 
} wmcpmdrv_mcvs_q6clk_speedType;

#define WMCVSDRV_SCENARIO_MASK_W2W_GAP_DATA_Q6                              (0x1 << (int32)WL1_W2W_GAP_DATA_Q6)
#define WMCVSDRV_SCENARIO_MASK_W2W_GAP_VOICE_Q6                             (0x1 << (int32)WL1_W2W_GAP_VOICE_Q6)
#define WMCVSDRV_SCENARIO_MASK_W2W_IDLE_Q6                                  (0x1 << (int32)WL1_W2W_IDLE_Q6)
#define WMCVSDRV_SCENARIO_MASK_W2G_GAP_DATA_Q6                              (0x1 << (int32)WL1_W2G_GAP_DATA_Q6)
#define WMCVSDRV_SCENARIO_MASK_W2G_GAP_VOICE_Q6                             (0x1 << (int32)WL1_W2G_GAP_VOICE_Q6)
#define WMCVSDRV_SCENARIO_MASK_W2G_IDLE_Q6                                  (0x1 << (int32)WL1_W2G_IDLE_Q6)
#define WMCVSDRV_SCENARIO_MASK_W2L_GAP_DATA_Q6                              (0x1 << (int32)WL1_W2L_GAP_DATA_Q6)
#define WMCVSDRV_SCENARIO_MASK_W2L_GAP_VOICE_Q6                             (0x1 << (int32)WL1_W2L_GAP_VOICE_Q6)
#define WMCVSDRV_SCENARIO_MASK_W2L_IDLE_Q6                                  (0x1 << (int32)WL1_W2L_IDLE_Q6)
#define WMCVSDRV_SCENARIO_MASK_WL1_DC_ASET_CELLS_OR_QSET_CELLS_GREATER_1_Q6 (0x1 << (int32)WL1_DC_ASET_CELLS_OR_QSET_CELLS_GREATER_1_Q6)
#define WMCVSDRV_SCENARIO_MASK_WL1_DC_ASET_CELLS_OR_QSET_CELLS_GREATER_2_Q6 (0x1 << (int32)WL1_DC_ASET_CELLS_OR_QSET_CELLS_GREATER_2_Q6)
#define WMCVSDRV_SCENARIO_MASK_WL1_IRAT_IHO_Q6                              (0x1 << (int32)WL1_IRAT_IHO_Q6)
#define WMCVSDRV_SCENARIO_MASK_WL1_RESUME_DS_TA_WITH_LTA_OPTS_Q6            (0x1 << (int32)WL1_RESUME_DS_TA_WITH_LTA_OPTS_Q6)


#define WMCVSDRV_SET_INT_Q6_BIMC_SCENARIO_BMSK(scenario) \
do{ \
    wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_q6_q6B_scenario_bmask |= ((0x1) << (uint64)scenario); \
}while(0)

#define WMCVSDRV_RESET_INT_Q6_BIMC_SCENARIO_BMSK(scenario) \
do{ \
    wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_q6_q6B_scenario_bmask &= ~((0x1) << (uint64)scenario); \
}while(0)

#define WMCVSDRV_CHECK_INT_Q6_BIMC_SCENARIO_BMSK(scenario_mask) (wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_q6_q6B_scenario_bmask & scenario_mask)

#define IS_W2G_CLIENT_ACTIVE()                                 (wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_q6_q6B_scenario_bmask & (WMCVSDRV_SCENARIO_MASK_W2G_GAP_DATA_Q6| WMCVSDRV_SCENARIO_MASK_W2G_GAP_VOICE_Q6| WMCVSDRV_SCENARIO_MASK_W2G_IDLE_Q6))? 1: 0
#define IS_W2L_ACTIVE()                                        (wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_q6_q6B_scenario_bmask & (WMCVSDRV_SCENARIO_MASK_W2L_GAP_DATA_Q6| WMCVSDRV_SCENARIO_MASK_W2L_GAP_VOICE_Q6| WMCVSDRV_SCENARIO_MASK_W2L_IDLE_Q6))? 1: 0
#define IS_WL1_IRAT_IHO_ACTIVE()                               (wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_q6_q6B_scenario_bmask & WMCVSDRV_SCENARIO_MASK_WL1_IRAT_IHO_Q6)? 1: 0
#define IS_WL1_RESUME_DS_TA_WITH_LTA_OPTS_ACTIVE()             (wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_q6_q6B_scenario_bmask & WMCVSDRV_SCENARIO_MASK_WL1_RESUME_DS_TA_WITH_LTA_OPTS_Q6)? 1:0
#define IS_WL1_DC_ASET_CELLS_OR_QSET_CELLS_GREATER_1_ACTIVE()  (wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_q6_q6B_scenario_bmask & WMCVSDRV_SCENARIO_MASK_WL1_DC_ASET_CELLS_OR_QSET_CELLS_GREATER_1_Q6)? 1:0
#define IS_WL1_DC_ASET_CELLS_OR_QSET_CELLS_GREATER_2_ACTIVE()  (wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_q6_q6B_scenario_bmask & WMCVSDRV_SCENARIO_MASK_WL1_DC_ASET_CELLS_OR_QSET_CELLS_GREATER_2_Q6)? 1:0

#define WMCVSDRV_SET_INT_Q6B_SCENARIO_BMSK(scenario) \
do{ \
    wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_q6_q6B_scenario_bmask |= ((0x1) << (uint64)scenario); \
}while(0)
#define WMCVSDRV_RESET_INT_Q6B_SCENARIO_BMSK(scenario) \
do{ \
	wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_q6_q6B_scenario_bmask &= ~((0x1) << (uint64)scenario); \
}while(0)



typedef enum
{
  WL1_W2W_GAP,
  WL1_W2G_GAP,
  WL1_W2L_GAP,
  WL1_DC_ASET_CELLS_OR_QSET_CELLS_GREATER_1,
  WL1_DC_ASET_CELLS_OR_QSET_CELLS_GREATER_2,
  WL1_IRAT_IHO,
  WL1_RESUME_DS_TA_WITH_LTA_OPTS,
  WL1_QICE,
  WL1_CONNECTED_TO_IDLE,
  WL1_FBRX_HS_OFFSET_1_9
}wl1_q6_bimc_clients;

typedef enum
{
  WMCVS_Q6,
  //WMCVS_Q6B,
  WMCVS_BMIC,
  WMCVS_MAX
}wl1_q6_bmic_update;

/* offline clock speed enums no longer needed */
typedef struct 
{
   /*# SHO fingers */
   uint8 num_aset_fingers[MAX_CARRIERS];
      /*# SHO cells */
   uint8 num_sho_cells[MAX_CARRIERS];
   /*number of neighbors*/
   uint8 num_neighbor_cells[MAX_CARRIERS];
   /*Total number of fingers*/
   uint8 tot_num_of_fingers[MAX_CARRIERS];
   /* total number of cells will be used for PULL in*/
   uint8 total_num_cells[MAX_CARRIERS];
   /*TxD is present or not*/	  
   boolean status_txd[MAX_CARRIERS];
   /*PCCPCH  info in DCH */
   boolean pccpch_enabled ;
}wmcpmdrv_mcvs_vpe_update_struct_type;

typedef struct
{  
  wl1_q6_q6B_scenarios wmcpmdrv_mcvs_int_q6clk_speed;
  uint32 wmcpmdrv_mcvs_int_q6_scalar_clk_speed ;
  uint32 wmcvsdrv_int_client_bmask;
  uint32 wmcvsdrv_int_concman_steady_scenario_bmask;
  uint32 wmcvsdrv_int_concman_transient_scenario_bmask;
  uint64 wmcvsdrv_int_q6_q6B_scenario_bmask;
  uint32 wmcvs_debug_bumpup_vpe_offline;
  mcpm_mcvsrequest_parms_type wmcpmdrv_wmcvs_request_params;
  wmcpmdrv_mcvs_vpe_update_struct_type wmcpmdrv_mcvs_vpe_update_params;
  uint16 wmcpm_mcvs_int_srch_period;
  uint8 wmcvsdrv_srch_cycle_count;
  /*Variable to track if idle wakeup was granted or rejected by concman.*/
  cmgr_scenario_status_e wmcvsdrv_conc_wkup_status;
}wmcvsdrv_ext_cntrl_params_type;

typedef struct
{  
  cmgr_scenario_type_e type;
  uint32 scenario;
  uint32 update_scenario;
  uint32 priority;
  uint32 flow_ctrl_time_msec;
  uint64 start_timestamp;
  uint32 duration_msec;
}wmcvsdrv_concman_config_params_type;


extern wmcvsdrv_ext_cntrl_params_type wmcvsdrv_ext_cntrl_params[WCDMA_NUM_SUBS];

#define WMCVSDRV_SET_INT_CONCMAN_STEADY_SCENARIO_BMSK(scenario) \
do{ \
    wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_concman_steady_scenario_bmask |= ((0x1) << scenario); \
}while(0)

#define WMCVSDRV_RESET_INT_CONCMAN_STEADY_SCENARIO_BMSK(scenario) \
do{ \
    wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_concman_steady_scenario_bmask &= ~((0x1) << scenario); \
}while(0)

#define WMCVSDRV_SET_INT_CONCMAN_TRANSIENT_SCENARIO_BMSK(scenario) \
do{ \
    wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_concman_transient_scenario_bmask |= ((0x1) << scenario); \
}while(0)

#define WMCVSDRV_RESET_INT_CONCMAN_TRANSIENT_SCENARIO_BMSK(scenario) \
do{ \
    wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_concman_transient_scenario_bmask &= ~((0x1) << scenario); \
}while(0)


#define WMCVSDRV_CONCMAN_MASK_WIDLE_WAKEUP                   (0x1 << (int32)CMGR_WCDMA_WIDLE_WAKEUP)

#define IS_CONCMAN_WIDLE_WAKEUP_ACTIVE()            (wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_concman_transient_scenario_bmask & WMCVSDRV_CONCMAN_MASK_WIDLE_WAKEUP)
#define IS_CONCMAN_WIDLE_WKUP_REJECTED()            (wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_conc_wkup_status == CMGR_REJECTED)
#define RESET_CONCMAN_WIDLE_WKUP_REJECTED()         (wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_conc_wkup_status = 0)


#define WMCVS_GET_MAX_VAL(a, b) ((a >= b) ? (a) : (b))
#define WMCVSDRV_CLIENT_MASK_VPE_UPDATE_CME             (0x1 << (int32)WCDMA_WMCPM_MCVSDRV_VPE_UPDATE_CME)
#define IS_VPE_CME_ACTIVE()                             (wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_client_bmask & WMCVSDRV_CLIENT_MASK_VPE_UPDATE_CME)
#define WMCVSDRV_FING_STEP 3
#define WMCVDRV_CELL_STEP 2
#define WMCVSDRV_SET_INT_CLIENT_BMSK(client) \
do{ \
    wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_client_bmask |= ((0x1) << client); \
}while(0)

#define WMCVSDRV_RESET_INT_CLIENT_BMSK(client) \
do{ \
    wmcvsdrv_ext_cntrl_params[wsub_id].wmcvsdrv_int_client_bmask &= ~((0x1) << client); \
}while(0)

#define WMCVS_SUM_SHO_CELLS(carr_idx)    ((carr_idx == 0) ? (wmcvsdrv_ext_cntrl_params[wsub_id].wmcpmdrv_mcvs_vpe_update_params.num_sho_cells[0]) : ( wmcvsdrv_ext_cntrl_params[wsub_id].wmcpmdrv_mcvs_vpe_update_params.num_sho_cells[0] + wmcvsdrv_ext_cntrl_params[wsub_id].wmcpmdrv_mcvs_vpe_update_params.num_sho_cells[1]))
#define WMCVS_SUM_TOT_FINGERS(carr_idx)  ((carr_idx == 0) ? (wmcvsdrv_ext_cntrl_params[wsub_id].wmcpmdrv_mcvs_vpe_update_params.tot_num_of_fingers[0]) : (wmcvsdrv_ext_cntrl_params[wsub_id].wmcpmdrv_mcvs_vpe_update_params.tot_num_of_fingers[0] + wmcvsdrv_ext_cntrl_params[wsub_id].wmcpmdrv_mcvs_vpe_update_params.tot_num_of_fingers[1]))
#define WMCVS_MAX_SRCHR_MODES 3

/*===========================================================================
FUNCTION     WMCPMDRV_MCVS_COMPUTE_VPE_SPEED

DESCRIPTION
  This function checks MCVS BUMP UP/DOWN criteria for Pure MCVS VPE update calls

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void wmcvsdrv_compute_q6_change(wmcvsdrv_mcvs_req_type req_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wmcvsdrv_q6B_scenario_update_through_MCPM

DESCRIPTION
function to send MCVS request when not already running at desired Q6 frequency

PARAMETERS
Q6 frequency wanted

RETURN VALUE
None

SIDE EFFECTS

===========================================================================*/

void wmcvsdrv_q6B_scenario_update_through_MCPM(boolean start_stop,wl1_q6_q6B_scenarios scenario_req_rel,mcvs_request_type req_type,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WMCVSDRV_SCENARIO_REQUEST_Q6_POWER_CONFIG

DESCRIPTION
  Wrapper function for W MCPM Scenarios to request for power resources.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void wmcvsdrv_scenario_request_q6_power_config(wl1_q6_bimc_clients client, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wmcpmdrv_vpe_update_in_mcvs_param

DESCRIPTION
function to send MCVS request when n ot already running at desired VPE frequency

PARAMETERS
VPE frequencywanted

RETURN VALUE
None

SIDE EFFECTS
Blocking call at max will take 100usec
===========================================================================*/


extern void wmcvsdrv_q6B_scenario_update_through_MCVS(boolean start_stop,wl1_q6_q6B_scenarios scenario_req_rel,mcvs_request_type req_type, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION     WMCVSDRV_SCENARIO_RELINQUISH_Q6_POWER_CONFIG

DESCRIPTION
  Wrapper function for W MCPM Scenarios to relinquish for power resources.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void wmcvsdrv_scenario_relinquish_q6_power_config(wl1_q6_bimc_clients client, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WMCVSDRV_SCENARIO_REQUEST_Q6B_POWER_CONFIG

DESCRIPTION
  Wrapper function for W MCPM Scenarios to request for power resources.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wmcvsdrv_scenario_request_q6B_power_config(wmcvsdrv_mcvs_req_type req_type,wl1_q6_q6B_scenarios scenario_req, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WMCVSDRV_SCENARIO_RELINQUISH_Q6B_POWER_CONFIG

DESCRIPTION
  Wrapper function for W MCPM Scenarios to request for power resources.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void wmcvsdrv_scenario_relinquish_q6B_power_config(wmcvsdrv_mcvs_req_type req_type,wl1_q6_q6B_scenarios scenario_rel, wsub_id_e_type wsub_id);



/*===========================================================================
FUNCTION     WMCVSDRV_Q6_SCENARIO_UPDATE

DESCRIPTION
  Wrapper function for W MCVS Q6 Scenario Update.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wmcvsdrv_q6_scenario_update(boolean start_stop, wmcpmdrv_mcvs_q6clk_speedType q6_speed, wl1_q6_q6B_scenarios scenario_req_rel, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION     WMCVSDRV_BMIC_SCENARIO_UPDATE

DESCRIPTION
  Wrapper function for W MCVS BMIC Scenario Update.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wmcvsdrv_bmic_scenario_update(boolean start_stop,uint32 boost_time_msec, uint32 boost_req,wl1_q6_q6B_scenarios scenario_req_rel, wsub_id_e_type wsub_id);


/*===========================================================================
                          COMMON LOCAL/EXTERN Function Declaration
============================================================================*/
/*===========================================================================
FUNCTION     wmcvsdrv_clear_mvcs_db()

DESCRIPTION
  memset WMCVS request_parms and all globals to zero.
  This will be called during W stack init and de-init
 
PARAMETERS
  None.
 
RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void wmcvsdrv_clear_wmvcs_db( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WMCPMDRV_MCVS_UPDATE_SRCH_PERIODICITY

DESCRIPTION
function to return the new/updated srch period.
 
PARAMETERS
  None.

RETURN VALUE
updated srch period

SIDE EFFECTS
  None.
===========================================================================*/
uint16 wmcpmdrv_mcvs_update_srch_periodicity( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WMCPMDRV_MCVS_UPDATE_SRCH_CLK_TO_SRCH

DESCRIPTION
function to return the offline clk freq in Mhz.
 
PARAMETERS
  None.

RETURN VALUE
offline clk freq in Mhz

SIDE EFFECTS
  None.
===========================================================================*/
uint8 wmcpmdrv_mcvs_update_srch_clk_to_srch( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wmcpmdrv_mcvs_get_triage_param_update

DESCRIPTION
  Wrapper function for retrieving TxD Status from WL1

PARAMETERS
  None.

RETURN VALUE
  TxD_OFF/TxD_ON/TxD_DC

SIDE EFFECTS
  None.
===========================================================================*/
void wmcpmdrv_mcvs_get_triage_param_update(wmcpmdrv_mcvs_vpe_update_struct_type* vpe_params , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wmcpmdrv_mcvs_get_Pullin_cell_cnt

DESCRIPTION
  Gets the cell cnt from demod during pull in procedure.
 
PARAMETERS
 global VPE struct Address

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void wmcpmdrv_mcvs_get_pullin_cell_cnt(wmcpmdrv_mcvs_vpe_update_struct_type *vpe_params, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WMCVSDRV_REQUEST_CLIENT_POWER_CONFIG

DESCRIPTION
  Wrapper function for W MCPM clients to request for power resources.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wmcvsdrv_request_client_power_config(wmcvsdrv_client_type client, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WMCVSDRV_RELINQUISH_CLIENT_POWER_CONFIG

DESCRIPTION
  Wrapper function for W MCPM clients to relinquish for power resources.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wmcvsdrv_relinquish_client_power_config(wmcvsdrv_client_type client, wsub_id_e_type wsub_id);

extern cmgr_scenario_status_e wmcvsdrv_concman_scenario_config_request(wmcvsdrv_concman_config_params_type scenario_params, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wmcpmdrv_get_mcpm_offline_clk_speed

DESCRIPTION
Called from inside the clock change callback to find out the new speed.

PARAMETERS
none

RETURN VALUE
none

SIDE EFFECTS
none 
 
===========================================================================*/
void wmcpmdrv_get_mcpm_clk_speed( wsub_id_e_type wsub_id);
uint8 wmcvsdrv_modulo(uint8 x,uint8 y);
extern void wmcvsdrv_q6Aclk_update_in_mcvs_param(uint32 q6Aspeed_req,mcvs_request_type req_type,wsub_id_e_type wsub_id);
extern void wmcvsdrv_check_Q6A(wsub_id_e_type wsub_id);
void wmcvsdrv_clear_mcvs_cme_info(wsub_id_e_type wsub_id);
uint32 wmcvsdrv_compute_q6b_for_qice_params(uint8 *qset_size, uint8 *qset_iter, boolean *txd_status, boolean *rxd_status,
	                                    uint8 *srch_cycle_count,uint8 *local_num_cells, wsub_id_e_type wsub_id);

extern void wmcvsdrv_send_clk_cfg_qta_gap(wsub_id_e_type wsub_id);


#define MCPM_Q6B_BASE_VALUE_IN_MHZ (WMCVS_Q6BCLK_SVS_12/1000)


/*===========================================================================
FUNCTION     wmcvsdrv_get_qset_txd_status

DESCRIPTION
  This function reads the structure from the CME module that has details on the CME TxD status and QSET size across all carriers.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wmcvsdrv_get_qset_txd_status( wsub_id_e_type wsub_id);

extern void wmcvsdrv_ext_cntrl_params_init( void );
extern void wmcvsdrv_int_cntrl_params_init( void );

extern uint8 wmcvsdrv_compute_vpe_for_qice_params(uint8 *qset_size, uint8 *qset_iter, uint8 *txd_status, uint8 *rxd_status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wmcvsdrv_clear_c1_mcvs_params

DESCRIPTION
  This function Clears C1 MCVS params.

PARAMETERS
  none

RETURN VALUE
  uint8 index

SIDE EFFECTS
  None.
===========================================================================*/
extern void wmcvsdrv_clear_c1_mcvs_params(wsub_id_e_type wsub_id);


#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION wl1_qsh_mdump_collect_wmcvsdrv_params

DESCRIPTION
  Wrapper

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_wmcvsdrv_params(wsub_id_e_type wsub_id);
#endif

#endif /*FEATURE_WCDMA_MCVS*/

#endif /* ifndef WMCVSDRV_H */
