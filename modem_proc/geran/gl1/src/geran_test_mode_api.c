/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                    GPL1 FACTORY TEST MODE API FUNCTIONS

GENERAL DESCRIPTION

  This module provides utility functions for FACTORY TEST MODE (FTM)

EXTERNALIZED FUNCTIONS
  geran_test_mode_api_restart_srb_loop
  geran_test_mode_api_set_vctcxo_pdm_val_leave_chipxn_div2_clock_on
  geran_test_mode_api_gprs_l1_mac_acc_func
  geran_api_GL1_DEFS_MAX_DL_MSGS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2010,2011-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/geran_test_mode_api.c#2 $

when       who       what, where, why
--------   ---       ----------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
18/01/16   zf        CR961805: Set first burst flag in x2g_startup cmd
20/02/18   shm      Change Request 2192580 : KW P1 errors on TA.3.0 fix
11/08/17   rc        CR2138578 ML logging - GL1 Changes
08/02/17   tsk       CR2004238: Fix KW error for uninitialized local variable.
28/02/17   og        CR2006578. FRs 39363 and 39364.
09/03/16   sk        CR986058 Klockwork error removal
10/06/15   jk        CR851734: In FTM mode bsic_expected to be set to GSM_INVALID_BSIC while handling MPH_SELECT_SPECIFIC_BCCH_REQ
29/05/15   pa        CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD. 
22/05/15   ws        CR790493 Check BSIC decoded in SCH matches requested BSIC from RR
02/06/15   pg        CR846507 Allow non-signaling RF devices to be controlled by FTM 
09/05/15   ch/pa     CRXXXXXX Split RX/TX changes for MSIM/SSIM
09/04/15  sw/dp      CR819956 Thor 2.x RF API change, G2W use MSGR
12/03/15   pa        CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
26/02/15   jk        CR800322: Set sglte_mode_active to FALSE while sending MPH_START_GSM_MODE_REQ in FTM mode
21/01/15   pa        CR758792:RxD support for multi-sim.
15/12/14   pg        CR769338 Enable/disable DRx by EFS in NS mode
15/12/14   cs        CR754853 Add support for Parallel SCH detection handling
16/12/14   jk        CR753423:LIF for Idle,voice and data
03/12/14   npt       CR759467 - Fix compiler warnings
17/11/14   dp        CR700052 Move g2w drivers into W filetree
10/10/14   cos       CR736560 BO 2.x Idle RxD mirror combined exit/sleep requests according to the enter/wakeup modes 
11/02/14   jk        CR614254:GL1 changes to support IP2 calibration
28/07/14   zf        CR700730: Removing RXLM buf idx in set GFW app mode
11/07/14   dp        CR674070 - Use new G2W drivers interface (phase 1)
14/04/14   ss       CR646261 FR GSM CM INIT via message in GL1 task context
17/04/14   cos       CR636543 - Changes for enabling Rx Diversity on Bolt - GL1/GRF intf updates
05/03/14   rb        CR625629  GL1 changes for FTM G2W IRAT
30/01/14   ws        CR607097 Fix FTM mode NS GSM/GPRS call establishment
20/01/14   ws        CR597962 - Non Signalling/RF cal mode changed to DSDA to allow Tx chain 2 to
                     be configured in GFW
19/12/13   jk       CR503359 - GL1 changes to support Rx diversity
03/01/14   jk         CR572347 - GL1 FTM API changes to support Rx diversity
10/12/13   ws        CR 587586 - Remove DUAL_SIM featurisation in API
09/11/13   mc        CR.572746 : COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
01/05/13   jk        CR477606:Changes to support both RF IQ logging and GSM XO CAL
22/04/13   jk        CR478252:GERAN L1 changes to use appropriate queue id
11/04/13   jk        CR473972:GERAN L1 changes to handle as_id as an input parameter to the APIs
28/03/13   jk        CR467869:Changes to send AFC command to FW in FTM mode
28/03/13   jk        CR452710: Changes to support continuous IQ logging and GSM XO calibration
18/03/13   pjr       CR464499 - removed featurasation from geran_test_mode_api_cm_gsm_init
08/03/12   ss        CR461209: Set multi_sim_mode to single sim while
                     transitioning online to ftm mode
11/03/13   pjr       CR461202 - DIME/TRITON FTM API compatability support
16/10/12   ws        CR409719 - DIME/TRITON FTM API compatability support
10-12-08   tjw       Initial Revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"

#include "gl1_hw_clk_ctl.h"
#include "gl1_msg_pdch.h"
#include "gpl1_gprs_test.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_utils.h"
#include "l1_task.h"
#include "gmacdef.h"
#include "gmacutil.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "geran_test_mode_api.h"
#include "wl1x2wsrchapi.h"
#include "vstmr_wcdma.h"
#include "gl1_hwi.h"
#include "mdsp_cmd.h"
#include "gl1_hw_g.h"
#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif

extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];
extern void gl1_msg_tch_loopback( gl1_defs_loopback_type loopback, gas_id_t gas_id );

static gl1_hw_sch_decode_t geran_test_mode_get_get_sch_decode_mode(
                             boolean sch_decode );



typedef void (*wl1_x2w_srch_mdsp_post_cb_fn)(gas_id_t gas_id);
typedef void (*wl1_x2w_srch_mdsp_pre_cb_fn)(gas_id_t gas_id);

extern void wl1_x2w_srch_mdsp_sync_send_mode_rf_cfg_cmd(
  /* G2W mode: idle or dedicated */
  wl1_x2w_srch_wfw_mode_enum_type mode,
  /* RxLM buffer index */
  uint32 g2w_rxlm_buf_idx,
  /* RF action time in WSTMR cx8 */
  uint32 rf_action_time,
  /* Freq error in Q6 Hz format */
  int32 freq_err_q6_hz,
  /* Inverse W DL freq */
  uint32 inv_dlo_freq,
  /* pre call back function */
  wl1_x2w_srch_mdsp_pre_cb_fn pre_cb,
  /* post call back function */
  wl1_x2w_srch_mdsp_post_cb_fn post_cb,
  gas_id_t gas_id);

extern void wl1_x2w_srch_mdsp_sync_send_sleep_wfw_cmd(
  /* pre-command callback */
  wl1_x2w_srch_mdsp_pre_cb_fn pre_cb,
  /* post-command callback */
  wl1_x2w_srch_mdsp_post_cb_fn post_cb,
  gas_id_t gas_id);

/*===========================================================================

FUNCTION  geran_test_mode_api_restart_srb_loop( )

DESCRIPTION
  FTM access function to restart the SRB loop after a TS_RECONFIG.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_restart_srb_loop(
#ifdef FEATURE_DUAL_SIM
sys_modem_as_id_e_type as_id
#else
void
#endif /*FEATURE_DUAL_SIM */
)
{
#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif /*FEATURE_DUAL_SIM */
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  transfer_data_T *transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

  gpl1_l1_test_mode_stop_ind(gas_id);
  gl1_msg_pdch_set_loopback( FALSE,gas_id );

  transfer_data_ptr->dynamic_tbf_ptr->test_mode = TEST_MODE_SRB;

  gpl1_l1_test_mode_start_ind(gas_id);
  gl1_msg_pdch_set_loopback( TRUE,gas_id );

  transfer_data_ptr->l1_test_mode = L1_TEST_MODE_SRB_ON;
}

