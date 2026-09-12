/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                GL1  RX DIVERSITY

GENERAL DESCRIPTION
   This file contains procedure related with GL1 Rx Diversity.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_rxdiversity.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
12/03/18   sp       CR2202251 Dont extend DRx chain beyond Prx Chain. Also Drx release can be inline
23/03/17   sp       CR2021993: To request for Drx chain Dual multimode data subscription also.
27/10/17   sp       CR2132978: Reset Dynamic RxD states also when putting PRx to sleep
11/05/17   br       CR2045130 Enable RXD for the first burst of CCCH block, if RXQUAL is less than 32767.
29/12/17   bg       CR2164888 Don't clear DivRx_band_supported information in FTM
13/10/17   bg       CR2123778 Ignore Unlock for Drx Client in SR-DSDS case
24/08/17   bg       CR2096995 Set RxD state to GL1_DYN_RXD_RF_WAKEUP only when RF wakeup requested
21/08/17   bg       CR2095914 Release the TRM lock for diversity client when there is a state transition to Idle
18/08/16   cws      CR1050728 Do RxD enter mode when changing state from idle to transfer mode
14/09/16   cws      CR1065201 Set the RxD force exit flag after the one frame wait when receiving RxD release
15/08/16   cws      CR1045632 Delay by one frame for immediate RxD unlock to avoid race condition with RxD enable burst
10/08/16   snjv     CR1050221 Avoid Error print when RxD state is Active
31/08/16   sp       CR1060518: When Trm status for diversity is granted,but drx enter is not done - then trigger the enter mode.
05/04/16   og       CR996300. Check the force exit flag before enabling burst rxdiv.
27/04/16   og       CR1001822. Check the abort flag before enabling the burst rxdiv.
27/06/16   tsk      CR1032267 When enabling rxd use get_frame_duration instead of get_post_decrement_frame api.
21/07/16   og       CR1027240. In the Unlock_by case, ensure enabling of RXD on burst by
                    burst basis is in sync with the exit diversity mode.
30/01/17   mk       CR1113152 Keep RxD always enabled in FTM mode based on RxDForcedON NV
05/08/16   tsk      CR1048165 Reset GL1 and Gdrivers flag to remain in sync after RxD exit.
08/12/16   mk       CR1097591 Enhancement for DRX: DRx and PRx difference will be applied to neighbor cell measurements, if DRx power level is greater than PRx
09/08/16   br       CR1051729 Dont Release the secondary chain  during  voice call in FTM mode
12/07/16   tsk      CR987216 For SSIM Idle mode RxD should be gated by Idle mode RxD NV.
12/07/16   tsk      CR978200 Support Dynamic Rxd with SSIM and WLAN Antenna Sharing configuration.
07/06/16   bg       CR1025289 Fix for KW errors in gl1_hw_rxdiv_handle_task_grm_rane()
12/01/15   tsk      CR943455 Transition RxD State from RACH to IDLE when main state transitions from RACH to IDLE.
18/11/15   cah      CR900214 - [WLAN+WAN] Failed in acquiring the Rxd chain in Single SIM Mode during Voice Call Attempt
11/11/15   pg       CR935148 Request TRM for DRx chain only for supported bands
11/10/15   tsk      CR937467: Relocate dynamic RxD handler to end of gstmr_frame_tick_process.
11/08/15   tsk      CR929882: Handle RxD exit processing.
20/10/15   ap       CR923515 Do not extend duration of diversity chain if denied by TRM
08/10/15   cjl      CR915017 Fix power meas lockup when RxD is aborted and granted in same frame
30/09/15   cws      CR912244 call disable_div_chain in connected mode only
28/09/15   pg       CR909305 Update dynamic RxD switching and remove pwr_meas suspension 
08/09/15   cah      CR892575 DR DSDS: Add support for diversity chain unlock in transfer mode
20/08/15   pa       CR893756 Grouped TRM apis for the diversity chain.
12/08/15   pa       CR887572 RxD: Schedule DRX wakeup after Rx/Tx and request TRM in task context.
27/08/15   jk       CR892120 Reset Data RxD ON/OFF control variables  introduced in CR 864645 during state transitions and during init
26/08/15   ws       CR896163 Fix compile errors for undefining FEATURE_DUAL_SIM
26/08/15   jk       CR896003:Check gl1_total_ps_burst_cnt is non-zero before averaging
17/08/15   jk       CR890035:RxD not getting enabled in voice and Data modes
29/07/15   pa       CR880410:GL1 RxD: Don't enable RxD in IRAT gap.
28/07/15   pa       CR880706: GL1 Dynamic RxD: Wait for FW exit command before enabling it.
28/07/15   jk/pa    CR864124:Clear RxD structure every time GERAN START is received.
28/07/15   pa       CR880774: Enable Dynamic RxD only for Th2.x modem.
28/07/15   jk       CR864124:Clear RxD structure every time GERAN START is recieved.
21/07/15   jk       CR864645:Data RxD ON/OFF changes for GCF TC 14.18.3 & 14.18.1
21/07/15   pg       CR871012 Apply FTM diversity band support capabilities passed by GRF for cal
20/07/15   pa       CR875967:Disable Data Rxd on Multimode sub if other sub is non3pp capable.
17/06/15   cjl      CR853555 Use dynamic connected mode reasons for CS/PS traffic
30/06/15   ws       CR852519 - Resolve GL1 enum conversion compile warnings
05/06/15   jk       CR826594:If RxD EFS is not confgured, enable MSRD s
01/06/15   cah      CR841239:  remove TRM_DEVICE_MAPPING trm_get_info type in GRM for rxdiv device handling. Type is no longer supported by TRM.
29/05/15   pa       CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD.
22/05/15   pg       CR841558 In FTM, use DRx device passed by GRF (if valid), rather than via TRM request
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session
23/02/15   sjv      CR799313: Using new mcfg APIs replacing efs_get for Geran
31/03/15   pa       CR815621:GRM restructuring for TH 2.0.
24/03/15   ap       CR811053:FR22272 - TRM Unification  - Access and Traffic attempts
16/03/15   pg       CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP
12/03/15   pa       CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
13/02/15   pa       CR756156:G2X RF device handling using TRM apis.
21/01/15   pa       CR758792:RxD support for multi-sim.
06/01/14   jk       CR773695:DRx device Id not requested for all bands
22/12/14   pa       CR773689: Set RF device to Invalid on RF exit.
15/12/14   pg       CR630281 implementation of RxD support for data/MSRD
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
03/12/14   pg       CR765847: Add support of Idle RxD for AGCH and NCELL SCH
26/11/14   jj       CR 760174 fix  compiler warning
13/11/14   pa       CR755557:Support for granted band/rf device in Idle mode RxD.
17/10/14   cs       CR735918 Assign supported bands for initial ACQ based on UE support
10/10/14   cos      CR736560 BO 2.x Idle RxD mirror combined exit/sleep requests according to the enter/wakeup modes
10/10/14   pg       CR729410 make DRX exit mode MSGR RSP handling non-blocking
22/09/14   ap       CR720862: FR 20912: SGLTE+G DSDA with WTR asymmetric GSM support
09/09/14   cos      CR720965 adding support for DR-DSDS - rfdev 2 and 3 handling wtr to RxD and calibration
29/08/14   cos      CR 716812 - Adding missing changes for imrd framework
01/09/14   ggu      CR712878:Not Kick in RxD mode for Single SIM SGLTE mode and Enable NV control for RxD feature
08/08/14   ka       CR706866 FR18739: DR-DSDS RxD Idle, part 2 - new TRM clients
06/05/14   dv       CR660054 RXLM enhancement to ensure that no tech de-allocates another techs buffers
06/08/14   ka       CR699964 Implement Idle RXD support in GL1
31/07/14   cos      CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
16/07/14   jk       CR695230:Reset enable_rxdiversity variable while exiting DRx mode
14/07/14   cja      CR692358 Ensure RxDiv RxLM buffer de-allocated.
04/07/14   cs       CR688003 GRM functions moved to gpl1_grm_intf.c
02/07/14   cos      CR 688739 - Residual BER in RxD mode when PRx is disconnected
27/06/14   cs       CR685966 Add DR INFO async callback support
25/06/14   cs       Mainline FEATURE_GSM_DIME_TRM
30/05/14   pg       CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
19/06/14   cs/ch    CR651795 TRM Updated changes for DR-DSDS
17/04/14   cos      CR636543 - Changes for enabling Rx Diversity on Bolt - GL1/GRF intf updates
17/04/14   cos      CR646420 Changes for enabling Rx Diversity on Bolt - GL1/GFW intf updates
07/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
19/02/14   jk       CR608290:GERAN RxD ON/OFF adaptive algorithm
27/01/13   jk       CR 607682: Disable GSM rx diversity by defualt via NV 71588
10/12/13   jk       Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif
#include "comdef.h"
#include "geran_nv.h"

#include "geran_multi_sim.h"

#include "gtmrs.h"
#include "gtmrs_g.h"         /* GSM timer services */
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hw_clk_ctl_g.h"
#include "gl1_hw_debug.h"
#include "l1_utils.h"
#include "gl1_hwi.h"
#include "gl1_hw_gsac.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_intf_shared_vars.h"
#include "mdsp_async_intf.h"

#include "gpl1_dual_sim.h"

#include "gpl1_grm_intf.h"

#include "rfm.h"
#include "tcxomgr.h"

#include "ftm.h"
#include "DALSys.h"
#include "msg.h"            /* Diagnostics MSGs */
#include "err.h"            /* ERR_FATAL call   */

#include "rex.h"

#include "slpc.h"
#include "gl1_hw_sleep_ctl.h"

#include "mcpm_api.h"

#include "gl1_hw_tcxo_mgr3.h"

#include "gl1_mutex.h"
#include "gl1_msg_g.h"
#include "l1i.h"

#include "gfw_sw_intf.h"
#include "fws.h"
#include "mcs_hwio.h"


#include "rxlm_intf.h"
#include "txlm_intf.h"

#include "ftm.h"

#include "fs_lib.h"

#include "rfgsm_msg.h"
#include "gprs_mem.h"

/* Add just for rfm_wcdma_convert_band_rfcom_to_rfi */
#include "rfm_wcdma.h"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "wmgr.h"
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#include "l1_drx.h"
#include "l1_task.h"
#include "l1_sc_int.h"
#include "l1i.h"
#include "lm_types.h"
#include "msgr.h"
#include "geran_msg.h"

#include "geran_tasks.h"

#include "gl1_hw_arbitration.h"

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#include "l1_os.h"
gl1_hw_rxdiv_data_t gl1_hw_rxdiv_data[NUM_GERAN_DATA_SPACES];

static gl1_hw_rxdiv_data_t *gl1_hw_rxdiv_data_ptr[NUM_GERAN_DATA_SPACES] =
         { INITIAL_VALUE_INDEXED( &gl1_hw_rxdiv_data ) };

/*  Rule:
 DSDS:
 Data RxD: Enabled in Multimode Sub in Single Multimode configuration.
           Else disabled.
 Idle RxD: Enabled.
 Voice RxD: Enabled.

DSDA:
 Not supported in Dual Multimode.
 RxD(I,D,V) is enabled only on Multimode Sub.           */

/* Data of SIM1 and SIM2 are same except in Single Multimode G+G scenario.
 However, keep it in the data structure to handle future enhancement */
const boolean gl1_hw_msim_rxdiv_data
                                     [GL1_RXDIV_MAX_MULTIMODE]
                                     [GL1_RXDIV_MAX_COMBINATION]
                                     [GL1_RXDIV_MAX_DEVICE_MODE]
                                     [GL1_RXDIV_MAX_SIM_TYPE] /*SIM1 or SIM2*/
                                     [GL1_RXDIV_MAX_SUB_STATE] /*I, D, V*/
                                      =
{
  /*V - Voice , I - Idle, D - Data, Y- V,I or D*/
  /*GL1_RXD_SINGLE_MUTIMODE*/
  {
    /*GL1_RXD_XPLUSG*/
    {
      /*GL1_RXD_DSDS*/
      {
        /*Doesn't matter SIM_1 or SIM 2 as other rat is X. Keep same values. */
        /* I    D     V */
        {TRUE, FALSE, TRUE}, /*SIM1*/
        {TRUE, FALSE, TRUE} /*SIM2*/
      },

      /*GL1_RXD_DSDA*/
      {
        /*Doesn't matter SIM_1 or SIM 2 as other rat is X. Keep same values.
        In X+G, DSDA, RxD is disabled.*/
        /* I    D     V */
        {FALSE, FALSE, FALSE}, /*SIM1*/
        {FALSE, FALSE, FALSE} /*SIM2*/
      }
    },

    /*GL1_RXD_GPLUSG*/
    {
      /*GL1_RXD_DSDS*/
      {
        /* I    D     V */
        {TRUE, TRUE, TRUE}, /*SIM1*/
        {TRUE, TRUE, TRUE} /*SIM2*/
      },

      /*GL1_RXD_DSDA*/
      {
        /* In G+G, DSDA, SIM 1 - Rxd Enabled SIM2 RxD is disabled.*/
        /* I    D     V */
        {TRUE, TRUE, TRUE}, /*SIM1*/
        {FALSE, FALSE, FALSE} /*SIM2*/
      }
    }
  },

  /*GL1_RXD_DUAL_MULTIMODE*/
  {
    /*GL1_RXD_XPLUSG*/
    {
      /*GL1_RXD_DSDS*/
      {
        /*Doesn't matter SIM_1 or SIM 2 as other rat is X. Keep same values. */
        /* I    D     V */
        {TRUE, FALSE, TRUE}, /*SIM1*/
        {TRUE, FALSE, TRUE} /*SIM2*/
      },

      /*GL1_RXD_DSDA*/
      {
        /*DSDA not supported in dual multimode*/
        /*Doesn't matter SIM_1 or SIM 2 as other rat is X. Keep same values. */
        /* I    D     V */
        {FALSE, FALSE, FALSE}, /*SIM1*/
        {FALSE, FALSE, FALSE} /*SIM2*/
      }
    },

    /*GL1_RXD_GPLUSG*/
    {
      /*GL1_RXD_DSDS*/
      {
        /* I    D     V */
        {TRUE, TRUE, TRUE}, /*SIM1*/
        {TRUE, TRUE, TRUE} /*SIM2*/
      },

      /*GL1_RXD_DSDA*/
      {
        /*DSDA not supported in dual multimode*/
        /* I    D     V */
        {FALSE, FALSE, FALSE}, /*SIM1*/
        {FALSE, FALSE, FALSE} /*SIM2*/
      }
    }
  }
};

/*===========================================================================

                           FUNCTION PROTOTYPES

===========================================================================*/
static void gl1_hw_async_intf_exit_mode_cmd_DivRx( gas_id_t gas_id );

static gl1_hw_rxdiv_sim_configuration gl1_hw_rxdiv_get_sim_configuration(gas_id_t gas_id);

static gl1_hw_rxdiv_device_mode gl1_hw_rxdiv_get_device_mode(gas_id_t gas_id);

static gl1_hw_rxdiv_rat_combination gl1_hw_rxdiv_get_rat_combination(gas_id_t gas_id);

static gl1_hw_rxdiv_sim_type gl1_hw_rxdiv_get_sim_type(gas_id_t gas_id);

static gl1_hw_rxdiv_sub_state gl1_hw_rxdiv_map_req_to_sub_state(
                                                  grm_divrx_request_type  divrx_req,
                                                  gas_id_t                gas_id);

static void gl1_hw_rxdiv_handle_l1_default_state(gl1_hw_rxdiv_l1_state  next_state,
                                                 gas_id_t               gas_id);

static void gl1_hw_rxdiv_handle_l1_idle_state(gl1_hw_rxdiv_l1_state next_state,
                                              gas_id_t              gas_id);

static void gl1_hw_rxdiv_handle_l1_access_state(gl1_hw_rxdiv_l1_state next_state,
                                                gas_id_t              gas_id);

static void gl1_hw_rxdiv_handle_l1_dedicated_state(gl1_hw_rxdiv_l1_state  next_state,
                                                   gas_id_t               gas_id);

static void gl1_hw_rxdiv_handle_l1_dtm_state(gl1_hw_rxdiv_l1_state  next_state,
                                             gas_id_t               gas_id);

static void gl1_hw_rxdiv_handle_l1_transfer_state(gl1_hw_rxdiv_l1_state next_state,
                                                  gas_id_t              gas_id);

static void gl1_hw_rxdiv_handle_divrx_change_state(grm_divrx_request_type divrx_req,
                                                   gas_id_t               gas_id);

static gl1_trm_state_t gl1_hw_enable_div_chain_rf_fw_config(grm_client_enum_t       prx_client_id,
                                                            trm_duration_t          duration,
                                                            grm_divrx_request_type  divrx_req,
                                                            gas_id_t                gas_id);

static void gl1_hw_disable_div_chain_rf_fw_config(grm_client_enum_t       prx_client_id,
                                                  boolean                 force_exit,
					          boolean                 delay,
                                                  gas_id_t                gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
static void gl1_reset_ps_rxdiv_params(gas_id_t gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/

static void gl1_hw_handle_dynamic_rxd_trm_requested(grm_client_enum_t   divrx_client_id,
                                                    gas_id_t            gas_id);

static void gl1_hw_handle_dynamic_rxd_rf_wakeup(grm_client_enum_t   divrx_client_id,
                                                gas_id_t            gas_id);

static void gl1_hw_handle_dynamic_rxd_active(grm_client_enum_t   divrx_client_id,
                                             gas_id_t            gas_id);

static void gl1_hw_handle_dynamic_rxd_fw_exit(grm_client_enum_t   divrx_client_id,
                                              gas_id_t            gas_id);
/*===========================================================================

                                FUNCTIONS

===========================================================================*/
/*===========================================================================

FUNCTION  gl1_enter_diversity_mode

DESCRIPTION
   Reset the gl1_hw_rxdiv_data static store data

===========================================================================*/
void gl1_hw_rxdiv_data_init(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx = MDSP_RXLM_BUF_IDX_INVALID;
  gl1_hw_set_gsm_rf_id(RFM_MAX_DEVICES,
                      grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id),
                      TRUE,
                      gas_id);
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_DivRx_band_supported = 0;
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_enable_rxdiv_agc = FALSE;

  gl1_hw_set_rf_enter_status(RFGSM_INVALID_ENTER_MODE, gas_id);
  gl1_hw_set_gfw_enter_status(GL1_GFW_INVALID_ENTER, gas_id);
  gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_INVALID_SLEEP_WAKEUP_MODE, gas_id);

  gl1_hw_set_rxdiv_wait_for_block_start(FALSE, gas_id);

  gl1_hw_set_force_divrx_exit(FALSE, gas_id);
  gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);
  gl1_hw_set_dynamic_rxd_tick_required(FALSE, gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  gl1_reset_ps_rxdiv_params(gas_id);
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */

  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_enabled_frames = 0;
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count = 0;

  MSG_GERAN_MED_0_G(GL1_RXD"gl1_hw_rxdiv_data_store  initialized");
  return;
}


