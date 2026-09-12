#ifndef L1_SC_LTE_IRAT_H
#define L1_SC_LTE_IRAT_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              l1_sc_lte_irat.h

GENERAL DESCRIPTION
   This module contains I/F declarations and prototypes related with LTE.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/GSM/L1/vcs/l1_sc.h_v   1.9   03 Jul 2002 15:44:46   gwatkins  $
$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/l1_sc_lte_irat.h#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
28/02/18   km        CR2189789 Handling g2l abort from g2l_trigger_mdsp
09/01/18   km        CR2166879 G2L abort handling in QTA/non-QTA scenarios
29/10/17   km        CR2133917 GL1 support for TA.3.0 L+L; FR37926 non-DDS sub IRAT optimization
16/02/17   cc        CR1010614 G2L idle time transfer and MEAS only support
23/11/15   cgc       CR944459 Fix regression due to CR937488, also requires change in GFW 
17/06/15   jj        CR 856570  fix the regression 853609 
12/06/15   cgc       CR853609 expose g2l_irat_x2l_meas_enter()
17/06/15   sv        CR856025 Revert CR853609
12/06/15   cgc       CR853609 expose g2l_irat_x2l_meas_enter()
08/06/15   cgc       CR849570 l1_g2l_connected_status() renamed l1_g2l_connected_restart()
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
09/04/15   cgc       CR819956 Thor 2.x RF API change, G2W use MSGR
06/03/15   cgc       CR794323 FR25799 support x8 LTE with fast idle search.
21/01/15   jj        CR 784462 remove FEATURE_THOR_G2X_MSGR_REWORK 
03/12/14   cgc       CR715394 G2L code Idle/dedicated rework FEATURE_THOR_G2L_REWORK
26/08/14   pa        CR715657 Band deregistration on finishing IRAT activity.
22/08/14   cgc       CR713634 obsoleted G2L code and featurisations FEATURE_GSM_G2L_TIMED_SRCH and FEATURE_CM_G2L removed
29/05/14   cgc       CR669540 Add l1_sc_start_G2L_timed_search() to GTOW_SRCH_STATE_MULTI_SIM_FW_WAIT 
08/16/01   mk        Initial version.  Minor Cleanup and Header Addition.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "l1_sc_drv.h"
#include "l1_sc_int.h"
#include "gl1_msg_g.h"
#include "gl1_msg_wcdma_meas.h"
#include "l1i.h" /* for running_avg_struct */
#include "gl1_sys_algo.h"
#include "gl1_sys_params.h"
#include "rex.h"

#ifdef  FEATURE_GSM_TO_LTE

#include "lte_as.h"
#include "lte_l1_types.h"
#include "lte_cphy_irat_meas_msg.h"
#include "lte_ml1_ext_api.h"
#include "lte_ml1_irat_ext_api.h"

/*===========================================================================

                         DATA DECLARATIONS

===========================================================================*/

typedef enum
{
   G2L_CM_INACTIVE     =0x00,
   G2L_CM_ACTIVE       =0x01,
   G2L_CM_CNF_PENDING  =0x04,
   G2L_CM_STOP         =0x08
} g2l_cm_mode_states_e;


typedef enum
{
    LTE_GAP_NULL,
    LTE_GAP_INIT, 
    LTE_GAP_START_REQ,
    LTE_GAP_NEXT_REQ,
    LTE_GAP_CNF,
    LTE_GAP_ABORT,
    LTE_GAP_INIT_CNF,
    LTE_GAP_TRIGGER_MDSP 
} g2l_cm_events_e;



typedef enum
{
    LTE_IDLE_GAP_NULL,
    LTE_IDLE_GAP_WAIT,
    LTE_IDLE_GAP_SCHEDULE,
    LTE_IDLE_GAP_TRIGGER_MDSP,

    LTE_IDLE_GAP_START_REQ,
    LTE_IDLE_GAP_NEXT_REQ,
    LTE_IDLE_GAP_STATUS_REQ,
    LTE_IDLE_GAP_CNF,
    LTE_IDLE_GAP_STATUS,
    LTE_IDLE_GAP_ABORT,
    LTE_IDLE_GAP_DEINIT_REQ,
    LTE_IDLE_GAP_DEINIT_CNF,
    LTE_IDLE_GAP_INIT_CNF
} g2l_idle_events_e;

extern lte_ml1_irat_gap_schedule_gap_state_s     ml1_gap_state[];                    /*  {0}; */
extern boolean l1_sc_g2l_build_script_pending[NUM_GERAN_DATA_SPACES];
extern boolean l1_sc_g2l_empty_qta_gap[NUM_GERAN_DATA_SPACES];

/* populate the content of the timed_srch_meas_req from rr_params */
void    l1_sc_populate_g2l( uint16 freq_idx, lte_cphy_irat_meas_timed_srch_meas_req_s *msg, lte_ml1_irat_gap_schedule_hint_e *schedule_hint, boolean g2l_idle, gas_id_t gas_id );
uint16  l1_sc_cm_g2l_state( g2l_cm_events_e gapEvent , uint16 freq_idx , boolean gapBool, gas_id_t gas_id);
void    g2l_irat_populate_rf_param ( lte_cphy_irat_meas_rf_param_s *rf_param, gas_id_t gas_id );
void    g2l_set_cm_inactive(gas_id_t gas_id);

void    l1_sc_idle_g2l_state( g2l_idle_events_e gapEvent, gas_id_t gas_id);
void    l1_isr_G2L_idle_trigger(gas_id_t gas_id);
void    l1_sci_g2l_idle_aborted( void* dummy, gas_id_t gas_id);
void    l1_sc_set_ml1_init(boolean in_ml1_init, gas_id_t gas_id);
void    l1_sc_schedule_G2L_timed_search( boolean fast_restart , gas_id_t gas_id);
void    l1_sc_g2l_fast_search_abort( gas_id_t gas_id);

void    l1_sc_g2l_init(gas_id_t gas_id );

uint16  l1_g2l_connected_restart(gas_id_t gas_id);

void    l1_g2l_trigger_mdsp( boolean g2l_idle, gas_id_t gas_id);
void    l1_g2l_cm_abort(gas_id_t gas_id);

void    g2l_irat_x2l_meas_enter( lte_earfcn_t lte_earfcn, gas_id_t gas_id);
void    g2l_irat_x2l_meas_exit(boolean aborted, gas_id_t gas_id);

void    l1_sc_g2l_shutdown (gas_id_t gas_id);

boolean gl1_hw_set_g2l_devinfo_for_qta(const geran_gl1_qta_msg_type* qta_data_p, gas_id_t gas_id);

#endif /*FEATURE_GSM_TO_LTE*/
#endif /*L1_SC_LTE_IRAT_H*/