/*===========================================================================
FUNCTION gl1_hw_set_vctcxo_pdm_val

DESCRIPTION
  This function sets the TRK_LO_ADJ PDM to the desired value at the next
  frame boundary used by FTM.

DEPENDENCIES
   None.

RETURN VALUE
   None.

SIDE EFFECTS
   None.

===========================================================================*/
void geran_test_mode_api_set_vctcxo_pdm_val_leave_chipxn_div2_clock_on(int32 pdm_val,sys_modem_as_id_e_type as_id)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_set_vctcxo_pdm_val(pdm_val,LEAVE_CHIPXN_DIV2_CLOCK_ON,0);
}

/*===========================================================================
===
===  FUNCTION      geran_test_mode_api_gprs_l1_mac_acc_func
===
===  DESCRIPTION
===  Used by FTM to send messages to L1. FTM is required to add the IMH
===  header to the message before sending it to L1 using gs_send_message
===
===  INPUT
===  Pointer to L1 message structure of the message to be sent
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===
===
===  SIDE EFFECTS
===
===
===========================================================================*/
void geran_test_mode_api_gprs_l1_mac_acc_func(geran_test_mode_api_mac_l1_sig_t *common_msg_ptr
#ifdef FEATURE_DUAL_SIM
,sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM */
)
{
#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif /*FEATURE_DUAL_SIM */
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gprs_l1_mac_acc_func((mac_l1_sig_t *)common_msg_ptr,gas_id);
}

/*===========================================================================

FUNCTION       GERAN_TEST_MODE_API_SEND_TO_L1

DESCRIPTION


DEPENDENCIES

    UE is in FTM mode.
    ftm_gsm_ber_init() has been called

===========================================================================*/
gs_status_T geran_test_mode_api_send_to_l1( void *msg,gas_id_t gas_id)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return gs_send_message( l1_queues[gas_id], msg, TRUE);
}

/*===========================================================================

FUNCTION        GERAN_TEST_MODE_API_SEND_MPH_START_GPRS_IDLE_MODE_REQ
DESCRIPTION     Sends a fixed message to GL1 to get into GPRS IDLE mode
                using the 51 multiframe CCCH.
.

DEPENDENCIES    GL1 is synchronized to BCCH already.


RETURN VALUE
   None.

SIDE EFFECTS    Sends configuration message to L1.


===========================================================================*/
void geran_test_mode_api_send_FTM_START_GPRS_IDLE_MODE_REQ(
#ifdef FEATURE_DUAL_SIM
sys_modem_as_id_e_type as_id
#else
void
#endif /*FEATURE_DUAL_SIM */
)
{

  mph_start_gprs_idle_mode_req_t        req;
  uint16                                message_size;
  gs_status_T                           status;

#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif /*FEATURE_DUAL_SIM */
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  req.message_header.message_set = MS_RR_L1;
  req.message_header.message_id  = (byte)MPH_START_GPRS_IDLE_MODE_REQ;


  /* Fill in with a default, valid configuration */

  req.ccch_or_pccch = CCCH_51;
  req.gas_id = gas_id;
  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.BA_list.no_of_entries          = 0;
  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.BS_AG_BLKS_RES                 = 0;
  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.BS_CC_CHANS                    = 1;
  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.BS_CCCH_SDCCH_COMB             = 0;
  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.BS_PA_MFRMS                    = 6;
  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.cbch_channel_information_valid = FALSE;
  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.MS_TXPWR_MAX_CCH               = 0;

  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.LB_MS_TXPWR_MAX_CCH_valid      = 0;
  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.LB_MS_TXPWR_MAX_CCH            = 0;

  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.multiband_reporting            = 0;
  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.NCC_permitted                  = 255;
  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.PWRC                           = 0;
  req.idle_mode_params.ccch_idle_mode_params.layer1_sys_info.TX_integer                     = 32;

  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.access_burst_type = FALSE;
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.bep_period        = 5;
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.decode_bcch       = FALSE;
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.drx_timer_max     = 64;
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.imsi_mod1000.lsb  = ( 100 & 0xFF);
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.imsi_mod1000.msb  = 0;
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.nmo               = 1;
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.non_drx_timer     = 0;
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.paging_mode       = NORMAL_PAGING;

  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.power_control.alpha        = 6;
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.power_control.n_avg_i      = 2;
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.power_control.pc_meas_chan = 0;
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.power_control.t_avg_t      = 10;
  req.idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.power_control.t_avg_w      = 12;

  message_size = sizeof( mph_start_gprs_idle_mode_req_t) - sizeof( IMH_T);
  PUT_IMH_LEN( message_size, &req.message_header);


  status = geran_test_mode_api_send_to_l1( (void *) &req, gas_id );

  if ( status == GS_SUCCESS)
  {
    /* Do something */
  }
}

/*===========================================================================

FUNCTION       GERAN_TEST_MODE_API_GET_BEST_SIX_INFO_NO_OF_ENTRIES

DESCRIPTION
               Access function

DEPENDENCIES
  none

===========================================================================*/
uint32 geran_test_mode_api_get_best_six_info_no_of_entries(void *msg)
{
  mph_surround_meas_ind_T *msg1 = msg;
  return msg1->best_six_info.no_of_entries;
}

/*===========================================================================

FUNCTION       GERAN_TEST_MODE_API_SEND_FTM_START_GSM_MODE_REQ

DESCRIPTION
               Sends a Start GSM Mode Request from FTM to L1 task

DEPENDENCIES
  none

===========================================================================*/
gs_status_T geran_test_mode_api_send_FTM_START_GSM_MODE_REQ(sys_modem_as_id_e_type as_id)
{

  mph_start_gsm_mode_req_T          req;
  uint16                            message_size;
  gs_status_T                       status;

  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

  message_size = sizeof( mph_start_gsm_mode_req_T) - sizeof( IMH_T);

  req.message_header.message_set           = MS_RR_L1;
  req.message_header.message_id            = (byte)MPH_START_GSM_MODE_REQ;
  PUT_IMH_LEN( message_size, &req.message_header);

  req.gsm_only = TRUE;
  req.gas_id = gas_id;
  req.multi_sim_mode = SYS_MODEM_DEVICE_MODE_SINGLE_SIM;
  req.sglte_mode_active = FALSE;
  
  status = geran_test_mode_api_send_to_l1( (void *) &req, gas_id );

#ifdef FEATURE_GSM_RX_DIVERSITY
  gl1_hw_get_rxd_efs_item(gas_id);
#else
  if(enableRxd & enableDrx & enablePrx )
    {
     MSG_GERAN_ERROR_0("GSM RX Diversity is disabled. gl1_rxd_ctl_flags are ignored!");
    }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  return( status);
}


/*===========================================================================

FUNCTION       geran_test_mode_api_send_FTM_SELECT_SPECIFIC_BCCH_REQ

DESCRIPTION
               Sends a Select Specific Bcch Req from FTM task to L1

DEPENDENCIES
  none

===========================================================================*/
gs_status_T geran_test_mode_api_send_FTM_SELECT_SPECIFIC_BCCH_REQ( uint16 chanNum, sys_band_T band
#ifdef FEATURE_DUAL_SIM
, sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM */
)
{

  mph_select_specific_bcch_req_T    req;
  uint16                            message_size;
  gs_status_T                       status;

#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  message_size = sizeof( mph_select_specific_bcch_req_T) - sizeof( IMH_T);

  req.message_header.message_set     = MS_RR_L1;
  req.message_header.message_id      = (byte)MPH_SELECT_SPECIFIC_BCCH_REQ;
  req.specific_channel_no.num        = chanNum;
  req.specific_channel_no.band       = band;
  req.bsic.BS_colour_code            = 0xFF;
  req.bsic.PLMN_colour_code          = 0xFF;
  req.gas_id = gas_id;

  PUT_IMH_LEN( message_size, &req.message_header);

  status = geran_test_mode_api_send_to_l1( (void *) &req, gas_id );

  if ( status == GS_SUCCESS)
  {
    /* Do something */
  }

  return( status);
}