/*===========================================================================

FUNCTION  gl1_enter_diversity_mode

DESCRIPTION
   This function is called when moving from idle to dedicated or packet transfer mode to DTM.
   This function calls APIs to allocated RxLM buffer for diversity chain and send enter mode
   command to RF and FW if a valid RFM device Id is allocated by TRM for diversity Rx.

===========================================================================*/
void gl1_enter_diversity_mode( gl1_init_rflm_type gsm_init_rflm_state , gas_id_t gas_id )
{
  boolean DivRx_rfm_dev_assigned = FALSE;
  lm_status_type lm_status;
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t divrx_client_id = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
  gl1_rxd_control_type gl1_rxd_ctl_flags = gl1_get_rxd_control_flags(gas_id);

  if(TRUE == gl1_rxd_ctl_flags.enableRxd)
  {
    if((
        (grm_get_trm_status(divrx_client_id, gas_id) == GL1_TRM_GRANTED) &&
#ifdef FEATURE_SGLTE
       (gl1_hw_get_sglte_mode(gas_id) == FALSE)&&
#endif
       (gl1_msg_get_multi_sim_sys_mode( ) != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE))||
       (ftm_get_mode() == FTM_MODE))
    {
      MSG_GERAN_MED_2_G(GL1_RXD"GL1 HW Init type %d, RxLM buffer %d",
                        gsm_init_rflm_state,
                        gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
      if ((gsm_init_rflm_state != GL1_HW_INIT_INACTIVE) &&
          (GL1_RF_PRX_WAKEUP_DIVRX_SLEEP_MODE == gl1_hw_get_rf_sleep_wakeup_mode(gas_id)))
      {
        /* Check that RxLM buffer not already allocated */
        if (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx != MDSP_RXLM_BUF_IDX_INVALID)
        {
#ifndef FEATURE_GSM_RX_DIVERSITY_IDLE
          MSG_GERAN_MED_1_G(GL1_RXD"GSM RxLM buffer %d already allocated - deallocate now",
                            gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
          lm_status = rflm_deallocate_rx_handle(LM_GSM,
                                                LM_GSM,
                                                gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
          MSG_GERAN_MED_1_G(GL1_RXD"GSM RxLM buffer dealloc status lm_status = %i",lm_status);
          gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx = MDSP_RXLM_BUF_IDX_INVALID;

          /* Allocate RxLM buffer */
          lm_status = rflm_allocate_rx_handle(LM_GSM,
                                              LM_GSM,
                                              &gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);

          if (lm_status == LM_SUCCESS)
          {
            MSG_GERAN_MED_1_G(GL1_RXD"GSM RxD RxLM buffer %d allocated",
                              gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
          }
          else
          {
            MSG_GERAN_ERROR_0_G(GL1_RXD"Unable to allocate RxLM buffer");
          }
#endif /*!FEATURE_GSM_RX_DIVERSITY_IDLE*/
        }
        else
        {
          /* Allocate RxLM buffer */
          lm_status = rflm_allocate_rx_handle(LM_GSM,
                                              LM_GSM,
                                              &gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
          if (lm_status == LM_SUCCESS)
          {
            MSG_GERAN_MED_1_G(GL1_RXD"GSM RxD RxLM buffer %d allocated",
                              gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
          }
          else
          {
            MSG_GERAN_ERROR_0_G(GL1_RXD"Unable to allocate RxLM buffer");
          }
        }
      }

      /* Initialize RF for Diversity Rx */
      DivRx_rfm_dev_assigned = gl1_hw_trm_init_DivRx(gas_id);

      /* Enter GSM mode if TRM assigns RF resource for diversity Rx */
      if ((DivRx_rfm_dev_assigned)
          && (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx != MDSP_RXLM_BUF_IDX_INVALID))
      {
        /*Wake up Rf first*/
        if (GL1_RF_PRX_WAKEUP_DIVRX_SLEEP_MODE == gl1_hw_get_rf_sleep_wakeup_mode(gas_id))
        {
          gl1_hw_rf_wakeup(gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx,
                           GL1_DRX_WAKEUP_MODE,
                           gas_id);
          gl1_hw_set_rf_sleep_wakeup_mode(GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE, gas_id);
          gl1_reset_divrx_sel_avg(TRUE, gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
          /* default RxD for Data state to TRUE */
          if (gl1_get_MSRD_ctl_nv(gas_id))
          {
             gl1_set_enable_rxdiversity_data(TRUE, gas_id);
             gl1_set_enable_rxdiversity_data_dynamic(TRUE, gas_id);
          }
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
        }
      }
    }
    else
    {
#ifdef FEATURE_SGLTE
      MSG_GERAN_MED_3_G(GL1_RXD"RxD Not supported in %d Multi SIM mode or in SS under %d sglte mode "
                        "divrx trm status %d",
                        gl1_msg_get_multi_sim_sys_mode( ),
                        gl1_hw_get_sglte_mode(gas_id),
                        grm_get_trm_status(divrx_client_id, gas_id));
#else
      MSG_GERAN_MED_2_G(GL1_RXD"RxD Not supported in %d Multi SIM mode divrx trm status %d",
                        gl1_msg_get_multi_sim_sys_mode( ),
                        grm_get_trm_status(divrx_client_id, gas_id));
#endif
    }
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_trm_init_DivRx

DESCRIPTION
  Request RF resource for diversity Rx for all the four GSM bands in sequence.
  If the allocated RF device does not support any of the four GSM bands,
  enter mode command for diversity Rx is sent. But in the RF and FW burst Rx
  commands, diversity flag is disabled if we were to receive on a band that
  diversity chain does not support.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if a TRM returns a valid device ID in which GSM Rx Diversity is supported.

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_hw_trm_init_DivRx(gas_id_t gas_id)
{
  int i = 0;
  rfm_device_enum_type  prx_rf_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type  drx_rf_device = RFM_INVALID_DEVICE;
  boolean               trm_rf_device_DivRx_assigned = FALSE;
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t divrx_client_id = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id,
                                                                     gas_id);


  if(FTM_MODE == ftm_get_mode())
  {
    prx_rf_device = gl1_get_prx_ftm_dev(gas_id);
  }
  else
  {
    prx_rf_device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
    drx_rf_device = gl1_hw_get_gsm_rf_id(divrx_client_id, gas_id);
  }


#ifdef FEATURE_GSM_RX_TX_SPLIT
  /* if we have an FTM DRx device allocated, then use it and skip TRM device mapping*/
  if ((FTM_MODE == ftm_get_mode()) && (gl1_get_drx_ftm_dev(gas_id) != RFM_INVALID_DEVICE))
  {
     gl1_hw_set_gsm_rf_id(gl1_get_drx_ftm_dev(gas_id),
                          divrx_client_id,
                          FALSE,
                          gas_id);

     /* clear band support bitmask */
     gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_DivRx_band_supported = 0;

     for (i = 0; i < MAX_BAND_INFO_GRM; i++)
     {
        gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_trm_rf_devices_DivRx[i].band = grm_valid_gsm_sys_bands_divrx[gas_id][i];
        gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_trm_rf_devices_DivRx[i].device = gl1_get_drx_ftm_dev(gas_id);
        gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_DivRx_band_supported |= gl1_get_drx_ftm_band_support(gas_id, i);
     }

     trm_rf_device_DivRx_assigned = TRUE;
  }
  else
#endif /* FEATURE_GSM_RX_TX_SPLIT */

  /*check we have the grant for PRX and DRX devices*/
  if (prx_rf_device != RFM_INVALID_DEVICE &&
      drx_rf_device != RFM_INVALID_DEVICE)
  {
      trm_rf_device_DivRx_assigned = TRUE;
  }
  else
  {
    MSG_GERAN_ERROR_2_G(GL1_RXD"Invalid primary or secondary RF device prx %d drx %d", prx_rf_device, drx_rf_device );
    gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_DivRx_band_supported = 0;
  }

  MSG_GERAN_HIGH_3_G(GL1_RXD"gl1_hw_trm_init_DivRx: DRX rfdev %i , "
                           "gl1_hw_DivRx_band_supported %d grm_client_id %d",
                     gl1_hw_get_gsm_rf_id(divrx_client_id, gas_id),
                     gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_DivRx_band_supported,
                     divrx_client_id);

  return trm_rf_device_DivRx_assigned;
}
/*===========================================================================

FUNCTION  gl1_exit_diversity_mode

DESCRIPTION
   This function is called when moving from dedicated/DTM mode to Idle.
   Exit mode commands are sent to RF and FW for releasing the Rx diversity device.

===========================================================================*/
void gl1_exit_diversity_mode(boolean force_exit, gas_id_t gas_id)
{
  gl1_rxd_control_type gl1_rxd_ctl_flags = gl1_get_rxd_control_flags(gas_id);
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t divrx_client_id = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id,
                                                                     gas_id);
  rfm_device_enum_type divrx_rf_device = gl1_hw_get_gsm_rf_id(divrx_client_id,
                                                              gas_id);

  if(TRUE == gl1_rxd_ctl_flags.enableRxd)
  {
    if ((!gl1_get_IMRD_ctl_nv(gas_id)) || force_exit)
    {
      MSG_GERAN_HIGH_3_G(GL1_RXD"DRx RF exit for device=%d rf_sleep_wakeup %d gfw_enter %d",
                          divrx_rf_device,
                          gl1_hw_get_rf_sleep_wakeup_mode(gas_id),
                          gl1_hw_get_gfw_enter_status(gas_id));

      if (GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE == gl1_hw_get_rf_sleep_wakeup_mode(gas_id))
      {
        /*Put RF to sleep*/
        gl1_hw_rf_sleep(gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx,
                        GL1_DRX_SLEEP_MODE,
                        gas_id);
      }
      /*Send exit mode command for diversity Rx to GFW*/
      if (gl1_hw_get_gfw_enter_status(gas_id) == GL1_GFW_PRX_DIVRX_ENTER)
      {
        gl1_hw_async_intf_exit_mode_cmd_DivRx(gas_id);
        MSG_GERAN_HIGH_0_G(GL1_RXD"Set GFW status to GL1_GFW_WAIT_FOR_DRX_EXIT");
        gl1_hw_set_gfw_enter_status(GL1_GFW_WAIT_FOR_DRX_EXIT, gas_id);
      }
    }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
    /* Don't clear DivRx_band_supported information in FTM for single SIM with RxDForcedOn */
    if(( ftm_get_mode() == FTM_MODE ) && !(!gl1_msg_get_multi_sim_mode()&& gl1_rxd_ctl_flags.RxdForcedOn))
    {
      gl1_hw_cleanup_divrx_rfdevice_info(gas_id);
    }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

    gl1_start_rxdiv_agc(FALSE,gas_id);
    gl1_reset_divrx_sel_avg (TRUE,gas_id);

    gl1_set_enable_rxdiversity(FALSE,gas_id);

    gl1_hw_set_rxdiv_wait_for_block_start(FALSE, gas_id);
    gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);

    /*Reset the counter on exiting from voice or data RxD. */
    gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_enabled_frames = 0;

    gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count = 0;
  }
  return;
}

/*===========================================================================

FUNCTION gl1_hw_deallocate_rxlm_buf_idx_DivRx

DESCRIPTION
  Returns None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_deallocate_rxlm_buf_idx_DivRx( gas_id_t gas_id )
{
  if (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx != MDSP_RXLM_BUF_IDX_INVALID)
  {
    if (!rflm_deallocate_rx_handle(LM_GSM,
                                   LM_GSM,
                                   gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx))
    {
      MSG_GERAN_ERROR_1_G(GL1_RXD"Unable to de-allocate RxD RxLM buffer %d",
                          gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
    }
    else
    {
      MSG_GERAN_HIGH_1_G(GL1_RXD"GSM RxD RxLM buffer %d deallocated",
                         gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
      gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx = MDSP_RXLM_BUF_IDX_INVALID;
    }
  }
}

/*===========================================================================

FUNCTION  gl1_hw_async_intf_exit_mode_cmd_DivRx

DESCRIPTION
  Send MSGR message to GFW to indicate that we are in idle mode. Use MSGR blocking wait
  to receive enter mode rsp from GFW before continuing

===========================================================================*/
static void gl1_hw_async_intf_exit_mode_cmd_DivRx( gas_id_t gas_id )
{
  GfwHostRfmExitModeCmd gfwHostRfmExitModeCmd;

  /* send msgr message to GFW */
  (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_RFM_EXIT_MODE_CMD,
                                          &gfwHostRfmExitModeCmd,
                                          NULL,
                                          gas_id);
}
/*===========================================================================

FUNCTION  gl1_hw_gsm_band_supported

DESCRIPTION
  Check if the requested band is supported by the diversity Rx device

===========================================================================*/
boolean gl1_hw_gsm_band_supported(sys_band_T sys_band,gas_id_t gas_id)
{
  boolean band_supported = FALSE;
  trm_band_t  trm_band = 0xFF;

  switch(sys_band)
  {
    case SYS_BAND_PGSM_900:
    case SYS_BAND_EGSM_900:
      trm_band = SYS_BAND_GSM_EGSM_900;
      break;

    case SYS_BAND_DCS_1800:
      trm_band = SYS_BAND_GSM_DCS_1800;
      break;

    case SYS_BAND_PCS_1900:
      trm_band = SYS_BAND_GSM_PCS_1900;
      break;

    case SYS_BAND_CELL_850:
      trm_band = SYS_BAND_GSM_850;
      break;

    default:
      MSG_GERAN_ERROR_1_G( "GL1:Unsupported trm_band_t type for sys_band_T=%d",
                         sys_band );
      break;
    }

  if ( gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_DivRx_band_supported &
       gl1_hw_map_sys_band_to_gsm_rxdiv_band(trm_band, gas_id) )
  {
    band_supported = TRUE;
  }

  return (band_supported);
}

/*===========================================================================

FUNCTION  gl1_hw_gsm_trm_band_supported

DESCRIPTION
  Check if the requested TRM band is supported by the diversity Rx device

===========================================================================*/
boolean gl1_hw_gsm_trm_band_supported(trm_band_t trm_band,gas_id_t gas_id)
{
  boolean band_supported = FALSE;

  if ( gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_DivRx_band_supported &
       gl1_hw_map_sys_band_to_gsm_rxdiv_band(trm_band, gas_id) )
  {
    band_supported = TRUE;
  }

  return (band_supported);
}

/*===========================================================================

FUNCTION  gl1_hw_map_sys_band_to_gsm_rxdiv_band

DESCRIPTION
  Check if the requested band is supported by the diversity Rx device

===========================================================================*/
uint32 gl1_hw_map_sys_band_to_gsm_rxdiv_band(trm_band_t  band,
                                             gas_id_t    gas_id)
{
  uint32 gsm_rxdiv_band = 0x0;

  switch(band)
  {
    case SYS_BAND_GSM_PGSM_900:
    case SYS_BAND_GSM_EGSM_900:
      gsm_rxdiv_band = GSM_RXDIV_BAND_900;
    break;

    case SYS_BAND_GSM_DCS_1800:
      gsm_rxdiv_band = GSM_RXDIV_BAND_1800;
    break;

    case SYS_BAND_GSM_PCS_1900:
      gsm_rxdiv_band = GSM_RXDIV_BAND_1900;
    break;

    case SYS_BAND_GSM_850:
      gsm_rxdiv_band = GSM_RXDIV_BAND_850;
    break;

    default:
      MSG_GERAN_ERROR_1_G(GL1_RXD"GL1:Unsupported RxD Sys Band %d", band );
    break;
  }

  return ( gsm_rxdiv_band );
}

/*===========================================================================

FUNCTION gl1_hw_get_gsm_rxdiv_data_ptr

DESCRIPTION
  Returns a pointer to static RxDiv data

DEPENDENCIES
  None

RETURN VALUE
  Pointer to GSM RxD data store

SIDE EFFECTS
  None
===========================================================================*/
gl1_hw_rxdiv_data_t *gl1_hw_get_gsm_rxdiv_data_ptr(gas_id_t gas_id)
{
  return (&gl1_hw_rxdiv_data[gas_id]);
}

/*===========================================================================

FUNCTION gl1_hw_get_gsm_rxdiv_rf_id

DESCRIPTION
  Returns the RF resource ID for GSM Diversity Rx

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
rfm_device_enum_type gl1_hw_get_gsm_rxdiv_rf_id(gas_id_t gas_id)
{
  return (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_trm_rf_device_DivRx);
}

/*===========================================================================

FUNCTION gl1_hw_set_gsm_rxdiv_rf_id

DESCRIPTION
  Sets the RF resource ID for GSM Diversity Rx

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_set_gsm_rxdiv_rf_id(rfm_device_enum_type  rf_device_id,
                                gas_id_t              gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_trm_rf_device_DivRx = rf_device_id;
}
/*===========================================================================

FUNCTION gl1_start_rxdiv_agc

DESCRIPTION
 Sets if AGC for diversity chain should be started

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_start_rxdiv_agc(boolean is_agc_rxdiv,gas_id_t gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_enable_rxdiv_agc = is_agc_rxdiv;
}

/*===========================================================================

FUNCTION gl1_get_rxdiv_agc_start

DESCRIPTION
 Indicates if AGC for diversity chain has started

DEPENDENCIES
  None

RETURN VALUE
  TRUE if AGC for diversity path has started

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_get_rxdiv_agc_start(gas_id_t gas_id)
{
  return(gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_enable_rxdiv_agc);
}

/*===========================================================================

FUNCTION gl1_hw_get_rxd_efs_item

DESCRIPTION
 Retrieve the RxD control flags from EFS item 71588

===========================================================================*/
void gl1_hw_get_rxd_efs_item(gas_id_t gas_id)
{
  uint32 gl1_rxd_ctl_flag = 0;
  gl1_rxd_control_type rxd_ctl_flag;
  int efs_error;

 efs_error = geran_efs_read_primary(GL1_EFS_RxD,
                                    &gl1_rxd_ctl_flag,
                                    sizeof(gl1_rxd_ctl_flag));

  MSG_GERAN_HIGH_2_G(GL1_RXD"GL1:EFS RxD Control flags %d %d",
                     efs_error,gl1_rxd_ctl_flag);

  if(efs_error < 0)
  {
    /*Enable MSRD if RxD EFS item is not configured. If RxD EFS item is configured, NV value takes into effect*/
    MSG_GERAN_HIGH_0_G(GL1_RXD"Enabling MSRD if EFS is not configured");
    rxd_ctl_flag.enableRxd = TRUE;
    rxd_ctl_flag.enableDrx = TRUE;
    rxd_ctl_flag.enablePrx = TRUE;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
    rxd_ctl_flag.enableIMRD = TRUE;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
    rxd_ctl_flag.enableMSRD = TRUE;
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
    rxd_ctl_flag.RxdForcedOn = TRUE;
    rxd_ctl_flag.threshold = 0;

  }
  else
  {
    /*Enable the RxD control flags as per the NV items set*/
    rxd_ctl_flag.enableRxd = (gl1_rxd_ctl_flag & ENABLE_RXDIVERSITY) ? TRUE : FALSE;
    rxd_ctl_flag.enableDrx = (gl1_rxd_ctl_flag & ENABLE_DIVERSITY_RX) ? TRUE : FALSE;
    rxd_ctl_flag.enablePrx = (gl1_rxd_ctl_flag & ENABLE_PRIMARY_RX) ? TRUE : FALSE;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
    rxd_ctl_flag.enableIMRD = (gl1_rxd_ctl_flag & ENABLE_IMRD) ? TRUE : FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
    rxd_ctl_flag.enableMSRD = (gl1_rxd_ctl_flag & ENABLE_MSRD) ? TRUE : FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
    rxd_ctl_flag.RxdForcedOn = (gl1_rxd_ctl_flag & RXDIV_FORCED_ON) ? TRUE : FALSE;
    rxd_ctl_flag.threshold = (gl1_rxd_ctl_flag & 0xE0) >> 5;
  }

  /*Update the RxD control flags*/
  gl1_set_rxd_control_flags(rxd_ctl_flag,gas_id);
}
/*===========================================================================

FUNCTION gl1_get_enable_rxdiversity

DESCRIPTION
 It sets Rx Diversity status.

===========================================================================*/
void gl1_set_enable_rxdiversity(boolean   enable_rxdiversity,
                                gas_id_t  gas_id)
{

  gl1_rxd_control_type gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);

  if (ftm_get_mode() == FTM_MODE && gl1_rxd_ctl_flag.RxdForcedOn)
  {
     MSG_GERAN_MED_0_G(GL1_RXD"Dynamic RxD: In FTM Mode Dynamic RxD disabled (RXD always ON).");
     enable_rxdiversity = TRUE;
  }
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_enable_rxdiversity = enable_rxdiversity;
}

/*===========================================================================

FUNCTION gl1_get_enable_rxdiversity

DESCRIPTION
 It returns Rx Diversity status.

===========================================================================*/
boolean gl1_get_enable_rxdiversity(gas_id_t gas_id)
{
  return(gl1_hw_rxdiv_data_ptr[gas_id]->gl1_enable_rxdiversity);
}

/*===========================================================================

FUNCTION gl1_hw_set_rf_enter_status

DESCRIPTION
 It sets RF PRX and DIVRX Enter Mode status.

===========================================================================*/
void gl1_hw_set_rf_enter_status(rfgsm_enter_mode_type rf_type,
                                gas_id_t gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_rf_enter_mode_type = rf_type;
}

/*===========================================================================

FUNCTION gl1_hw_get_rf_enter_status

DESCRIPTION
 It returns RF PRX and DIVRX Enter Mode status.

===========================================================================*/
rfgsm_enter_mode_type gl1_hw_get_rf_enter_status(gas_id_t gas_id)
{
  return gl1_hw_rxdiv_data_ptr[gas_id]->gl1_rf_enter_mode_type;
}

/*===========================================================================

FUNCTION gl1_hw_set_imrd_gfw_enter_status

DESCRIPTION
 It sets Rx Diversity GFW status.

===========================================================================*/
void gl1_hw_set_gfw_enter_status(gl1_hw_gfw_enter_mode_type gfw_type,
                                 gas_id_t gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_imrd_gfw_enter_mode_type = gfw_type;
}

/*===========================================================================

FUNCTION gl1_hw_get_imrd_gfw_enter_status

DESCRIPTION
 It returns Rx Diversity GFW status.

===========================================================================*/
gl1_hw_gfw_enter_mode_type gl1_hw_get_gfw_enter_status(gas_id_t gas_id)
{
  return gl1_hw_rxdiv_data_ptr[gas_id]->gl1_imrd_gfw_enter_mode_type;
}

/*===========================================================================

FUNCTION gl1_hw_set_rf_sleep_wakeup_mode

DESCRIPTION
 It sets PRX and DIVRX sleep wakeup status.

===========================================================================*/
void gl1_hw_set_rf_sleep_wakeup_mode(gl1_hw_rf_sleep_wakeup_mode_type sleep_wakeup_type,
                                     gas_id_t                         gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_rf_sleep_wakeup_mode_type = sleep_wakeup_type;
}

/*===========================================================================

FUNCTION gl1_hw_get_rf_sleep_wakeup_mode

DESCRIPTION
 It returns PRX and DIVRX sleep wakeup status.

===========================================================================*/
gl1_hw_rf_sleep_wakeup_mode_type gl1_hw_get_rf_sleep_wakeup_mode(gas_id_t gas_id)
{
  return gl1_hw_rxdiv_data_ptr[gas_id]->gl1_rf_sleep_wakeup_mode_type;
}

/*===========================================================================

FUNCTION gl1_hw_set_rxdiv_wait_for_block_start

DESCRIPTION
 It sets whether GL1 should wait for block start or not.

===========================================================================*/
void gl1_hw_set_rxdiv_wait_for_block_start(boolean  block_start,
                                           gas_id_t gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_rxd_wait_for_block_start
                          = block_start;
}

/*===========================================================================

FUNCTION gl1_hw_get_rxdiv_wait_for_block_start

DESCRIPTION
 It returns wait for block start.

===========================================================================*/
boolean gl1_hw_get_rxdiv_wait_for_block_start(gas_id_t gas_id)
{
  return gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_rxd_wait_for_block_start;
}

#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
/*===========================================================================

FUNCTION gl1_get_enable_rxdiversity_data

DESCRIPTION
 It sets Rx Diversity status.

===========================================================================*/
void gl1_set_enable_rxdiversity_data(boolean  enable_rxdiversity_data,
                                     gas_id_t gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_enable_rxdiversity_data = enable_rxdiversity_data;
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG
  MSG_GERAN_HIGH_1_G(GL1_RXD"Enable RxD for data: %d", enable_rxdiversity_data);
#endif
}

/*===========================================================================

FUNCTION gl1_get_enable_rxdiversity_data

DESCRIPTION
 It returns Rx Diversity status.

===========================================================================*/
boolean gl1_get_enable_rxdiversity_data(gas_id_t gas_id)
{
  return(gl1_hw_rxdiv_data_ptr[gas_id]->gl1_enable_rxdiversity_data);
}
/*===========================================================================

FUNCTION gl1_get_enable_rxdiversity_data_dynamic

DESCRIPTION
 It sets Rx Diversity status.

===========================================================================*/
void gl1_set_enable_rxdiversity_data_dynamic(boolean  enable_rxdiversity_data_dynamic,
                                             gas_id_t gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_enable_rxdiversity_data_dynamic = enable_rxdiversity_data_dynamic;
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG
  MSG_GERAN_HIGH_1_G(GL1_RXD"Dynamic enable RxD for data: %d", enable_rxdiversity_data_dynamic);
#endif
}

/*===========================================================================

FUNCTION gl1_get_enable_rxdiversity_data_dynamic

DESCRIPTION
 It returns Rx Diversity status.

===========================================================================*/
boolean gl1_get_enable_rxdiversity_data_dynamic(gas_id_t gas_id)
{
  return(gl1_hw_rxdiv_data_ptr[gas_id]->gl1_enable_rxdiversity_data_dynamic);
}
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE

/*===========================================================================

FUNCTION gl1_exit_idle_diversity_mode

DESCRIPTION
 Exit RxDiversity without deallocating RXLM buffer indexes. Clearing only
 entermode flags and rfdevices stored.

===========================================================================*/
void gl1_exit_idle_diversity_mode(gas_id_t gas_id)
{
  gl1_hw_cleanup_divrx_rfdevice_info(gas_id);
}

/*===========================================================================

FUNCTION gl1_alloc_IMRD_rxdiv_rxlm_buff_idx

DESCRIPTION
 Get RXLM buff idx for IMRD for DRX mirrored with PRX (i.e. at hw init)

===========================================================================*/
boolean gl1_alloc_IMRD_rxdiv_rxlm_buff_idx(gas_id_t gas_id)
{
  boolean IMRD_rxlm_allocated = FALSE;
  lm_status_type lm_status;

  if(
#ifdef FEATURE_SGLTE
     (gl1_hw_get_sglte_mode(gas_id) == TRUE)&&
#endif
     (gl1_msg_get_multi_sim_sys_mode( ) == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE))
  {
  MSG_GERAN_FATAL_0_G(GL1_RXD" IDLE RXD is NOT supported in DSDA or SGLTE SS or in FTM mode");
    return FALSE;
  }

  /* Check that RxLM buffer not already allocated */
  if (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx != MDSP_RXLM_BUF_IDX_INVALID)
  {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
    MSG_GERAN_MED_1_G(GL1_RXD"RxLM buffer %d already allocated - deallocate now",
                      gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

    lm_status = rflm_deallocate_rx_handle(LM_GSM,
                                          LM_GSM,
                                          gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
MSG_GERAN_MED_1_G(GL1_RXD"RxLM buffer dealloc status lm_status = %i",lm_status);
#endif

    gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx = MDSP_RXLM_BUF_IDX_INVALID;
  }

  /* Allocate RxLM buffer */
  lm_status = rflm_allocate_rx_handle(LM_GSM,
                                      LM_GSM,
                                      &gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
  if (lm_status == LM_SUCCESS)
  {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
    MSG_GERAN_MED_1_G(GL1_RXD"GSM IMRD RxLM buffer %d allocated for idle rxd",
                      gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
#endif

    IMRD_rxlm_allocated = TRUE;
  }
  else
  {
    MSG_GERAN_ERROR_0_G(GL1_RXD"Unable to allocate RxLM buffer");
  }
  return (IMRD_rxlm_allocated);
}

/*===========================================================================

FUNCTION gl1_get_imrd_rxdiv_rxlm_buff_idx

DESCRIPTION
Returns the stored RXLM buff index for DRX

===========================================================================*/
uint32 gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id_t gas_id)
{
  return(gl1_hw_rxdiv_data_ptr[gas_id]->gl1_gsm_rxlm_buf_idx_DivRx);
}
/*===========================================================================

FUNCTION gl1_hw_cleanup_divrx_rfdevice

DESCRIPTION
  Set any eventually saved value for DRX rf device to RFM_INVALID_DEVICE.
  Cleaning up used to ensure we don't use an possibly unavailable device.
===========================================================================*/
void gl1_hw_cleanup_divrx_rfdevice_info(gas_id_t gas_id)
{
  uint8 i = 0;
  /*Reset band bitmap and DivRX band info*/

  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_DivRx_band_supported = 0;
  for (i = 0; i < MAX_BAND_INFO_GRM; i++)
  {
    gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_trm_rf_devices_DivRx[i].band =
                                                              SYS_BAND_CLASS_NONE;
    gl1_hw_rxdiv_data_ptr[gas_id]->gl1_hw_trm_rf_devices_DivRx[i].device =
                                                              RFM_INVALID_DEVICE;
  }
}
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

#ifdef FEATURE_GSM_RX_DIVERSITY
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

/*===========================================================================

FUNCTION gl1_hw_rxdiv_get_sim_configuration

DESCRIPTION
  Get Single Multimode or Dual Multimode configuration.
===========================================================================*/
static gl1_hw_rxdiv_sim_configuration gl1_hw_rxdiv_get_sim_configuration(gas_id_t gas_id)
{
  gl1_hw_rxdiv_sim_configuration sim_config = GL1_RXDIV_INVALID_MULTIMODE;

  boolean g_only_sub = gl1_hw_get_gsm_only_mode(GERAN_ACCESS_STRATUM_ID_1);
  boolean g_only_other_sub = gl1_hw_get_gsm_only_mode(GERAN_ACCESS_STRATUM_ID_2);

  if (gl1_hw_rxdiv_get_device_mode(gas_id) != GL1_RXDIV_INVALID_DEVICE_MODE)
  {
    /*T/F - True/False value of g_only_xxxx variable*/
    /* TT: if both subs are in 2G mode (user setting), follow G+G configuration in
       Dual Multimode*/
    if (g_only_sub && g_only_other_sub)
    {
      sim_config = GL1_RXDIV_SINGLE_MULTIMODE;
    }
    /* FF : Both subs are multimode capable*/
    else if ((!g_only_sub) && (!g_only_other_sub))
    {
      /*G+G, X+G, G+W*/
      sim_config = GL1_RXDIV_DUAL_MULTIMODE;
    }
    /*FT, TF - > X+G or G+G*/
    else
    {
      sim_config = GL1_RXDIV_SINGLE_MULTIMODE;
    }
  }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_3_G(GL1_RXD"gl1_hw_rxdiv_get_sim_configuration sim_config %d "
                            "g_only_sub %d g_only_other_sub %d",
                     sim_config,
                     g_only_sub,
                     g_only_other_sub);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

  return sim_config;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_get_device_mode

DESCRIPTION
  Get DSDS/DSDA configuration
===========================================================================*/
static gl1_hw_rxdiv_device_mode gl1_hw_rxdiv_get_device_mode(gas_id_t gas_id)
{
  sys_modem_device_mode_e_type sys_dev_mode = gl1_msg_get_multi_sim_sys_mode();
  gl1_hw_rxdiv_device_mode rxdiv_dev_mode = GL1_RXDIV_INVALID_DEVICE_MODE;

  switch (sys_dev_mode)
  {
  case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY:
    rxdiv_dev_mode = GL1_RXDIV_DSDS;
    break;

  case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE:
    rxdiv_dev_mode = GL1_RXDIV_DSDA;
    break;

  default:
    MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid sys_dev_mode %d", sys_dev_mode);
    break;
  }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_1_G(GL1_RXD"gl1_hw_rxdiv_get_device_mode rxdiv_dev_mode %d",
                     rxdiv_dev_mode);
#endif

  return rxdiv_dev_mode;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_get_rat_combination

DESCRIPTION
  Get X+G/G+G combination
===========================================================================*/
static gl1_hw_rxdiv_rat_combination gl1_hw_rxdiv_get_rat_combination(gas_id_t gas_id)
{
  gl1_hw_rxdiv_rat_combination rat_combination = GL1_RXDIV_INVALID_RAT_COMBINATION;

  if (gl1_hw_rxdiv_get_device_mode(gas_id) != GL1_RXDIV_INVALID_DEVICE_MODE)
  {
    if (GONLY_OPERATION_IS_ACTIVE())
    {
      rat_combination = GL1_RXDIV_GPLUSG;
    }
    else
    {
      rat_combination = GL1_RXDIV_XPLUSG;
    }
  }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_1_G(GL1_RXD"gl1_hw_rxdiv_get_rat_combination rat_combination %d",
                     rat_combination);
#endif

  return rat_combination;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_get_sim_type

DESCRIPTION
  Get SIM1 or SIM2
===========================================================================*/
gl1_hw_rxdiv_sim_type gl1_hw_rxdiv_get_sim_type(gas_id_t gas_id)
{
  gl1_hw_rxdiv_sim_type sim_type = GL1_RXDIV_INVALID_SIM_TYPE;
  gl1_hw_rxdiv_rat_combination rat_combination = GL1_RXDIV_INVALID_RAT_COMBINATION;

  if ((gas_id == GERAN_ACCESS_STRATUM_ID_1) || (gas_id == GERAN_ACCESS_STRATUM_ID_2))
  {
    rat_combination = gl1_hw_rxdiv_get_rat_combination(gas_id);

    switch (rat_combination)
    {
    case GL1_RXDIV_XPLUSG:
      sim_type = (gl1_hw_rxdiv_sim_type)gas_id;
      break;

    case GL1_RXDIV_GPLUSG:
      if(gl1_hw_get_gsm_only_mode(gas_id))
      {
        sim_type = (gl1_hw_rxdiv_sim_type)gas_id;
      }
      else
      {
        sim_type = (gl1_hw_rxdiv_sim_type)gas_id;
      }
      break;

    default:
      MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid rat_combination %d", rat_combination);
      break;
    }
  }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_1_G(GL1_RXD"gl1_hw_rxdiv_get_sim_type sim_type %d",
                     sim_type);
#endif
  return sim_type;
}

#endif /* (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )   */
/*===========================================================================

FUNCTION gl1_hw_rxdiv_map_req_to_sub_state

DESCRIPTION
  This function is called in multimode scenario.
  Get RxD request type: Idle, Data, Voice
===========================================================================*/
static gl1_hw_rxdiv_sub_state gl1_hw_rxdiv_map_req_to_sub_state(
                                                  grm_divrx_request_type  divrx_req,
                                                  gas_id_t                gas_id)
{
  gl1_hw_rxdiv_sub_state state = GL1_RXDIV_INVALID_SUB_STATE;

  switch (divrx_req)
  {
  case GRM_DIVRX_TRM_REQ_IDLE:
    state = GL1_RXDIV_IDLE;
    break;

  case GRM_DIVRX_TRM_REQ_DED:
    state = GL1_RXDIV_VOICE;
    break;

  case GRM_DIVRX_TRM_REQ_DATA:
    state = GL1_RXDIV_DATA;
    break;

  default:
    MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid divrx_req %d", divrx_req);
    break;
  }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_2_G(GL1_RXD"gl1_hw_rxdiv_map_req_to_sub_state divrx_req %d state %d",
                     divrx_req, state);
#endif
  return state;
}

/*===========================================================================

FUNCTION gl1_hw_is_rxdiv_allowed

DESCRIPTION
  Find whether RxD is allowed for input request
===========================================================================*/
boolean gl1_hw_is_rxdiv_allowed(grm_divrx_request_type divrx_req, gas_id_t gas_id)
{
  gl1_hw_rxdiv_sim_type sim_type = GL1_RXDIV_INVALID_SIM_TYPE;
  gl1_hw_rxdiv_sub_state state = GL1_RXDIV_INVALID_SUB_STATE;
  gl1_hw_rxdiv_rat_combination rat_combination = GL1_RXDIV_INVALID_RAT_COMBINATION;
  gl1_hw_rxdiv_sim_configuration sim_config = GL1_RXDIV_INVALID_MULTIMODE;
  gl1_hw_rxdiv_device_mode rxdiv_dev_mode = GL1_RXDIV_INVALID_DEVICE_MODE;
  sys_modem_device_mode_e_type sys_dev_mode = gl1_msg_get_multi_sim_sys_mode();
  boolean rxd_allowed = FALSE, nv_rxd_item = FALSE;

  switch (divrx_req)
  {
  case GRM_DIVRX_TRM_REQ_IDLE:
    nv_rxd_item = gl1_get_IMRD_ctl_nv(gas_id);
    break;

  case GRM_DIVRX_TRM_REQ_DED:
    nv_rxd_item = gl1_get_rxd_ctl_nv(gas_id);
    break;

  case GRM_DIVRX_TRM_REQ_DATA:
    nv_rxd_item = gl1_get_MSRD_ctl_nv(gas_id);
    break;

  default:
    MSG_GERAN_ERROR_1_G(GL1_RXD"Diversity Request %d", divrx_req);
    break;
  }

  if (nv_rxd_item)
  {
  if (SYS_MODEM_DEVICE_MODE_SINGLE_SIM == sys_dev_mode)
  {
    rxd_allowed = TRUE;
  }
  /*DSDS/DSDA*/
  else if (gl1_msg_get_multi_sim_mode())
  {
    sim_config = gl1_hw_rxdiv_get_sim_configuration(gas_id);
    rxdiv_dev_mode = gl1_hw_rxdiv_get_device_mode(gas_id);
    /*SIM1/SIM2*/
    sim_type = gl1_hw_rxdiv_get_sim_type(gas_id);
    state = gl1_hw_rxdiv_map_req_to_sub_state(divrx_req, gas_id);

    rat_combination = gl1_hw_rxdiv_get_rat_combination(gas_id);

    if (GL1_RXDIV_INVALID_RAT_COMBINATION != rat_combination)
    {
      if ((sim_config != GL1_RXDIV_INVALID_MULTIMODE)
          && (rxdiv_dev_mode != GL1_RXDIV_INVALID_DEVICE_MODE)
          && (sim_type != GL1_RXDIV_INVALID_SIM_TYPE)
          && (state != GL1_RXDIV_INVALID_SUB_STATE))
      {
        rxd_allowed = gl1_hw_msim_rxdiv_data[sim_config]
                                            [rat_combination]
                                            [rxdiv_dev_mode]
                                            [sim_type]
                                            [state];
        if (rxd_allowed)
        {
          /*Check any other RxD restrictions.*/
          rxd_allowed &= gl1_hw_msim_rxd_allowed(state, gas_id);
        }
      }
      
        MSG_GERAN_MED_4_G(GL1_RXD"gl1_hw_is_rxd_allowed sim_config %d "
                          "rxdiv_dev_mode %d sim_type %d state %d ",
                          sim_config, rxdiv_dev_mode, sim_type, state);
      }
    else
    {
      MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid rat_combination %d", rat_combination);
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid sys_dev_mode %d", sys_dev_mode);
  }
  }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_5_G(GL1_RXD"gl1_hw_is_rxd_allowed rxd_allowed %d sim_config %d "
                          "rxdiv_dev_mode %d sim_type %d state %d ",
                          rxd_allowed, sim_config, rxdiv_dev_mode, sim_type, state);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

  return rxd_allowed;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_handle_l1_change_state

DESCRIPTION
  Handle Rx Diversity on changing L1 state.
===========================================================================*/
void gl1_hw_rxdiv_handle_l1_change_state(gl1_hw_rxdiv_l1_state  current_state,
                                         gl1_hw_rxdiv_l1_state  next_state,
                                         gas_id_t               gas_id)
{
  switch (current_state)
  {
  case GL1_RXDIV_L1_DEFAULT_STATE:
    gl1_hw_rxdiv_handle_l1_default_state(next_state, gas_id);
    break;

  case GL1_RXDIV_L1_IDLE_STATE:
    gl1_hw_rxdiv_handle_l1_idle_state(next_state, gas_id);
    break;

  case GL1_RXDIV_L1_RANDOM_ACCESS_STATE:
    gl1_hw_rxdiv_handle_l1_access_state(next_state, gas_id);
    break;

  case GL1_RXDIV_L1_DEDICATED_STATE:
    gl1_hw_rxdiv_handle_l1_dedicated_state(next_state, gas_id);
    break;

  case GL1_RXDIV_L1_DTM_STATE:
    gl1_hw_rxdiv_handle_l1_dtm_state(next_state, gas_id);
    break;

  case GL1_RXDIV_L1_TRANSFER_STATE:
    gl1_hw_rxdiv_handle_l1_transfer_state(next_state, gas_id);
    break;

  default:
    MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid RxD L1 current state %d", current_state);
    break;
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_handle_l1_change_state

DESCRIPTION

===========================================================================*/
static void gl1_hw_rxdiv_handle_l1_default_state(gl1_hw_rxdiv_l1_state  next_state,
                                                 gas_id_t               gas_id)
{
  gl1_rxd_control_type gl1_rxd_ctl_flags = gl1_get_rxd_control_flags(gas_id);

  switch (next_state)
  {
  case GL1_RXDIV_L1_DEDICATED_STATE:
    {
      /*Called from all L1 states except L1_DTM_MODE, L1_RANDOM_ACCESS_MODE and
        L1_PACKET_ACCESS_MODE on transition to dedicated state. */
      if(gl1_rxd_ctl_flags.enableRxd == TRUE)
      {
        /*TRM has already been requested for grant*/
        gl1_enter_diversity_mode(GL1_HW_INIT_RX_ONLY,gas_id);
      }
    }
    break;

  case GL1_RXDIV_L1_TRANSFER_STATE:
    {
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
      if(gl1_get_MSRD_ctl_nv(gas_id))
      {
        gl1_reset_ps_rxdiv_params(gas_id);
      }
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
    }
    break;

  default:
    MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid RxD L1 change state from Default to  %d", next_state);
    break;
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_handle_l1_change_state

DESCRIPTION

===========================================================================*/
static void gl1_hw_rxdiv_handle_l1_idle_state(gl1_hw_rxdiv_l1_state next_state,
                                              gas_id_t              gas_id)
{
  switch (next_state)
  {
  case GL1_RXDIV_L1_RANDOM_ACCESS_STATE:
    {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
      if (gl1_get_rxd_ctl_nv(gas_id))
      {
        /*TRM has already been requested for grant*/
        gl1_enter_diversity_mode(GL1_HW_INIT_RX_ONLY,gas_id);
      }
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
    }
    break;

  case GL1_RXDIV_L1_TRANSFER_STATE:
    {
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
      if(gl1_get_MSRD_ctl_nv(gas_id))
      {
	    gl1_enter_diversity_mode(GL1_HW_INIT_RX_ONLY,gas_id);
         gl1_reset_ps_rxdiv_params(gas_id);
       }
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
    }
    break;

    #ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
    /* GL1_RXDIV_L1_DEFAULT_STATE covers below two transitions.
       1) IDLE mode -> FIND BCCH transition.
       2) IDLE mode -> INACTIVE transition.
       And this will not affect the IDLE -> DECOUPLED state transition, as we are doing the TRM release for
       diversity chain in Single SIM only. 
    */
    case GL1_RXDIV_L1_DEFAULT_STATE:
    {
      if ((gl1_msg_get_multi_sim_mode() == FALSE)
        && (gl1_get_IMRD_ctl_nv(gas_id) == TRUE)
        && (gl1_get_rxd_ctl_nv(gas_id) == TRUE)
      )
      {
        volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        grm_client_enum_t divrx_client_id = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);

        if ((grm_get_trm_status(divrx_client_id, gas_id) == GL1_TRM_GRANTED)
             || (grm_get_trm_status(divrx_client_id, gas_id) == GL1_TRM_CALLBACK_PENDING)
        )
        {
          grm_release_rx_sec(divrx_client_id, gas_id);
          MSG_GERAN_HIGH_0_G(GL1_RXD"ANT_SHARING: Release TRM lock when leaving IDLE to RXD DEFAULT State");
        }
        else
        {
          MSG_GERAN_HIGH_2_G(GL1_RXD"ANT_SHARING: TRM Status %d transition from leaving IDLE to RXD DEFAULT State",
            divrx_client_id, grm_get_trm_status(divrx_client_id, gas_id));
        }
      }
    }
    break;
    #endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

  default:
    MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid RxD L1 change state from Idle to  %d", next_state);
    break;
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_handle_l1_change_state

DESCRIPTION

===========================================================================*/
static void gl1_hw_rxdiv_handle_l1_access_state(gl1_hw_rxdiv_l1_state next_state,
                                                gas_id_t              gas_id)
{
  gl1_rxd_control_type gl1_rxd_ctl_flags = gl1_get_rxd_control_flags(gas_id);
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  switch (next_state)
  {
  case GL1_RXDIV_L1_DEDICATED_STATE:
    {
      if(gl1_rxd_ctl_flags.enableRxd == TRUE)
      {
        /*TRM has already been requested for grant. Need it ? as GL1 is coming
          to this state from acess state. Keep it, as it checks for TRM status
          first*/
        gl1_enter_diversity_mode(GL1_HW_INIT_RX_ONLY,gas_id);
      }
    }
    break;

  case GL1_RXDIV_L1_TRANSFER_STATE:
    {
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
      if((gl1_rxd_ctl_flags.enableMSRD == TRUE)
         && (gl1_rxd_ctl_flags.enableRxd == TRUE))
      {
        /*TRM has already been requested for grant. Need it ? as GL1 is coming
        to this state from acess state. Keep it, as it checks for TRM status
        first*/
        gl1_enter_diversity_mode(GL1_HW_INIT_RX_ONLY,gas_id);
      }

      if(gl1_get_MSRD_ctl_nv(gas_id))
      {
         gl1_reset_ps_rxdiv_params(gas_id);
      }
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
    }
    break;
	
  case GL1_RXDIV_L1_IDLE_STATE:
	{
	  grm_client_enum_t divrx_client_id = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
	  gl1_hw_handle_idle_rxd_rf_fw_exit(divrx_client_id, gas_id);
	}
	break;

  default:
    MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid RxD L1 change state from Access to %d", next_state);
    break;
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_handle_l1_change_state

DESCRIPTION

===========================================================================*/
static void gl1_hw_rxdiv_handle_l1_dedicated_state(gl1_hw_rxdiv_l1_state  next_state,
                                                   gas_id_t               gas_id)
{
  gl1_hw_rxd_dynamic_rxd_state dynamic_rxd_state = gl1_hw_get_dynamic_rxd_state(gas_id);
  grm_client_enum_t divrx_client_id = GRM_NO_CLIENT;

  switch (next_state)
  {
  case GL1_RXDIV_L1_IDLE_STATE:
    /* If dynamic RxD configuration is not in progess, exit Rx Diversity.*/
    if ((GL1_DYN_RXD_NULL == dynamic_rxd_state)
        ||(GL1_DYN_RXD_ACTIVE == dynamic_rxd_state)
        ||(GL1_DYN_RXD_TRM_REQUESTED == dynamic_rxd_state))
    {
    /*IMRD is disabled, Release FW/RF.
      IMRD is enabled, release FW/RF on sleep.
      TRM will release later as a part of grm_release*/
    gl1_exit_diversity_mode(FALSE, gas_id);

      gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_NULL, gas_id);

      gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);

      /*Disable Dynamic RxD Handler*/
      gl1_hw_set_dynamic_rxd_tick_required(FALSE, gas_id);

      /* Release TRM for Dedicated to IDLE case before UE goes to sleep while holding TRM chain. */
      divrx_client_id = grm_map_prx_to_divrx_client_id(gl1_get_current_grm_client_id(gas_id), gas_id);
 
      if ((grm_get_trm_status(divrx_client_id, gas_id) == GL1_TRM_GRANTED)
           || (grm_get_trm_status(divrx_client_id, gas_id) == GL1_TRM_CALLBACK_PENDING))
      {
        MSG_GERAN_HIGH_0_G(GL1_RXD"Release TRM lock when transitioning from Dedicated to IDLE");
        grm_release_rx_sec(divrx_client_id, gas_id);
      }
      else
      {
        MSG_GERAN_HIGH_1_G(GL1_RXD"TRM status (%d) during transitioning from Dedicated to IDLE",
        grm_get_trm_status(divrx_client_id, gas_id));
      }
    }
    else
    {
      gl1_hw_set_dynamic_rxd_abort(TRUE, gas_id);
    }
    break;

  case GL1_RXDIV_L1_DTM_STATE:
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
    if(gl1_get_MSRD_ctl_nv(gas_id))
    {
       gl1_reset_ps_rxdiv_params(gas_id);
    }
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
    break;

  default:
    MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid RxD L1 change state from Dedicated to %d", next_state);
    break;
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_handle_l1_change_state

DESCRIPTION

===========================================================================*/
static void gl1_hw_rxdiv_handle_l1_dtm_state(gl1_hw_rxdiv_l1_state  next_state,
                                             gas_id_t               gas_id)
{
  gl1_hw_rxd_dynamic_rxd_state dynamic_rxd_state = gl1_hw_get_dynamic_rxd_state(gas_id);
  grm_client_enum_t divrx_client_id = GRM_NO_CLIENT;

  switch (next_state)
  {
  case GL1_RXDIV_L1_IDLE_STATE:
    /* If dynamic RxD configuration is not in progess, exit Rx Diversity.*/
    if ((GL1_DYN_RXD_NULL == dynamic_rxd_state)
        ||(GL1_DYN_RXD_ACTIVE == dynamic_rxd_state)
        ||(GL1_DYN_RXD_TRM_REQUESTED == dynamic_rxd_state))
    {
    /*IMRD is disabled, Release FW/RF.
      IMRD is enabled, release FW/RF on sleep.
      TRM will release later as a part of grm_release*/
    gl1_exit_diversity_mode(FALSE, gas_id);

      gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_NULL, gas_id);

      gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);

      /*Disable Dynamic RxD Handler*/
      gl1_hw_set_dynamic_rxd_tick_required(FALSE, gas_id);
    }
    else
    {
      gl1_hw_set_dynamic_rxd_abort(TRUE, gas_id);
    }

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
    if(gl1_get_MSRD_ctl_nv(gas_id))
    {
       gl1_reset_ps_rxdiv_params(gas_id);
    }
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
    break;

  case GL1_RXDIV_L1_DEDICATED_STATE:
    /* Request or Release diversity chain*/
    gl1_hw_rxdiv_handle_divrx_change_state(GRM_DIVRX_TRM_REQ_DED, gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
    if(gl1_get_MSRD_ctl_nv(gas_id))
    {
       gl1_reset_ps_rxdiv_params(gas_id);
    }
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
    break;

  case GL1_RXDIV_L1_TRANSFER_STATE:
    /* Request or Release diversity chain*/
    gl1_hw_rxdiv_handle_divrx_change_state(GRM_DIVRX_TRM_REQ_DATA, gas_id);
    break;

  default:
    MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid RxD L1 change state from DTM to %d", next_state);
    break;
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_handle_l1_change_state

DESCRIPTION

===========================================================================*/
static void gl1_hw_rxdiv_handle_l1_transfer_state(gl1_hw_rxdiv_l1_state next_state,
                                                  gas_id_t              gas_id)
{
  gl1_hw_rxd_dynamic_rxd_state dynamic_rxd_state = gl1_hw_get_dynamic_rxd_state(gas_id);
  grm_client_enum_t divrx_client_id = GRM_NO_CLIENT;

  switch (next_state)
  {
  case GL1_RXDIV_L1_IDLE_STATE:
    /* If dynamic RxD configuration is not in progess, exit Rx Diversity.*/
    if ((GL1_DYN_RXD_NULL == dynamic_rxd_state)
        ||(GL1_DYN_RXD_ACTIVE == dynamic_rxd_state)
        ||(GL1_DYN_RXD_TRM_REQUESTED == dynamic_rxd_state))
    {
    /*IMRD is disabled, Release FW/RF.
      IMRD is enabled, release FW/RF on sleep.
      TRM will release later as a part of grm_release*/
    gl1_exit_diversity_mode(FALSE, gas_id);

      gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_NULL, gas_id);

      gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);

      /*Disable Dynamic RxD Handler*/
      gl1_hw_set_dynamic_rxd_tick_required(FALSE, gas_id);
 
      /* Release TRM for Transfer to IDLE case before UE goes to sleep while holding TRM chain. */
      divrx_client_id = grm_map_prx_to_divrx_client_id(gl1_get_current_grm_client_id(gas_id), gas_id);
 
      if ((grm_get_trm_status(divrx_client_id, gas_id) == GL1_TRM_GRANTED)
           || (grm_get_trm_status(divrx_client_id, gas_id) == GL1_TRM_CALLBACK_PENDING))
      {
        MSG_GERAN_HIGH_0_G(GL1_RXD"Release TRM lock when transitioning from Transfer to IDLE");
        grm_release_rx_sec(divrx_client_id, gas_id);
      }
      else
      {
        MSG_GERAN_HIGH_1_G(GL1_RXD"TRM status (%d) during transitioning from Transfer to IDLE",
        grm_get_trm_status(divrx_client_id, gas_id));
      }
    }
    else
    {
      gl1_hw_set_dynamic_rxd_abort(TRUE, gas_id);
    }

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
    if(gl1_get_MSRD_ctl_nv(gas_id))
    {
      gl1_reset_ps_rxdiv_params(gas_id);
    }
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
    break;

  case GL1_RXDIV_L1_DTM_STATE:
    /* Request or Release diversity chain*/
    gl1_hw_rxdiv_handle_divrx_change_state(GRM_DIVRX_TRM_REQ_DED, gas_id);
    break;

  default:
    MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid RxD L1 change state from Transfer to %d", next_state);
    break;
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_handle_l1_change_state

DESCRIPTION

===========================================================================*/
static void gl1_hw_rxdiv_handle_divrx_change_state(grm_divrx_request_type divrx_req,
                                                   gas_id_t               gas_id)
{
  boolean rxd_allowed = FALSE;
  gl1_trm_state_t divrx_trm_status = GL1_TRM_NULL;
  grm_client_enum_t prx_client_id = gl1_map_gas_id_to_client_id(gas_id);
  grm_client_enum_t divrx_client_id = grm_map_prx_to_divrx_client_id(prx_client_id,
                                                                     gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_1_G(GL1_RXD"gl1_hw_rxdiv_handle_divrx_change_state divrx_req %d  ",
                          divrx_req);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

  rxd_allowed = gl1_hw_is_rxdiv_allowed(divrx_req, gas_id);
  divrx_trm_status = grm_get_trm_status(divrx_client_id, gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_2_G(GL1_RXD"gl1_hw_rxdiv_handle_divrx_change_state rxd_allowed %d divrx_trm_status %d",
                          rxd_allowed, divrx_trm_status);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

  if (rxd_allowed && (GL1_TRM_NULL == divrx_trm_status))
  {
    (void)gl1_hw_enable_div_chain_rf_fw_config(prx_client_id,
                                               GRM_ACCESS_DURATION_SCLKS,
                                               divrx_req,
                                               gas_id);
  }
  else if ((FALSE == rxd_allowed)
           && (GL1_TRM_GRANTED == divrx_trm_status))
  {
    gl1_hw_disable_div_chain_rf_fw_config(prx_client_id,
                                          TRUE,
					  FALSE,
                                          gas_id);
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_enable_div_chain_rf_fw_config

DESCRIPTION
  - Request TRM for diversity chain
  - Wake up RF
  - Configure FW
===========================================================================*/
static gl1_trm_state_t gl1_hw_enable_div_chain_rf_fw_config(grm_client_enum_t       prx_client_id,
                                                            trm_duration_t          duration,
                                                            grm_divrx_request_type  divrx_req,
                                                            gas_id_t                gas_id)
{
  trm_grant_return_enum_type prx_trm_grant_status = grm_get_trm_grant_status(prx_client_id,
                                                                             gas_id);
  gl1_trm_state_t trm_status = GL1_TRM_NULL;

  gl1_hw_gfw_enter_mode_type gfw_rxd_status = gl1_hw_get_gfw_enter_status(gas_id);

  gl1_hw_rxd_dynamic_rxd_state dynamic_rxd_state = gl1_hw_get_dynamic_rxd_state(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_3_G(GL1_RXD"gl1_hw_enable_div_chain_rf_fw_config  prx_trm_grant_status %d "
                            "gfw_rxd_status %d dynamic rxd state %d",
                     prx_trm_grant_status,
                     gfw_rxd_status,
                     dynamic_rxd_state);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

  /* If GL1 is waiting for GFW DRX exit, then skip enabling of diversity  and
     wait for next occasion to enable voice/data RxD dynamically. Configure diversity chain
     when GFW status is only configured for PRX.*/
  if ((TRM_DENIAL != prx_trm_grant_status)
      && (GL1_GFW_PRX_ENTER == gfw_rxd_status)
      && (GL1_DYN_RXD_NULL == dynamic_rxd_state))
  {

    /* Request DRX chain */
    grm_request_data_t  grm_request_data;
    grm_request_data_t *grm_request_data_p = &grm_request_data;

    MSG_GERAN_MED_0_G(GL1_RXD"Dynamic RxD: Request Chain and enable Dynamic RxD tick");

    gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);

    /*Tick Dynamic Rxd Handler*/
    gl1_hw_set_dynamic_rxd_tick_required(TRUE, gas_id);

    memset( &grm_request_data, NULL, sizeof(grm_request_data_t));

    grm_request_data_p->client_id = prx_client_id;
    grm_request_data_p->resource = TRM_RX_DIVERSITY;
    grm_request_data_p->duration = duration;
    grm_request_data_p->reason = TRM_DIVERSITY;
    grm_request_data_p->sub_reason = SYS_PROC_TYPE_DIVERSITY_CHAIN;
    grm_request_data_p->divrx_req = divrx_req;
    grm_request_data_p->gas_id = gas_id;

    gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_TRM_REQUESTED, gas_id);

    call_in_task_grm_divrx_request(grm_request_data_p);

  }
  else
  {
    MSG_GERAN_MED_3_G(GL1_RXD"Unable to configure diversity chain  prx_trm_grant_status %d "
                             "gfw_rxd_status %d dynamic rxd state %d",
                      prx_trm_grant_status,
                      gfw_rxd_status,
                      dynamic_rxd_state);
  }
  return trm_status;
}
/*===========================================================================

FUNCTION gl1_hw_disable_div_chain_rf_fw_config

DESCRIPTION
  - Release TRM for diversity chain
  - Sleep RF
  - Configure FW
===========================================================================*/
static void gl1_hw_disable_div_chain_rf_fw_config(grm_client_enum_t       prx_client_id,
                                                  boolean                 force_exit,
						  boolean                 delay,
                                                  gas_id_t                gas_id)
{
  gl1_hw_rxd_dynamic_rxd_state dynamic_rxd_state = gl1_hw_get_dynamic_rxd_state(gas_id);
  gl1_rxd_control_type gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_1_G(GL1_RXD"gl1_hw_disable_div_chain_rf_fw_config dynamic rxd state %d",
                     dynamic_rxd_state);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

  if ((GL1_DYN_RXD_NULL == dynamic_rxd_state)
      ||(GL1_DYN_RXD_ACTIVE == dynamic_rxd_state))
  {
    if (ftm_get_mode() == FTM_MODE && gl1_rxd_ctl_flag.RxdForcedOn)
    {
      MSG_GERAN_MED_0_G(GL1_RXD"Dynamic RxD: In FTM Mode Dynamic RxD not force exit state.");
      /* Set both l1 diversity and geran l1 diversity flags to TRUE */
      l1_ded_set_l1_diversity_status(TRUE, gas_id);
      gl1_set_enable_rxdiversity(TRUE, gas_id);
      return;
    }
    MSG_GERAN_MED_0_G(GL1_RXD"Dynamic RxD: Release Chain and enable Dynamic RxD tick");

    /* Set both l1 diversity and geran l1 diversity flags to FALSE */
    l1_ded_set_l1_diversity_status(FALSE, gas_id);
    gl1_set_enable_rxdiversity(FALSE, gas_id);

	/*For the case of immediate release by RxD unlock cb,  
	  delay RxD exit by one frame to avoid race condition with RxD enable burst */
	if(force_exit && delay)
	{
	  mdsp_wait_for_frame_tick_non_blocking(l1_task_tcb_read(gas_id),L1_TIMEOUT_HW_SIG, gas_id);
	}
    /*Set force exit flag after the one frame wait so RxD exit cmd (which is sent in ISR) is sent after the wait*/
    gl1_hw_set_force_divrx_exit(force_exit, gas_id);
    gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_ACTIVE, gas_id);
    /*Tick Dynamic Rxd Handler*/
    gl1_hw_set_dynamic_rxd_tick_required(TRUE, gas_id);
  }
  else
  {
    gl1_hw_set_dynamic_rxd_abort(TRUE, gas_id);
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_dtm_process_rxd_cfg

DESCRIPTION
  - Process RxD Configuration.
===========================================================================*/
boolean gl1_hw_dtm_process_rxd_cfg(const mdsp_dtm_rx_params_type  *rx_params_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
                                   boolean                        imrd_channel,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
                                   gas_id_t                       gas_id)
{
  boolean               enable_rxd = FALSE;
  boolean               gl1_in_ftm_mode = (ftm_get_mode() == FTM_MODE)? TRUE:FALSE;

  /*If DRx enter mode command is sent in FN N, in the burst Rx command to FW and RF, Rx
    * diversity control flags are appropriately enabled only after FN N+3 during the block start.
    * This applies only for normal mode.
    * In FTM mode, we don't wait to enable Rx diversity in burt RX command to FW and RF after
    * sending the DRX enter mode command.
     */
  if (gl1_in_ftm_mode)
  {
    enable_rxd = TRUE;
  }
  else if (gl1_get_rxd_ctl_nv(gas_id))
  {
    /*For Idle RxD: Idle Channel, NV Item and FW status
      should be PRX+DIVRX*/
    if (imrd_channel
        && gl1_get_IMRD_ctl_nv(gas_id)
        && (GL1_GFW_PRX_DIVRX_ENTER == gl1_hw_get_gfw_enter_status(gas_id))
        )
    {
      enable_rxd = TRUE;
    }
    else if (
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
             ((rx_params_ptr->dedicated) || gl1_get_MSRD_ctl_nv(gas_id))
#else
             (rx_params_ptr->dedicated)
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
          )
    {
      /* Dynamic Dedicated and Data RxD:
         Start of Block, Dedicated Rx and NV item*/
      enable_rxd = TRUE;
    }
  }

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
/* For Data RxD, we need to know if AEQ for 8PSK will be enabled in the next block, so the dynamic AEQ setting
   must be moved from the MDSP driver to here, as RxD needs to know if AEQ is on (and hence RxD is possible) before
   calling RF to build the Rx burst scripts */
   /* Dynamically control Data RxD and ensure that switching can only occur at a block boundary */
  if(gl1_get_MSRD_ctl_nv(gas_id))
  {
    if(rx_params_ptr->slot_params[0].burst_num == 0)
    {
      gl1_set_enable_rxdiversity_data(gl1_get_enable_rxdiversity_data_dynamic(gas_id), gas_id);
    }
  }
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  MSG_GERAN_HIGH_4_G(GL1_RXD"gl1_hw_dtm_process_rxd_cfg  enable_rxd %d imrd_channel %d "
                            "dedicated %d fw %d",
                     enable_rxd,
                     imrd_channel,
                     rx_params_ptr->dedicated,
                     gl1_hw_get_gfw_enter_status(gas_id));
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
  return enable_rxd;
}

/*===========================================================================

FUNCTION gl1_hw_dtm_process_slot_rxd_cfg

DESCRIPTION
  - Process RxD on each slot.
===========================================================================*/
boolean gl1_hw_dtm_process_slot_rxd_cfg(mdsp_dtm_rx_params_type *rx_params_ptr,
                                        sys_band_T              band,
                                        boolean                 is_block_start,
                                        gas_id_t                gas_id)
{
  uint8 tn = 0;
  gl1_rxd_control_type gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
  boolean enable_rxd = FALSE;
  boolean    gl1_in_ftm_mode = (ftm_get_mode() == FTM_MODE)? TRUE:FALSE;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t divrx_client_id = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id,
                                                                     gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

  gl1_hw_dtm_validate_cfg_cs_ps_slot_rxd(rx_params_ptr,
                                         is_block_start,
                                         gl1_rxd_ctl_flag,
                                         gas_id);

  for (tn = 0; tn < rx_params_ptr->num_slots; tn++)
  {
    rx_params_ptr->slot_params[tn].rxdiv_control.enableRxd   = FALSE;
    rx_params_ptr->slot_params[tn].rxdiv_control.enableDrxOnly = FALSE;
    rx_params_ptr->slot_params[tn].rxdiv_control.enablePrxOnly = FALSE;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
    MSG_GERAN_HIGH_4_G(GL1_RXD"tn:%d,channel_type:%d,saic:%d enable_rx_diversity %d",
                       tn,
                       rx_params_ptr->slot_params[tn].channel_type,
                       rx_params_ptr->saic,
                       enable_rxd);
    MSG_GERAN_HIGH_4_G(GL1_RXD"gfw_enter %d rxd_channel %d band_supported %d "
                       "trm_grant %d",
                       gl1_hw_get_gfw_enter_status(gas_id),
                       rx_params_ptr->slot_params[tn].rxd_channel_type,
                       gl1_hw_gsm_band_supported(band,gas_id),
                       grm_get_trm_status(divrx_client_id, gas_id));
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG
    MSG_GERAN_HIGH_3_G(GL1_RXD"enableRxd %d enableMSRD %d enable_rxdiversity_data %d",
                       gl1_rxd_ctl_flag.enableRxd,
                       gl1_rxd_ctl_flag.enableMSRD,
                       gl1_get_enable_rxdiversity_data(gas_id));
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG*/
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/


    if(gl1_in_ftm_mode)
    {
      enable_rxd |= gl1_hw_dtm_process_ftm_slot_rxd_cfg(rx_params_ptr,
                                                        band,
                                                        tn,
                                                        gl1_rxd_ctl_flag,
                                                        gas_id);

    }
    else
    {
      switch (rx_params_ptr->slot_params[tn].rxd_channel_type)
      {
        case GL1_RXD_CS_CHANNEL:
        {
          enable_rxd |= gl1_hw_dtm_process_cs_slot_rxd_cfg(rx_params_ptr,
                                                           band,
                                                           tn,
                                                           gl1_rxd_ctl_flag,
                                                           gas_id);
        }
        break;

        case GL1_RXD_IDLE_CHANNEL:
        {
          enable_rxd |= gl1_hw_dtm_process_idle_slot_rxd_cfg(rx_params_ptr,
                                                             band,
                                                             tn,
                                                             gl1_rxd_ctl_flag,
                                                             gas_id);
        }
        break;

        case GL1_RXD_PS_CHANNEL:
        {
          enable_rxd |= gl1_hw_dtm_process_ps_slot_rxd_cfg(rx_params_ptr,
                                                           band,
                                                           tn,
                                                           gl1_rxd_ctl_flag,
                                                           gas_id);
        }
        break;

        default:
          MSG_GERAN_ERROR_2_G(GL1_RXD"gl1_hw_dtm_process_slot_rxd_cfg Invalid Ch Type %d tn %d",
                              tn,
                              rx_params_ptr->slot_params[tn].rxd_channel_type);
        break;
      }
    }
  }
  return enable_rxd;
}
/*===========================================================================

FUNCTION gl1_hw_dtm_process_ftm_slot_rxd_cfg

DESCRIPTION
  - Enable RxD in FTM
===========================================================================*/
boolean gl1_hw_dtm_process_ftm_slot_rxd_cfg(mdsp_dtm_rx_params_type *rx_params_ptr,
                                            sys_band_T              band,
                                            uint8                   tn,
                                            gl1_rxd_control_type    rxd_ctl_flag,
                                            gas_id_t                gas_id)
{
  boolean enable_rxd = FALSE;
  volatile ISRTIM_CMD_BLK*  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t         divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
  grm_info_t*               grm_info_p = grm_get_info_ptr_from_client(divrx_client, gas_id);

  if( NULL == grm_info_p )
  {
    MSG_GERAN_ERROR_0_G(GL1_RXD"NULL grm_info_p " );

    return  FALSE;
  }

  if((rxd_ctl_flag.enableRxd)
     && (rxd_ctl_flag.enableDrx)
     && ((rx_params_ptr->dedicated) || gl1_get_MSRD_ctl_nv(gas_id))
     && (gl1_hw_gsm_band_supported(band, gas_id))
     && (GL1_GFW_PRX_DIVRX_ENTER == gl1_hw_get_gfw_enter_status(gas_id))
     && ( FALSE == gl1_hw_get_dynamic_rxd_abort( gas_id ) )
     && (!( ( grm_info_p->grm_unlock_cb_data.event == TRM_UNLOCK_BY ) && (grm_get_frame_duration(divrx_client, gas_id) <= (GL1_UNLOCK_RXDIV_DELAY + 2)) ))
     && ( FALSE == gl1_hw_get_force_divrx_exit( gas_id ) )
     )
  {
    if (((rx_params_ptr->slot_params[tn].rxd_channel_type == GL1_RXD_IDLE_CHANNEL)&&(mdsp_idle_use_aeq(TRUE, rx_params_ptr->saic, gas_id))) ||
        ((rx_params_ptr->slot_params[tn].rxd_channel_type == GL1_RXD_CS_CHANNEL)&& (mdsp_dedicated_use_aeq(rx_params_ptr->dedicated,rx_params_ptr->saic,gas_id))) ||
        ((rx_params_ptr->slot_params[tn].rxd_channel_type == GL1_RXD_PS_CHANNEL) && (mdsp_dtm_use_aeq_8psk(MDSP_PDTCHD, rx_params_ptr->saic, gas_id))))
    {
    rx_params_ptr->slot_params[tn].rxdiv_control.enableRxd     = rxd_ctl_flag.enableRxd;
    rx_params_ptr->slot_params[tn].rxdiv_control.enableDrxOnly = rxd_ctl_flag.enableDrx;
    rx_params_ptr->slot_params[tn].rxdiv_control.enablePrxOnly = rxd_ctl_flag.enablePrx;

    enable_rxd = TRUE;

    gl1_start_rxdiv_agc(TRUE,gas_id);

    MSG_GERAN_HIGH_3_G(GL1_RXD"Enabling Rx Diversity in Fn:%d,tn:%d,saic:%d",
                       GSTMR_GET_FN_GERAN(gas_id),
                       tn,
                       rx_params_ptr->saic);
    }
  }
  else
  {
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG) || defined (FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG)
    MSG_GERAN_HIGH_5_G(GL1_RXD"entermodesent=%d, sleepwakeup = %d gfwstatus = %d "
                              "rxd_channel_type=%d band supported=%d",
                       gl1_hw_get_rf_enter_status(gas_id),
                       gl1_hw_get_rf_sleep_wakeup_mode(gas_id),
                       gl1_hw_get_gfw_enter_status(gas_id),
                       rx_params_ptr->slot_params[tn].rxd_channel_type,
                       gl1_hw_gsm_band_supported(band,gas_id));
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG || FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
  }
  return enable_rxd;
}
/*===========================================================================

FUNCTION gl1_hw_dtm_process_cs_slot_rxd_cfg

DESCRIPTION
  - Enable RxD on CS slot.
===========================================================================*/
boolean gl1_hw_dtm_process_cs_slot_rxd_cfg(mdsp_dtm_rx_params_type  *rx_params_ptr,
                                           sys_band_T               band,
                                           uint8                    tn,
                                           gl1_rxd_control_type     rxd_ctl_flag,
                                           gas_id_t                 gas_id)
{
  boolean enable_rxd = FALSE;

  volatile ISRTIM_CMD_BLK*  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t         divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
  grm_info_t*               grm_info_p = grm_get_info_ptr_from_client(divrx_client, gas_id);

  if( NULL == grm_info_p )
  {
    MSG_GERAN_ERROR_0_G(GL1_RXD"NULL grm_info_p " );

    return  FALSE;
  }

  if ((rxd_ctl_flag.enableRxd)
      && (rxd_ctl_flag.enableDrx)
      && (rx_params_ptr->dedicated)
      && (gl1_hw_gsm_band_supported(band,gas_id))
      && (GL1_GFW_PRX_DIVRX_ENTER == gl1_hw_get_gfw_enter_status(gas_id))
      && ( FALSE == gl1_hw_get_dynamic_rxd_abort( gas_id ) )
      && (!( ( grm_info_p->grm_unlock_cb_data.event == TRM_UNLOCK_BY ) && (grm_get_frame_duration(divrx_client, gas_id) <= (GL1_UNLOCK_RXDIV_DELAY + 2)) ))
      && ( FALSE == gl1_hw_get_force_divrx_exit( gas_id ) )
      )
  {
    if(!rxd_ctl_flag.enablePrx)
    {
      rx_params_ptr->slot_params[tn].rxdiv_control.enableRxd   = rxd_ctl_flag.enableRxd;
      rx_params_ptr->slot_params[tn].rxdiv_control.enableDrxOnly = rxd_ctl_flag.enableDrx;
      rx_params_ptr->slot_params[tn].rxdiv_control.enablePrxOnly = rxd_ctl_flag.enablePrx;

      gl1_start_rxdiv_agc(TRUE,gas_id);

      MSG_GERAN_LOW_3_G(GL1_RXD"Enabling Rx Diversity in Fn:%d,tn:%d,saic:%d",
                         GSTMR_GET_FN_GERAN(gas_id),
                         tn,
                         rx_params_ptr->saic);

      enable_rxd = TRUE;
    }
#ifdef FEATURE_THOR_MODEM
    else if((FALSE == gl1_hw_get_rxdiv_wait_for_block_start(gas_id))
            && (rxd_ctl_flag.enablePrx)
            && (gl1_get_enable_rxdiversity(gas_id)))
#else
    else if((rxd_ctl_flag.enablePrx)
            && (gl1_get_enable_rxdiversity(gas_id)))
#endif
    {
      if(mdsp_dedicated_use_aeq(rx_params_ptr->dedicated,
                                rx_params_ptr->saic,
                                gas_id))
      {
        rx_params_ptr->slot_params[tn].rxdiv_control.enableRxd     = rxd_ctl_flag.enableRxd;
        rx_params_ptr->slot_params[tn].rxdiv_control.enableDrxOnly = rxd_ctl_flag.enableDrx;
        rx_params_ptr->slot_params[tn].rxdiv_control.enablePrxOnly = rxd_ctl_flag.enablePrx;

        gl1_start_rxdiv_agc(TRUE,gas_id);

        MSG_GERAN_LOW_3_G(GL1_RXD"Enabling Rx Diversity in Fn:%d,tn:%d,saic:%d",
                           GSTMR_GET_FN_GERAN(gas_id),
                           tn,
                           rx_params_ptr->saic);

        enable_rxd = TRUE;
      }
      else
      {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
        MSG_GERAN_HIGH_3_G(GL1_RXD"Voice RxD is NOT enabled because AEQ is off: "
                                  "saic=%i, rxdchannel_type=%i dedicated %d",
                           rx_params_ptr->saic,
                           rx_params_ptr->slot_params[tn].rxd_channel_type,
                           rx_params_ptr->dedicated);
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG*/
      }
    }
    else
    {
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG) || defined (FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG)
      MSG_GERAN_HIGH_3_G(GL1_RXD"Voice RxD is NOT enabled "
                                "enable_rxdiversity=%d, wait_for_blk = %d FN %d",
                       gl1_get_enable_rxdiversity(gas_id),
                       gl1_hw_get_rxdiv_wait_for_block_start(gas_id),
                       GSTMR_GET_FN_GERAN(gas_id));
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG || FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
    }
  }
  else
  {
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG) || defined (FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG)
    MSG_GERAN_HIGH_5_G(GL1_RXD"Voice RxD is NOT enabled "
                              "entermodesent=%d, sleepwakeup = %d gfwstatus = %d "
                              "rxd_channel_type=%d band supported=%d",
                       gl1_hw_get_rf_enter_status(gas_id),
                       gl1_hw_get_rf_sleep_wakeup_mode(gas_id),
                       gl1_hw_get_gfw_enter_status(gas_id),
                       rx_params_ptr->slot_params[tn].rxd_channel_type,
                       gl1_hw_gsm_band_supported(band,gas_id));
    MSG_GERAN_HIGH_3_G(GL1_RXD"enable_rxdiversity=%d, wait_for_blk = %d FN %d",
                       gl1_get_enable_rxdiversity(gas_id),
                       gl1_hw_get_rxdiv_wait_for_block_start(gas_id),
                       GSTMR_GET_FN_GERAN(gas_id));
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG || FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
  }
  return enable_rxd;
}
/*===========================================================================

FUNCTION gl1_hw_dtm_process_idle_slot_rxd_cfg

DESCRIPTION
  - Enable RxD on Idle channel
===========================================================================*/
boolean gl1_hw_dtm_process_idle_slot_rxd_cfg(mdsp_dtm_rx_params_type  *rx_params_ptr,
                                             sys_band_T               band,
                                             uint8                    tn,
                                             gl1_rxd_control_type     rxd_ctl_flag,
                                             gas_id_t                 gas_id)
{
  boolean enable_rxd = FALSE;

  volatile ISRTIM_CMD_BLK*  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t         divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
  grm_info_t*               grm_info_p = grm_get_info_ptr_from_client(divrx_client, gas_id);

  if( NULL == grm_info_p )
  {
    MSG_GERAN_ERROR_0_G(GL1_RXD"NULL grm_info_p " );

    return  FALSE;
  }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  if ((rxd_ctl_flag.enableRxd)
      && (rxd_ctl_flag.enableIMRD)
      && (gl1_hw_gsm_band_supported(band,gas_id))
      && (gl1_hw_get_rf_sleep_wakeup_mode(gas_id) == GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE)
      && ( FALSE == gl1_hw_get_dynamic_rxd_abort( gas_id ) )
      && (!( ( grm_info_p->grm_unlock_cb_data.event == TRM_UNLOCK_BY ) && (grm_get_frame_duration(divrx_client, gas_id) <= (GL1_UNLOCK_RXDIV_DELAY + 2)) ))
      && ( FALSE == gl1_hw_get_force_divrx_exit( gas_id ) )
      )
  {
    if(mdsp_idle_use_aeq(TRUE, rx_params_ptr->saic, gas_id))
    {
      rx_params_ptr->slot_params[tn].rxdiv_control.enableRxd     = rxd_ctl_flag.enableRxd;
      rx_params_ptr->slot_params[tn].rxdiv_control.enableDrxOnly = rxd_ctl_flag.enableDrx;
      rx_params_ptr->slot_params[tn].rxdiv_control.enablePrxOnly = rxd_ctl_flag.enablePrx;

      gl1_start_rxdiv_agc(TRUE,gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
      MSG_GERAN_HIGH_4_G(GL1_RXD"AGC_LOOP : Enabling IDLE Rx Diversity in Fn:%d,tn:%d, "
                                "saic:%d (AEQ ON), rxd_channel_type=%i",
                         GSTMR_GET_FN_GERAN(gas_id),
                         tn,
                         rx_params_ptr->saic,
                         rx_params_ptr->slot_params[tn].rxd_channel_type);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
      enable_rxd = TRUE;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
      MSG_GERAN_HIGH_4_G(GL1_RXD"AGC_LOOP : Enabling IDLE Rx Diversity in Fn:%d,tn:%d, "
                                "saic:%d (AEQ ON), rxd_channel_type=%i",
                         GSTMR_GET_FN_GERAN(gas_id),
                         tn,
                         rx_params_ptr->saic,
                         rx_params_ptr->slot_params[tn].rxd_channel_type);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
    }
    else
    {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
      MSG_GERAN_HIGH_3_G(GL1_RXD"AGC_LOOP: IDLE RXD is NOT enabled because AEQ is off: "
                                "saic=%i, idle=%i, rxd_channel_type=%i",
                         rx_params_ptr->saic,
                         rx_params_ptr->idle,
                         rx_params_ptr->slot_params[tn].rxd_channel_type);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
    }
  }
  else
  {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
    MSG_GERAN_HIGH_5_G(GL1_RXD"entermodesent=%d, sleepwakeup = %d gfwstatus = %d "
                              "rxd_channel_type=%d band supported=%d",
                              gl1_hw_get_rf_enter_status(gas_id),
                              gl1_hw_get_rf_sleep_wakeup_mode(gas_id),
                              gl1_hw_get_gfw_enter_status(gas_id),
                              rx_params_ptr->slot_params[tn].rxd_channel_type,
                              gl1_hw_gsm_band_supported(band,gas_id));
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
  }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
  return enable_rxd;
}
/*===========================================================================

FUNCTION gl1_hw_dtm_process_ps_slot_rxd_cfg

DESCRIPTION
  - Enable RxD on PS slot.
===========================================================================*/
boolean gl1_hw_dtm_process_ps_slot_rxd_cfg(mdsp_dtm_rx_params_type  *rx_params_ptr,
                                           sys_band_T               band,
                                           uint8                    tn,
                                           gl1_rxd_control_type     rxd_ctl_flag,
                                           gas_id_t                 gas_id)
{
  boolean enable_rxd = FALSE;

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  volatile ISRTIM_CMD_BLK*  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t         divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
  grm_info_t*               grm_info_p = grm_get_info_ptr_from_client(divrx_client, gas_id);

  if( NULL == grm_info_p )
  {
    MSG_GERAN_ERROR_0_G(GL1_RXD"NULL grm_info_p " );

    return  FALSE;
  }

#ifdef FEATURE_THOR_MODEM
  if ((FALSE == gl1_hw_get_rxdiv_wait_for_block_start(gas_id))
      && (rxd_ctl_flag.enableRxd)
      && (rxd_ctl_flag.enableMSRD)
      && (gl1_hw_gsm_band_supported(band,gas_id))
      && (gl1_get_enable_rxdiversity_data(gas_id))
      && (GL1_GFW_PRX_DIVRX_ENTER == gl1_hw_get_gfw_enter_status(gas_id))
      && (!gl1_get_disable_data_diversity(gas_id))
      && ( FALSE == gl1_hw_get_dynamic_rxd_abort( gas_id ) )
      && (!( ( grm_info_p->grm_unlock_cb_data.event == TRM_UNLOCK_BY ) && (grm_get_frame_duration(divrx_client, gas_id) <= (GL1_UNLOCK_RXDIV_DELAY + 2)) ))
      && ( FALSE == gl1_hw_get_force_divrx_exit( gas_id ) )
      )
#else
  if ((rxd_ctl_flag.enableRxd)
      && (rxd_ctl_flag.enableMSRD)
      && (gl1_hw_gsm_band_supported(band,gas_id))
      && (gl1_get_enable_rxdiversity_data(gas_id))
      && (GL1_GFW_PRX_DIVRX_ENTER == gl1_hw_get_gfw_enter_status(gas_id))
      && (!gl1_get_disable_data_diversity(gas_id))
      && (!( ( grm_info_p->grm_unlock_cb_data.event == TRM_UNLOCK_BY ) && (  grm_get_frame_duration(divrx_client, gas_id) <= (GL1_UNLOCK_RXDIV_DELAY + 2)) ))
      && ( FALSE == gl1_hw_get_force_divrx_exit( gas_id ) )
     )
#endif
  {
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG
    MSG_GERAN_HIGH_2_G(GL1_RXD"DATA RXD enable_rx_diversity %d "
                              "saic=%i",
                       enable_rxd,
                       rx_params_ptr->saic);
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG*/

    if(mdsp_dtm_use_aeq_8psk(MDSP_PDTCHD, rx_params_ptr->saic, gas_id))
    {
      rx_params_ptr->slot_params[tn].rxdiv_control.enableRxd     = rxd_ctl_flag.enableRxd;
      rx_params_ptr->slot_params[tn].rxdiv_control.enableDrxOnly = rxd_ctl_flag.enableDrx;
      rx_params_ptr->slot_params[tn].rxdiv_control.enablePrxOnly = rxd_ctl_flag.enablePrx;

      gl1_start_rxdiv_agc(TRUE,gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG
      MSG_GERAN_HIGH_4_G(GL1_RXD"Enabling DATA Rx Diversity in Fn:%d,tn:%d, "
                                "saic:%d (AEQ ON), rxd_channel_type=%i",
                         GSTMR_GET_FN_GERAN(gas_id),
                         tn,
                         rx_params_ptr->saic,
                         rx_params_ptr->slot_params[tn].rxd_channel_type);
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG*/
      enable_rxd = TRUE;
    }
    else
    {
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG
      MSG_GERAN_HIGH_2_G(GL1_RXD"DATA RXD is NOT enabled because AEQ is off: "
                                "saic=%i, rxdchannel_type=%i",
                         rx_params_ptr->saic,
                         rx_params_ptr->slot_params[tn].rxd_channel_type);
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG*/
    }
  }
  else
  {
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG) || defined (FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG)
    MSG_GERAN_HIGH_5_G(GL1_RXD"DATA RXD is NOT enabled "
                              "entermodesent=%d, sleepwakeup = %d gfwstatus = %d "
                              "rxd_channel_type=%d band supported=%d",
                       gl1_hw_get_rf_enter_status(gas_id),
                       gl1_hw_get_rf_sleep_wakeup_mode(gas_id),
                       gl1_hw_get_gfw_enter_status(gas_id),
                       rx_params_ptr->slot_params[tn].rxd_channel_type,
                       gl1_hw_gsm_band_supported(band,gas_id));
    MSG_GERAN_HIGH_3_G(GL1_RXD"enable_rxdiversity_data=%d, wait_for_blk = %d FN %d",
                       gl1_get_enable_rxdiversity_data(gas_id),
                       gl1_hw_get_rxdiv_wait_for_block_start(gas_id),
                       GSTMR_GET_FN_GERAN(gas_id));
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG || FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
  }
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/

  return enable_rxd;
}
/*===========================================================================

FUNCTION gl1_hw_dtm_validate_cfg_cs_ps_slot_rxd

DESCRIPTION
  - Request for diversity chain and enable RxD at RF and FW level if AEQ enables RxD.
  - If AEQ disaables RxD, keep chain and RxD configuration (Rf and FW) for 104 frames.
  - If AEQ disables RxD and diversity chain is not granted, wait till AEQ decides to
    enable RxD.

===========================================================================*/
void gl1_hw_dtm_validate_cfg_cs_ps_slot_rxd(const mdsp_dtm_rx_params_type *rx_params_ptr,
                                            boolean                       is_block_start,
                                            gl1_rxd_control_type          rxd_ctl_flag,
                                            gas_id_t                      gas_id)
{
  uint8 tn = 0;
  boolean cs_chan = FALSE, ps_chan = FALSE;
  boolean algo_rxd_status = FALSE;
  boolean rxd_allowed = FALSE;
  gl1_trm_state_t divrx_trm_status = GL1_TRM_NULL;
  grm_divrx_request_type divrx_req = GRM_DIVRX_TRM_REQ_INVALID;

  boolean gfw_rxd_status = FALSE;
  boolean    gl1_in_ftm_mode = (ftm_get_mode() == FTM_MODE)? TRUE:FALSE;

  grm_client_enum_t prx_client_id = GRM_NO_CLIENT; 
  grm_client_enum_t divrx_client_id = GRM_NO_CLIENT;

  if (rxd_ctl_flag.enableRxd)
  {
	      /* FTM mode, disable dynamic rxd, enable RxD on block boundary.*/
    if (gl1_in_ftm_mode) 
    {
      if (gl1_hw_get_rxdiv_wait_for_block_start(gas_id))
      {
        if (is_block_start) 
        {
          gl1_hw_set_rxdiv_wait_for_block_start(FALSE, gas_id);
        }
      }
    }
    
	else
	{

    gfw_rxd_status = (gl1_hw_get_gfw_enter_status(gas_id) == GL1_GFW_PRX_DIVRX_ENTER)? TRUE:FALSE;
    prx_client_id = gl1_map_gas_id_to_client_id(gas_id);
    divrx_client_id = grm_map_prx_to_divrx_client_id(prx_client_id, gas_id);

    for (tn = 0; tn < rx_params_ptr->num_slots; tn++)
    {
      if ((FALSE == cs_chan)
          && (GL1_RXD_CS_CHANNEL == rx_params_ptr->slot_params[tn].rxd_channel_type))
      {
        cs_chan = TRUE;

        if (rx_params_ptr->slot_params[tn].channel_type == MDSP_SACCH)
        {
           is_block_start = FALSE;
        }
        /* First check algorithm + NV output : diversity allowed or not*/
        if ((FALSE == algo_rxd_status)
            && (rxd_ctl_flag.enableDrx)
            && (rx_params_ptr->dedicated)
            )
        {
          if(!rxd_ctl_flag.enablePrx)
          {
            algo_rxd_status = TRUE;
          }
          else if((rxd_ctl_flag.enablePrx)
                  && (gl1_get_enable_rxdiversity(gas_id)))
          {
            if(mdsp_dedicated_use_aeq(rx_params_ptr->dedicated,
                                  rx_params_ptr->saic,
                                  gas_id))
            {
              algo_rxd_status = TRUE;
            }
          }
        }
      }
      else if ((FALSE == ps_chan)
               && (GL1_RXD_PS_CHANNEL == rx_params_ptr->slot_params[tn].rxd_channel_type))
      {
        ps_chan = TRUE;
        /* First check algorithm + NV output : diversity allowed or not*/
        if ((FALSE == algo_rxd_status)
            && (rxd_ctl_flag.enableMSRD)
            && (gl1_get_enable_rxdiversity_data(gas_id))
            )
        {
          if(mdsp_dtm_use_aeq_8psk(MDSP_PDTCHD, rx_params_ptr->saic, gas_id))
          {
            algo_rxd_status = TRUE;
          }
        }
      }

      /* Only one burst of CS and PS is required */
      if (cs_chan && ps_chan)
      {
        break;
      }
    }

    if ((FALSE == algo_rxd_status) && (ftm_get_mode() == FTM_MODE) && rxd_ctl_flag.RxdForcedOn)
    {
      algo_rxd_status = TRUE;

      MSG_GERAN_HIGH_5_G(GL1_RXD"gl1_hw_dtm_validate_cfg_cs_ps_slot_rxd cs_chan=%d, ps_chan = %d "
                              "algo_rxd_status = %d is_block_start=%d wait_for_blk=%d",
                       cs_chan, ps_chan, algo_rxd_status, is_block_start,
                       gl1_hw_get_rxdiv_wait_for_block_start(gas_id));
      MSG_GERAN_HIGH_5_G(GL1_RXD"gl1_hw_dtm_validate_cfg_cs_ps_slot_rxd gl1_dynamic_rxd_enabled_frames=%d "
                              "FN %d TRM status %d gfw_rxd_status %d gl1_dynamic_rxd_trm_denial_count %d",
                       gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_enabled_frames,
                       GSTMR_GET_FN_GERAN(gas_id),
                       grm_get_trm_status(divrx_client_id, gas_id),
                       gfw_rxd_status,
                       gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count);
    }
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG) || defined (FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG)
    MSG_GERAN_HIGH_5_G(GL1_RXD"gl1_hw_dtm_validate_cfg_cs_ps_slot_rxd cs_chan=%d, ps_chan = %d "
                              "algo_rxd_status = %d is_block_start=%d wait_for_blk=%d",
                       cs_chan, ps_chan, algo_rxd_status, is_block_start,
                       gl1_hw_get_rxdiv_wait_for_block_start(gas_id));
    MSG_GERAN_HIGH_5_G(GL1_RXD"gl1_hw_dtm_validate_cfg_cs_ps_slot_rxd gl1_dynamic_rxd_enabled_frames=%d "
                              "FN %d TRM status %d gfw_rxd_status %d gl1_dynamic_rxd_trm_denial_count %d",
                       gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_enabled_frames,
                       GSTMR_GET_FN_GERAN(gas_id),
                       grm_get_trm_status(divrx_client_id, gas_id),
                       gfw_rxd_status,
                       gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count);
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG || FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/

    if (cs_chan || ps_chan)
    {
      /*CS Channel and GL1 has tried once. Don't enable RxD*/
      if (cs_chan
          && (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count
              >= GL1_MAX_CS_RXD_TRM_DENIAL_COUNT))
      {
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG) || defined (FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG)
        MSG_GERAN_HIGH_0_G(GL1_RXD"TRM denial on CS channel");
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG || FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
      }
      else if (ps_chan
               && (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count != 0))
      {
        /*PS Channel and GL1 has tried once. Got denial, wait for 26 frames*/
        if (rxd_ctl_flag.enableMSRD)
        {
          if (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count
              < GL1_MAX_FRAMES_ENABLE_PS_RXD)
          {
            gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count++;
          }
          else
          {
            gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count = 0;
          }
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG) || defined (FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG)
          MSG_GERAN_HIGH_1_G(GL1_RXD"TRM denial on PS channel gl1_dynamic_rxd_trm_denial_count %d",
                             gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count);
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA_DEBUG || FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
        }
      }
      else
      {
        /* CS slot or PS slot. */
        /* Check start of block*/
        if (gl1_hw_get_rxdiv_wait_for_block_start(gas_id))
        {
          if (is_block_start)
          {
            gl1_hw_set_rxdiv_wait_for_block_start(FALSE, gas_id);
          }
        }

        if ((TRUE == algo_rxd_status)
            && (TRUE == gfw_rxd_status)
            && (FALSE == gl1_hw_get_rxdiv_wait_for_block_start(gas_id)))
        {
          /*Case 1: algo_rxd_status = T gfw_rxd_status = T
            Keep diversity on and track number of consecutive frames for which
            diversity is on.*/
          if (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_enabled_frames
              < GL1_MAX_DYNAMIC_RXD_ON_FRAMES)
          {
            (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_enabled_frames)++;
          }
        }
        else if ((TRUE == algo_rxd_status)
                 && (FALSE == gfw_rxd_status)
                 && (FALSE == l1_sc_wcdma_srch_active(gas_id)))
        {
          /*Case 2: algo_rxd_status : T gfw_rxd_status = F
            Enable RxD if IRAT is not in progress. Reset frame counter. */
          gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_enabled_frames = 0;

          /*Check RxD is allowed or not.*/
          divrx_trm_status = grm_get_trm_status(divrx_client_id, gas_id);

          if (GL1_TRM_NULL == divrx_trm_status)
          {
            /* Voice or DTM, make request for dedicated.*/
            if (cs_chan)
            {
              divrx_req = GRM_DIVRX_TRM_REQ_DED;
            }
            else if (ps_chan)
            {
              divrx_req = GRM_DIVRX_TRM_REQ_DATA;
            }

            rxd_allowed = gl1_hw_is_rxdiv_allowed(divrx_req, gas_id);
            if (rxd_allowed)
            {
              divrx_trm_status = gl1_hw_enable_div_chain_rf_fw_config(prx_client_id,
                                                                    GRM_ACCESS_DURATION_SCLKS,
                                                                    divrx_req,
                                                                    gas_id);

            if (GL1_TRM_NOT_GRANTED == divrx_trm_status)
            {
              if (cs_chan)
              {
                /*CS channel TRM not granted. Max retry is 1*/
                if (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count
                      < GL1_MAX_CS_RXD_TRM_DENIAL_COUNT)
                {
                    (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count)++;
                }
              }
              else
              {
                /*PS Channel. TRM not granted */
                  if (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count
                      < GL1_MAX_FRAMES_ENABLE_PS_RXD)
                  {
                    (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count)++;
                  }
              }
            }
            else if (GL1_TRM_NULL == divrx_trm_status)
            {
              /*Do nothing. Gl1 has triggered Drx FW exit.*/
            }
            else
            {
                gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_trm_denial_count = 0;
            }
          }
        }
        else if ((GL1_TRM_GRANTED == divrx_trm_status) && (gl1_hw_get_dynamic_rxd_state(gas_id) != GL1_DYN_RXD_FW_EXIT))
        {
          /*TRM is arleady with Diversity client... just need to enable FW*/
          gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_TRM_REQUESTED, gas_id);
          gl1_hw_set_dynamic_rxd_tick_required(TRUE, gas_id);
        }
          
      }
      else if ((FALSE == algo_rxd_status)
                 && (TRUE == gfw_rxd_status)
                 && (FALSE == gl1_hw_get_rxdiv_wait_for_block_start(gas_id)))
      {
          /*Case 3: algo_rxd_status : F gfw_rxd_status = T */
          /* Keep diversity on for GL1_MAX_DYNAMIC_RXD_ON_FRAMES*/
          if (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_enabled_frames
              >= GL1_MAX_DYNAMIC_RXD_ON_FRAMES)
          {
            if (is_block_start)
            {
              gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_enabled_frames = 0;
              /*Disable RxD*/
              divrx_trm_status = grm_get_trm_status(divrx_client_id, gas_id);
              if(GL1_TRM_GRANTED == divrx_trm_status)
              {
                gl1_hw_disable_div_chain_rf_fw_config(prx_client_id,
                                                      TRUE,
						      FALSE,
                                                      gas_id);
              }
            }
          }
          else
          {
            (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_enabled_frames)++;
          }
        }
        else
        {
          /*Case 4: algo_rxd_status : F gfw_rxd_status = F */
          /*Do nothing. For future enhancement. */
        }
      }
    }
	}
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_msim_rxd_allowed

DESCRIPTION
  Find whether RxD is allowed or not because of other requirements.
===========================================================================*/
#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

boolean gl1_hw_msim_rxd_allowed(gl1_hw_rxdiv_sub_state sub_state, gas_id_t gas_id)
{
  boolean rxd_allowed = TRUE;

  /* FR28100: Disable Data RxD if other SUB is non3GPP capable.
     Configuration G+1X and G.
     gsm_only = T for both subs. GL1_RXDIV_DUAL_MULTIMODE configuration: Data RxD Off.
     Configuration: G+1X  and W+G
     1X(gsm_only=F) and G(gsm_only=F) -> GL1_RXDIV_DUAL_MULTIMODE. Data RxD Off.
     G(gsm_only=T) and W(gsm_only=F) -> GL1_RXDIV_SINGLE_MULTIMODE -> Data RxD off
     G(gsm_only=T) and G(gsm_only=F) -> GL1_RXDIV_SINGLE_MULTIMODE -> Data RxD On -> MM SUB
                                                                      Data RxD Off -> SM SUB

     FR28100: Disable Data Rxd on MM sub if SM sub is non3pp capable*/
  if((GL1_RXDIV_DATA == sub_state) &&
     (gl1_hw_sub_non3gpp_capable(OTHER_GAS_ID(gas_id))))
  {
    MSG_GERAN_MED_0_G(GL1_RXD"RxD not allowed: Other SUB is non3gpp capable");
    rxd_allowed = FALSE;
  }
  return rxd_allowed;
}
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA

/*===========================================================================
FUNCTION gl1_run_ps_divrx_stream_sel_avg

DESCRIPTION
Maintain a running average of stream selection for DRx during one SACCH period.

===========================================================================*/
void gl1_run_ps_divrx_stream_sel_avg (boolean prx_stream_selected, gas_id_t gas_id)
{
  (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_total_ps_burst_cnt)++;
  if(prx_stream_selected)
   {
   (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_ps_divrx_sel_cnt)++;
   }
}

/*===========================================================================

FUNCTION gl1_get_ps_divrx_stream_sel_rate

DESCRIPTION
Returns TRUE if the percentage rate of DRx selection during the last SACCH period
 is higher than 70 %

===========================================================================*/
boolean gl1_get_ps_divrx_stream_sel_rate (gas_id_t gas_id)
{
  uint32 gl1_ps_divrx_stream_avg = 0;

  if(gl1_hw_rxdiv_data_ptr[gas_id]->gl1_total_ps_burst_cnt !=0 )
  {
    gl1_ps_divrx_stream_avg = (gl1_hw_rxdiv_data_ptr[gas_id]->gl1_ps_divrx_sel_cnt * 100)/gl1_hw_rxdiv_data_ptr[gas_id]->gl1_total_ps_burst_cnt;
  }

  MSG_GERAN_HIGH_1_G(GL1_RXD"PRx stream selected for %d percent during the measurement period",gl1_ps_divrx_stream_avg);

  if (gl1_ps_divrx_stream_avg >= GL1_PRX_STREAM_SELECTION)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION gl1_reset_ps_divrx_sel_avg

DESCRIPTION
Cleans / resets the burst cnt and divrx sel cnt at a SACCH boundary or at
  diversity enter/exit mode requests
if clean_last_avg is set (enter/exit mode req) we reset also the last saved avg

===========================================================================*/
void gl1_reset_ps_divrx_sel_avg (gas_id_t gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_total_ps_burst_cnt = 0;
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_ps_divrx_sel_cnt = 0;
  MSG_GERAN_MED_0_G(GL1_RXD"In gl1_reset_ps_divrx_sel_avg");
}

/*===========================================================================

FUNCTION gl1_set_disable_data_diversity

DESCRIPTION
With RxD enabled if PRx olny stream is selected for 70% of the PS bursts in measurement period RxD will be turned
OFF for PS bursts. With LIF ON, and RxD ON, Chipx4 can't be used GFW can't detect ACI and leds to failure of GCF
test cases TCs 14.18.3 & 14.18.1.With this change to turn OFF RxD, chipx4 can be enabled and it give performance
improvement.

===========================================================================*/
void gl1_set_disable_data_diversity(boolean disable_rxdiversity_data,gas_id_t gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_disable_rxdiversity_data = disable_rxdiversity_data;
  MSG_GERAN_HIGH_1_G(GL1_RXD"Disable Data Diveristy if PRx only stream selcted for 70% of PS bursts:%d",disable_rxdiversity_data);
}

/*===========================================================================

FUNCTION gl1_get_disable_data_diversity

DESCRIPTION
Returns gl1_disable_rxdiversity_data

===========================================================================*/
boolean gl1_get_disable_data_diversity(gas_id_t gas_id)
{
  return(gl1_hw_rxdiv_data_ptr[gas_id]->gl1_disable_rxdiversity_data);
}

/*===========================================================================

FUNCTION gl1_set_disable_data_diversity

DESCRIPTION
If increment_cnt is true, keep counting the number of data measurement periods. And reset number of measurement
periods when increment_cnt is FALSE.

===========================================================================*/
void gl1_packet_meas_period_cnt(boolean increment_cnt,gas_id_t gas_id)
{
  if(increment_cnt)
  {
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_packet_meas_period_cnt++;
  }
  else
  {
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_packet_meas_period_cnt = 0;
  MSG_GERAN_HIGH_1_G(GL1_RXD"gl1_packet_meas_period_cnt:%d",gl1_hw_rxdiv_data_ptr[gas_id]->gl1_packet_meas_period_cnt);
  }
}

/*===========================================================================

FUNCTION gl1_set_disable_data_diversity

DESCRIPTION
Return gl1_packet_meas_period_cnt

===========================================================================*/

uint32 gl1_get_packet_meas_period_cnt(gas_id_t gas_id)
{
 return(gl1_hw_rxdiv_data_ptr[gas_id]->gl1_packet_meas_period_cnt);
}

/*===========================================================================

FUNCTION gl1_reset_ps_rxdiv_params

DESCRIPTION
Resets  variables related to PS RxDiv algorithm that disables Data RxD if PRx only stream selection is reported for 70%
of the bursts recieved in measurement period. And then Data RxD will be enabled after 20 measurement periods and
the the cycle repeats.
===========================================================================*/
void gl1_reset_ps_rxdiv_params(gas_id_t gas_id)
{
   gl1_reset_ps_divrx_sel_avg(gas_id);
   gl1_set_disable_data_diversity(FALSE,gas_id);
   gl1_packet_meas_period_cnt(FALSE,gas_id);
}

#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/

/*===========================================================================

FUNCTION gl1_hw_set_dynamic_rxd_state

DESCRIPTION
 Set Dynamic RxD State.

===========================================================================*/
void gl1_hw_set_dynamic_rxd_state(gl1_hw_rxd_dynamic_rxd_state  state,
                                  gas_id_t                      gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_state
                          = state;
  return;
}

/*===========================================================================

FUNCTION gl1_hw_get_dynamic_rxd_state

DESCRIPTION
 Get Dynamic RxD State.

===========================================================================*/
gl1_hw_rxd_dynamic_rxd_state gl1_hw_get_dynamic_rxd_state(gas_id_t gas_id)
{
  return gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_state;
}

/*===========================================================================

FUNCTION gl1_hw_set_force_divrx_exit

DESCRIPTION
 Sets force DRX exit is required or not on coming out from dedicated mode.

===========================================================================*/
void gl1_hw_set_force_divrx_exit(boolean  force_divrx_exit,
                                 gas_id_t gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_force_divrx_exit
                          = force_divrx_exit;
}

/*===========================================================================

FUNCTION gl1_hw_get_rxdiv_force_divrx_exit

DESCRIPTION
 Returns DRX force exit.

===========================================================================*/
boolean gl1_hw_get_force_divrx_exit(gas_id_t gas_id)
{
  return gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_force_divrx_exit;
}

/*===========================================================================

FUNCTION gl1_hw_set_dynamic_rxd_abort

DESCRIPTION
 Sets Dynamic RxD abort variable for aborting dynamic RxD.

===========================================================================*/
void gl1_hw_set_dynamic_rxd_abort(boolean  abort,
                                  gas_id_t gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_abort
                          = abort;
}

/*===========================================================================

FUNCTION gl1_hw_get_dynamic_rxd_abort

DESCRIPTION
 Returns abort dynamic RxD variable.

===========================================================================*/
boolean gl1_hw_get_dynamic_rxd_abort(gas_id_t gas_id)
{
  return gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_abort;
}

/*===========================================================================

FUNCTION gl1_hw_set_dynamic_rxd_tick_required

DESCRIPTION
 Sets Dynamic RxD tick.

===========================================================================*/
void gl1_hw_set_dynamic_rxd_tick_required(boolean  tick,
                                          gas_id_t gas_id)
{
  gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_tick_required
                          = tick;
}

/*===========================================================================

FUNCTION gl1_hw_get_dynamic_rxd_tick_required

DESCRIPTION
 Returns abort dynamic RxD tick.

===========================================================================*/
boolean gl1_hw_get_dynamic_rxd_tick_required(gas_id_t gas_id)
{
  return gl1_hw_rxdiv_data_ptr[gas_id]->gl1_dynamic_rxd_data.gl1_dynamic_rxd_tick_required;
}
/*===========================================================================

FUNCTION call_in_task_grm_divrx_request

DESCRIPTION
 Request TRM for diversity chain

===========================================================================*/
void call_in_task_grm_divrx_request(grm_request_data_t *params_p)
{
  grm_request_data_t msg;
  gas_id_t           gas_id;

  if (params_p)
  {
    gas_id = params_p->gas_id;
    /* load the outgoing message with the supplied parameters */
    memscpy( &msg,
             sizeof(grm_request_data_t),  /* destination size */
             params_p,
             sizeof(grm_request_data_t) ); /* source size */

    msg.header.message_set = MS_L1_L1;
    msg.header.message_id  = L1_GRM_REQUEST;

    PUT_IMH_LEN( sizeof( grm_request_data_t ) - sizeof( IMH_T ), &msg );

    msg.gas_id             = gas_id;

    MSG_GERAN_MED_0_G( GL1_RXD"call_in_task_grm_rxdiv_request");

    /* When we are in idle mode we need to tell the DRX manager
     * when we send a callback to the task so it knows that the
     * task is active.  This call is ignore when we are not in
     * Idle Mode because the DRX manager is off.
     */
    gl1_drx_task_active( gas_id );

    grm_set_trm_status(GL1_TRM_CALLBACK_PENDING,
                       grm_map_prx_to_divrx_client_id(params_p->client_id, gas_id),
                       gas_id);

    ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );
  }
  else
  {
    MSG_GERAN_ERROR_0( GL1_RXD"call_in_task_grm_rxdiv_request no valid params_p"  );
  }
  return;
}
/*===========================================================================

FUNCTION call_in_task_grm_divrx_release

DESCRIPTION
 Release diversity chain.

===========================================================================*/
void call_in_task_grm_divrx_release(grm_client_enum_t   divrx_client_id,
                                    gas_id_t            gas_id)
{
  grm_release_data_t       msg;

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_GRM_DIVRX_RELEASE;

  msg.client_id          = divrx_client_id;

  msg.gas_id             = gas_id;

  MSG_GERAN_HIGH_1_G( "ISR CALL to call_in_task_grm_divrx_release for client_id:%d",
                      divrx_client_id );

  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg );

  /* When we are in idle mode we need to tell the DRX manager
   * when we send a callback to the task so it knows that the
   * task is active.  This call is ignore when we are not in
   * Idle Mode because the DRX manager is off.
   */
   
  gl1_drx_task_active(gas_id);

  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );
  return;
}
/*===========================================================================

FUNCTION gl1_hw_rxdiv_handle_task_grm_rane

DESCRIPTION
 Handle DRX GRM request received by Task.

===========================================================================*/
void gl1_hw_rxdiv_handle_task_grm_rane(grm_request_data_t *msg)
{
  gas_id_t gas_id = msg->gas_id;
  grm_client_enum_t divrx_client_id = grm_map_prx_to_divrx_client_id(msg->client_id,
                                                                     gas_id);


  rfm_device_enum_type    rf_device = RFM_INVALID_DEVICE;
  trm_request_notify_input_info        *gl1_trm_request_input_info_p;
  trm_get_info_data                    *gl1_trm_get_info_input_info_p; 
  trm_request_notify_input_info  gl1_trm_request_input_info  = { 0 };
  trm_get_info_data              gl1_trm_get_info_input_info = { {0} };
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(divrx_client_id, gas_id);
  grm_info_t *grm_prx_info_p = NULL;
  gl1_hw_rxdiv_data_t     *rxdiv_data_ptr = gl1_hw_get_gsm_rxdiv_data_ptr(gas_id);
  int32 i;                                                          
   gl1_trm_request_input_info_p	  = &gl1_trm_request_input_info;
   gl1_trm_get_info_input_info_p  = &gl1_trm_get_info_input_info;
   /* ensure trm request for secondary chain is populated with same (or subset) of
	  frequency information as that for the primary chain request*/
   if(grm_info_p)
   {
     grm_prx_info_p = grm_get_info_ptr_from_client(grm_info_p->prx_client, gas_id);
   }
   /* If PRx made multi-band request, then get DRx device mapping before making DRx request
	  in order to avoid requesting for unsupported DRx bands */
	 /* Get diversity device band mapping */
   gl1_trm_get_info_input_info_p->band_mapping_info.resource = TRM_RX_DIVERSITY;
   /* Request all default supported bands */
   for ( i = 0; i < grm_valid_gsm_sys_bands_cnt_divrx[gas_id]; i++ )
   {
	 gl1_trm_get_info_input_info_p->band_mapping_info.band[i] = grm_valid_gsm_sys_bands_divrx[gas_id][i];
   }
   gl1_trm_get_info_input_info_p->band_mapping_info.num_bands = grm_valid_gsm_sys_bands_cnt_divrx[gas_id];
  
   rf_device = grm_get_device_mapping( divrx_client_id, gl1_trm_get_info_input_info_p, gas_id );
  
   /* clear supported RxD band mask */
   rxdiv_data_ptr->gl1_hw_DivRx_band_supported = 0;
  
   if ( grm_info_p )
   {
	 for (i = 0; ((i < gl1_trm_get_info_input_info_p->band_mapping_info.num_bands) && (i < grm_valid_gsm_sys_bands_cnt_divrx[gas_id])); i++)
	 {
	   if (grm_info_p->grm_get_info_data.info.band_mapping_info.device[i] >= RFM_MAX_DEVICES)
	   {
		 MSG_GERAN_ERROR_1_G( "gl1_hw_trm_init_DivRx: INVALID DEVICE detected for band %d",
							 gl1_trm_get_info_input_info_p->band_mapping_info.band[i] );
		 rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band = 0;
	   }
	   else
	   {
		 rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band = 
			gl1_trm_get_info_input_info_p->band_mapping_info.band[i];
  
		 rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].device = 
			grm_info_p->grm_get_info_data.info.band_mapping_info.device[i];
  
		 rxdiv_data_ptr->gl1_hw_DivRx_band_supported |=
			gl1_hw_map_sys_band_to_gsm_rxdiv_band(rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band,gas_id);
	   }
	 }
   }
   /* preset num_bands */
   gl1_trm_request_input_info_p->freq_info.num_bands = 0;
  
   if(grm_prx_info_p)
   {
	 for(i=0; ((i<grm_prx_info_p->grm_request_data.num_bands) && (i < grm_valid_gsm_sys_bands_cnt_divrx[gas_id])); i++)
	 {
	   if(gl1_hw_gsm_trm_band_supported(grm_prx_info_p->grm_request_data.band_alloc[i].band.band, gas_id))
	   {
		  gl1_trm_request_input_info_p->freq_info.bands[gl1_trm_request_input_info_p->freq_info.num_bands].band 
			 = grm_prx_info_p->grm_request_data.band_alloc[i].band.band;
		  gl1_trm_request_input_info_p->freq_info.num_bands++;
	   }
	 }
	 MSG_GERAN_HIGH_2_G(" grm_request_rx_sec: Map drx freq info to supported prx info, num bands %d, first band %d",
						gl1_trm_request_input_info_p->freq_info.num_bands,gl1_trm_request_input_info_p->freq_info.bands[0].band);
   }
   /* If device mapping returned some supported bands, but there are no bands common with PRx request, then request
	  for all supported DRx bands */
   if((gl1_trm_request_input_info_p->freq_info.num_bands == 0) && (rxdiv_data_ptr->gl1_hw_DivRx_band_supported != 0))
   {
	 for (i = 0; ((i < gl1_trm_get_info_input_info_p->band_mapping_info.num_bands) && (i < grm_valid_gsm_sys_bands_cnt_divrx[gas_id])); i++)
	 {
	   if(gl1_hw_gsm_trm_band_supported(rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band, gas_id))
	   {
		  gl1_trm_request_input_info_p->freq_info.bands[gl1_trm_request_input_info_p->freq_info.num_bands].band 
			 = rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band;
		  gl1_trm_request_input_info_p->freq_info.num_bands++;
	   }
	 }
	 MSG_GERAN_HIGH_2_G(" grm_request_rx_sec: No valid PRx bands, request for all diversity bands, num bands %d, first band %d",
						gl1_trm_request_input_info_p->freq_info.num_bands,gl1_trm_request_input_info_p->freq_info.bands[0].band);
   }

   if(grm_info_p)
   {
     grm_info_p->grm_freq_info =  gl1_trm_request_input_info_p->freq_info;
   }

  grm_request_and_notify(divrx_client_id,
      TRM_RX_SECONDARY,
      msg->duration,
      TRM_DIVERSITY,
      msg->divrx_req, 
      msg->sub_reason,
      GRM_RAN_ACCESS_RETRY);

  return;
  
}
/*===========================================================================

FUNCTION grm_rx_diversity_unlock_cb

DESCRIPTION 
  - handle diversity unlock cb from TRM.
  - post message to task queue 
===========================================================================*/
void grm_rx_diversity_unlock_cb( trm_unlock_callback_data *unlock_data_p )
{
    gl1_grm_diversity_unlock_callback_t diversity_unlock;
    gl1_grm_diversity_unlock_callback_t *diversity_unlock_p = &diversity_unlock;
    gas_id_t gas_id;   
    
    diversity_unlock_p->diversity_client_id = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);
    diversity_unlock_p->event = unlock_data_p->event;
    gas_id = gl1_map_client_id_to_gas_id(diversity_unlock_p->diversity_client_id);
    
    diversity_unlock_p->gas_id = gas_id;
    diversity_unlock_p->unlock_in_sclk = unlock_data_p->unlock_by_sclk;
    diversity_unlock_p->winning_client_id = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->winning_client_info.client_id);
    diversity_unlock_p->winning_client_reason = unlock_data_p->winning_client_info.reason;

    diversity_unlock_p->header.message_set = MS_L1_L1;
    diversity_unlock_p->header.message_id = L1_GRM_DRX_UNLOCK_CALLBACK;

    MSG_GERAN_HIGH_4_G(GL1_RXD"grm_rx_diversity_unlock_cb: Div client %d, Winning Client %d, unlock event %d, winning reason %d",
         diversity_unlock_p->diversity_client_id,
         diversity_unlock_p->winning_client_id,
         diversity_unlock_p->event,
         diversity_unlock_p->winning_client_reason );

    PUT_IMH_LEN( sizeof( gl1_grm_diversity_unlock_callback_t ) - sizeof(IMH_T), diversity_unlock_p );

    
    ( void )L1_isr_send_message( l1_queues[gas_id], diversity_unlock_p, FALSE, gas_id );
    
    
}

/*===========================================================================

FUNCTION gl1_hw_disable_div_chain_rf_fw_config

DESCRIPTION 
  - Release TRM for diversity chain
  - Sleep RF
  - Configure FW
===========================================================================*/

void gl1_handle_drx_unlock(grm_client_enum_t drx_client, gas_id_t gas_id)
{
    grm_client_enum_t unlock_client = drx_client;
    grm_client_enum_t prx_client = gl1_map_gas_id_to_client_id(gas_id);
    gl1_trm_state_t  divrx_trm_status = grm_get_trm_status(unlock_client, gas_id);
    volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    
    /* Check if UE configured as DR-DSDS capable and Ignore Unlock for Drx Client in SR-DSDS case */
    if(!grm_device_config_is_drdsds())
    {
      MSG_GERAN_HIGH_0_G(GL1_RXD"Ignore Unlock for Drx Client in SR-DSDS case");
      return;
    }
    if(GL1_TRM_GRANTED == divrx_trm_status)
    {
      MSG_GERAN_HIGH_1_G(GL1_RXD"gl1_handle_drx_unlock for client %d: Release Chain and Exit Diversity Mode",unlock_client );
      
      if((l1_tskisr_blk->l1_state == L1_DEDICATED_MODE) ||
	  	 (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE) ||
	  	 (l1_tskisr_blk->l1_state == L1_DTM_MODE))
      {        
      gl1_hw_disable_div_chain_rf_fw_config(prx_client,
                                            TRUE,
		                            TRUE,
                                            gas_id);
      }

      grm_release_rx_sec(drx_client,gas_id);
    }

    return;

}

/*===========================================================================

FUNCTION gl1_msg_dynamic_rxd_tick

DESCRIPTION
 Dynamic RxD handler to enable/disable RxD.

===========================================================================*/
void gl1_msg_dynamic_rxd_tick(gas_id_t gas_id)
{
  gl1_hw_rxd_dynamic_rxd_state dynamic_rxd_state = gl1_hw_get_dynamic_rxd_state(gas_id);
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);

  MSG_GERAN_MED_1_G(GL1_RXD"Dynamic RxD Handler state %d", dynamic_rxd_state);

  switch (dynamic_rxd_state)
  {
  case GL1_DYN_RXD_NULL:
    gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);
    break;

  case GL1_DYN_RXD_TRM_REQUESTED:
    gl1_hw_handle_dynamic_rxd_trm_requested(divrx_client, gas_id);
    break;

  case GL1_DYN_RXD_RF_WAKEUP:
    gl1_hw_handle_dynamic_rxd_rf_wakeup(divrx_client, gas_id);
    break;

  case GL1_DYN_RXD_ACTIVE:
  /* Rxd Process for GL1_DYN_RXD_ACTIVE case is handled at the
   * frame boundary to avoid delaying RF sleep confirmation cmd.
   * Check function gl1_msg_process_rxd_active().
   */
    break;

  case GL1_DYN_RXD_FW_EXIT:
    gl1_hw_handle_dynamic_rxd_fw_exit(divrx_client, gas_id);
    break;

  default:
    MSG_GERAN_ERROR_1_G(GL1_RXD"Invalid dynamic RxD state %d", dynamic_rxd_state);
    break;
  }
  return;
}
/*===========================================================================

FUNCTION gl1_hw_handle_dynamic_rxd_trm_requested

DESCRIPTION
 Handle GL1_DYN_RXD_TRM_REQUESTED state.

===========================================================================*/
static void gl1_hw_handle_dynamic_rxd_trm_requested(grm_client_enum_t   divrx_client_id,
                                                    gas_id_t            gas_id)
{
  gl1_trm_state_t  divrx_trm_status = grm_get_trm_status(divrx_client_id, gas_id);

  /* If abort is received, Release the chain, don't wakeup RF.*/
  if (gl1_hw_get_dynamic_rxd_abort(gas_id))
  {
    MSG_GERAN_MED_0_G(GL1_RXD"Dynamic RxD Abort Received in GL1_DYN_RXD_TRM_REQUESTED");

    call_in_task_grm_divrx_release(divrx_client_id, gas_id);

    gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_NULL, gas_id);

    gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);

    /*Disable Dynamic RxD Handler*/
    gl1_hw_set_dynamic_rxd_tick_required(FALSE, gas_id);
  }

  else if ((GL1_TRM_GRANTED == divrx_trm_status)
      && (FALSE == l1_sc_wcdma_srch_active(gas_id)))
  {
    gl1_enter_diversity_mode(GL1_HW_INIT_RX_ONLY, gas_id);

      /* Change RxD state to GL1_DYN_RXD_RF_WAKEUP, only if enter diversity mode is successful */
      if (GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE == gl1_hw_get_rf_sleep_wakeup_mode(gas_id))
      {
        gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_RF_WAKEUP, gas_id);
      }
      else
      {
        gl1_hw_set_dynamic_rxd_abort(TRUE, gas_id);
      }
  }
  else if (GL1_TRM_NULL == divrx_trm_status)
  {
    /* Remove handler and reset state machine*/
    gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_NULL, gas_id);

    gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);

    /*Disable Dynamic RxD Handler*/
    gl1_hw_set_dynamic_rxd_tick_required(FALSE, gas_id);
  }

  /* Task is still processing trm_request*/
  return;
}
/*===========================================================================

FUNCTION gl1_hw_handle_dynamic_rxd_rf_wakeup

DESCRIPTION
 Handle GL1_DYN_RXD_RF_WAKEUP state.

===========================================================================*/
static void gl1_hw_handle_dynamic_rxd_rf_wakeup(grm_client_enum_t   divrx_client_id,
                                                gas_id_t            gas_id)
{
  gl1_hw_gfw_enter_mode_type gfw_rxd_status = gl1_hw_get_gfw_enter_status(gas_id);

  if (GL1_GFW_PRX_DIVRX_ENTER == gfw_rxd_status)
  {
    /*Rf wakeup cnf has received and FW has been configured*/
    gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_ACTIVE, gas_id);

    /* Received Abort, keep dynamic handler active and exit DRX on next tick*/
    if (gl1_hw_get_dynamic_rxd_abort(gas_id))
    {
      MSG_GERAN_MED_0_G(GL1_RXD"Dynamic RxD Abort Received in GL1_DYN_RXD_RF_WAKEUP");
    }
    else
    {
      /*Disable Dynamic RxD Handler*/
      gl1_hw_set_dynamic_rxd_tick_required(FALSE, gas_id);
    }
  }

  /* Task is still waiting for RF wakeup CNF*/
  return;
}
/*===========================================================================

FUNCTION gl1_hw_handle_dynamic_rxd_active

DESCRIPTION
 Handle GL1_DYN_RXD_ACTIVE state.

===========================================================================*/
static void gl1_hw_handle_dynamic_rxd_active(grm_client_enum_t   divrx_client_id,
                                             gas_id_t            gas_id)
{
  /*Blocking call to Rf sleep and RFM exit to GFW*/
  if (gl1_hw_get_dynamic_rxd_abort(gas_id) || gl1_hw_get_force_divrx_exit(gas_id))
  {
   gl1_exit_diversity_mode(gl1_hw_get_force_divrx_exit(gas_id), gas_id);
   gl1_hw_set_force_divrx_exit(FALSE, gas_id);

   gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_FW_EXIT, gas_id);
  }

  return;
}
/*===========================================================================

FUNCTION gl1_hw_handle_dynamic_rxd_fw_exit

DESCRIPTION
 Handle GL1_DYN_RXD_FW_EXIT state.

===========================================================================*/
static void gl1_hw_handle_dynamic_rxd_fw_exit(grm_client_enum_t   divrx_client_id,
                                              gas_id_t            gas_id)
{
  gl1_hw_gfw_enter_mode_type gfw_rxd_status = gl1_hw_get_gfw_enter_status(gas_id);

  /* If abort is received, keep it TRUE for the next state.*/
  if (gl1_hw_get_dynamic_rxd_abort(gas_id))
  {
    MSG_GERAN_MED_0_G(GL1_RXD"Dynamic RxD Abort Received in GL1_DYN_RXD_FW_EXIT");
  }

  if (GL1_GFW_PRX_ENTER == gfw_rxd_status)
  {
    /*call_in_task_grm_release won't work as it does so many other things which are
      not relevant for DRX release*/
    call_in_task_grm_divrx_release(divrx_client_id, gas_id);

    gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_NULL, gas_id);

    gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);

    /*Disable Dynamic RxD Handler*/
    gl1_hw_set_dynamic_rxd_tick_required(FALSE, gas_id);
  }

  /* Task is still waiting for RFM EXIT Rsp*/

  return;
}

/*===========================================================================

FUNCTION gl1_hw_handle_idle_rxd_rf_fw_exit

DESCRIPTION
 Handle the case when in Idle the diversity chain needs to be released. An
 example use case may be that a trm extension has been denied.
 

===========================================================================*/
void gl1_hw_handle_idle_rxd_rf_fw_exit(  grm_client_enum_t divrx_client_id , gas_id_t gas_id )
{
  /* force exit diversity mode */
  gl1_exit_diversity_mode(TRUE, gas_id);

  gl1_hw_set_dynamic_rxd_state(GL1_DYN_RXD_NULL, gas_id);

  gl1_hw_set_dynamic_rxd_abort(FALSE, gas_id);

  /*Disable Dynamic RxD Handler*/
  gl1_hw_set_dynamic_rxd_tick_required(FALSE, gas_id);

  /* call to release the diversity chain */
  grm_release_rx_sec(divrx_client_id, gas_id);
  return;
}


/*===========================================================================

FUNCTION GL1_MSG_PROCESS_RXD_ACTIVE

DESCRIPTION
 Handle the frame boundary event when RxD dynamic
 state is GL1_DYN_RXD_ACTIVE.

===========================================================================*/
void gl1_msg_process_rxd_active(gas_id_t gas_id)
{
  if (gl1_hw_get_dynamic_rxd_state(gas_id) == GL1_DYN_RXD_ACTIVE)
  {
    volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    grm_client_enum_t divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);
    gl1_hw_handle_dynamic_rxd_active(divrx_client, gas_id);
  }
}

#endif /*FEATURE_GSM_RX_DIVERSITY*/
/* EOF */