/*===========================================================================

FUNCTION      geran_test_mode_api_send_START_IDLE_REQ

DESCRIPTION
              Sends a Start IDLE Request from FTM to L1 task

DEPENDENCIES
  none

===========================================================================*/
gs_status_T geran_test_mode_api_send_FTM_START_IDLE_REQ(
#ifdef FEATURE_DUAL_SIM
sys_modem_as_id_e_type as_id
#else
void
#endif /*FEATURE_DUAL_SIM */
)
{

  mph_start_idle_req_T           req;
  uint16                         message_size;
  gs_status_T                    status;

#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  message_size = sizeof( mph_start_idle_req_T) - sizeof( IMH_T);

  req.message_header.message_set        = MS_RR_L1;
  req.message_header.message_id         = (byte)MPH_START_IDLE_REQ;

  /* Default parameters for GSM IDLE Mode.  For non-signaling mode, these parameters
   * are not significant.
   */
  req.gas_id = gas_id;
  req.imsi_valid                        = TRUE;
  req.imsi_mod_1000.msb                 = 0;
  req.imsi_mod_1000.lsb                 = ( 100 & 0xFF );
  req.decode_bcch                       = FALSE;

  req.layer1_sys_info.BA_list.no_of_entries                     = 0;
  req.layer1_sys_info.BS_PA_MFRMS                               = 2;
  req.layer1_sys_info.BS_AG_BLKS_RES                            = 0;
  req.layer1_sys_info.BS_CC_CHANS                               = 1;
  req.layer1_sys_info.BS_CCCH_SDCCH_COMB                        = 1;
  req.layer1_sys_info.MS_TXPWR_MAX_CCH                          = 0;
  req.layer1_sys_info.PWRC                                      = 0;
  req.layer1_sys_info.NCC_permitted                             = 255;
  req.layer1_sys_info.TX_integer                                = 7;


  req.layer1_sys_info.cbch_channel_information_valid                       = FALSE;
  req.layer1_sys_info.cbch_channel_information.channel_type                = 0;
  req.layer1_sys_info.cbch_channel_information.subchannel                  = 0;
  req.layer1_sys_info.cbch_channel_information.TN                          = 0;
  req.layer1_sys_info.cbch_channel_information.TSC                         = 0;
  req.layer1_sys_info.cbch_channel_information.hopping_flag                = FALSE;
  req.layer1_sys_info.cbch_channel_information.MAIO                        = 0;
  req.layer1_sys_info.cbch_channel_information.frequency_list.no_of_items  = 0;

  PUT_IMH_LEN( message_size, &req.message_header);

  status = geran_test_mode_api_send_to_l1( (void *) &req, gas_id );

  if ( status == GS_SUCCESS)
  {
    /* Do something */
  }

  return( status);
}


/*===========================================================================

FUNCTION       geran_test_mode_api_send_FTM_CHANNEL_ASSIGN_REQ

DESCRIPTION
               Sends Channel Assign Request from FTM to L1 task

DEPENDENCIES
  none

===========================================================================*/
gs_status_T geran_test_mode_api_send_FTM_CHANNEL_ASSIGN_REQ(geran_test_mode_api_ftm_gsm_ber_status_type ber_status
#ifdef FEATURE_DUAL_SIM
, sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM */
)
{

  mph_channel_assign_req_T        req;
  uint16                          message_size;
  gs_status_T                     status;
  uint32                          start_time;

  channel_mode_T                  channel_mode;
  channel_type_T                  channel_type;

#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  message_size = sizeof( mph_channel_assign_req_T) - sizeof( IMH_T);

  req.message_header.message_set         = MS_RR_L1;
  req.message_header.message_id          = (byte)MPH_CHANNEL_ASSIGN_REQ;

  req.multirate_config.valid = FALSE;
  req.gas_id = gas_id;

  memset( (void *) &req.multirate_config,
          0,
          sizeof(multirate_config_T));

  memset( (void *) &req.channel_spec.channel_info_1_before,
          0,
          sizeof( channel_information_T) );

  memset( (void *) &req.channel_spec.channel_info_2_before,
          0,
          sizeof( channel_information_T) );

  memset( (void *) &req.channel_spec.channel_info_2_after,
          0,
          sizeof( channel_information_T) );


  switch ( ber_status.tch_mode)
  {

  case API_FTM_GSM_BER_TCH_FR:

    channel_mode = SPEECH_V1;
    channel_type = FULL_RATE_TRAFFIC;

    break;

  case API_FTM_GSM_BER_TCH_HR:

    channel_mode = SPEECH_V1;
    channel_type = HALF_RATE_TRAFFIC;

    break;

  case API_FTM_GSM_BER_TCH_EFR:

    channel_mode = SPEECH_V2;
    channel_type = FULL_RATE_TRAFFIC;

    break;

  case API_FTM_GSM_BER_TCH_AMR:

    channel_mode = SPEECH_V3;
    channel_type = FULL_RATE_TRAFFIC;

    break;

  default:

    channel_mode = SPEECH_V1;
    channel_type = FULL_RATE_TRAFFIC;

    break;
  }


  /* parameters copied from L1_set_to_tch_mode() */

  start_time = 0;

  req.channel_spec.channel_info_1_after.channel_type               = channel_type;
  req.channel_spec.channel_info_1_after.hopping_flag               = FALSE;
  req.channel_spec.channel_info_1_after.HSN                        = 0;
  req.channel_spec.channel_info_1_after.MAIO                       = 0;
  req.channel_spec.channel_info_1_after.subchannel                 = 0;
  req.channel_spec.channel_info_1_after.TN                         = (byte)ber_status.tch_slot;
  req.channel_spec.channel_info_1_after.frequency_list.channel[0]  = ber_status.tch_arfcn;
  req.channel_spec.channel_info_1_after.frequency_list.no_of_items = 1;
  req.channel_spec.channel_info_1_after.TSC                        = ber_status.tsc;

  req.channel_spec.before_parameters_valid = FALSE;
  req.channel_spec.channel_mode_1          = channel_mode;
  req.channel_spec.channel_mode_2          = channel_mode;
  req.channel_spec.cipher_algorithm        = 0;
  req.channel_spec.cipher_flag             = FALSE;
  req.channel_spec.DTX_indicator           = FALSE;
  req.channel_spec.number_of_ded_channels  = 0;
  req.channel_spec.power_level             = ber_status.tch_ordered_power_level;
  req.channel_spec.starting_time[0]        = (byte) (start_time >> 8);
  req.channel_spec.starting_time[1]        = (byte) (start_time & 0x000000FF);
  req.channel_spec.starting_time_valid     = FALSE;


  PUT_IMH_LEN( message_size, &req.message_header);

  status = geran_test_mode_api_send_to_l1( (void *) &req, gas_id );

  if ( status == GS_SUCCESS)
  {
    /* Do something */
  }

  return( status);

}

void geran_test_mode_api_send_FTM_HWTC_CHANNEL_ASSIGN_REQ(byte ts, geran_test_mode_api_ftm_gsm_ber_status_type ber_status
#ifdef FEATURE_DUAL_SIM
, sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM */
)
{
  mph_channel_assign_req_T        req;
  uint16                          message_size;
  gs_status_T                     status;
  uint32                          start_time;

#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  message_size = sizeof( mph_channel_assign_req_T) - sizeof( IMH_T);

  req.message_header.message_set         = MS_RR_L1;
  req.message_header.message_id          = (int)MPH_CHANNEL_ASSIGN_REQ;
  req.gas_id = gas_id;

  req.multirate_config.valid = FALSE;

  memset( (void *) &req.multirate_config,
          0,
          sizeof(multirate_config_T));

  memset( (void *) &req.channel_spec.channel_info_1_before,
          0,
          sizeof( channel_information_T) );

  memset( (void *) &req.channel_spec.channel_info_2_before,
          0,
          sizeof( channel_information_T) );

  memset( (void *) &req.channel_spec.channel_info_2_after,
          0,
          sizeof( channel_information_T) );


  /* parameters copied from L1_set_to_tch_mode() */

  start_time = (gl1_get_FN(gas_id)+10) %42432;

  req.channel_spec.channel_info_1_after.channel_type               = FULL_RATE_TRAFFIC;
  req.channel_spec.channel_info_1_after.hopping_flag               = FALSE;
  req.channel_spec.channel_info_1_after.HSN                        = 0;
  req.channel_spec.channel_info_1_after.MAIO                       = 0;
  req.channel_spec.channel_info_1_after.subchannel                 = 0;
  req.channel_spec.channel_info_1_after.TN                         = ts;
  req.channel_spec.channel_info_1_after.frequency_list.channel[0]  = ber_status.arfcn;
  req.channel_spec.channel_info_1_after.frequency_list.no_of_items = 1;
  req.channel_spec.channel_info_1_after.TSC                        = 0;

  req.channel_spec.before_parameters_valid = FALSE;
  req.channel_spec.channel_mode_1          = SPEECH_V1;
  req.channel_spec.channel_mode_2          = SPEECH_V1;
  req.channel_spec.cipher_algorithm        = 0;
  req.channel_spec.cipher_flag             = FALSE;
  req.channel_spec.DTX_indicator           = FALSE;
  req.channel_spec.number_of_ded_channels  = 0;
  req.channel_spec.power_level             = 15;
  req.channel_spec.starting_time[0]        = (byte) (start_time >> 8);
  req.channel_spec.starting_time[1]        = (byte) (start_time & 0x000000FF);
  req.channel_spec.starting_time_valid     = FALSE;


  PUT_IMH_LEN( message_size, &req.message_header);

  status = geran_test_mode_api_send_to_l1( (void *) &req, gas_id );

  if ( status == GS_SUCCESS)
  {
    /* Do something */
  }

}



/*===========================================================================

FUNCTION       geran_test_mode_api_send_FTM_CHANNEL_RELEASE_REQ

DESCRIPTION
               Sends Channel Release Request from FTM to L1 task

DEPENDENCIES
  none

===========================================================================*/
gs_status_T geran_test_mode_api_send_FTM_CHANNEL_RELEASE_REQ(geran_test_mode_api_ftm_gsm_ber_status_type ber_status
#ifdef FEATURE_DUAL_SIM
, sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM */
)
{

  mph_channel_release_req_T       req;
  uint16                          message_size;
  gs_status_T                     status;

#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  message_size = sizeof( mph_channel_release_req_T) - sizeof( IMH_T);

  req.message_header.message_set         = MS_RR_L1;
  req.message_header.message_id          = (byte)MPH_CHANNEL_RELEASE_REQ;

  req.valid_bcch_carrier   = TRUE;
  req.serving_bcch_carrier = ber_status.arfcn;
  req.BSIC                 = ber_status.network_parms.bsic;
  req.gas_id = gas_id;

  PUT_IMH_LEN( message_size, &req.message_header);

  status = geran_test_mode_api_send_to_l1( (void *) &req, gas_id );

  if ( status == GS_SUCCESS)
  {
    /* Do something */
  }

  return( status);
}

/*===========================================================================

FUNCTION  geran_test_mode_api_send_FTM_STOP_GSM_MODE_REQ

DESCRIPTION
  This function sends a MPH_STOP_GSM_MODE_REQ to GSM L1

DEPENDENCIES
  None

RETURN VALUE
  Error code for

SIDE EFFECTS
  None

===========================================================================*/
gs_status_T geran_test_mode_api_send_FTM_STOP_GSM_MODE_REQ(
#ifdef FEATURE_DUAL_SIM
sys_modem_as_id_e_type as_id
#else
void
#endif /*FEATURE_DUAL_SIM */
)
{
  mph_stop_gsm_mode_req_T  req;
  uint16                   message_size;
  gs_status_T              status;

#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  message_size = sizeof( mph_stop_gsm_mode_req_T) - sizeof( IMH_T);

  req.message_header.message_set            = MS_RR_L1;
  req.message_header.message_id             = (byte)MPH_STOP_GSM_MODE_REQ;
  PUT_IMH_LEN( message_size, &req.message_header);

  req.rr_l1_gsm_stop_reason = RR_L1_RAT_CHANGE;
  req.gas_id = gas_id;

  status = gs_send_message(l1_queues[gas_id], (void *)&req, TRUE);

#ifdef FEATURE_GSM_RX_DIVERSITY
  gl1_reset_rxd_control_flags(gas_id);
#endif

  gl1_hw_rf_reset_ftm_devices(gas_id);

  return( status);

} /* geran_test_mode_api_send_FTM_STOP_GSM_MODE_REQ */

/*===========================================================================
===
===  FUNCTION      geran_api_GL1_DEFS_MAX_DL_MSGS
===
===  DESCRIPTION
===  Accessor function to provide the conditionally compiled constant GL1_DEFS_MAX_DL_MSGS
===
===  INPUT
===  None
===
===  DEPENDENCIES
===
===
===  RETURN VALUE
===  GL1_DEFS_MAX_DL_MSGS
===
===  SIDE EFFECTS
===  None
===
===========================================================================*/
uint32 geran_api_GL1_DEFS_MAX_DL_MSGS(void)
{
  return GL1_DEFS_MAX_DL_MSGS;
}

/*===========================================================================

FUNCTION  geran_test_mode_api_start_geran

DESCRIPTION
  This function initializes the GSM hardware devices.  It also registers
  a TDMA frame processing handler.


  task_wait_handler - Needed by the RF component of hardware to implement
  a polling mechanism on the calling tasks context.

  task_wait_sig_1   - Signal used to notify the calling task that the
  RF driver is ready to proceed.

  task_wait_sig_2   - Signal used for mdsp timeout download.

===========================================================================*/
static void (*ftm_wait_handler)(rex_sigs_type, sys_modem_as_id_e_type) = NULL;

static void local_wait_handler(rex_sigs_type sigs, gas_id_t gas_id)
{
  if ( ftm_wait_handler != NULL )
  {
      ftm_wait_handler(sigs,geran_map_gas_id_to_nas_id(gas_id));
  }
}

void geran_test_mode_api_start_geran(void                             (*tdma_isr_ptr)(sys_modem_as_id_e_type),
                                      rex_tcb_type                    *task_ptr,
                                      void                            (*task_wait_handler)(rex_sigs_type, sys_modem_as_id_e_type),
                                      rex_sigs_type                   task_wait_sig_1,
                                      rex_sigs_type                   task_wait_sig_2,
                                      gl1_init_rflm_type              gsm_init_rflm_state,
                                      geran_test_mode_api_rxd_type    rxd_control,
                                      geran_test_mode_api_device_type devices,
                                      sys_modem_as_id_e_type          as_id
                                     )
{
#ifdef FEATURE_GSM_RX_DIVERSITY
  gl1_rxd_control_type gl1_rxd_ctl_flags;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  ftm_wait_handler = task_wait_handler;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

#ifdef FEATURE_GSM_RX_DIVERSITY
  gl1_rxd_ctl_flags.enableRxd = rxd_control.enableRxd;
  gl1_rxd_ctl_flags.enableDrx = rxd_control.enableDrx;
  gl1_rxd_ctl_flags.enablePrx = rxd_control.enablePrx;
  gl1_rxd_ctl_flags.enableIMRD = rxd_control.enableRxd;
  gl1_rxd_ctl_flags.enableMSRD = rxd_control.enableRxd;
  gl1_rxd_ctl_flags.RxdForcedOn = FALSE;
  gl1_rxd_ctl_flags.threshold  = 0;
  gl1_set_rxd_control_flags(gl1_rxd_ctl_flags,gas_id);
#endif

  gl1_hw_rf_set_ftm_devices(devices, gas_id);

  gl1_hw_init(tdma_isr_ptr,task_ptr,local_wait_handler,task_wait_sig_1,task_wait_sig_2,gsm_init_rflm_state, gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
  if(gl1_rxd_ctl_flags.enableRxd == FALSE)
  {
  #ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
  #endif
    MSG_GERAN_FATAL_4_G("FTM RF CALLED WITH FLAGS enableRxd=%i, enableDrx=%i, enablePrx=%i FN:%d",
                        rxd_control.enableRxd,rxd_control.enableDrx,rxd_control.enablePrx,GSTMR_GET_FN_GERAN(gas_id));  
  }
#else
  if(rxd_control.enableRxd & rxd_control.enableDrx & rxd_control.enablePrx)
  {
    MSG_GERAN_ERROR_0("GSM RX Diversity is disabled. gl1_rxd_ctl_flags are ignored!");
  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

}

/*===========================================================================

FUNCTION  geran_test_mode_api_stop_geran

DESCRIPTION
  This function performs any cleanup necessary when leaving GSM mode.

  If deep_sleep is TRUE then the Layer 1 hardware will be powered
  down.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_stop_geran(boolean deep_sleep, sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
  if(RFGSM_PRX_DRX_ENTER_MODE == gl1_hw_get_rf_enter_status(gas_id))
  {
    gl1_exit_diversity_mode(TRUE, gas_id);
    gl1_reset_rxd_control_flags(gas_id);
    MSG_GERAN_HIGH_1("ExitModeCmd Sent in FN:%d",GSTMR_GET_FN_GERAN(gas_id));
  }
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_inact(deep_sleep, gas_id);

  gl1_hw_rf_reset_ftm_devices(gas_id);
}

/*===========================================================================

FUNCTION  geran_test_mode_api_turn_tx_on

DESCRIPTION
  This function turns the transmit RF chain on

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_turn_tx_on (sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_turn_tx_on(gas_id);
}

/*===========================================================================

FUNCTION  geran_test_mode_api_set_dedicated

DESCRIPTION
  This function sets dedicated mode state info in GERAN drivers

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_set_dedicated( boolean dedicated,sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_set_dedicated(dedicated, gas_id);
}

/*===========================================================================

FUNCTION  geran_test_mode_api_rf_set_tx_band

DESCRIPTION
  This function performs any cleanup necessary when leaving GSM mode.

  If deep_sleep is TRUE then the Layer 1 hardware will be powered
  down.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_rf_set_tx_band(rfgsm_band_type band,sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_rf_set_tx_band(band, gas_id);
}

/*===========================================================================

FUNCTION  geran_test_mode_api_rf_get_freq_error

DESCRIPTION
  Gets a copy of the current stored frequency error to be applied via DPLL

DEPENDENCIES
  None

RETURN VALUE
  frequency error int32
SIDE EFFECTS
  None

===========================================================================*/
int32 geran_test_mode_api_rf_get_freq_error(sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return(gl1_hw_rf_get_freq_error(gas_id));
}
/*===========================================================================

FUNCTION  geran_test_mode_api_set_idle_mode

DESCRIPTION
  This function sets idle mode state info in GERAN drivers

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_set_idle_mode(boolean idle,sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_set_idle_mode(idle, gas_id);
}

/*===========================================================================

FUNCTION geran_test_mode_api_get_FN

DESCRIPTION
  This function returns the current GERAN TDMA Frame number based on the
  assigned AS ID
DEPENDENCIES
  None

RETURN VALUE
  uint32 TDMA frame number

SIDE EFFECTS
  None
===========================================================================*/

uint32 geran_test_mode_api_get_FN(sys_modem_as_id_e_type as_id)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return(GSTMR_GET_FN_GERAN(geran_map_nas_id_to_gas_id(as_id)) );
}
/*===========================================================================

FUNCTION geran_test_mode_api_get_qsym_count

DESCRIPTION
  This function returns the current GERAN TDMA quater symbol count based on the
  assigned AS ID
DEPENDENCIES
  None

RETURN VALUE
  uint32 quater symbol count 0- 5000

SIDE EFFECTS
  None
===========================================================================*/

uint32 geran_test_mode_api_get_qsym_count(sys_modem_as_id_e_type as_id)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return(gstmr_rd_qsymbol_count_geran(as_id));
}

/*===========================================================================

FUNCTION  geran_test_mode_api_set_idle_mode

DESCRIPTION
  This function sets idle mode state info in GERAN drivers

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_schedule_frame_tick (uint32 timestamp,sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_schedule_frame_tick(timestamp, gas_id);
}

/*===========================================================================

FUNCTION geran_test_mode_api_sched_rx_tx

DESCRIPTION
   Called every frame tick after the rx and tx handlers have been executed
   to issue any scheduled receives or transmits.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_sched_rx_tx (sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_dtm_frame_tick(gas_id);
}
/*===========================================================================

FUNCTION gl1_hw_rx_dtm_gprs

DESCRIPTION
  Schedule a GPRS receive.

===========================================================================*/
void geran_test_mode_api_rx_dtm_gprs
(
   const gl1_hw_rx_signal_type      signal_info[GERAN_API_MAX_GL1_DEFS_MAX_DL_MSGS],
   uint8                            num_tn,
   const gl1_hw_rx_pkt_burst_type   *burst_info,
   gl1_hw_rx_tags_type              tags[GERAN_API_MAX_GL1_DEFS_MAX_DL_MSGS],
   sys_modem_as_id_e_type           as_id
)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_rx_dtm_gprs(signal_info,
                     num_tn,
                     burst_info,
                     tags, 
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                     FALSE,
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
                     gas_id);
}

/*===========================================================================

FUNCTION geran_test_mode_api_rx_dtm_gsm

DESCRIPTION
  Schedule a GSM receive.

===========================================================================*/
void geran_test_mode_api_rx_dtm_gsm
(
   const gl1_hw_rx_signal_type   *signal_info,
   const gl1_hw_rx_burst_type    *burst_info,
   gl1_hw_rx_tags_type           *tags,
   sys_modem_as_id_e_type        as_id

)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
 gl1_hw_rx_dtm_gsm(signal_info,burst_info,tags,
#ifdef FEATURE_GSM_RX_DIVERSITY
          FALSE,
#endif
                    gas_id);
}

/*=============================================================================
FUNCTION  geran_test_mode_api_get_rx_metrics

DESCRIPTION
   Retrieves metrics information. This function is to be called every frame ISR.
   If there is no data or metrics to be returned then the corresponding buffer
   pointer will be set to 'NULL'.

DEPENDENCIES

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void geran_test_mode_api_get_rx_metrics
(
   gl1_defs_rx_pkt_metrics_type **metrics_info,
   uint8                        num_msgs,
   boolean                      get_usf_info,
   boolean                      get_hard_decisions,
   gl1_hw_rx_tags_type          *tags,
   sys_modem_as_id_e_type       as_id
)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_get_rx_metrics(metrics_info,num_msgs,get_usf_info,get_hard_decisions,tags,
#ifdef GERAN_L1_ENHANCED_RX
                        GL1_MSG_PS_PACCH_PDTCH,
#endif /*GERAN_L1_ENHANCED_RX*/
                        gas_id);

}

/*=============================================================================
FUNCTION geran_test_mode_api_get_rx_data

DESCRIPTION
   Retrieves data information. This function is to be called every frame ISR. If
   there is no data or metrics to be returned then the corresponding buffer
   pointer will be set to 'NULL'.

DEPENDENCIES
   The frame layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
boolean geran_test_mode_api_get_rx_data
(
   boolean                   ptcch_type,
   gl1_defs_tn_type          tn,
   gl1_hw_rx_tags_type       *tag,
   boolean                   last_attempt,
   gl1_defs_rx_pkt_data_type *msg_info,
   sys_modem_as_id_e_type    as_id

)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return(gl1_hw_get_rx_data(ptcch_type,tn,tag,last_attempt,msg_info, gas_id));
}

/*===========================================================================

FUNCTION geran_test_mode_api_get_sync_rx_metrics

DESCRIPTION
  This function retreives the metrics for the burst received in the previous
  frame.

DEPENDENCIES
  A burst receive command must have been issued two frames previously to
  cause a burst to be received in the previous frame.

RETURN VALUE
  True/False depending on if a Interferer Blocker is seen.

SIDE EFFECTS
  None
===========================================================================*/
boolean geran_test_mode_api_get_sync_rx_metrics
(
   gl1_defs_metrics_rpt     *rpt,
   gl1_hw_rx_tags_type      *tags,
   sys_modem_as_id_e_type   as_id
)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return(gl1_hw_get_sync_rx_metrics(rpt,tags, 
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
           LOG_INVALID_OR_FTM,
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
           gas_id));

}

/*===========================================================================

FUNCTION  geran_test_mode_api_rf_tx_burst

DESCRIPTION
  Send MSGR message to RF Task for Tx Burst processing

DEPENDENCIES


RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void geran_test_mode_api_rf_tx_burst(uint8                    num_assigned,
                                     uint8                    num_slots,
                                     rfgsm_band_type          band,
                                     uint16                   arfcn,
                                     int32                    freq_error,
                                     uint16                   pa_index[GL1_DEFS_MAX_ASSIGNED_UL_TS],
                                     gl1_defs_modulation_type mod[GL1_DEFS_MAX_ASSIGNED_UL_TS],
                                     sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  uint32 rf_seq_num = 0;

  int16 coex_backoff_desense[GL1_DEFS_MAX_ASSIGNED_UL_TS] = {0};
  int16 coex_backoff_vbatt[GL1_DEFS_MAX_ASSIGNED_UL_TS] = {0};
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_rf_tx_burst(num_assigned,num_slots,band,arfcn,freq_error,pa_index,mod,coex_backoff_desense,coex_backoff_vbatt,rf_seq_num, gas_id);
}
/*===========================================================================
FUNCTION  geran_test_mode_api_log_burst_metrics

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.
RETURN VALUE
  None

SIDE EFFECTS
  Calls l1_log_receive_metrics_packet() when done
===========================================================================*/
void geran_test_mode_api_log_burst_metrics
(
  gl1_msg_chan_type            channel,
  gl1_msg_metrics_rpt          *rpt_ptr[],
  sys_modem_as_id_e_type       as_id
)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  l1_log_burst_metrics(channel,rpt_ptr,geran_map_nas_id_to_gas_id(as_id));
}

/*===========================================================================
FUNCTION  geran_test_mode_api_is_gfw_error_detected

DESCRIPTION
  This function should be called when burst metrics are available
  It will send a log packet.
RETURN VALUE
  None

SIDE EFFECTS
  Calls l1_log_receive_metrics_packet() when done
===========================================================================*/
boolean geran_test_mode_api_is_gfw_error_detected( sys_modem_as_id_e_type as_id )
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return(gl1_is_gfw_error_detected_geran(as_id));

}

/*===========================================================================

FUNCTION  gl1_hw_start_acquisition

DESCRIPTION
  This function initiates a tone detection on the specified frequency
  at the specified offset into the frame and at the specified
  received signal level

DEPENDENCIES
  ARFCN              - Radio Channel Number on which to detect tone.
  rx_signal_strength - Anticipated Signal Strength at Antenna.
  offset             - Offset in quarter symbols at which to start tone
                       detection
  sch_decode         - TRUE:  decode SCH after tone is detected
                       FALSE: don't decode SCH
  AFC                - TRUE: adjust TCXO based on tone results
                       FALSE: don't adjust TCXO
  tags               - Pointer to where the gain will be stored

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_start_acquisition
(
   ARFCN_T ARFCN,
   dBx16_T rx_signal_strength,
   uint16  timing_offset,
   int16   frequency_offset,
   boolean sch_decode,
   gl1_hw_acq_afc_type AFC,
   gl1_hw_rx_tags_type *tags,
   sys_modem_as_id_e_type as_id
)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /*Enable IQ logging flag*/
  mdsp_set_iq_log_flag(TRUE);
  /* Make sure that frequency error is set to zero before sending
     * AFC command to GFW.*/
  gl1_hw_set_afc_mode(AFC);
  /* Send AFC command to GFW.*/
  gl1_hw_set_active_cycle_swallower_adj(ARFCN,gas_id);
  gl1_hw_gfw_config_cycle_swallower(gas_id);
  gl1_hw_start_acquisition_geran(ARFCN, rx_signal_strength, timing_offset, frequency_offset,
                                 geran_test_mode_get_get_sch_decode_mode( sch_decode ),
                                 AFC, tags, FALSE, as_id);
}


/*===========================================================================

FUNCTION geran_test_mode_get_get_sch_decode_mode

DESCRIPTION
  Set SCH decode mode for test mode

===========================================================================*/
static gl1_hw_sch_decode_t geran_test_mode_get_get_sch_decode_mode(
                             boolean sch_decode )
{
  /* Default to SCH disabled */
  gl1_hw_sch_decode_t gtm_sch_decode_mode = GL1_NO_SCH_DECODE;

  /* Now if sch decode required ascertain if Parallel SCH enabled */
  if ( sch_decode )
  {
    gtm_sch_decode_mode = GL1_CONVENTIONAL_SCH_DECODE;
  }

  return ( gtm_sch_decode_mode );
}

/*===========================================================================

FUNCTION  gl1_hw_stop_acquisition

DESCRIPTION
  This function stops the tone detection at the specified offset into the
  frame. The offset does not include the DSP's frame tick setup margin

DEPENDENCIES
  This function should only be called after starting a tone detection
  by invoking gl1_hw_start_acquisition

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_stop_acquisition(uint16 offset,sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

 /*Disable IQ logging*/
  mdsp_set_iq_log_flag(FALSE);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_stop_acquisition_geran(offset, gas_id);
}

/*===========================================================================

FUNCTION  GL1_SET_SACCH_TX_PWR_CONTROL

DESCRIPTION
  This function will set an indication as to use or ignore the
  SACCH tx power control level.

DEPENDENCIES

RETURN VALUE
  gs_status_T GS_SUCCESS

SIDE EFFECTS
  None

===========================================================================*/
gs_status_T geran_test_mode_api_set_sacch_tx_pwr_control(boolean tx_pwr_control,sys_modem_as_id_e_type as_id)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
 return(gl1_set_sacch_tx_pwr_control(tx_pwr_control, geran_map_nas_id_to_gas_id(as_id)));
}

/*===========================================================================

FUNCTION geran_test_mode_api_do_tch_loopback

DESCRIPTION
  This function tells the mdsp to put the TCH into loopback A,B or C or no
  loopback.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void geran_test_mode_api_do_tch_loopback(gl1_defs_loopback_type loopback, sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_msg_tch_loopback(loopback, gas_id);
}
/*=============================================================================
FUNCTION  gl1_hw_write_tx_pkt_data

DESCRIPTION
   Writes a block/frame of data to the DSP memory. If the message buffer in the
   mDSP is full then one of the messages in the mDSP buffer will be discarded
   to make space for the submitted message (if the submitted message is a higher
   priority message).
   Returns TRUE if the message was successfully written to the mDSP.
============================================================================= */
boolean geran_test_mode_api_write_tx_pkt_data
(
   gl1_defs_tx_pkt_data_type *msg_ptr,
   uint8                     len,
   gl1_defs_tn_type          tn,
   gl1_hw_pkt_msg_type       msg_type,
   gl1_hw_channel_type       chan_type,
   gl1_hw_tx_tags_type       *tags,
   sys_modem_as_id_e_type    as_id
)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return(gl1_hw_write_tx_pkt_data(msg_ptr,len,tn,msg_type,chan_type,tags,gas_id));
}

/*=============================================================================
FUNCTION  gl1_hw_get_tx_scheduled_result

DESCRIPTION
   Returns if the message associated with the given tag was transmitted.
   If discard_untxed is set then untransmitted messages are cleared.

============================================================================= */
boolean geran_test_mode_api_get_tx_scheduled_result
(
   gl1_hw_tx_tags_type      *tags,
   boolean                  discard_untxed,
   sys_modem_as_id_e_type   as_id

)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return(gl1_hw_get_tx_scheduled_result(tags,discard_untxed, gas_id));
}

/*===========================================================================

FUNCTION geran_test_mode_api_tx_dtm_gprs

DESCRIPTION
  Schedule a GPRS transmit.

===========================================================================*/
void geran_test_mode_api_tx_gprs
(
   const gl1_hw_tx_signal_type       *signal_info,
   uint8                             num_tn,
   const gl1_hw_tx_pkt_burst_type    *burst_info,
   gl1_hw_tx_tags_type               *tags,
   uint8                             num_msgs,
   uint8                             num_ul_timeslots,
   sys_modem_as_id_e_type            as_id
)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_tx_dtm_gprs(signal_info,num_tn,burst_info,tags,num_msgs,num_ul_timeslots, gas_id);
}

 /*===========================================================================

FUNCTION  geran_test_mode_api_get_power_measure_results

DESCRIPTION
  Returns 9 rssi values.

DEPENDENCIES
  gl1_hw_cm_power_measure must have been called two frames previously

RETURN VALUE
  samples       - array of 9 measured powers (received signal strength)
  num           - number of results to process. This number should match
                  the num parameter used in calling theu
  tags          - Pointers to where the gain will be stored

  (It is the responsiblity of the calling function to provide storage for
   results, regardless of how many bursts were recieved.  It is also
   the responsibility of the calling function to know how many bursts were
   received.)

  TRUE  - power results were available
  FALSE - power results were not available

SIDE EFFECTS
  None

===========================================================================*/
boolean geran_test_mode_api_cm_get_power_measure_results
(
   gl1_hw_measurement_T         samples[],
   uint8                        num,      /* Number of results to process  */
   gl1_hw_rx_tags_type          tags[],
   sys_modem_as_id_e_type       as_id
)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  return gl1_hw_cm_get_power_measure_results_geran ( samples, num, tags, as_id );
}

/*===========================================================================

FUNCTION  geran_test_mode_api_cm_power_measure

DESCRIPTION
  This function initiates one to nine power measurements in the next frame.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_cm_power_measure
(
   ARFCN_T                      ARFCN[],               /* Absolute Radio Frequency Channels  */
   dBx16_T                      rx_signal_strength[],  /* Signal Strengths at Antenna        */
   uint8                        num,                                           /* Number of measurements to make     */
   uint16                       offset[],              /* When to make a measurement in qs   */
   gl1_hw_burst_position_type   position[],            /* position of burst within a gap     */
   gl1_hw_rx_tags_type          tags[],
   sys_modem_as_id_e_type       as_id
)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_cm_power_measure_geran ( ARFCN, rx_signal_strength, num, offset,
                            position, tags, as_id );
}
/*===========================================================================

FUNCTION  geran_test_mode_api_async_intf_app_mode_config

DESCRIPTION
  Send MSGR message to GFW Task to run app mode config. Use MSGR blocking wait
  to receive app mode config rsp from GFW before continuing

===========================================================================*/
void geran_test_mode_api_async_intf_app_mode_config
(
   GfwAppModeType               app_mode,
   uint32                       rxlm_buf_idx,
   sys_modem_as_id_e_type       as_id
)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_async_intf_app_mode_config ( app_mode, as_id );
}


/*===========================================================================

FUNCTION  geran_test_mode_api_cm_startup

DESCRIPTION
  Calls to invoke the CM startup at the beginning of the gap.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void  geran_test_mode_api_cm_startup
(
   uint16                       offset,
   const gl1_hw_cm_rf_info_t*   cm_rf_info_ptr,
   sys_modem_as_id_e_type       as_id
)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  geran_l1_x2g_irat_enter ( cm_rf_info_ptr, as_id );
  gl1_hw_cm_startup_geran ( offset, cm_rf_info_ptr, TRUE, as_id );
}


/*===========================================================================

FUNCTION  geran_test_mode_api_cm_gsm_init

DESCRIPTION
  Function to initialize the GSM Layer 1 interfaces that are required for
  compressed mode operation.  Interfaces include GSTMR, MDSP, RF.
  This function should be invoked whenever the WCDMA stack initializes.
  It also registers a TDMA frame processing handler.

===========================================================================*/
void geran_test_mode_api_cm_gsm_init
(
   void                         (* tdma_isr_ptr)(sys_modem_as_id_e_type as_id),
   rex_tcb_type                 *  task_ptr,
   void                         (* task_wait_handler)(rex_sigs_type),
   rex_sigs_type                task_wait_sig,
   rex_sigs_type                task_wait_sig_1,
   sys_modem_as_id_e_type       as_id
)
{
  rfcom_mode_enum_type x2g_serving_rat;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /*Need to know calling function is TD or not*/
  if (rcinit_lookup_rextask("tds_l1") == task_ptr   )
  {
      x2g_serving_rat =  RFM_TDSCDMA_MODE ;
  }else
  {
      x2g_serving_rat =  RFM_ALL_MODES ;
  }

  gl1_hw_cm_gsm_init_geran ( tdma_isr_ptr, task_ptr, task_wait_handler, task_wait_sig
                      ,task_wait_sig_1
                      ,as_id , x2g_serving_rat );
}

/*===========================================================================

FUNCTION  geran_test_mode_api_cm_cleanup

DESCRIPTION
  Call to invoke the CM cleanup at the end of the gap.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void geran_test_mode_api_cm_cleanup
(
   uint16                       offset,
   sys_modem_as_id_e_type       as_id
)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_cm_cleanup_geran ( offset, as_id );
}

/*===========================================================================

FUNCTION geran_test_mode_api_cm_gsm_terminate

DESCRIPTION
  Cleanup the gl1_hw_cm layer interface.

DEPENDENCIES
  gl1_hw_cm_gsm_init() must be called first.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void geran_test_mode_api_cm_gsm_terminate
(
   sys_modem_as_id_e_type       as_id
)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
#ifdef FEATURE_DUAL_SIM
  gl1_hw_cm_gsm_terminate ( as_id );
#else
  gl1_hw_cm_gsm_terminate();
#endif
}
/*===========================================================================

FUNCTION geran_test_mode_api_g2w_init

DESCRIPTION
  Send the x2w mdsp init command to initialise the x2w sync interface

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void geran_test_mode_api_g2w_init
(
sys_modem_as_id_e_type       as_id
)
{
  gas_id_t gas_id;
  gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  gl1_hw_wcdma_init(gas_id);

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
#ifdef FEATURE_QSH_MDUMP
	  QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif

  ERR_FATAL("NOT CURRENTLY SUPPORTED", 0, 0, 0);

#else /* FEATURE_GSM_TO_WCDMA_MSGR */
  wl1_x2w_srch_g2w_wfw_sw_init(gas_id);
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
}
/*===========================================================================

FUNCTION geran_test_mode_api_start_g2w

DESCRIPTION
  Send the x2w start mode cmd to WFW

DEPENDENCIES
  geran_test_mode_api_g2w_init() must be called first.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void geran_test_mode_api_start_g2w
(
/* DL channel */
uint16 uarfcn,
/* Start Time */
uint16 start_time,
sys_modem_as_id_e_type       as_id
)
{
  gas_id_t gas_id;
  gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  /* Initiate dedicated search which sends x2w start mode cmd to WFW  */
#ifdef FEATURE_GSM_TO_WCDMA_MSGR

#ifdef FEATURE_QSH_MDUMP
	  QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif

  ERR_FATAL("NOT CURRENTLY SUPPORTED", 0, 0, 0);
  
#else /* FEATURE_GSM_TO_WCDMA_MSGR */
  wl1_x2w_srch_g2w_initiate_dedicated_search( WFW_IRAT_G2W_DEDICATED_MODE,
                                              uarfcn,
                                              start_time,
                                              #if defined(FEATURE_DUAL_SIM) && defined(FEATURE_GSM_COEX)
                                              gl1_msg_get_multi_sim_sys_mode(),
                                              #endif
                                              gas_id );
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
}
/*===========================================================================

FUNCTION geran_test_mode_api_g2w_rf_cfg

DESCRIPTION
  Send config and tune commands to WFW in X2W

DEPENDENCIES
  geran_test_mode_api_g2w_init() must be called first.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void geran_test_mode_api_g2w_rf_cfg
(
/* RxLM buffer index for neighbour */
uint32 g2w_rxlm_buf_idx,
/* Freq error in Q6 Hz format */
int32 freq_err_q6_hz,
/* Inverse W DL freq */
uint32 inv_dlo_freq,
/* pre call back function */
g2w_pre_cb_fn pre_cb,
/* post call back function */
g2w_post_cb_fn post_cb,
sys_modem_as_id_e_type       as_id
)
{
  gas_id_t gas_id;
  vstmr_wcdma_view_s *wcdma_ref_view;
  vstmr_wstmr_t       wcdma_stmr;
  uint32              g2w_start_time_xo;

  gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

  g2w_start_time_xo = gstmr_qs_in_next_frame_to_xo( mdsp_ftsm( gas_id ), gas_id);
  wcdma_ref_view = vstmr_wcdma_get_view_handle(VSTMR_RTC_WCDMA_SUB0, VSTMR_WCDMA_REF_COUNT_VIEW);
  wcdma_stmr = vstmr_wcdma_xo_to_wstmr(wcdma_ref_view, g2w_start_time_xo);

  MSG_GERAN_MED_1_G("G2W: startup time Cx8 %d",wcdma_stmr.subframecx8);

  /* Send the mode cfg + RF tune cmd to WFW */
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
#ifdef FEATURE_QSH_MDUMP
	QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif

  ERR_FATAL("NOT CURRENTLY SUPPORTED", 0, 0, 0);
  
#else /* FEATURE_GSM_TO_WCDMA_MSGR */

  wl1_x2w_srch_mdsp_sync_send_mode_rf_cfg_cmd(WFW_IRAT_G2W_DEDICATED_MODE, g2w_rxlm_buf_idx, wcdma_stmr.subframecx8, freq_err_q6_hz,
                                     inv_dlo_freq, pre_cb, post_cb,gas_id);
  
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
}
/*===========================================================================

FUNCTION geran_test_mode_api_g2w_sleep

DESCRIPTION
  Send the x2w sleep mode cmd to WFW

DEPENDENCIES
  geran_test_mode_api_send_g2w_mdsp_sync_init() must be called first.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void geran_test_mode_api_g2w_sleep
(
/* pre call back function */
g2w_pre_cb_fn pre_cb,
/* post call back function */
g2w_post_cb_fn post_cb,
sys_modem_as_id_e_type       as_id
)
{
  gas_id_t gas_id;
  gas_id = geran_map_nas_id_to_gas_id(as_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */


#ifdef FEATURE_GSM_TO_WCDMA_MSGR
#ifdef FEATURE_QSH_MDUMP
	  QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif

  ERR_FATAL("NOT CURRENTLY SUPPORTED", 0, 0, 0);
  
#else /* FEATURE_GSM_TO_WCDMA_MSGR */
  wl1_x2w_srch_mdsp_sync_send_sleep_wfw_cmd( pre_cb, 
                                             post_cb,
                                             gas_id );
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
}


/*===========================================================================

FUNCTION geran_test_mode_api_do_ip2cal

DESCRIPTION
  RF calls this API to trigger GL1 to schedule IP2 cal  frame

DEPENDENCIES
 geran_test_mode_api_start_geran or 
 geran_test_mode_api_send_FTM_START_GSM_MODE_REQ must be called
 before calling this funtion.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void geran_test_mode_api_do_ip2cal( uint8 NoOfIP2CalSteps,uint32 ip2CalStepSizeQS,sys_modem_as_id_e_type as_id )
{
#ifdef GERAN_L1_IP2CAL
  gas_id_t gas_id           = geran_map_nas_id_to_gas_id(as_id);
  uint8 num_of_ip2cal_steps = NoOfIP2CalSteps;
  uint32 ip2Cal_StepSizeQS  = ip2CalStepSizeQS;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
  MSG_GERAN_HIGH_1_G("IP2 Cal request received in FN:%d",gl1_get_FN(gas_id));
  if(gl1_hw_get_ip2cal_support(gas_id))
   {
    gl1_hw_send_ip2cal_cmd(num_of_ip2cal_steps,ip2Cal_StepSizeQS,gas_id);
   }
#endif
}

/*===========================================================================

FUNCTION geran_test_mode_api_set_ftm_devices

DESCRIPTION
  Allow RF FTM to pre-set PRx, DRx & Tx device allocation before starting FTM
  or Non-signaling

DEPENDENCIES
  Must be called before calling geran_test_mode_api_start_geran (NS) or
  geran_test_mode_api_send_FTM_START_GSM_MODE_REQ (FTM)

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void geran_test_mode_api_set_ftm_devices(geran_test_mode_api_device_type devices, sys_modem_as_id_e_type as_id)
{

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
   gl1_hw_rf_set_ftm_devices(devices, geran_map_nas_id_to_gas_id(as_id));
}


void geran_test_mode_api_set_chain_select( geran_test_mode_api_device_type  devices,
                                           sys_modem_as_id_e_type           as_id
                                         )
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
   gl1_hw_rf_set_ftm_devices_chain_select(devices, geran_map_nas_id_to_gas_id(as_id));
}

/*===========================================================================

FUNCTION geran_test_mode_api_get_as_id_from_rf_id

DESCRIPTION
  Allow RF to convert RF sub ID (passed in by GL1 in each RF API) to AS_ID

DEPENDENCIES
  None

RETURN VALUE
  sys_modem_as_id_e_type as_id

SIDE EFFECTS
  None
===========================================================================*/
sys_modem_as_id_e_type geran_test_mode_api_map_rf_sub_id_to_as_id(uint8 rf_sub_id)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
   return(gl1_hw_rf_map_rf_sub_id_to_as_id(rf_sub_id));
}

