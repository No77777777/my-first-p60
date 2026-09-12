/*----------------------------------------------------------------------------*/
/*!

  @brief
  This is the FTM RF file which contains RF specific commands

Copyright (c) 1994-2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.


                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rftech_gsm.mpss/3.11/ftm/src/ftm_gsm_dispatch.c#3 $ //source/qcom/qct/modem/rftech/gsm/main/1.0/ftm/src/ftm_gsm_dispatch.c#9
  $DateTime: 2022/03/09 21:16:23 $
  $Author: mplcsds1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/04/22   sma     [CR3135116] Added fix to avoid FTM crash when QCN data is not loaded
04/05/21   aad     [CR2882992] Removing unused case condition FTM_SET_PDM 
01/06/15   zbz     Fix DRx IQ capture in sensitivity estimation 
11/18/15   zbz     DRx IQ capture changes  
11/03/15   sas     Fix for avoiding Kw warnings and device status register read problem
10/08/15   dpk     Fixing response packet for GSM FTM IQ capture
08/18/15   sc      Deprecate subscription 3 reducing memory footprint
08/07/15   ck      support gsm fbrx power meas
07/31/15   hoh     adapt frame sweep cal to work with Rx/Tx device split architecture
07/30/15   ck      add FTM GSM cmd to override QPOET mode and bias voltage
07/24/15   tsr     Fix Idle Frame processing in FTM 
07/16/15   tsr     Added GSM Support for FTM Device manager 
07/17/15   ck      apply freq error on both tx and rx devices in FTM
06/24/15   tsr     Implemented subscription based handling of FTM<->RF Tasks MSGR messages 
06/24/15   jb/sc   Add FTM command proc API calls to enh debug dispatch
06/23/15   jb/sc   Add FTM GSM enh APIs for clks, msgr, lm, and mem latency
06/19/15   sc      Add FTM GSM enh debug dispatcher command IDs
06/16/15   yb      Added support for get tune code API
06/12/15   ndb     Fix the legacy FTM command "FTM_SET_TUNER_TUNE_CODE_OVERRIDE" to work with the new Rx/Tx Split.
06/11/15   tsr     Fix KW eror from previous check-in 
06/10/15   tsr     Added FTM and Non-Signalling Support on a chain if TX device is not supported 
05/22/15   sc      Add FTM GSM enh debug dispatcher command
05/21/15   jfc     [RxTx Split] Use remapped device for FTM_SET_PDM command 
04/24/15   tsr     Add support for GSM C4 calibration 
04/23/15   zbz     Pass sub_id from chain vars thoughout tx cal callflow 
04/15/15   sc      Rely on chain vars for split Rx/Tx info throughout call-flow
04/14/15   ck      add ftm command to return nv container 
04/08/15   tsr     Introduced ftm gsm chains based framework for split RX/TX devices  
03/08/15   mz	   Added support for FTM GSM Sensitivity Estimation
12/09/14   ck      add a dispatch command to overwrite alt_path
12/10/14   zbz     Add command to return both PRx DRx RSSI 
12/02/14   sml     Read WTR or RF front end devices status in between Rx/Tx 
                   burst along with GRFC status
11/14/14   ck      Add FTM feature to set pa bias NVs via diag 
11/13/14   ck      add capability to disable/enable gsm diversity 
10/30/14   ck      move ftm concurrency manager to ftm gsm enter for future
10/30/14   zbz     Add new FTM command to return DRx RSSI 
10/28/14   ck      unhook concurrency manager to avoid being blocked 
09/24/14   ch/ndb  Passing device info to tuner tune over-ride dispatch API 
08/20/14   ndb     Add Tuner_ID as part of the response in "FTM_SET_TUNER_TUNE_CODE_OVERRIDE"
07/25/14   sb      bug fixes for V1 IP2 cal functionality 
07/14/14   ec      Add SAWLess control APIs to dispatchers
05/29/14   sb      IP2 cal bug fixes, updates 
04/16/14   sb      IP2 cal Interface and functionality changes 
03/31/14   tsr     Add support for GSM RX Diversity
03/26/14   sb      Add support for IP2 cal for Dime PM 
08/05/13   cj      Implementation added to receive GSM RSSI in meas mode
05/15/13   svi     Fixed GSM RF FTM failure due to unexpected diag return info. 
05/06/13   aka     Changes to add payload length as a parameter in FRAME SWEEP APIs
04/23/13   ck      Add FTM IQ capture APIs to move from HWTC to FTM 
04/17/13   aka     Changes to support predist override in FTM Mode
04/16/13   svi     Replaced memcpy() by safer memscpy(). 
04/03/13   ec	   Changes how ftm_gsm_rfctl_vars are initialised
04/02/13   aka     Allocate result buffer dynamically to avoid stack overflow
03/28/13   aka     GSM frame sweep feature changes.
02/21/13   tws     Fix KW errors.
02/19/13   tws     The ftm gsm vars are now created during enter_mode.
02/15/13   sb      Added FTM hook to override tuner tune code 
01/18/13   tws     Remove include msm.h
12/05/12   aka     Added rfm_dev in ftm_gsm_get_env_gain_nv
11/27/12   tws     Add device id & as_id to FTM.
11/19/12   aka     Added ftm_gsm_env_gain_nv_data in GSM union
07/20/12   sr      Made changes to pass the RFM_DEVICE to rfc gsm functions 
05/31/12   zc      Changing order of when result.rssi gets populated in FTM_GSM_GET_MEAS_PWR
04/18/12   ars     Modifed FTM frequency adjust logic to apply correction using XO manager API
04/03/12   ggs     Added ftm_gsm_msgr_dispatch_response_handler for handling 
                   response messages sent from the GSM module in RF task 
02/28/12   sb      Added changes to support disabling Idle frame in FTM mode
01/16/12   sr      Made changes not todo idle_frame processing
                   (temp comp and vbatt comp) during calibration.
11/29/11   sb      Added functionality for GSM IQ CAPTURE
11/17/11   av     Added RXCal verification in FTM
09/08/11   rsr     Port Non-IQ cal feature from older targets
08/29/11   sr      KW fixes.
07/07/11   vrb     Support for KV Cal V2
05/09/11   sar     Included ftm_rf_cmd.h for rfa/api cleanup.
03/24/11   vrb     Compiler Warning Fixes
03/16/11   sr      Removed un-used code.
03/11/11   sr      Removed un-used featurization
03/03/11   sr      off-target error fix.
03/03/11   sr      merge from mdm9k branch
31/01/11   sr      Removed un-used code
11/26/10   ggs     Added support for enhanced AMAM cal (swapped waveform)
08/03/10   ggs     Replaced ftm_polar.h with rfgsm_mdsp_cal.h. Replaced
                   ftm_polar_gen_amam_linear_sweep with rfgsm_mdsp_cal_gen_amam_linear_sweep
07/27/10   ggs     Fixed compiler warnings
04/05/10   rsr     Added feature for Multislot RX.
01/20/10   tws     Use PACK(...) instead of PACKED for qdsp6 compatibility.
01/06/10   sar     Renamed ftm_task.h to rf_test_task_v.h.
12/28/09   sr      High/Medium lint errors fix
12/22/09   sr      changed the debug msg macro from legacy to new one.
12/08/09   sr      removed un-used code.
12/02/09   sr      removed im2 related code since we don't need it for now.
10/25/09   dyc     Featurized ftm_polar.h with FEATURE_POLAR
10/12/09   sr      Fixed warnings.
08/21/09   sar     Updated ADC interface for CMI conformance.
03/31/09   sr      include file cleanup
10/26/07   rsr     Added case for FTM_SET_SMPS_PDM_TBL is dispatch function
08/08/07   jfc     Mainlined T_MSM6280 feature
06/13/07   jfc     Added FTM_TX_CS_SWEEP case in ftm_gsm_extended_dispatch()
10/17/06   ycl     Modifications for initial MSM7600 build.
03/28/06   rsr     Lint cleanup.
                   Changed RX burst API to accept the number of frames over which
                   RSSI is averaged.
                   Added new command to perform RX calibration for all gain ranges.
03/15/06   rsr     Added command for TX RSB sweep calibration.
03/14/06   jac     Added dispatch for GSM IM2 Cal command for MSM6280.
02/27/06   ka      Updated for 6280.
05/26/05   jfc     Added additional 5ms wait before returning RSSI.
05/04/05   rsr     Removed warnings.Cleanup
04/14/05   rsr     Made changes to allow external cal to use DSP waveform generation.
03/30/05   lcl     Removed modulator configure API.
03/11/05   ka      Added support for gsm extended commands.
03/04/05   rsr     Cleaned up API's. Added OPLL calibration API.
02/09/05   rsr/lcl Renamed FEATURE_POLAR_RF to FEATURE_POLAR
12/09/04   rsr     Added support for autocalibration
10/18/04   fpe     Removed FTM_HAS_ASYNC_PKT
5/6/04     fpe     Added gsm commands to ftm_id_type and removed ftm_gsm_id_type
12/15/03   jwh     Added resp to ftm_mc_dispatch() for FTM_HAS_ASYNC_PKT.
04/11/03   rsr     Renamed functions with "ftm_" prefix
03/18/03   lcl     Removed MSM6200 specific header file.
03/15/03   lcl     Added FTM_ to ftm_gsm_id_type.
01/09/03   prk     Removed call to dog_report(DOG_ALL_TASKS_RPT).
12/12/02   xw      Commented out gsm_tx_cont_command due to reliability issue
12/10/02   xw      Code clean up
08-21-02   rjr     Created
*/

/*------------------------------------------------------------------------------
                               INCLUDES
------------------------------------------------------------------------------*/

#include "rfa_variation.h"
#include "target.h"    /* Target specific definitions            */
#include "comdef.h"    /* Common stuff                           */
#include "stringl.h"
#include "rfcom.h"
#include "ftmicap.h"
#include "ftm_rf_cmd.h"
#include "ftm_msg.h"

#ifdef FEATURE_FACTORY_TESTMODE

#ifdef FEATURE_GSM

#include "ftm.h"
#include "ftm_gsm_dispatch.h"
#include "ftm_gsm_rfctl.h"
#include "ftm_common.h"
#include "ftm_common_control.h"
#include "ftm_gsm.h"
#include "ftm_gsm_prot.h"
#include "ftm_gsm_rfprot.h"
#include "rf_test_task_v.h"
#include "rfgsm_mdsp_cal.h" /* Replaces ftm_polar.h  */
#include "dog.h"
#include "rfc_card_gsm.h"
#include "ftm_common_concurrency_manager.h"
#include "ftm_gsm_device.h"
#include "ftm_gsm_ctl.h"
#include "qurt.h"
#include "npa.h"
#include "rfgsm_mc.h"
#include "rxlm_intf.h"
#include "txlm_intf.h"
#include "rfgsm_core_intf.h"



/*=============================================================================

                                Macros

=============================================================================*/
/*! @def FTM_GSM_ENH_DBG_CMD_MSG                                             */
/*! @brief Common command message to print details of subscription, cmd ID   */
/*!        and message specific to command                                   */
#define FTM_GSM_ENH_DBG_CMD_MSG(msg) \
          MSG_2( MSG_SSID_RF, MSG_LEGACY_HIGH, \
                 "|SUB %d| FTM GSM Enh Debug Dispatch %d " msg, sub_id, cmd_ptr->ftm_hdr.cmd_id );



/*=============================================================================

                              DEFINITIONS

=============================================================================*/
typedef PACK(union)
{
  ftm_gsm_extended_result_type     gen_result;
  ftm_gsm_detector_cal_result_type det_result;
  ftm_gsm_amam_sweep_return_type amam_result;
  ftm_gsm_rssi_to_dbm_return_type rssi_to_dbm_result; /*Added from SCMM*/
  ftm_gsm_rssi_return_type rssi_result;
  ftm_gsm_env_gain_nv_data_type  ftm_gsm_env_gain_nv_data;
  ftm_gsm_frame_sweep_result_buffer_type frame_sweep_result;
  ftm_gsm_rssi_prx_drx_return_type rssi_prx_drx_result;
} ftm_gsm_extended_result_union;



/*===========================================================================*/
/*! @union ftm_gsm_enh_debug_result_union                                    */
/*! @brief union for defining ftm enh debug dispatch response packet types   */
/*! @details this union contains all the types for each different response to*/
/*!          be handled in the ftm enh debug dispatcher where the response   */
/*!          may carry details of execution time, rflm data, etc             */
typedef PACK(union)
{
  ftm_gsm_enh_debug_generic_result_type generic_result;
  /* @TODO add more return response types here */
}ftm_gsm_enh_debug_result_union;



/*===========================================================================*/
/*! @enum ftm_gsm_enh_debug_cmd_id                                           */
/*! @brief Command IDs to trigger specific commands to RF GSM and RFLM GSM   */
/*! @details The enum value used in the FTM request packet will trigger a    */
/*!          specific internal FTM GSM function to either directly call or   */
/*!          send a MSGR command to RFLM or RF GSM, respectively.            */
typedef enum
{
  /* RF CONFIGURATION - IDs 0-9 */
  FTM_GSM_INIT_RF                    = 0,
  FTM_GSM_CMD_PROC_SET_CLK_SPEED     = 1,
  FTM_GSM_CMD_PROC_SET_BIMC_RATE     = 2,
  FTM_GSM_CMD_PROC_READ_MEM_LATENCY  = 3,
  FTM_GSM_INIT_MSGR_CLIENT           = 4,
  FTM_GSM_DEINIT_MSGR_CLIENT         = 5,

  /* RFLM HANDLES - IDs 10-19 */
  FTM_GSM_ALLOCATE_RXLM_HANDLE       = 10,
  FTM_GSM_DEALLOCATE_RXLM_HANDLE     = 11,
  FTM_GSM_ALLOCATE_TXLM_HANDLE       = 12,
  FTM_GSM_DEALLOCATE_TXLM_HANDLE     = 13,

  /* RF GSM CMD PROC CALLS - IDs 20-49 */
  FTM_GSM_START_GSM                  = 20,
  FTM_GSM_STOP_GSM                   = 21,
  FTM_GSM_RX_WAKEUP                  = 22,
  FTM_GSM_TX_WAKEUP                  = 23,
  FTM_GSM_RX_SLEEP                   = 24,
  FTM_GSM_TX_SLEEP                   = 25,
  FTM_GSM_CMD_PROC_RX_BURST          = 26,
  FTM_GSM_CMD_PROC_SET_TX_BAND       = 27,
  FTM_GSM_CMD_PROC_TX_BURST          = 28,
  FTM_GSM_CMD_PROC_IDLE_FRAME_PROC   = 29,
  FTM_GSM_CMD_PROC_SET_ANTENNA       = 30,

  FTM_GSM_CMD_PROC_CM_ENTER          = 31,
  FTM_GSM_CMD_PROC_CM_EXIT           = 32,

  FTM_GSM_CMD_PROC_SET_SAR_LIMIT     = 33,
  FTM_GSM_CMD_PROC_SET_VBATT_LIMIT   = 34,

  /* RFM MEAS IRAT APIS - IDs 50-69 */
  FTM_GSM_CMD_INLINE_MEAS_INIT_ENTER_PARAMS = 50,
  FTM_GSM_CMD_INLINE_MEAS_INIT_SETUP_PARAMS = 51,
  FTM_GSM_CMD_INLINE_MEAS_INIT_EXIT_PARAMS = 52,
  FTM_GSM_CMD_INLINE_MEAS_INIT_RF_HEADER = 53,
  FTM_GSM_CMD_INLINE_MEAS_INIT_PARAMS = 54,
  FTM_GSM_CMD_INLINE_MEAS_ENTER      = 55,
  FTM_GSM_CMD_INLINE_MEAS_EXIT       = 56,
  FTM_GSM_CMD_INLINE_MEAS_BUILD_SCRIPTS = 57,
  FTM_GSM_CMD_INLINE_MEAS_GET_IRAT_INFO = 58,

  /* RFM MEAS IRAT APIS - IDs 70-79 */
  FTM_GSM_CMD_SUB_SM_TEST_INIT      = 70,
  FTM_GSM_CMD_SUB_SM_TEST_DESTROY   = 71,
  FTM_GSM_CMD_SUB_SM_TEST_DRIVE     = 72,
  FTM_GSM_CMD_RX_SM_TEST_INIT       = 73,
  FTM_GSM_CMD_RX_SM_TEST_DESTROY    = 74,
  FTM_GSM_CMD_RX_SM_TEST_DRIVE      = 75,
  FTM_GSM_CMD_TX_SM_TEST_INIT       = 76,
  FTM_GSM_CMD_TX_SM_TEST_DESTROY    = 77,
  FTM_GSM_CMD_TX_SM_TEST_DRIVE      = 78,

  /* RFLM GSM CALLS - IDs 100 - 109 */
  FTM_GSM_RFLM_GET_TX_ALPHA         = 100,
  FTM_GSM_RFLM_GET_RX_ALPHA         = 101,
  FTM_GSM_RFLM_GET_RF_GAIN          = 102,
  FTM_GSM_RFLM_GET_KV_DATA          = 103,
  FTM_GSM_RFLM_GET_RSB_DATA         = 104,
  FTM_GSM_RFLM_GET_LIF_DATA         = 105,

  FTM_GSM_RFLM_REG_CB               = 106,
  FTM_GSM_RFLM_DEREG_CB             = 107,

  /* RFLM GSM TXFE CALLS - IDs 110 - 111 */
  FTM_GSM_RFLM_PROG_TXFE            = 110,
  FTM_GSM_RFLM_TX_FRAME_CONFIG      = 111,
  
  /* RFLM GSM INIT/DE-INIT CALLS - IDs 112 - 113 */
  FTM_GSM_RFLM_START                = 112,
  FTM_GSM_RFLM_RELEASE              = 113,

  /* RFLM CCS CALLS - IDs 114 - 129 */
  FTM_GSM_RFLM_PROG_CCS_RX_WAKEUP   = 114,
  FTM_GSM_RFLM_PROG_CCS_TX_WAKEUP   = 115,
  FTM_GSM_RFLM_PROG_CCS_RX          = 116,
  FTM_GSM_RFLM_PROG_CCS_TX          = 117,
  FTM_GSM_RFLM_PROG_CCS_TX_BAND     = 118,
  FTM_GSM_RFLM_PROG_CCS_THERM       = 119,

  /* DEBUG PRINT CALLS - IDs 130 - 139 */
  FTM_GSM_RFLM_PRINT_EVENT_DETAILS  = 130,
  FTM_GSM_RFLM_PRINT_CCS_TQ_DATA    = 131,
  FTM_GSM_CORE_SET_DEBUG_FLAGS      = 132,

  FTM_GSM_ENH_DEBUG_CMD_MAX
}ftm_gsm_enh_debug_cmd_id;



/* Max number of subscriptions supported */
#define FTM_GSM_ENH_DEBUG_MAX_SUBS 2

/* Max clock speed in KHz */
/* This ensures tool does not try and vote for clk speed higher than capable */
/* Note that npa do not have a definition for max clock speed */
#define FTM_GSM_MAX_CLK_SPEED_KHZ 844800

/* Max BIMC rata in Kbps */
/* This ensures tool does not try and vote for bimc rate higher than capable */
#define FTM_GSM_MAX_BIMC_RATE_KBPS 10000000

/* Invalid RFLM handle */
/* This ensures only one handle for Rx and Tx is created per sub */
#define FTM_GSM_RFLM_HANDLE_INVALID -1


/* Number of entries in the confirmation list tables for MSGR client */
#define FTM_GSM_ENH_DEBUG_CNF_DIAG_CNT \
                (sizeof(ftm_gsm_enh_debug_umid_cnf_list)/sizeof(msgr_umid_type))
#define FTM_GSM_ENH_DEBUG_SUB2_CNF_DIAG_CNT \
                (sizeof(ftm_gsm_enh_debug_umid_sub2_cnf_list)/sizeof(msgr_umid_type))

/* Number of bands supported by the enh debug tool */
#define FTM_GSM_ENH_DEBUG_MAX_BANDS 4



/*=============================================================================

                             DECLARATIONS

=============================================================================*/
ftm_gsm_meas_results g_meas_results = {FALSE, 0};


int16 ftm_gsm_rssi_to_dbm = 0; /*Added from SCMM*/


/* Flag to determine whether to use GL1 cnfs or FTM enh debug cnfs */
boolean ftm_gsm_enh_debug_msgr_client = FALSE;

/* client variable to create message route client for RF GSM command cnfs */
static msgr_client_t rfgsm_ftm_dispatch_msgr_client;

/* UMID list for sub 1 FTM enh debug dispatcher */
static msgr_umid_type ftm_gsm_enh_debug_umid_cnf_list[] = 
{
  RFA_RF_GSM_FTM_START_GSM_CNF,
  RFA_RF_GSM_FTM_STOP_GSM_CNF,
  RFA_RF_GSM_FTM_RX_WAKEUP_CNF,
  RFA_RF_GSM_FTM_TX_WAKEUP_CNF,
  RFA_RF_GSM_FTM_RX_SLEEP_CNF,
  RFA_RF_GSM_FTM_TX_SLEEP_CNF,
  RFA_RF_GSM_FTM_RX_BURST_CNF,
  RFA_RF_GSM_FTM_SET_TX_BAND_CNF,
  RFA_RF_GSM_FTM_TX_BURST_CNF,
  RFA_RF_GSM_FTM_IDLE_FRAME_PROCESSING_CNF,
  RFA_RF_GSM_FTM_CM_ENTER_CNF,
  RFA_RF_GSM_FTM_CM_EXIT_CNF,
};

/* UMID list for sub 2 FTM enh debug dispatcher */
static msgr_umid_type ftm_gsm_enh_debug_umid_sub2_cnf_list[] = 
{
  RFA_RF_GSM_FTM_START_GSM_SUB2_CNF,
  RFA_RF_GSM_FTM_STOP_GSM_SUB2_CNF,
  RFA_RF_GSM_FTM_RX_WAKEUP_SUB2_CNF,
  RFA_RF_GSM_FTM_TX_WAKEUP_SUB2_CNF,
  RFA_RF_GSM_FTM_RX_SLEEP_SUB2_CNF,
  RFA_RF_GSM_FTM_TX_SLEEP_SUB2_CNF,
  RFA_RF_GSM_FTM_RX_BURST_SUB2_CNF,
  RFA_RF_GSM_FTM_SET_TX_BAND_SUB2_CNF,
  RFA_RF_GSM_FTM_TX_BURST_SUB2_CNF,
  RFA_RF_GSM_FTM_IDLE_FRAME_PROCESSING_SUB2_CNF,
  RFA_RF_GSM_FTM_CM_ENTER_SUB2_CNF,
  RFA_RF_GSM_FTM_CM_EXIT_SUB2_CNF,
};

/* Confirmation types union for MSGR client registration */
typedef union
{
  rfa_rf_gsm_start_gsm_cnf_s              rfa_rf_gsm_start_gsm_cnf;
  rfa_rf_gsm_stop_gsm_cnf_s               rfa_rf_gsm_stop_gsm_cnf;
  rfa_rf_gsm_rx_wakeup_cnf_s              rfa_rf_gsm_rx_wakeup_cnf;
  rfa_rf_gsm_tx_wakeup_cnf_s              rfa_rf_gsm_tx_wakeup_cnf;
  rfa_rf_gsm_rx_sleep_cnf_s               rfa_rf_gsm_rx_sleep_cnf;
  rfa_rf_gsm_tx_sleep_cnf_s               rfa_rf_gsm_tx_sleep_cnf;
  rfa_rf_gsm_rx_burst_cnf_s               rfa_rf_gsm_rx_burst_cnf;
  rfa_rf_gsm_set_tx_band_cnf_s            rfa_rf_gsm_set_tx_band_cnf;
  rfa_rf_gsm_tx_burst_cnf_s               rfa_rf_gsm_tx_burst_cnf;
  rfa_rf_gsm_idle_frame_processing_cnf_s  rfa_rf_gsm_idle_frame_processing_cnf;
  rfa_rf_gsm_cm_enter_cnf_s               rfa_rf_gsm_cm_enter_cnf;
  rfa_rf_gsm_cm_exit_cnf_s                rfa_rf_gsm_cm_exit_cnf;
}ftm_gsm_enh_debug_msgr_cnfs_u;



/* Input vars union for RF and RFLM APIs */
typedef union
{
  rflm_gsm_get_rx_alpha_input_type              rx_alpha_in_vars;
  rflm_gsm_get_tx_alpha_input_type              tx_alpha_in_vars;
  rflm_gsm_get_rf_gain_input_type               rf_gain_in_vars;
  rflm_gsm_tx_ramp_deltas_input_type            tx_ramp_deltas_in_vars;
  rflm_gsm_tx_kv_data_input_type                tx_kv_data_in_vars;
  rflm_gsm_spec_inv_data_input_type             spec_inv_data_in_vars;
  rflm_gsm_tx_cal_dma_data_input_type           tx_cal_dma_data_in_vars;
  rflm_gsm_rx_frame_input_type                  rx_frame_in_vars;
  rflm_gsm_tx_frame_input_type                  tx_frame_in_vars;
  rflm_gsm_txc_prog_amxm_input_type             txc_prog_amxm_in_vars;
  rflm_gsm_txc_frame_config_input_type          txc_frame_config_in_vars;
  rflm_gsm_rxfe_input_type                      rxfe_in_vars;
  rflm_gsm_txfe_input_type                      txfe_in_vars;
  rflm_gsm_tx_cal_step_txfe_input_type          tx_cal_step_in_vars;
  rflm_gsm_start_input_type                     start_in_vars;
  rflm_gsm_release_input_type                   release_in_vars;
  rflm_gsm_init_ccs_input_type                  init_in_vars;
  rflm_gsm_rx_wakeup_ccs_input_type             rx_wakeup_in_vars;
  rflm_gsm_tx_wakeup_ccs_input_type             tx_wakeup_in_vars;
  rflm_gsm_rx_ccs_input_type                    rx_in_vars;
  rflm_gsm_tx_ccs_input_type                    tx_in_vars;
  rflm_gsm_tx_band_ccs_input_type               tx_band_in_vars;
  rflm_gsm_temp_comp_ccs_input_type             temp_comp_in_vars;
  rflm_gsm_tx_cal_ccs_step_input_type           tx_cal_in_vars;
  rflm_gsm_irat_ccs_input_type                  irat_in_vars;
  rflm_gsm_init_umplt_input_type                init_umplt_in_vars;
  rflm_gsm_ip2_cal_step_ccs_input_type          ip2_step_in_vars;
  rflm_gsm_ip2_cal_setup_cleanup_ccs_input_type ip2_setup_cleanup_in_vars;
  rflm_gsm_prog_jdet_read_ccs_input_type        ip2_jdet_read_ccs_in_vars;
  rflm_gsm_get_jdet_reading_input_type          ip2_jdet_read_in_vars;
}ftm_gsm_enh_debug_in_vars_u;



/* Output vars union for RF and RFLM APIs */
typedef union
{
  rflm_gsm_get_rx_alpha_output_type              rx_alpha_out_vars;
  uint16                                         tx_alpha_out_vars;
  rflm_gsm_get_rf_gain_output_type               rf_gain_out_vars;
  rflm_gsm_tx_ramp_deltas_output_type            tx_ramp_deltas_out_vars;
  rflm_gsm_tx_kv_data_output_type                tx_kv_data_out_vars;
  rflm_gsm_spec_inv_data_output_type             spec_inv_data_out_vars;
  rflm_gsm_tx_cal_dma_data_output_type           tx_cal_dma_data_out_vars;
  rflm_gsm_rx_frame_output_type                  rx_frame_out_vars;
  rflm_gsm_tx_frame_output_type                  tx_frame_out_vars;
  rflm_gsm_txc_prog_amxm_output_type             txc_prog_amxm_out_vars;
  rflm_gsm_txc_frame_config_output_type          txc_frame_config_out_vars;
  rflm_gsm_rxfe_output_type                      rxfe_out_vars;
  rflm_gsm_txfe_output_type                      txfe_out_vars;
  rflm_gsm_tx_cal_step_txfe_output_type          tx_cal_step_out_vars;
  rflm_gsm_start_output_type                     start_out_vars;
  rflm_gsm_release_output_type                   release_out_vars;
  rflm_gsm_init_ccs_output_type                  init_out_vars;
  rflm_gsm_rx_wakeup_ccs_output_type             rx_wakeup_out_vars;
  rflm_gsm_tx_wakeup_ccs_output_type             tx_wakeup_out_vars;
  rflm_gsm_rx_ccs_output_type                    rx_out_vars;
  rflm_gsm_tx_ccs_output_type                    tx_out_vars;
  rflm_gsm_tx_band_ccs_output_type               tx_band_out_vars;
  rflm_gsm_temp_comp_ccs_output_type             temp_comp_out_vars;
  rflm_gsm_tx_cal_ccs_step_output_type           tx_cal_out_vars;
  rflm_gsm_irat_ccs_output_type                  irat_out_vars;
  rflm_gsm_init_umplt_output_type                init_umplt_out_vars;
  rflm_gsm_ip2_cal_step_ccs_output_type          ip2_step_out_vars;
  rflm_gsm_ip2_cal_setup_cleanup_ccs_output_type ip2_setup_cleanup_out_vars;
  rflm_gsm_prog_jdet_read_ccs_output_type        ip2_jdet_read_ccs_out_vars;
  rflm_gsm_get_jdet_reading_output_type          ip2_jdet_read_out_vars;
}ftm_gsm_enh_debug_out_vars_u;



/* Map the value from tools used for GSM band to SYS band for cmd processing */
/* To ensure tool doesn't overflow this array we do bitwise AND of           */
/* (FTM_GSM_ENH_DEBUG_MAX_BANDS-1) to mask                                   */
rfgsm_band_type ftm_gsm_tool_band_to_sys_band[FTM_GSM_ENH_DEBUG_MAX_BANDS] = 
{
  RFGSM_BAND_GSM850, //tool input 0
  RFGSM_BAND_GSM900, //tool input 1
  RFGSM_BAND_GSM1800,//tool input 2
  RFGSM_BAND_GSM1900 //tool input 3
};


/* This variable is used to emulate the presence of interface index GFW      */
/* shared memory where usually a pointer would be passed from GL1            */
/* @TODO Note that the GFW shared mem is quite small so global is fine here  */
/*       However, would be better to dynamically allocate memory in init     */
/*       function                                                            */
GfwRfEventTable ftm_gsm_gfw_shared_mem[FTM_GSM_ENH_DEBUG_MAX_SUBS] = {0};


/* This variable is used to emulate the presence of GL1 timing var */
rfgsm_timing_info_type timing_info;


/* This variable is used to emulate the presence of GL1 gain var */
rf_gain_T gain;


/* These variables are used to emulate the presence of GL1 local vars        */
/* populated. These are global to ensure memory still available for          */
/* non-blocking calls to RF GSM command proc                                 */
rfa_rf_gsm_rx_burst_type rx_burst_parameters;
rfa_rf_gsm_tx_burst_type tx_burst_parameters;

/* Initialise the PRx rxlm handles to invalid */
static uint32 rxlm_handle_prx[FTM_GSM_ENH_DEBUG_MAX_SUBS] = 
{
  FTM_GSM_RFLM_HANDLE_INVALID, FTM_GSM_RFLM_HANDLE_INVALID
};

/* Initialise the DRx rxlm handles to invalid */
static uint32 rxlm_handle_drx[FTM_GSM_ENH_DEBUG_MAX_SUBS] = 
{
  FTM_GSM_RFLM_HANDLE_INVALID, FTM_GSM_RFLM_HANDLE_INVALID
};

/* Initialise the txlm handles to invalid */
static uint32 txlm_handle[FTM_GSM_ENH_DEBUG_MAX_SUBS] = 
{
  FTM_GSM_RFLM_HANDLE_INVALID, FTM_GSM_RFLM_HANDLE_INVALID
};



/*=============================================================================

                           FUNCTION PROTOTYPES

=============================================================================*/
extern void ftm_gsm_configure_and_start_polar_tx_sweep_cal(ftm_gsm_sweep_waveform_type *waveform_ptr,
                                                           uint16 num_steps);

/* Enhanced Debug commands - internal use only */
static uint32 ftm_gsm_enh_debug_set_clk( uint32 clk_speed_khz );
static uint32 ftm_gsm_enh_debug_set_bimc( uint32 bimc_rate_kbps );
static uint32 ftm_gsm_enh_debug_rd_mem_latency( void );
static void ftm_gsm_enh_debug_dccleaninva( void * addr );
static uint32 ftm_gsm_enh_debug_reg_msgr_client( boolean reg_client );
static uint32 ftm_gsm_enh_debug_rxlm_handle( uint8 sub_id, boolean alloc );
static uint32 ftm_gsm_enh_debug_txlm_handle( uint8 sub_id, boolean alloc );
static uint32 ftm_gsm_enh_debug_start_gsm_cmd( uint8 sub_id, 
                                               ftm_gsm_cmd_api_cmn_driver_type * params );
static uint32 ftm_gsm_enh_debug_stop_gsm_cmd( uint8 sub_id, 
                                               ftm_gsm_cmd_api_cmn_driver_type * params );
static uint32 ftm_gsm_enh_debug_rx_wakeup_cmd( uint8 sub_id, 
                                               ftm_gsm_cmd_api_cmn_driver_type * params );
static uint32 ftm_gsm_enh_debug_tx_wakeup_cmd( uint8 sub_id, 
                                               ftm_gsm_cmd_api_cmn_driver_type * params );
static uint32 ftm_gsm_enh_debug_rx_sleep_cmd( uint8 sub_id, 
                                              ftm_gsm_cmd_api_cmn_driver_type * params );
static uint32 ftm_gsm_enh_debug_tx_sleep_cmd( uint8 sub_id, 
                                              ftm_gsm_cmd_api_cmn_driver_type * params );
static uint32 ftm_gsm_enh_debug_rx_burst_cmd( uint8 sub_id, 
                                              ftm_gsm_cmd_api_cmn_driver_type * params,
                                              ftm_gsm_cmd_api_rx_driver_type * rx_params );
static uint32 ftm_gsm_enh_debug_set_tx_band_cmd( uint8 sub_id, 
                                                 ftm_gsm_cmd_api_cmn_driver_type * params,
                                                 ftm_gsm_cmd_api_tx_driver_type * tx_params );
static uint32 ftm_gsm_enh_debug_tx_burst_cmd( uint8 sub_id, 
                                              ftm_gsm_cmd_api_cmn_driver_type * params,
                                              ftm_gsm_cmd_api_tx_driver_type * tx_params );
static void ftm_gsm_enh_debug_set_antenna_req( uint8 sub_id, 
                                               ftm_gsm_cmd_api_antenna_driver_type * ant_params );



/*=============================================================================

                         FUNCTION DEFINITIONS

=============================================================================*/
/*!
   @fn 
   ftm_gsm_enh_debug_set_clk

   @brief
   This function sets the clock speed according to the input value

   @param clk_speed_khz - clock speed to vote for in KHz

   @retval
   Response packet execution time us
*/
uint32 ftm_gsm_enh_debug_set_clk( uint32 clk_speed_khz )
{
  static npa_client_handle npa_handle_q6clk = NULL;
  static uint32 curr_clk_speed_khz = 0;

  /* Get profiling start data */
  uint32 start_ustmr = rfgsm_mc_read_ustmr_count();

  /* Create an NPA client for Q6 if not done before */
  if( npa_handle_q6clk == NULL )
  {
    npa_handle_q6clk = npa_create_sync_client("/clk/cpu", "RF_FTM", NPA_CLIENT_REQUIRED);
  }

  /* If client is successfully created the vote for new clk speed */
  if( npa_handle_q6clk != NULL )
  {
    /* Vote for new clk speed only if not same value */
    if( (clk_speed_khz <= FTM_GSM_MAX_CLK_SPEED_KHZ) && 
        (clk_speed_khz != curr_clk_speed_khz) )
    {
      npa_issue_required_request(npa_handle_q6clk, clk_speed_khz);
      MSG_1( MSG_SSID_RF, MSG_LEGACY_MED, "FTM GSM voted clock speed: %d", clk_speed_khz );
      curr_clk_speed_khz = clk_speed_khz;
    }
  }
  else
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, 
         "FTM GSM q6 npa client NULL and failed to create handle");
  }

  /* Get profiling stop data */
  return rfgsm_ustmr_count_diff_a_b_us_rtn( start_ustmr, rfgsm_mc_read_ustmr_count() );
}



/*---------------------------------------------------------------------------*/
/*!
   @fn 
   ftm_gsm_enh_debug_set_bimc

   @brief
   This function sets the Bus Integrated Memory Controller rate according to 
   the input value

   @param bimc_rate_kbps - bimc rate to vote for in Kbps

   @retval
   Response packet execution time us
*/
uint32 ftm_gsm_enh_debug_set_bimc( uint32 bimc_rate_kbps )
{
  static npa_client_handle npa_handle_bimc_rate = NULL;
  static uint32 curr_bimc_rate_khz = 0;

  /* Get profiling start data */
  uint32 start_ustmr = rfgsm_mc_read_ustmr_count();

  /* Create an NPA client for Q6 if not done before */
  if( npa_handle_bimc_rate == NULL )
  {
    npa_handle_bimc_rate = npa_create_sync_client("/mcpm/bus/bimc", "RF_FTM", NPA_CLIENT_REQUIRED);
  }

  /* If client is successfully created the vote for new BIMC rate */
  if( npa_handle_bimc_rate != NULL )
  {
    /* Vote for new BIMC rate only if not same value */
    if( (bimc_rate_kbps <= FTM_GSM_MAX_BIMC_RATE_KBPS) && 
        (bimc_rate_kbps != curr_bimc_rate_khz) )
    {
      npa_issue_required_request(npa_handle_bimc_rate, bimc_rate_kbps);
      MSG_1( MSG_SSID_RF, MSG_LEGACY_MED, "FTM GSM voted BIMC rate: %d", bimc_rate_kbps );
      curr_bimc_rate_khz = bimc_rate_kbps;
    }
  }
  else
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, 
         "FTM GSM BIMC npa client NULL and failed to create handle");
  }

  /* Get profiling stop data */
  return rfgsm_ustmr_count_diff_a_b_us_rtn( start_ustmr, rfgsm_mc_read_ustmr_count() );
}



/*---------------------------------------------------------------------------*/
/*!
   @fn 
   ftm_gsm_enh_debug_dccleaninva

   @brief
   This function performs a cache line clean and invalidation which is a
   measure of how long it takes to go over the AXI for caching data when we
   get a cache miss which adds delay to execution time of functionality

   @param void

   @retval
   Response packet memory latency ns
*/
void ftm_gsm_enh_debug_dccleaninva( void * addr )
{
  asm volatile ("dccleaninva(%[addr])" : : [addr] "r" (addr) : "memory" );
}



/*---------------------------------------------------------------------------*/
/*!
   @fn 
   ftm_gsm_enh_debug_rd_mem_latency

   @brief
   This function sets calculates the memory latency based on a cache clean and
   invalidate execution

   @param void

   @retval
   Response packet memory latency ns
*/
uint32 ftm_gsm_enh_debug_rd_mem_latency( void )
{
  uint32 index;
  static volatile uint8 ftm_gsm_mem_test_var;

  /* Get profiling start data */
  uint32 start_ustmr = rfgsm_mc_read_ustmr_count();

  /* Loop cache line clean and invalidate 1024 interations to get average */
  /* 1000 allows for a good USTMR resolution estimation down to 1ns */
  for( index = 0; index < 1000; index++ )
  {
    ftm_gsm_enh_debug_dccleaninva((void*)&ftm_gsm_mem_test_var);
  }

  /* Get profile stop data */
  /* The value returned is us/1000 because 1000 iterations */
  return rfgsm_ustmr_count_diff_a_b_us_rtn( start_ustmr, rfgsm_mc_read_ustmr_count() );
}



/*---------------------------------------------------------------------------*/
/*!
   @fn 
   ftm_gsm_enh_debug_reg_msgr_client

   @brief
   This function registers/de-registers the msgr client to receive cnf from 
   RFGSM CMD processing.
   The commands registered must be unique to FTM enh debug dispatcher because 
   GL1 have already registered to receive the standard cnfs

   @param reg_client - register or deregister the client

   @retval
   Response packet execution time us
*/
uint32 ftm_gsm_enh_debug_reg_msgr_client( boolean reg_client )
{
  static msgr_id_t rfgsm_ftm_dispatch_msgr_msg_id;
  errno_enum_type result;

  /* Get profiling start data */
  uint32 start_ustmr = rfgsm_mc_read_ustmr_count();

  if( reg_client == TRUE )
  {
    /* Create a message router client */
    result = msgr_client_create( &rfgsm_ftm_dispatch_msgr_client );
    MSG_1( MSG_SSID_RF, MSG_LEGACY_MED, "FTM GSM Create MSGR client result %d", result);

    /* Create a message router message queue */
    result = msgr_client_add_mq( "RF GSM FTM Enh Dispatch Mailbox",
                                 &rfgsm_ftm_dispatch_msgr_client,
                                 1, //Priority
                                 (uint32)3, //Depth
                                 (uint32)sizeof(ftm_gsm_enh_debug_msgr_cnfs_u),
                                 &rfgsm_ftm_dispatch_msgr_msg_id );
    MSG_1( MSG_SSID_RF, MSG_LEGACY_MED, "FTM GSM Create MSGR client msg q result %d", result);

    /* Register a message router block for subscription 1 */
    result = msgr_register_block( MSGR_RFA_RF_GSM_FTM,
                                  &rfgsm_ftm_dispatch_msgr_client,
                                  rfgsm_ftm_dispatch_msgr_msg_id,
                                  ftm_gsm_enh_debug_umid_cnf_list,
                                  (uint32)FTM_GSM_ENH_DEBUG_CNF_DIAG_CNT);
    MSG_1( MSG_SSID_RF, MSG_LEGACY_MED, "FTM GSM Create MSGR client register SUB1 msg block result %d", result);

    /* Register a message router block for subscription 2 */
    result = msgr_register_block( MSGR_RFA_RF_GSM_FTM,
                                  &rfgsm_ftm_dispatch_msgr_client,
                                  rfgsm_ftm_dispatch_msgr_msg_id,
                                  ftm_gsm_enh_debug_umid_sub2_cnf_list,
                                  (uint32)FTM_GSM_ENH_DEBUG_SUB2_CNF_DIAG_CNT);
    MSG_1( MSG_SSID_RF, MSG_LEGACY_MED, "FTM GSM Create MSGR client register SUB2 msg block result %d", result);
    
    /* Use FTM enh debug confirmations */
    ftm_gsm_enh_debug_msgr_client = TRUE;
  }
  else
  {
    /* Delete the message router client */
    result = msgr_client_delete( &rfgsm_ftm_dispatch_msgr_client );
    MSG_1( MSG_SSID_RF, MSG_LEGACY_MED, "FTM GSM Delete MSGR client result %d", result);
    
    /* Use GL1 confirmations */
    ftm_gsm_enh_debug_msgr_client = FALSE;
  }

  /* Get profiling stop data */
  return rfgsm_ustmr_count_diff_a_b_us_rtn( start_ustmr, rfgsm_mc_read_ustmr_count() );
}



/*---------------------------------------------------------------------------*/
/*!
   @fn 
   ftm_gsm_enh_debug_rxlm_handle

   @brief
   This function allocates/deallocates a rxlm handle for a subscription

   @param sub_id - subscription to allocate/deallocate handle for
   @param alloc - allocate/deallocate handle

   @retval
   Response packet execution time us
*/
uint32 ftm_gsm_enh_debug_rxlm_handle( uint8 sub_id, boolean alloc )
{
  /* Get profiling start data */
  uint32 start_ustmr = rfgsm_mc_read_ustmr_count();

  if( sub_id < FTM_GSM_ENH_DEBUG_MAX_SUBS )
  {
    if (alloc == TRUE) 
    {
      /* Allocate the PRx RFLM handle */
      if( rxlm_handle_prx[sub_id] != FTM_GSM_RFLM_HANDLE_INVALID )
      {
        MSG_2( MSG_SSID_RF, MSG_LEGACY_HIGH, 
               "FTM GSM RxLM PRx allocate failed as handle %d already created for sub %d", 
               rxlm_handle_prx[sub_id], sub_id );
      }
      else
      {
        (void)rflm_allocate_rx_handle(LM_GSM, LM_GSM, &rxlm_handle_prx[sub_id]); 
      }

      /* Allocate the DRx RFLM handle */
      if( rxlm_handle_drx[sub_id] != FTM_GSM_RFLM_HANDLE_INVALID )
      {
        MSG_2( MSG_SSID_RF, MSG_LEGACY_HIGH, 
               "FTM GSM RxLM DRx allocate failed as handle %d already created for sub %d", 
               rxlm_handle_drx[sub_id], sub_id );
      }
      else
      {
        (void)rflm_allocate_rx_handle(LM_GSM, LM_GSM, &rxlm_handle_drx[sub_id]); 
      }
    }
    else
    {
      /* Deallocate the PRx RFLM handle */
      if( rxlm_handle_prx[sub_id] == FTM_GSM_RFLM_HANDLE_INVALID )
      {
        MSG_2( MSG_SSID_RF, MSG_LEGACY_HIGH, 
               "FTM GSM RxLM PRx deallocate failed as handle %d already destroyed for sub %d", 
               rxlm_handle_prx[sub_id], sub_id );
      }
      else
      {
        (void)rflm_deallocate_rx_handle(LM_GSM, LM_GSM, rxlm_handle_prx[sub_id]);
        rxlm_handle_prx[sub_id] = FTM_GSM_RFLM_HANDLE_INVALID;
      }

      /* Deallocate the DRx RFLM handle */
      if( rxlm_handle_drx[sub_id] == FTM_GSM_RFLM_HANDLE_INVALID )
      {
        MSG_2( MSG_SSID_RF, MSG_LEGACY_HIGH, 
               "FTM GSM RxLM DRx deallocate failed as handle %d already destroyed for sub %d", 
               rxlm_handle_drx[sub_id], sub_id );
      }
      else
      {
        (void)rflm_deallocate_rx_handle(LM_GSM, LM_GSM, rxlm_handle_drx[sub_id]);
        rxlm_handle_drx[sub_id] = FTM_GSM_RFLM_HANDLE_INVALID;
      }
    }
  }
  else
  {
    MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, 
           "FTM GSM RxLM alloc/dealloc failed invalid sub %d", 
           sub_id );
  }

  /* Get profiling stop data */
  return rfgsm_ustmr_count_diff_a_b_us_rtn( start_ustmr, rfgsm_mc_read_ustmr_count() );
}



/*---------------------------------------------------------------------------*/
/*!
   @fn 
   ftm_gsm_enh_debug_txlm_handle

   @brief
   This function allocates/deallocates a txlm handle for a subscription

   @param sub_id - subscription to allocate/deallocate handle for
   @param alloc - allocate/deallocate handle

   @retval
   Response packet execution time us
*/
uint32 ftm_gsm_enh_debug_txlm_handle( uint8 sub_id, boolean alloc )
{
  /* Get profiling start data */
  uint32 start_ustmr = rfgsm_mc_read_ustmr_count();

  if( sub_id < FTM_GSM_ENH_DEBUG_MAX_SUBS )
  {
    if (alloc == TRUE) 
    {
      if( txlm_handle[sub_id] != FTM_GSM_RFLM_HANDLE_INVALID )
      {
        MSG_2( MSG_SSID_RF, MSG_LEGACY_HIGH, 
               "FTM GSM TxLM allocate failed as handle %d already created for sub %d", 
               txlm_handle[sub_id], sub_id );
      }
      else
      {
        (void)rflm_allocate_tx_handle(LM_GSM, LM_GSM, &txlm_handle[sub_id]); 
      }
    }
    else
    {
      if( txlm_handle[sub_id] == FTM_GSM_RFLM_HANDLE_INVALID )
      {
        MSG_2( MSG_SSID_RF, MSG_LEGACY_HIGH, 
               "FTM GSM TxLM deallocate failed as handle %d already destroyed for sub %d", 
               txlm_handle[sub_id], sub_id );
      }
      else
      {
        (void)rflm_deallocate_tx_handle(LM_GSM, LM_GSM, txlm_handle[sub_id]);
        txlm_handle[sub_id] = FTM_GSM_RFLM_HANDLE_INVALID;
      }
    }
  }
  else
  {
    MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, 
           "FTM GSM TxLM alloc/dealloc failed invalid sub %d", 
           sub_id );
  }

  /* Get profiling stop data */
  return rfgsm_ustmr_count_diff_a_b_us_rtn( start_ustmr, rfgsm_mc_read_ustmr_count() );
}



/*---------------------------------------------------------------------------*/
/*! @fn ftm_gsm_enh_debug_start_gsm_cmd                                      */
/*! @brief This function sends the command to RFGSM to allocate RFLM memory  */
/*! @details The RFLM memory is sub based so only subscription is required   */
/*!          If MSGR client is set up the confirmations will be received here*/
/*! @param sub_id - subscription to allocate RFLM memory for                 */
/*! @retval duration_us - execution time of the RF command processing API    */
uint32 ftm_gsm_enh_debug_start_gsm_cmd( uint8 sub_id, 
                                        ftm_gsm_cmd_api_cmn_driver_type * params )
{
  rfa_rf_gsm_start_gsm_req_s rf_gsm_start_gsm_req;
  rfa_rf_gsm_start_gsm_cnf_s rf_gsm_start_gsm_cnf;
  msgr_umid_type rfa_umid_to_send = RFA_RF_GSM_START_GSM_REQ;
  uint32 bytes_received;
  uint32 duration_us = 0;

  rfa_umid_to_send = (sub_id == 0) ? RFA_RF_GSM_START_GSM_REQ : RFA_RF_GSM_START_GSM_SUB2_REQ;

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_start_gsm_req.hdr, MSGR_GERAN_GL1, rfa_umid_to_send);

  rf_gsm_start_gsm_req.sub_id = sub_id;
  rf_gsm_start_gsm_req.cnf_required = ftm_gsm_enh_debug_msgr_client;

  (void)msgr_send(&rf_gsm_start_gsm_req.hdr, sizeof(rf_gsm_start_gsm_req));

  if( ftm_gsm_enh_debug_msgr_client == TRUE )
  {
    (void)msgr_receive( &rfgsm_ftm_dispatch_msgr_client, 
                        (uint8 *)&rf_gsm_start_gsm_cnf,
                        (uint32)sizeof(rfa_rf_gsm_start_gsm_cnf_s),
                        &bytes_received );

    MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH, 
           "FTM GSM Start GSM Confirmation execution time %d us", 
           rf_gsm_start_gsm_cnf.exec_time_us );

    duration_us = rf_gsm_start_gsm_cnf.exec_time_us;
  }

  return duration_us;
}



/*---------------------------------------------------------------------------*/
/*! @fn ftm_gsm_enh_debug_stop_gsm_cmd                                       */
/*! @brief This function sends the command to RFGSM to free RFLM memory      */
/*! @details The RFLM memory is sub based so only subscription is required   */
/*!          If MSGR client is set up the confirmations will be received here*/
/*! @param sub_id - subscription to deallocate RFLM memory for               */
/*! @retval duration_us - execution time of the RF command processing API    */
uint32 ftm_gsm_enh_debug_stop_gsm_cmd( uint8 sub_id, ftm_gsm_cmd_api_cmn_driver_type * params )
{
  rfa_rf_gsm_stop_gsm_req_s rf_gsm_stop_gsm_req;
  rfa_rf_gsm_stop_gsm_cnf_s rf_gsm_stop_gsm_cnf;
  msgr_umid_type rfa_umid_to_send = RFA_RF_GSM_STOP_GSM_REQ;
  uint32 bytes_received;
  uint32 duration_us = 0;

  rfa_umid_to_send = (sub_id == 0) ? RFA_RF_GSM_STOP_GSM_REQ : RFA_RF_GSM_STOP_GSM_SUB2_REQ;

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_stop_gsm_req.hdr, MSGR_GERAN_GL1, rfa_umid_to_send);

  rf_gsm_stop_gsm_req.sub_id = sub_id;
  rf_gsm_stop_gsm_req.cnf_required = ftm_gsm_enh_debug_msgr_client;

  (void)msgr_send(&rf_gsm_stop_gsm_req.hdr, sizeof(rf_gsm_stop_gsm_req));

  if( ftm_gsm_enh_debug_msgr_client == TRUE )
  {
    (void)msgr_receive( &rfgsm_ftm_dispatch_msgr_client, 
                        (uint8 *)&rf_gsm_stop_gsm_cnf,
                        (uint32)sizeof(rfa_rf_gsm_stop_gsm_cnf_s),
                        &bytes_received );

    MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH, 
           "FTM GSM Stop GSM Confirmation execution time %d us", 
           rf_gsm_stop_gsm_cnf.exec_time_us );

    duration_us = rf_gsm_stop_gsm_cnf.exec_time_us;
  }

  return duration_us;
}



/*---------------------------------------------------------------------------*/
/*! @fn ftm_gsm_enh_debug_rx_wakeup_cmd                                      */
/*! @brief This function sends the command to RFGSM to prepare for waking up */
/*!        Rx for GSM mode.                                                  */
/*! @details Rx wakeup is RFM device and RFLM handle based where we can      */
/*!          prepare for PRx, DRx, or PRx+DRx, mapped to a specific Rx chain.*/
/*!          If MSGR client is set up the confirmations will be received here*/
/*! @param sub_id - subscription to Rx wakeup for                            */
/*! @param params - pointer to FTM Rx wakeup data                            */
/*! @retval duration_us - execution time of the RF command processing API    */
uint32 ftm_gsm_enh_debug_rx_wakeup_cmd( uint8 sub_id, 
                                        ftm_gsm_cmd_api_cmn_driver_type * params )
{
  rfa_rf_gsm_rx_wakeup_req_s rf_gsm_rx_wakeup_req;
  rfa_rf_gsm_rx_wakeup_cnf_s rf_gsm_rx_wakeup_cnf;
  msgr_umid_type rfa_umid_to_send = RFA_RF_GSM_RX_WAKEUP_REQ;
  uint32 bytes_received;
  uint32 duration_us = 0;

  rfa_umid_to_send = (sub_id == 0) ? RFA_RF_GSM_RX_WAKEUP_REQ : RFA_RF_GSM_RX_WAKEUP_SUB2_REQ;

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_rx_wakeup_req.hdr, MSGR_GERAN_GL1, rfa_umid_to_send);

  rf_gsm_rx_wakeup_req.sub_id = sub_id;
  rf_gsm_rx_wakeup_req.op_mode = params->prx_drx_mode;
  rf_gsm_rx_wakeup_req.prx_device = params->primary_device;
  rf_gsm_rx_wakeup_req.drx_device = params->drx_device;
  rf_gsm_rx_wakeup_req.rxlm_buf_index.prx_idx = rxlm_handle_prx[sub_id];

  if( rf_gsm_rx_wakeup_req.op_mode == RFGSM_DRX_OP_MODE || 
      rf_gsm_rx_wakeup_req.op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    rf_gsm_rx_wakeup_req.rxlm_buf_index.drx_idx = rxlm_handle_drx[sub_id]; 
  }
  else
  {
    rf_gsm_rx_wakeup_req.rxlm_buf_index.drx_idx = FTM_GSM_RFLM_HANDLE_INVALID;
  }

  rf_gsm_rx_wakeup_req.gfw_buff_ptr = (void*)&ftm_gsm_gfw_shared_mem[sub_id].rfInitEvent;
  rf_gsm_rx_wakeup_req.timing_info = &timing_info;
  rf_gsm_rx_wakeup_req.cnf_required = ftm_gsm_enh_debug_msgr_client;

  (void)msgr_send(&rf_gsm_rx_wakeup_req.hdr, sizeof(rf_gsm_rx_wakeup_req));

  if( ftm_gsm_enh_debug_msgr_client == TRUE )
  {
    (void)msgr_receive( &rfgsm_ftm_dispatch_msgr_client, 
                        (uint8 *)&rf_gsm_rx_wakeup_cnf,
                        (uint32)sizeof(rfa_rf_gsm_rx_wakeup_cnf_s),
                        &bytes_received );

    MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH, 
           "FTM GSM Rx Wakeup Confirmation execution time %d us", 
           rf_gsm_rx_wakeup_cnf.exec_time_us );

    duration_us = rf_gsm_rx_wakeup_cnf.exec_time_us;
  }

  return duration_us;
}



/*---------------------------------------------------------------------------*/
/*! @fn ftm_gsm_enh_debug_tx_wakeup_cmd                                      */
/*! @brief This function sends the command to RFGSM to prepare for waking up */
/*!        Tx for GSM mode.                                                  */
/*! @details Tx wakeup is RFM device and RFLM handle based where we can      */
/*!          prepare for a specific Tx chain.                                */
/*!          If MSGR client is set up the confirmations will be received here*/
/*! @param sub_id - subscription to Tx wakeup for                            */
/*! @param params - pointer to FTM Tx wakeup data                            */
/*! @retval duration_us - execution time of the RF command processing API    */
uint32 ftm_gsm_enh_debug_tx_wakeup_cmd( uint8 sub_id, ftm_gsm_cmd_api_cmn_driver_type * params )
{
  rfa_rf_gsm_tx_wakeup_req_s rf_gsm_tx_wakeup_req;
  rfa_rf_gsm_tx_wakeup_cnf_s rf_gsm_tx_wakeup_cnf;
  msgr_umid_type rfa_umid_to_send = RFA_RF_GSM_TX_WAKEUP_REQ;
  uint32 bytes_received;
  uint32 duration_us = 0;

  rfa_umid_to_send = (sub_id == 0) ? RFA_RF_GSM_TX_WAKEUP_REQ : RFA_RF_GSM_TX_WAKEUP_SUB2_REQ;

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_tx_wakeup_req.hdr, MSGR_GERAN_GL1, rfa_umid_to_send);

  rf_gsm_tx_wakeup_req.sub_id = sub_id;
  rf_gsm_tx_wakeup_req.tx_device = params->primary_device;
  rf_gsm_tx_wakeup_req.txlm_buf_index = txlm_handle[sub_id];
  rf_gsm_tx_wakeup_req.gfw_buff_ptr = (void*)&ftm_gsm_gfw_shared_mem[sub_id].rfInitEvent;
  rf_gsm_tx_wakeup_req.therm_read_buff_ptr = (void*)&ftm_gsm_gfw_shared_mem[sub_id].thermReadEvent;
  rf_gsm_tx_wakeup_req.timing_info = &timing_info;
  rf_gsm_tx_wakeup_req.cnf_required = ftm_gsm_enh_debug_msgr_client;

  (void)msgr_send(&rf_gsm_tx_wakeup_req.hdr, sizeof(rf_gsm_tx_wakeup_req));

  if( ftm_gsm_enh_debug_msgr_client == TRUE )
  {
    (void)msgr_receive( &rfgsm_ftm_dispatch_msgr_client, 
                        (uint8 *)&rf_gsm_tx_wakeup_cnf,
                        (uint32)sizeof(rfa_rf_gsm_tx_wakeup_cnf_s),
                        &bytes_received );

    MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH, 
           "FTM GSM Tx Wakeup Confirmation execution time %d us", 
           rf_gsm_tx_wakeup_cnf.exec_time_us );

    duration_us = rf_gsm_tx_wakeup_cnf.exec_time_us;
  }

  return duration_us;
}



/*---------------------------------------------------------------------------*/
/*! @fn ftm_gsm_enh_debug_rx_sleep_cmd                                       */
/*! @brief This function sends the command to RFGSM to prepare for sleeping  */
/*!        Rx from GSM mode.                                                 */
/*! @details Rx sleep is RFM device and RFLM handle based where we can       */
/*!          sleep PRx, DRx, or PRx+DRx, mapped to a specific Rx chain.      */
/*!          If MSGR client is set up the confirmations will be received here*/
/*! @param sub_id - subscription to Rx sleep for                             */
/*! @param params - pointer to FTM Rx sleep data                             */
/*! @retval duration_us - execution time of the RF command processing API    */
uint32 ftm_gsm_enh_debug_rx_sleep_cmd( uint8 sub_id, ftm_gsm_cmd_api_cmn_driver_type * params )
{
  rfa_rf_gsm_rx_sleep_req_s rf_gsm_rx_sleep_req;
  rfa_rf_gsm_rx_sleep_cnf_s rf_gsm_rx_sleep_cnf;
  msgr_umid_type rfa_umid_to_send = RFA_RF_GSM_RX_SLEEP_REQ;
  uint32 bytes_received;
  uint32 duration_us = 0;

  rfa_umid_to_send = (sub_id == 0) ? RFA_RF_GSM_RX_SLEEP_REQ : RFA_RF_GSM_RX_SLEEP_SUB2_REQ;

  

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_rx_sleep_req.hdr, MSGR_GERAN_GL1, rfa_umid_to_send);

  rf_gsm_rx_sleep_req.sub_id = sub_id;
  rf_gsm_rx_sleep_req.op_mode = params->prx_drx_mode;
  rf_gsm_rx_sleep_req.prx_device = params->primary_device;
  rf_gsm_rx_sleep_req.drx_device = params->drx_device;
  rf_gsm_rx_sleep_req.cnf_required = ftm_gsm_enh_debug_msgr_client;

  (void)msgr_send(&rf_gsm_rx_sleep_req.hdr, sizeof(rf_gsm_rx_sleep_req));

  if( ftm_gsm_enh_debug_msgr_client == TRUE )
  {
    (void)msgr_receive( &rfgsm_ftm_dispatch_msgr_client, 
                        (uint8 *)&rf_gsm_rx_sleep_cnf,
                        (uint32)sizeof(rfa_rf_gsm_rx_sleep_cnf_s),
                        &bytes_received );
    

    MSG_2( MSG_SSID_RF, MSG_LEGACY_HIGH, 
           "FTM GSM Rx Sleep Confirmation execution time %d us, Testing... prx %d", 
           rf_gsm_rx_sleep_cnf.exec_time_us, rf_gsm_rx_sleep_req.prx_device );

    duration_us = rf_gsm_rx_sleep_cnf.exec_time_us;
  }

  return duration_us;
}



/*---------------------------------------------------------------------------*/
/*! @fn ftm_gsm_enh_debug_tx_sleep_cmd                                       */
/*! @brief This function sends the command to RFGSM to prepare for sleeping  */
/*!        Tx from GSM mode.                                                 */
/*! @details Tx sleep is RFM device and RFLM handle based where we can       */
/*!          sleep from a specific Tx chain.                                 */
/*!          If MSGR client is set up the confirmations will be received here*/
/*! @param sub_id - subscription to Tx sleep for                             */
/*! @param params - pointer to FTM Tx sleep data                             */
/*! @retval duration_us - execution time of the RF command processing API    */
uint32 ftm_gsm_enh_debug_tx_sleep_cmd( uint8 sub_id, ftm_gsm_cmd_api_cmn_driver_type * params )
{
  rfa_rf_gsm_tx_sleep_req_s rf_gsm_tx_sleep_req;
  rfa_rf_gsm_tx_sleep_cnf_s rf_gsm_tx_sleep_cnf;
  msgr_umid_type rfa_umid_to_send = RFA_RF_GSM_TX_SLEEP_REQ;
  uint32 bytes_received;
  uint32 duration_us = 0;

  rfa_umid_to_send = (sub_id == 0) ? RFA_RF_GSM_TX_SLEEP_REQ : RFA_RF_GSM_TX_SLEEP_SUB2_REQ;

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_tx_sleep_req.hdr, MSGR_GERAN_GL1, rfa_umid_to_send);

  rf_gsm_tx_sleep_req.sub_id = sub_id;
  rf_gsm_tx_sleep_req.tx_device = params->primary_device;
  rf_gsm_tx_sleep_req.cnf_required = ftm_gsm_enh_debug_msgr_client;

  (void)msgr_send(&rf_gsm_tx_sleep_req.hdr, sizeof(rf_gsm_tx_sleep_req));

  if( ftm_gsm_enh_debug_msgr_client == TRUE )
  {
    (void)msgr_receive( &rfgsm_ftm_dispatch_msgr_client, 
                        (uint8 *)&rf_gsm_tx_sleep_cnf,
                        (uint32)sizeof(rfa_rf_gsm_tx_sleep_cnf_s),
                        &bytes_received );

    MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH, 
           "FTM GSM Tx Sleep Confirmation execution time %d us", 
           rf_gsm_tx_sleep_cnf.exec_time_us );

    duration_us = rf_gsm_tx_sleep_cnf.exec_time_us;
  }

  return duration_us;
}



/*---------------------------------------------------------------------------*/
/*! @fn ftm_gsm_enh_debug_rx_burst_cmd                                       */
/*! @brief This function sends the command to RFGSM to build Rx bursts       */
/*! @details Rx bursts can be build for specific band and channels           */
/*!          If MSGR client is set up the confirmations will be received here*/
/*! @param sub_id - subscription to Rx for                                   */
/*! @param params - pointer to FTM Rx burst data                             */
/*! @params rx_params - Rx burst specific parameters                         */
/*! @retval duration_us - execution time of the RF command processing API    */
uint32 ftm_gsm_enh_debug_rx_burst_cmd( uint8 sub_id, 
                                      ftm_gsm_cmd_api_cmn_driver_type * params,
                                      ftm_gsm_cmd_api_rx_driver_type * rx_params )
{
  rfa_rf_gsm_rx_burst_req_s rf_gsm_rx_burst_req;
  rfa_rf_gsm_rx_burst_cnf_s rf_gsm_rx_burst_cnf;
  msgr_umid_type rfa_umid_to_send = RFA_RF_GSM_RX_BURST_REQ;
  uint32 bytes_received;
  uint32 duration_us = 0;
  rfgsm_rx_chan_params_type * ftm_rx_chan_params = NULL;

  rfa_umid_to_send = (sub_id == 0) ? RFA_RF_GSM_RX_BURST_REQ : RFA_RF_GSM_RX_BURST_SUB2_REQ;

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_rx_burst_req.hdr, MSGR_GERAN_GL1, rfa_umid_to_send);

  rf_gsm_rx_burst_req.sub_id = sub_id;
  rf_gsm_rx_burst_req.device = params->primary_device;
  rf_gsm_rx_burst_req.rx_diversity_device = params->drx_device;
  rf_gsm_rx_burst_req.buffer_id = params->triple_buffer_index;
  rf_gsm_rx_burst_req.enable_rx_diversity = rx_params->enable_drx;
  rf_gsm_rx_burst_req.cnf_required = ftm_gsm_enh_debug_msgr_client;
  rf_gsm_rx_burst_req.rx_burst_params = &rx_burst_parameters;

  rf_gsm_rx_burst_req.rx_burst_params->num_rx_bursts = 1;
  rf_gsm_rx_burst_req.rx_burst_params->gl1_lower_rx_ccs_prio = rx_params->enable_gl1_lower;

  MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH, 
           "Enable G1 Lower : %d.", 
           rx_params->enable_gl1_lower );

  /* Slot 0 Rx paramters GFW shared memory */
  /* ftm_gsm_gfw_shared_mem is an FTM definition of the GFW shared memory type */
  rf_gsm_rx_burst_req.rx_burst_params->gfw_rf_burst_event[0] = 
    (void*)&ftm_gsm_gfw_shared_mem[sub_id].burstEvent[rf_gsm_rx_burst_req.buffer_id].rxEvent[GFW_SYNC_RX_EVENT_INDEX];
  
  /* Assign local pointer for burst params and assign params */
  /*@TODO currently only executing single burst Rx's to be expanded to more later */
  ftm_rx_chan_params = &rx_burst_parameters.rx_chan_params[0];
  ftm_rx_chan_params->band = ftm_gsm_tool_band_to_sys_band[rx_params->slot_params[0].rx_band & (FTM_GSM_ENH_DEBUG_MAX_BANDS-1)];
  ftm_rx_chan_params->arfcn = rx_params->slot_params[0].rx_arfcn;
  ftm_rx_chan_params->freq_err = rx_params->rx_freq_error; 
  ftm_rx_chan_params->rx_lvl_dbm = ((rx_params->slot_params[0].rx_lvl_dbm)*-1); //-1 because tool cannot pass negative number
  ftm_rx_chan_params->gain_ptr = &gain;
  ftm_rx_chan_params->lna_range = 0;
  ftm_rx_chan_params->burst_type = rx_params->slot_params[0].burst_type;
  ftm_rx_chan_params->rx_burst_type = rx_params->slot_params[0].rx_burst_type;
  ftm_rx_chan_params->probe_burst = rx_params->slot_params[0].probe_burst;
  ftm_rx_chan_params->high_lin_mode_active = NULL;
  ftm_rx_chan_params->enh_rx_params.enh_rx_enable_flag = rx_params->slot_params[0].enh_rx_enable_flag;
  ftm_rx_chan_params->enh_rx_params.enh_rx_offset_freq = rx_params->slot_params[0].enh_rx_offset_flag;
  ftm_rx_chan_params->blanked = rx_params->slot_params[0].blanked;
  ftm_rx_chan_params->rf_sequence_number = rx_params->slot_params[0].rf_sequence_number;

  (void)msgr_send(&rf_gsm_rx_burst_req.hdr, sizeof(rf_gsm_rx_burst_req));

  if( ftm_gsm_enh_debug_msgr_client == TRUE )
  {
    (void)msgr_receive( &rfgsm_ftm_dispatch_msgr_client, 
                        (uint8 *)&rf_gsm_rx_burst_cnf,
                        (uint32)sizeof(rfa_rf_gsm_rx_burst_cnf_s),
                        &bytes_received );

    MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH, 
           "FTM GSM Rx burst Confirmation execution time %d us", 
           rf_gsm_rx_burst_cnf.exec_time_us );

    duration_us = rf_gsm_rx_burst_cnf.exec_time_us;
  }

  return duration_us;
}



/*---------------------------------------------------------------------------*/
/*! @fn ftm_gsm_enh_debug_set_tx_band_cmd                                    */
/*! @brief This function sends the command to RFGSM to build Tx band script  */
/*! @details Builds the scripts for a specified Tx band                      */
/*!          If MSGR client is set up the confirmations will be received here*/
/*! @param sub_id - subscription to Tx band for                              */
/*! @param params - pointer to FTM Tx common data                            */
/*! @params tx_params - pointer to Tx band parameters                        */
/*! @retval duration_us - execution time of the RF command processing API    */
uint32 ftm_gsm_enh_debug_set_tx_band_cmd( uint8 sub_id, 
                                          ftm_gsm_cmd_api_cmn_driver_type * params,
                                          ftm_gsm_cmd_api_tx_driver_type * tx_params )
{
  rfa_rf_gsm_set_tx_band_req_s rf_gsm_tx_band_req;
  rfa_rf_gsm_set_tx_band_cnf_s rf_gsm_tx_band_cnf;
  msgr_umid_type rfa_umid_to_send = RFA_RF_GSM_SET_TX_BAND_REQ;
  uint32 bytes_received;
  uint32 duration_us = 0;

  rfa_umid_to_send = (sub_id == 0) ? RFA_RF_GSM_SET_TX_BAND_REQ : RFA_RF_GSM_SET_TX_BAND_SUB2_REQ;

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_tx_band_req.hdr, MSGR_GERAN_GL1, rfa_umid_to_send);

  rf_gsm_tx_band_req.sub_id = sub_id;
  rf_gsm_tx_band_req.device = params->primary_device;
  rf_gsm_tx_band_req.buffer_id = params->triple_buffer_index;
  rf_gsm_tx_band_req.band = ftm_gsm_tool_band_to_sys_band[tx_params->tx_band & (FTM_GSM_ENH_DEBUG_MAX_BANDS-1)];
  rf_gsm_tx_band_req.sbi_set_tx_band_buff_ptr = (void*)&ftm_gsm_gfw_shared_mem[sub_id].setTxBandEvent;
  rf_gsm_tx_band_req.cnf_required = ftm_gsm_enh_debug_msgr_client;

  (void)msgr_send(&rf_gsm_tx_band_req.hdr, sizeof(rf_gsm_tx_band_req));

  if( ftm_gsm_enh_debug_msgr_client == TRUE )
  {
    (void)msgr_receive( &rfgsm_ftm_dispatch_msgr_client, 
                        (uint8 *)&rf_gsm_tx_band_cnf,
                        (uint32)sizeof(rfa_rf_gsm_set_tx_band_cnf_s),
                        &bytes_received );

    MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH, 
           "FTM GSM Tx Band Confirmation execution time %d us", 
           rf_gsm_tx_band_cnf.exec_time_us );

    duration_us = rf_gsm_tx_band_cnf.exec_time_us;
  }

  return duration_us;
}



/*---------------------------------------------------------------------------*/
/*! @fn ftm_gsm_enh_debug_tx_burst_cmd                                       */
/*! @brief This function sends the command to RFGSM to build Tx band script  */
/*! @details Builds the scripts for a specified Tx band                      */
/*!          If MSGR client is set up the confirmations will be received here*/
/*! @param sub_id - subscription to Tx burst for                             */
/*! @param params - pointer to FTM Tx common data                            */
/*! @params tx_params - pointer to Tx burst parameters                       */
/*! @retval duration_us - execution time of the RF command processing API    */
uint32 ftm_gsm_enh_debug_tx_burst_cmd( uint8 sub_id, 
                                      ftm_gsm_cmd_api_cmn_driver_type * params,
                                      ftm_gsm_cmd_api_tx_driver_type * tx_params )
{
  rfa_rf_gsm_tx_burst_req_s rf_gsm_tx_burst_req;
  rfa_rf_gsm_tx_burst_cnf_s rf_gsm_tx_burst_cnf;
  msgr_umid_type rfa_umid_to_send = RFA_RF_GSM_TX_BURST_REQ;
  uint32 bytes_received;
  uint32 duration_us = 0;
  uint8 number_tx_slots = 0;
  rfgsm_tx_chan_params_type * ftm_tx_chan_params = NULL;
  rfgsm_power_profile_type * ftm_tx_pwr_ctl_params = NULL;

  rfa_umid_to_send = (sub_id == 0) ? RFA_RF_GSM_TX_BURST_REQ : RFA_RF_GSM_TX_BURST_SUB2_REQ;

  /* initialise msgr header */
  msgr_init_hdr(&rf_gsm_tx_burst_req.hdr, MSGR_GERAN_GL1, rfa_umid_to_send);

  rf_gsm_tx_burst_req.sub_id = sub_id;
  rf_gsm_tx_burst_req.device = params->primary_device;
  rf_gsm_tx_burst_req.buffer_id= params->triple_buffer_index; 
  rf_gsm_tx_burst_req.cnf_required = ftm_gsm_enh_debug_msgr_client;
  rf_gsm_tx_burst_req.tx_burst_params = &tx_burst_parameters;

  /* Tx frame parameters */
  ftm_tx_chan_params = &rf_gsm_tx_burst_req.tx_burst_params->tx_chan_params;
  ftm_tx_chan_params->band = ftm_gsm_tool_band_to_sys_band[tx_params->tx_band & (FTM_GSM_ENH_DEBUG_MAX_BANDS-1)];
  ftm_tx_chan_params->arfcn = tx_params->tx_arfcn;
  ftm_tx_chan_params->freq_err = tx_params->tx_freq_error;
  ftm_tx_chan_params->rf_sequence_number = params->rf_sequence_number;

  /* Slot 0 Tx Power control params */
  if( tx_params->slot_params[0].slot_active == TRUE ) 
  {
    ftm_tx_pwr_ctl_params = &rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[0];
    ftm_tx_pwr_ctl_params->power_level = tx_params->slot_params[0].slot_pcl;
    ftm_tx_pwr_ctl_params->backoff[RFGSM_COEX_VBATT_BACKOFF] = tx_params->slot_params[0].slot_vbatt_backoff;
    ftm_tx_pwr_ctl_params->backoff[RFGSM_COEX_DESENSE_BACKOFF] = tx_params->slot_params[0].slot_desense_backoff;
    ftm_tx_pwr_ctl_params->modulation = tx_params->slot_params[0].slot_modulation;
    number_tx_slots++;
  }

  /* Slot 1 Tx Power control params    */                                                                                             
  if( tx_params->slot_params[1].slot_active == TRUE )
  {
    ftm_tx_pwr_ctl_params = &rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[1];
    ftm_tx_pwr_ctl_params->power_level = tx_params->slot_params[1].slot_pcl;
    ftm_tx_pwr_ctl_params->backoff[RFGSM_COEX_VBATT_BACKOFF] = tx_params->slot_params[1].slot_vbatt_backoff;
    ftm_tx_pwr_ctl_params->backoff[RFGSM_COEX_DESENSE_BACKOFF] = tx_params->slot_params[1].slot_desense_backoff;
    ftm_tx_pwr_ctl_params->modulation = tx_params->slot_params[1].slot_modulation;
    number_tx_slots++;
  }

  /* Slot 2 Tx Power control params */                                                                                              
  if( tx_params->slot_params[2].slot_active == TRUE ) 
  {
    ftm_tx_pwr_ctl_params = &rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[2];
    ftm_tx_pwr_ctl_params->power_level = tx_params->slot_params[2].slot_pcl;
    ftm_tx_pwr_ctl_params->backoff[RFGSM_COEX_VBATT_BACKOFF] = tx_params->slot_params[2].slot_vbatt_backoff;
    ftm_tx_pwr_ctl_params->backoff[RFGSM_COEX_DESENSE_BACKOFF] = tx_params->slot_params[2].slot_desense_backoff;
    ftm_tx_pwr_ctl_params->modulation = tx_params->slot_params[2].slot_modulation;
    number_tx_slots++;
  }

  /* Slot 3 Tx Power control params  */                                                                                               
  if( tx_params->slot_params[3].slot_active == TRUE ) 
  {
    ftm_tx_pwr_ctl_params = &rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[3];
    ftm_tx_pwr_ctl_params->power_level = tx_params->slot_params[3].slot_pcl;
    ftm_tx_pwr_ctl_params->backoff[RFGSM_COEX_VBATT_BACKOFF] = tx_params->slot_params[3].slot_vbatt_backoff;
    ftm_tx_pwr_ctl_params->backoff[RFGSM_COEX_DESENSE_BACKOFF] = tx_params->slot_params[3].slot_desense_backoff;
    ftm_tx_pwr_ctl_params->modulation = tx_params->slot_params[3].slot_modulation;
    number_tx_slots++;
  }

  /* Slot 4 Tx Power control params  */                                                                                               
  if( tx_params->slot_params[4].slot_active == TRUE )
  {
    ftm_tx_pwr_ctl_params = &rf_gsm_tx_burst_req.tx_burst_params->tx_pwr_ctl_params[4];
    ftm_tx_pwr_ctl_params->power_level = tx_params->slot_params[4].slot_pcl;
    ftm_tx_pwr_ctl_params->backoff[RFGSM_COEX_VBATT_BACKOFF] = tx_params->slot_params[4].slot_vbatt_backoff;
    ftm_tx_pwr_ctl_params->backoff[RFGSM_COEX_DESENSE_BACKOFF] = tx_params->slot_params[4].slot_desense_backoff;
    ftm_tx_pwr_ctl_params->modulation = tx_params->slot_params[4].slot_modulation;
    number_tx_slots++;
  }                                                                                                
                                                                                                                                    
  rf_gsm_tx_burst_req.tx_burst_params->num_tx_slots = number_tx_slots;
  rf_gsm_tx_burst_req.tx_burst_params->num_assigned_uplink_slots = number_tx_slots;

  /* Tx GFW shared memory pointer */
  rf_gsm_tx_burst_req.tx_burst_params->tx_burst_event_ptr = (void*)&ftm_gsm_gfw_shared_mem[sub_id].burstEvent[rf_gsm_tx_burst_req.buffer_id].txEvent;

  (void)msgr_send(&rf_gsm_tx_burst_req.hdr, sizeof(rf_gsm_tx_burst_req));

  if( ftm_gsm_enh_debug_msgr_client == TRUE )
  {
    (void)msgr_receive( &rfgsm_ftm_dispatch_msgr_client, 
                        (uint8 *)&rf_gsm_tx_burst_cnf,
                        (uint32)sizeof(rfa_rf_gsm_tx_burst_cnf_s),
                        &bytes_received );

    MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH, 
           "FTM GSM Tx Burst Confirmation execution time %d us", 
           rf_gsm_tx_burst_cnf.exec_time_us );

    duration_us = rf_gsm_tx_burst_cnf.exec_time_us;
  }

  return duration_us;
}



/*---------------------------------------------------------------------------*/
/*! @fn ftm_gsm_enh_debug_set_antenna_req                                    */
/*! @struct rfa_rf_gsm_set_antenna_req_s                                               */
/*! @brief This structure is the payload for updating the antenna position information */ 
/*! @param  hdr - MSGR details for each UMID i.e priority, sending module etc          */
/*! @param  device - RFM device type to be passed from L1 to RF device                 */ 
/*! @param  sub_id - Subscription ID to be returned to GL1                             */
/*! @param  ant_pos - set antenna payload to be updated in rf task                     */
/*! @param  cnf_required - L1 to request RF to send confirmation msg or not            */
/*! @param  req_result - Result detailing the success/failure of the command           */
/*! @param  l1_cb_func - Function to call when switch complete ack comes from FW       */
void ftm_gsm_enh_debug_set_antenna_req( uint8 sub_id, 
                                        ftm_gsm_cmd_api_antenna_driver_type * ant_params )
{
#ifdef FEATURE_RF_ASDIV
  rfa_rf_gsm_set_antenna_req_s  rf_gsm_set_antenna;
  msgr_umid_type rfa_umid_to_send = RFA_RF_GSM_SET_ANTENNA_REQ;

  rfa_umid_to_send = (sub_id == 0) ? RFA_RF_GSM_SET_ANTENNA_REQ : RFA_RF_GSM_SET_ANTENNA_SUB2_REQ;

  msgr_init_hdr(&rf_gsm_set_antenna.hdr, MSGR_GERAN_GL1, rfa_umid_to_send);

  rf_gsm_set_antenna.sub_id = sub_id;
  rf_gsm_set_antenna.device = ant_params->device;
  rf_gsm_set_antenna.ant_pos = ant_params->antenna_position;
  rf_gsm_set_antenna.cnf_required = FALSE;
  rf_gsm_set_antenna.req_result = RFA_RF_GSM_SUCCESS;
  rf_gsm_set_antenna.l1_cb_func = NULL;

  (void)msgr_send(&rf_gsm_set_antenna.hdr, sizeof(rf_gsm_set_antenna));

#else
  MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "FTM GSM API Driver - Set Antenna - Feature Not Enabled");
#endif
}



/*----------------------------------------------------------------------------*/
/*!
   @brief
   This function handles requests to run GSM tests and other primitives
   by dispatching the appropriate functions.

   @param
   cmd_ptr -

   @retval
   Response packet
*/
ftm_rsp_pkt_type ftm_gsm_dispatch( rfm_device_enum_type rfm_dev,
                                   ftm_pkt_type  *cmd_ptr )
{
  ftm_rsp_pkt_type rsp_pkt = {FTM_RSP_DO_LEGACY, 0, NULL, FALSE};
  ftm_gsm_rfctl_vars_s * rx_vars_ptr = NULL;
  ftm_gsm_chain_vars_s * chain_vars_ptr = NULL;
  ftm_gsm_cmn_vars_s* cmn_vars_ptr = NULL;

  /*rfm device passed in the assigned rx device from ftm device manager*/
  rfm_device_enum_type rx_device = rfm_dev;
  rfm_device_enum_type rxd_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type tx_device = RFM_INVALID_DEVICE;

  ftm_common_cmd_id_type cmd_type = (ftm_common_cmd_id_type)(cmd_ptr->rf_params.ftm_rf_cmd);

  if( rfm_dev >= RFM_MAX_WAN_DEVICES  )
  {
    FTM_MSG_1( FTM_ERROR,
               "ftm_gsm_dispatch():"
               "Received invalid assigned device %d", 
               rfm_dev );
    return rsp_pkt;
  }

  FTM_MSG_1( FTM_HIGH,
             "ftm_gsm_dispatch(): "
             "Received assigned device %d", 
             rfm_dev );


  /*Allow below commands to be run in online mode as well*/

  if(!((cmd_type == FTM_READ_DEVICE_STATUS) ||
     (cmd_type == FTM_READ_DEVICE_STATUS_ENABLE ) ||
     (cmd_type == FTM_READ_DEVICE_STATUS_DISABLE )||
     (cmd_type == FTM_READ_GRFC_DEVICE_STATUS_ENABLE )||
     (cmd_type == FTM_READ_GRFC_DEVICE_STATUS_DISABLE )))
  {
    tx_device = ftm_gsm_get_associated_tx_device( rx_device );
    rxd_device = ftm_gsm_get_associated_rx_device( rx_device );

    /* Allocate PRx device FTM vars */
    /*dispatch device is now assigned rx_device*/
    ftm_gsm_create_and_init_vars( rx_device, rx_device );

    if( rxd_device == RFM_INVALID_DEVICE )
    {
      FTM_MSG_1( FTM_ERROR,
                 "ftm_gsm_dispatch():"
                 "Received invalid rxd device %d",
                 rxd_device);
      
      return rsp_pkt;
    }

    /* Allocate DRx device FTM vars */
    /*dispatch device is now assigned rx_device*/
    ftm_gsm_create_and_init_vars( rxd_device, rx_device );

    /* Allocate Tx device FTM vars */
    /*dispatch device is now assigned rx_device*/
    if( tx_device != RFM_INVALID_DEVICE )
    {
      ftm_gsm_create_and_init_vars( tx_device, rx_device );
    }

    /*Update the GSM chain variables by retrieving RX/TX/DRX devices from RFC*/
    /*dispatch device is now assigned rx_device*/
    chain_vars_ptr = ftm_gsm_device_get_and_update_chain_vars(rx_device);

    if( chain_vars_ptr               == NULL ||
        chain_vars_ptr->cmn_vars_ptr == NULL ||
        chain_vars_ptr->rx_vars_ptr  == NULL  )
    {
      FTM_MSG_1( FTM_ERROR, 
                 "ftm_gsm_dispatch() chain_vars_ptr/cmn_vars_ptr/"
                 "rx_vars_ptr NULL for device %d",
                 rx_device);
      return rsp_pkt;
    }

    cmn_vars_ptr = chain_vars_ptr->cmn_vars_ptr;

    /*get the variables ptr for RX device*/
    rx_vars_ptr = chain_vars_ptr->rx_vars_ptr;
    
    /*Check if TX is supported on the given chain*/
    if (ftm_gsm_device_is_tx_supported(chain_vars_ptr->associated_tx_dev))
    {
      if (chain_vars_ptr->tx_vars_ptr == NULL)
      {
        FTM_MSG_1( FTM_ERROR,
                   "ftm_gsm_dispatch() tx_vars_ptr not created for tx device  %d", 
                   chain_vars_ptr->associated_tx_dev );
        return rsp_pkt;
      }
    } /* ftm_gsm_device_is_tx_supported */
    else
    {
      FTM_MSG_1( FTM_MED, 
                 "ftm_gsm_dispatch() TX device is not supported for ftm chain  %d", 
                 chain_vars_ptr->gsm_chain);
    }  /* !ftm_gsm_device_is_tx_supported */

  }
 
    /* Some commands requires ftm chain back as part of response. 
     Extract ftm chain from command for populating into payload */
  ftm_diagpkt_subsys_header_type *header = (ftm_diagpkt_subsys_header_type*)cmd_ptr; 
  diagpkt_subsys_cmd_code_type ftm_chain = header->subsys_cmd_code;

  switch ( (ftm_common_cmd_id_type)(cmd_ptr->rf_params.ftm_rf_cmd) )
  {
  case FTM_SET_MODE:
    ftm_gsm_set_mode( rx_device,
                      (ftm_mode_id_type) cmd_ptr->rf_params.ftm_rf_factory_data.mode );
    break;

  case FTM_SET_TX_ON:
     /*pass the Tx device here*/
    ftm_gsm_set_tx( chain_vars_ptr->associated_tx_dev, ON);
    break;

  case FTM_SET_TX_OFF:
     /*pass the Tx device here*/
    ftm_gsm_set_tx( chain_vars_ptr->associated_tx_dev, OFF);
    break;

  case FTM_SET_LNA_RANGE:
    ftm_gsm_set_lna_range( rx_device,
                           (ftm_lna_range_type) cmd_ptr->rf_params.ftm_rf_factory_data.range );
    
    /* Also set the LNA range for RXD device*/
    ftm_gsm_set_lna_range( rxd_device,
                           (ftm_lna_range_type) cmd_ptr->rf_params.ftm_rf_factory_data.range );
    break;

  case FTM_SET_CHAN:
    ftm_gsm_set_arfcn( chain_vars_ptr,
                       cmd_ptr->rf_params.ftm_rf_factory_data.chan );
    break;

    /*  16  - Transmits a continuous waveform (rnd data,PN seq, tone) */
  case FTM_SET_TRANSMIT_CONT:
     /*pass the Tx device here*/
    ftm_gsm_tx_cont_command( chain_vars_ptr->associated_tx_dev,
                             &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union).ftm_tx_burst );
    break;

    /*  17  - Transmits a specified (or infinite) number of TX bursts                  */
  case FTM_SET_TRANSMIT_BURST:
     /*pass the Tx device here*/
    ftm_gsm_tx_burst_command(chain_vars_ptr->associated_tx_dev,
                              &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union).ftm_tx_burst );
    break;

    /*  25  - Start to receive or stop receiving Rx burst                              */
  case FTM_SET_RX_BURST:
    ftm_gsm_rx_burst_command( rx_device,
                              &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union).ftm_rx_burst,
                              FTM_DIV_FACTOR );
    break;

    /*  30  - Get Raw RSSI                                 */
  case FTM_GET_GSM_RSSI:
    {
      ftm_clk_rex_wait(FTM_DIV_FACTOR*5+5);  // wait FTM_DIV_FACTOR frames, plus 5ms more to be safe
      cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_rssi.rssi = rx_vars_ptr->ftm_gsm_state.ftm_rssi;
    }
    break;

  case FTM_GET_GSM_PRX_DRX_RSSI:
    {
      ftm_clk_rex_wait(FTM_DIV_FACTOR*5+5);  // wait FTM_DIV_FACTOR frames, plus 5ms more to be safe
      cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_rssi.rssi = rx_vars_ptr->ftm_gsm_state.ftm_rssi;
      cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_rssi.rssi_rxd = rx_vars_ptr->ftm_gsm_state.ftm_rssi_drx;
    }
    break;

    /*  31 - SET_PA_START_DELTA  */
  case FTM_SET_PA_START_DELTA:
     /*pass the Tx device here*/
    ftm_gsm_set_pa_start_delta( chain_vars_ptr->associated_tx_dev,
                                (int16)cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.generic_16bit_u);
    break;

    /*  32 - SET_PA_STOP_DELTA */
  case FTM_SET_PA_STOP_DELTA:
     /*pass the Tx device here*/
    ftm_gsm_set_pa_stop_delta( chain_vars_ptr->associated_tx_dev,
                               (int16)cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.generic_16bit_u);
    break;


    /*  34 - SET_RX_CONTINUOUS */
  case FTM_SET_RX_CONTINUOUS:
    ftm_gsm_rx_burst_command( rx_device,
                              &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union).ftm_rx_burst,
                              FTM_DIV_FACTOR );
    break;

    /* 37 */
  case FTM_SET_PATH_DELAY:
    cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.generic_32bit_u =
    ftm_gsm_set_delay_match_cfg( chain_vars_ptr->associated_tx_dev,
                             (int32)cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.generic_32bit_u);
    break;

    /* 40 */
  case FTM_SET_TX_FRAME_MATRIX:
     /*pass the Tx device here*/
    ftm_gsm_set_frame_matrix( chain_vars_ptr->associated_tx_dev,
                          &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union).ftm_frame_matrix);
    break;

    /* 46 */
  case FTM_RX_GAIN_RANGE_CAL:
    cmn_vars_ptr->ftm_gsm_cal_flag = TRUE;
    ftm_gsm_do_rx_calibration_prx( rx_device,
                                   &cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_rxcal_parms);
    cmn_vars_ptr->ftm_gsm_cal_flag = FALSE;
    break;
  case FTM_RX_GAIN_RANGE_PRX_DRX_CAL:
    cmn_vars_ptr->ftm_gsm_cal_flag = TRUE;
    ftm_gsm_do_rx_calibration_prx_drx( rx_device,
                                       &cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_rxcal_prx_drx_parms);
    cmn_vars_ptr->ftm_gsm_cal_flag = FALSE;
    break;

    /* 51 */
  case FTM_TX_KV_CAL: /* review the kv cal methods we have and go with the one method */
    cmn_vars_ptr->ftm_gsm_cal_flag = TRUE;
    /*pass the Tx device here*/
    ftm_gsm_do_tx_kv_cal( chain_vars_ptr->associated_tx_dev,
                          &cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.generic_2x16bit_u );
    cmn_vars_ptr->ftm_gsm_cal_flag = FALSE;
    break;

  case FTM_GSM_TX_KV_CAL_V2:
    cmn_vars_ptr->ftm_gsm_cal_flag = TRUE;
    /*pass the Tx device here*/
    ftm_gsm_do_tx_kv_cal_v2( chain_vars_ptr->associated_tx_dev,
                             &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union).ftm_kv_val_v2_params );
    cmn_vars_ptr->ftm_gsm_cal_flag = FALSE;
   break;

  case FTM_SET_GSM_LINEAR_PA_RANGE:
     /*pass the Tx device here*/
    ftm_gsm_set_pa_range ( chain_vars_ptr->associated_tx_dev,
                       &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_pa_range_params));
  break;

  case FTM_GSM_SET_LINEAR_RGI:
     /*pass the Tx device here*/
      ftm_gsm_set_linear_rgi( chain_vars_ptr->associated_tx_dev,
                              &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_lin_rgi_params));
  break;

	
  case FTM_GSM_SET_QPOET_MODE_BIAS:
     /*pass the Tx device here*/
      rsp_pkt = ftm_common_malloc_rsp_pkt(cmd_ptr, sizeof(ftm_composite_cmd_header_type)+sizeof(ftm_gsm_set_qpoet_mode_bias_resp_type));
      ftm_gsm_set_qpoet_mode_bias( chain_vars_ptr->associated_tx_dev,
                              &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_gsm_qpoet_mode_bias_params),
                              &rsp_pkt );
  break;

  case FTM_GSM_SET_TX_SLOT_CONFIG:
     /*pass the Tx device here*/
    ftm_gsm_set_tx_slot_config( chain_vars_ptr->associated_tx_dev,
                              &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_gsm_tx_slot_config));	  	
  break;

  case FTM_GSM_SET_PREDIST_OVERRIDE:
     /*pass the Tx device here*/
      ftm_gsm_set_predist_override(chain_vars_ptr->associated_tx_dev, 
                                   cmd_ptr->rf_params.ftm_rf_factory_data.on_off);  

  break;

  case FTM_GSM_CAPTURE_IQ:
      ftm_gsm_start_stop_iq_capture( chain_vars_ptr->prx_dev,
                                     (word)cmd_ptr->rf_params.ftm_rf_factory_data.gen_w);
  break;
    
  case FTM_IQ_CAPTURE_GET_CONFIG:
      cmd_ptr->rf_params.ftm_rf_factory_data.ftm_iq_capture_cmd.ftm_iq_capture_get_config.ftm_error_code =
      ftm_gsm_iq_capture_get_config(&cmd_ptr->rf_params.ftm_rf_factory_data.ftm_iq_capture_cmd.ftm_iq_capture_get_config);
  break;

  case FTM_IQ_CAPTURE_ACQUIRE_SAMPLES:

      cmd_ptr->rf_params.ftm_rf_factory_data.ftm_iq_capture_cmd.ftm_iq_capture_acquire_samples.ftm_error_code =
      ftm_gsm_iq_capture_acquire_samples(chain_vars_ptr->prx_dev, &cmd_ptr->rf_params.ftm_rf_factory_data.ftm_iq_capture_cmd.ftm_iq_capture_acquire_samples);
  break;

  case FTM_IQ_CAPTURE_GET_SAMPLES:
      ftm_gsm_iq_capture_get_samples(rx_device, &cmd_ptr->rf_params.ftm_rf_factory_data.ftm_iq_capture_cmd.ftm_iq_capture_get_samples,ftm_chain, &rsp_pkt);
  break;
	
  case FTM_IQ_CAPTURE_ANALYZE_SAMPLES:
	  ftm_gsm_iq_capture_analyze_samples(chain_vars_ptr->prx_dev, rx_device, &cmd_ptr->rf_params.ftm_rf_factory_data.ftm_iq_capture_cmd.ftm_iq_capture_analyze_samples, ftm_chain, &rsp_pkt);
  break;

  case FTM_SET_FREQ_ADJUST:
      ftm_gsm_override_gl1_freq_err( rx_device );
      ftm_gsm_override_gl1_freq_err( chain_vars_ptr->associated_tx_dev );
      cmd_ptr->rf_params.ftm_rf_factory_data.ftm_set_freq_adjust.total_freq_adjust_hz.total_freq_adjust_hz_gsm =
            ftm_gsm_set_freq_err( rx_device,
                                  cmd_ptr->rf_params.ftm_rf_factory_data.ftm_set_freq_adjust.user_freq_adjust_ppb.user_freq_adjust_ppb_gsm,
                                  cmd_ptr->rf_params.ftm_rf_factory_data.ftm_set_freq_adjust.center_frequency_hz.center_frequency_hz_gsm,
                                  cmd_ptr->rf_params.ftm_rf_factory_data.ftm_set_freq_adjust.enable_xo_freq_adjust
                                );
  break;


  case FTM_SET_TUNER_TUNE_CODE_OVERRIDE: 
      cmd_ptr->rf_params.ftm_rf_factory_data.ftm_tuner_tunecode_override_data.result =(ftm_tuner_override_status_type)ftm_gsm_rf_task_tuner_override(rx_device,
                                                                                                                                                     (uint8)cmd_ptr->rf_params.ftm_rf_factory_data.ftm_tuner_tunecode_override_data.override_flag,
                                                                                                                                                     (void*)&(cmd_ptr->rf_params.ftm_rf_factory_data.ftm_tuner_tunecode_override_data.config_data),
                                                                                                                                                     (uint8*)&(cmd_ptr->rf_params.ftm_rf_factory_data.ftm_tuner_tunecode_override_data.nv_data),
                                                                                                                                                     (uint8*)&(cmd_ptr->rf_params.ftm_rf_factory_data.ftm_tuner_tunecode_override_data.tuner_id));

  break;

  case FTM_GET_TUNE_CODE:
    rsp_pkt = ftm_common_process_get_tune_code(cmd_ptr,rx_device,
                                               FTM_STATE_GSM,
                                               cmd_ptr->rf_params.ftm_rf_factory_data.ftm_tuner_get_tune_code_data.mode,
                                               cmd_ptr->rf_params.ftm_rf_factory_data.ftm_tuner_get_tune_code_data.device_type);
    break;

  case FTM_GSM_DO_IP2_CAL:

      /*Allocate response packet*/
      rsp_pkt = ftm_common_malloc_rsp_pkt(cmd_ptr, sizeof(ftm_composite_cmd_header_type)+sizeof(ftm_gsm_ip2_cal_resp_type));
      ftm_gsm_set_ip2_cal_params_override(rfm_dev, cmd_ptr, &rsp_pkt);
  break;

  case FTM_GSM_GET_IP2_CAL_RESULTS:
       /*Changes pending*/
//      ftm_gsm_get_ip2_cal_results(rfm_dev,
//                                  (uint8*)&(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.generic_8bit_u),
//                                  0,
//                                  (int16*)rsp_pkt.pkt_payload[0],
//                                  (int16*)rsp_pkt.pkt_payload[1],
//                                  (int32*)rsp_pkt.pkt_payload[2] );
//
//      rsp_pkt.pkt_len        = iRequestPacketSize;
//      rsp_pkt.pkt_payload    = pRequestBytes;
//      rsp_pkt.delete_payload = FALSE;
  break;

  case FTM_GSM_DISABLE_IDLE_FRAME:
    cmn_vars_ptr->ftm_gsm_idle_proc_flag = (word)cmd_ptr->rf_params.ftm_rf_factory_data.gen_w;
  break;

  case FTM_GSM_SAWLESS_LIN_OVERRIDE:
    (void)ftm_gsm_sawless_lin_override( rx_device, 
                                        (ftm_gsm_sawless_lin_type*)&(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_gsm_sawless_lin_params) );
  break;

  case FTM_GSM_GET_SAWLESS_LIN_MODE:
    (void)ftm_gsm_get_sawless_lin_mode( rx_device, 
                                        (ftm_gsm_sawless_lin_type*)&(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_gsm_sawless_lin_params) );
  break;
  case FTM_SET_SECONDARY_CHAIN:
    ftm_gsm_set_diversity(rx_device, (word)cmd_ptr->rf_params.ftm_rf_factory_data.gen_w);
  break;

  case FTM_GSM_SET_PA_BIAS_NV:
    cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_set_pa_bias_nv_params.error_code = 
    ftm_gsm_set_pa_bias_nv(chain_vars_ptr->associated_tx_dev, 
                        cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_set_pa_bias_nv_params.band, 
                        &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_set_pa_bias_nv_params.online_smps[0][0]), 
                        cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_set_pa_bias_nv_params.num_pa_state, 
                       &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_set_pa_bias_nv_params.ftm_gsm_pa_bias_params[0]));
    break;

  case FTM_READ_DEVICE_STATUS:
  	(void)ftm_gsm_get_device_status_reg_list( rfm_dev,
                                                  (ftm_read_device_status*)&(cmd_ptr->rf_params.ftm_rf_factory_data.device_status) );
  break;
  case FTM_READ_DEVICE_STATUS_ENABLE:
    (void)ftm_gsm_set_device_status_enable( rfm_dev,
                                            (rf_burst_type*)&(cmd_ptr->rf_params.ftm_rf_factory_data.device_status_read_burst_type), TRUE );
  break;
  case FTM_READ_DEVICE_STATUS_DISABLE:
  	(void)ftm_gsm_set_device_status_enable( rfm_dev,
                                                (rf_burst_type*)&(cmd_ptr->rf_params.ftm_rf_factory_data.device_status_read_burst_type), FALSE );
  break;
  case FTM_READ_GRFC_DEVICE_STATUS_ENABLE:
	(void)ftm_gsm_set_grfc_status_read_enable( rfm_dev,
                                                   (rf_burst_type*)&(cmd_ptr->rf_params.ftm_rf_factory_data.device_status_read_burst_type), TRUE );
  break;
  case FTM_READ_GRFC_DEVICE_STATUS_DISABLE:
	(void)ftm_gsm_set_grfc_status_read_enable( rfm_dev,
                                                   (rf_burst_type*)&(cmd_ptr->rf_params.ftm_rf_factory_data.device_status_read_burst_type), FALSE );
  break;

  case FTM_GSM_SET_ALT_PATH:   
  	 ftm_gsm_overwrite_alt_path(rx_device, cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_gsm_set_alt_path_params.alt_path ,
  	 	                          cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_gsm_set_alt_path_params.overwrite);
  break;
  
  case FTM_GSM_GET_NV_CONTAINER:
    ftm_gsm_get_nv_container( rx_device,
                              (ftm_mode_id_type)(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_gsm_get_nv_container_params.band),    	                        
                              cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_gsm_get_nv_container_params.rx_tx,
                              &(cmd_ptr->rf_params.ftm_rf_factory_data.gsm_struct.ftm_gsm_data_union.ftm_gsm_get_nv_container_params.nv_container));
  break;
#ifdef FEATURE_GERAN_RF_SELF_TEST
  case FTM_RF_MEAS_TX_PARAMS:

    ftm_gsm_fbrx_meas_tx_params( rx_device,
                                 chain_vars_ptr->associated_tx_dev,
                                 cmd_ptr->rf_params.ftm_rf_factory_data.ftm_rf_fbrx_test_cmd.ftm_rf_fbrx_meas_tx_params.num_averages,
                                 cmd_ptr->rf_params.ftm_rf_factory_data.ftm_rf_fbrx_test_cmd.ftm_rf_fbrx_meas_tx_params.capture_offset,
                                 cmd_ptr->rf_params.ftm_rf_factory_data.ftm_rf_fbrx_test_cmd.ftm_rf_fbrx_meas_tx_params.tx_measurement_config,
                                 (void*)cmd_ptr,
                                 &rsp_pkt );
    break;
#endif

    /*  Illegal, or unknown GSM test id  */
  default:
    break;
  }

  return rsp_pkt;
} /* end ftm_gsm_dispatch */

/*----------------------------------------------------------------------------*/
/*!
   @brief
   This function is used to handle RF GSM Responses which are received via
   Message Router.
   For the initial implementation, all this function does is print the message ID.
   Future enhancements may require additional handling/processing of the response message.
 
   @param
   msg_ptr, msg_size
 
   @retval
   void
*/
void ftm_gsm_msgr_dispatch_response_handler
(
    rfgsm_ftm_msg_u *msg_ptr,
    uint32  msg_size

)
{
  msgr_hdr_s   *msgr_hdr_ptr;

  msgr_hdr_ptr = (msgr_hdr_s*) msg_ptr;

  switch (msgr_hdr_ptr->id) 
  {
    case RFA_RF_GSM_FTM_SET_LINEAR_PA_RANGE_SUB1_RSP:
    case RFA_RF_GSM_FTM_SET_LINEAR_PA_RANGE_SUB2_RSP:
      MSG_1( MSG_SSID_FTM, MSG_LEGACY_HIGH, 
             "ftm_gsm_msgr_dispatch_response_handler()RSP message received: UMID=0x%x", 
              msgr_hdr_ptr->id );
      break;
    default:
      MSG_1( MSG_SSID_FTM, MSG_LEGACY_ERROR,
             "ftm_gsm_msgr_dispatch_response_handler()Bad RSP message received: UMID=0x%x",
             msgr_hdr_ptr->id );
      break;
  }

}/* end of ftm_gsm_msgr_dispatch_response_handler */
/*!
   @brief
   This function handles requests to run GSM tests and other primitives
   by dispatching the appropriate functions.

   @param
   cmd_ptr -

   @retval
   Response packet
*/
ftm_rsp_pkt_type ftm_gsm_extended_dispatch( rfm_device_enum_type rfm_dev,
                                            ftm_gsm_extended_pkt_type *cmd_ptr)
{
  ftm_rsp_pkt_type rsp_pkt = {FTM_RSP_DO_LEGACY, 0, NULL, FALSE};
  ftm_composite_cmd_header_type  rsp_headers;
  /* local_buffer and extra_data_len allow return data size to depend on a
     request parameter. Unused for now */
  byte                           *local_buffer  = NULL;
  uint16                         extra_data_len = 0;
  uint8                           *buff_ptr;
  /* buffer to hold result data*/
  ftm_gsm_extended_result_union   *result_buff = NULL;

  /*rfm device passed in the assigned rx device from ftm device manager*/
  rfm_device_enum_type rx_device = rfm_dev;
  rfm_device_enum_type rxd_device = RFM_INVALID_DEVICE;
  rfm_device_enum_type tx_device = RFM_INVALID_DEVICE;

  ftm_gsm_rfctl_vars_s * rx_vars_ptr = NULL;
  ftm_gsm_chain_vars_s * chain_vars_ptr = NULL;
  ftm_gsm_cmn_vars_s* cmn_vars_ptr = NULL;

  if( rfm_dev >= RFM_MAX_WAN_DEVICES )
  {
    FTM_MSG_1( FTM_ERROR,
               "ftm_gsm_extended_dispatch():"
               "Received invalid assigned device %d", 
               rfm_dev);
    return rsp_pkt;
  }

  FTM_MSG_1( FTM_HIGH,
             "ftm_gsm_extended_dispatch(): "
             "Received assigned device %d", 
             rfm_dev);

  tx_device = ftm_gsm_get_associated_tx_device( rx_device );
  rxd_device = ftm_gsm_get_associated_rx_device( rx_device );
  
  /* Allocate PRx device FTM vars. rx_device is dispatch device */
  ftm_gsm_create_and_init_vars( rx_device, rx_device );

  /* Allocate DRx device FTM vars */
  ftm_gsm_create_and_init_vars( rxd_device, rx_device );

  if( rxd_device == RFM_INVALID_DEVICE )
  {
    FTM_MSG_1( FTM_ERROR,
               "ftm_gsm_dispatch():"
               "Received invalid rxd device %d",
               rxd_device);
    
    return rsp_pkt;
  }
  
  if( tx_device != RFM_INVALID_DEVICE )
  {
    ftm_gsm_create_and_init_vars( tx_device, rx_device );
  }

  /*Update the GSM chain variables by retrieving RX/TX/DRX devices from RFC*/
  chain_vars_ptr = ftm_gsm_device_get_and_update_chain_vars( rx_device);

  if( chain_vars_ptr               == NULL ||
      chain_vars_ptr->cmn_vars_ptr == NULL ||
      chain_vars_ptr->rx_vars_ptr  == NULL  )
  {
    FTM_MSG_1( FTM_ERROR, 
               "ftm_gsm_extended_dispatch() chain_vars_ptr/cmn_vars_ptr/"
               "rx_vars_ptr NULL for device %d",
                rx_device );
    return rsp_pkt;
  }

  cmn_vars_ptr = chain_vars_ptr->cmn_vars_ptr;
  /*get the variables ptr for RX device*/
  rx_vars_ptr = chain_vars_ptr->rx_vars_ptr;

  /*Check if TX device is supported*/
  if (ftm_gsm_device_is_tx_supported(chain_vars_ptr->associated_tx_dev)) 
  {
    if (chain_vars_ptr->tx_vars_ptr == NULL)
    {
      MSG_1( MSG_SSID_FTM, MSG_LEGACY_ERROR, "ftm_gsm_extended_dispatch() tx_vars_ptr not created for tx device  %d", 
             chain_vars_ptr->associated_tx_dev );
      return rsp_pkt;
    }
  } /* ftm_gsm_device_is_tx_supported */
  else
  {
    MSG_1( MSG_SSID_FTM, MSG_LEGACY_MED, "ftm_gsm_extended_dispatch() TX device is not supported for ftm chain  %d", 
           chain_vars_ptr->gsm_chain );
  } /* !ftm_gsm_device_is_tx_supported */

  result_buff = (ftm_gsm_extended_result_union *)ftm_malloc(sizeof(ftm_gsm_extended_result_union));

  if (result_buff == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"failed to allocate memory for RSP packet", 0);
    return rsp_pkt;
  }

  /* Initialize response packet size to length of the common headers */
  /* the size of the return parameters will be added in the dispatch cases below */
  rsp_headers.ftm_hdr.cmd_rsp_pkt_size = sizeof(ftm_composite_cmd_header_type);

  buff_ptr = (uint8*)&(cmd_ptr->ftm_gsm_extended_params[0]);

  switch ( cmd_ptr->ftm_hdr.cmd_id )
  {
  case FTM_GSM_SET_RX_TIMING:
    (void)rfc_gsm_command_dispatch( rx_device, 
                                    (int)RFC_GSM_DEBUG_UPDATE_RX_TIMING, 
                                    (void *)buff_ptr);
    result_buff->amam_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_gsm_amam_sweep_return_type);
    break;

  case FTM_GSM_SET_TX_TIMING:
    /*pass the Tx device here*/
    (void)rfc_gsm_command_dispatch(chain_vars_ptr->associated_tx_dev, 
                                   (int)RFC_GSM_DEBUG_UPDATE_TX_TIMING, 
                                   (void *)buff_ptr);
    result_buff->amam_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_gsm_amam_sweep_return_type);
    break;

  case FTM_GSM_TX_GAIN_SWEEP:
    /*pass the Tx device here*/
    cmn_vars_ptr->ftm_gsm_cal_flag = TRUE;
    ftm_gen_tx_da_sweep( tx_device,
                         chain_vars_ptr->sub_id,
                         (ftm_linear_da_sweep_data_type *)buff_ptr,
                         ((uint16)( cmd_ptr->ftm_hdr.cmd_data_len / ( 5 * sizeof(uint8) )  )) );
    cmn_vars_ptr->ftm_gsm_cal_flag = FALSE;
    result_buff->gen_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_gsm_extended_result_type);
    break;

    //@todo: anupamav: fix later: gives compilation error
  case FTM_GSM_TX_ENVDC_CS_SWEEP:
    /*pass the Tx device here*/
    cmn_vars_ptr->ftm_gsm_cal_flag = TRUE;
    ftm_gen_tx_envdc_cs_sweep( tx_device,
                               chain_vars_ptr->sub_id,
                               (ftm_linear_envdc_cs_data_type *)buff_ptr,
                               ((uint16)( cmd_ptr->ftm_hdr.cmd_data_len / ( 2 * sizeof(uint16) ) )) );
    cmn_vars_ptr->ftm_gsm_cal_flag = FALSE;
    result_buff->gen_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_gsm_extended_result_type);
    break;

  case FTM_GSM_RSSI_TO_DB: /*Added from SCMM*/
    result_buff->rssi_to_dbm_result.db_x16 = ftm_gsm_rssi_to_dbm;
    result_buff->rssi_to_dbm_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_gsm_rssi_to_dbm_return_type);
    break;

  /* AMAM Sweep V2 (First Step: 8PSK extended; Second Step: DC) */
  case FTM_TX_CFG2_AMAM_SWEEP_V2:
    /*pass the Tx device here*/
    cmn_vars_ptr->ftm_gsm_cal_flag = TRUE;
    result_buff->amam_result.pa_ramp_max = rfgsm_mdsp_cal_gen_amam_linear_sweep_v2 ( tx_device,
                                                                                     chain_vars_ptr->sub_id,
                                                                                     (uint16*) buff_ptr);
    cmn_vars_ptr->ftm_gsm_cal_flag = FALSE;
    result_buff->amam_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_gsm_amam_sweep_return_type);
    break;

  case FTM_GSM_SET_RX_MULTISLOT :
    ftm_gsm_set_multislot_rx( rfm_dev, (uint8*) buff_ptr);
    result_buff->gen_result.error_code = (int32)FTM_SUCCESS;
    break;

  case FTM_GSM_SETUP_RX_BURST_FOR_EXPECTED_PWR:
    ftm_gsm_core_setup_rx_burst_calver(rx_device, (int16*) buff_ptr);
    result_buff->gen_result.error_code = FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size+=sizeof(ftm_gsm_extended_result_type);
    break;

  case FTM_GSM_GET_MEAS_PWR:
    if(g_meas_results.x2g_irat_enabled)
    {
      result_buff->rssi_result.db_x16 = g_meas_results.db_x16;
      result_buff->rssi_result.raw_rssi = g_meas_results.db_x16;
      result_buff->rssi_result.error_code = (int32)FTM_SUCCESS;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size +=sizeof(ftm_gsm_rssi_return_type);
      g_meas_results.x2g_irat_enabled = FALSE;
    }
    else
    {
      /* TODO.. Clean up in Phase 2 */
      result_buff->rssi_result.db_x16 = ftm_gsm_get_rssi_dbx16(rx_device);
      result_buff->rssi_result.raw_rssi = ftm_gsm_get_vars_ptr(rx_device)->ftm_gsm_state.ftm_rssi;
      result_buff->rssi_result.error_code = (int32)FTM_SUCCESS;
      rsp_headers.ftm_hdr.cmd_rsp_pkt_size +=sizeof(ftm_gsm_rssi_return_type);
    }
    break;

  case FTM_GSM_GET_MEAS_PWR_DRX:
    /* TODO.. Clean up in Phase 2 */
    result_buff->rssi_result.db_x16 = ftm_gsm_get_rssi_dbx16(rxd_device);
    /*Raw RSSI is stored in PRX vars only. No DRX vars are used in legacy*/
    result_buff->rssi_result.raw_rssi = ftm_gsm_get_vars_ptr(rx_device)->ftm_gsm_state.ftm_rssi_drx;
    result_buff->rssi_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size +=sizeof(ftm_gsm_rssi_return_type);
    break;

  case FTM_GSM_GET_MEAS_PWR_PRX_DRX:
    /* TODO.. Clean up in Phase 2 */ 
    result_buff->rssi_prx_drx_result.db_x16_prx = ftm_gsm_get_rssi_dbx16(rx_device);
    result_buff->rssi_prx_drx_result.raw_rssi_prx = ftm_gsm_get_vars_ptr(rx_device)->ftm_gsm_state.ftm_rssi;
    /* For DRX device */
    result_buff->rssi_prx_drx_result.db_x16_drx = ftm_gsm_get_rssi_dbx16(rxd_device);
    /*Raw RSSI is stored in PRX vars only. No DRX vars are used in legacy*/
    result_buff->rssi_prx_drx_result.raw_rssi_drx = ftm_gsm_get_vars_ptr(rx_device)->ftm_gsm_state.ftm_rssi_drx;
    result_buff->rssi_prx_drx_result.error_code = (int32)FTM_SUCCESS;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size +=sizeof(ftm_gsm_rssi_prx_drx_return_type);
    break;
  
  case FTM_GSM_GET_ENV_GAIN_NV_DATA:
    ftm_gsm_get_env_gain_nv(chain_vars_ptr->associated_tx_dev, &(result_buff->ftm_gsm_env_gain_nv_data));
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size +=sizeof(ftm_gsm_env_gain_nv_data_type);
    break;
  
  case FTM_GSM_FRAME_SWEEP_CFG_CMD:
    ftm_gsm_frame_sweep_cmd(FTM_GSM_FRAME_SWEEP_CFG, (uint8*) buff_ptr, cmd_ptr->ftm_hdr.cmd_data_len);
    break;

  case FTM_GSM_FRAME_SWEEP_EXEC_CMD:
    ftm_gsm_frame_sweep_cmd(FTM_GSM_FRAME_SWEEP_EXECUTE, (uint8*) buff_ptr, cmd_ptr->ftm_hdr.cmd_data_len);
    break;    

  case FTM_GSM_FRAME_SWEEP_GET_RESULTS_CMD:
    ftm_gsm_frame_sweep_cmd(FTM_GSM_FRAME_SWEEP_GET_RESULTS, (uint8 *)(&(result_buff->frame_sweep_result)), cmd_ptr->ftm_hdr.cmd_data_len);
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size +=sizeof(ftm_gsm_frame_sweep_result_buffer_type);
    break;

  case FTM_GSM_FRAME_SWEEP_CLEANUP_CMD:
    ftm_gsm_frame_sweep_cmd(FTM_GSM_FRAME_SWEEP_CLEANUP, (uint8 *)buff_ptr, cmd_ptr->ftm_hdr.cmd_data_len);
    break;
	  
  default:
    result_buff->gen_result.error_code = (int32)FTM_FAILURE;
    rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_gsm_extended_result_type);
    break;
  }


  /* Create response packet */
  /* Copy common header information from the request to the response */
  rsp_headers.diag_hdr             = cmd_ptr->diag_hdr;
  rsp_headers.ftm_hdr.cmd_id       = cmd_ptr->ftm_hdr.cmd_id;
  rsp_headers.ftm_hdr.cmd_data_len = cmd_ptr->ftm_hdr.cmd_data_len;

  /* At this point the response packet size equals the size of the headers plus
     the size of the fixed return parameters. If the response contains extra data
     whose size is determined by a parameter in the request, then we add it here. */

  rsp_pkt = ftmdiag_create_new_pkt(rsp_headers.ftm_hdr.cmd_rsp_pkt_size + extra_data_len);

  if ( rsp_pkt.pkt_payload != NULL )
  {
    /* copy header information */
    memscpy((void*)rsp_pkt.pkt_payload,
           rsp_headers.ftm_hdr.cmd_rsp_pkt_size + extra_data_len,
           (void*)&rsp_headers,
           sizeof(ftm_composite_cmd_header_type));

    /* copy fixed return parameters */
    memscpy((void*)((byte*)rsp_pkt.pkt_payload + sizeof(ftm_composite_cmd_header_type)),
           (rsp_headers.ftm_hdr.cmd_rsp_pkt_size + extra_data_len - sizeof(ftm_composite_cmd_header_type)),
           (void*)result_buff,
           (rsp_headers.ftm_hdr.cmd_rsp_pkt_size - sizeof(ftm_composite_cmd_header_type)));

    if ( extra_data_len > 0 )
    {
      /* copy extra variable length return data if present */
      memscpy((void*)((byte*)rsp_pkt.pkt_payload + rsp_headers.ftm_hdr.cmd_rsp_pkt_size),
             extra_data_len,
             local_buffer,
             extra_data_len);
    }
  }
  else
  {
    MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR,
          "Cannot allocate memory for response packet payload, cmd id = %d",
          cmd_ptr->ftm_hdr.cmd_id);
  }
  if ( local_buffer != NULL )
  {
    ftm_free(local_buffer);
  }

  if (result_buff != NULL)
  {
    ftm_free(result_buff);
  }

  return rsp_pkt;
} /* end ftm_gsm_extended_dispatch */




/*----------------------------------------------------------------------------*/
/*!
   @brief
   This function handles requests to run GSM debug and profiling tests by 
   dispatching the appropriate functions.

   @param sub_id - GSM subscription to execute commands for
   @param cmd_ptr - FTM dispatch data

   @retval
   Response packet
*/
ftm_rsp_pkt_type ftm_gsm_enh_debug_dispatch( uint8 sub_id,
                                             ftm_gsm_enh_debug_pkt_type * cmd_ptr )
{
  /* Initialise local variables */
  ftm_rsp_pkt_type rsp_pkt = {FTM_RSP_DO_LEGACY, 0, NULL, FALSE};
  ftm_composite_cmd_header_type rsp_headers;
  uint8 * buff_ptr;
  ftm_gsm_enh_debug_result_union * result_buff = NULL;
  uint32 cmd_return_val = 0;
  ftm_gsm_enh_debug_in_vars_u  in_vars;
  ftm_gsm_enh_debug_out_vars_u out_vars;

  /* Allocate memory for results */
  result_buff = (ftm_gsm_enh_debug_result_union *)ftm_malloc(sizeof(ftm_gsm_enh_debug_result_union));
  if (result_buff == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "ftm_gsm_enh_debug_dispatch() failed to allocate memory for RSP packet");
    return rsp_pkt;
  } 

  /* Initialise the response packet size */
  rsp_headers.ftm_hdr.cmd_rsp_pkt_size = sizeof(ftm_composite_cmd_header_type);

  /*************************************/
  switch( cmd_ptr->ftm_hdr.cmd_id )
  {
    case FTM_GSM_INIT_RF:
      FTM_GSM_ENH_DBG_CMD_MSG( "init FTM mode" );
      (void)ftm_common_resource_config_request( TRUE );
       ftm_mode = DMSS_MODE;
      break;

    case FTM_GSM_CMD_PROC_SET_CLK_SPEED:
      FTM_GSM_ENH_DBG_CMD_MSG( "set clock speed" );
      cmd_return_val = ftm_gsm_enh_debug_set_clk( cmd_ptr->ftm_gsm_cmn_params.q6_clock_speed_khz );
      break;

    case FTM_GSM_CMD_PROC_SET_BIMC_RATE:
      FTM_GSM_ENH_DBG_CMD_MSG( "set BIMC" );
      cmd_return_val = ftm_gsm_enh_debug_set_bimc( cmd_ptr->ftm_gsm_cmn_params.bimc_rate_kbps );
      break;

    case FTM_GSM_CMD_PROC_READ_MEM_LATENCY:
      FTM_GSM_ENH_DBG_CMD_MSG( "read memory latency" );
      cmd_return_val = ftm_gsm_enh_debug_rd_mem_latency();
      break;

    case FTM_GSM_INIT_MSGR_CLIENT:
      FTM_GSM_ENH_DBG_CMD_MSG( "init MSGR client" );
      cmd_return_val = ftm_gsm_enh_debug_reg_msgr_client( TRUE );
      break;

    case FTM_GSM_DEINIT_MSGR_CLIENT:
      FTM_GSM_ENH_DBG_CMD_MSG( "deinit MSGR client" );
      cmd_return_val = ftm_gsm_enh_debug_reg_msgr_client( FALSE );
      break;

    case FTM_GSM_ALLOCATE_RXLM_HANDLE:
      FTM_GSM_ENH_DBG_CMD_MSG( "alloc RxLM handle" );
      cmd_return_val = ftm_gsm_enh_debug_rxlm_handle( sub_id, TRUE );
      break;

    case FTM_GSM_DEALLOCATE_RXLM_HANDLE:
      FTM_GSM_ENH_DBG_CMD_MSG( "dealloc RxLM handle" );
      cmd_return_val = ftm_gsm_enh_debug_rxlm_handle( sub_id, FALSE );
      break;

    case FTM_GSM_ALLOCATE_TXLM_HANDLE:
      FTM_GSM_ENH_DBG_CMD_MSG( "alloc TxLM handle" );
      cmd_return_val = ftm_gsm_enh_debug_txlm_handle( sub_id, TRUE );
      break;

    case FTM_GSM_DEALLOCATE_TXLM_HANDLE:
      FTM_GSM_ENH_DBG_CMD_MSG( "dealloc TxLM handle" );
      cmd_return_val = ftm_gsm_enh_debug_txlm_handle( sub_id, FALSE );
      break;

    case FTM_GSM_START_GSM:
      FTM_GSM_ENH_DBG_CMD_MSG( "start GSM" );
      cmd_return_val = ftm_gsm_enh_debug_start_gsm_cmd( sub_id, &cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details );
      break;

    case FTM_GSM_STOP_GSM:
      FTM_GSM_ENH_DBG_CMD_MSG( "stop GSM" );
      cmd_return_val = ftm_gsm_enh_debug_stop_gsm_cmd( sub_id, &cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details );
      break;

    case FTM_GSM_RX_WAKEUP:
      FTM_GSM_ENH_DBG_CMD_MSG( "Rx wakeup" );
      cmd_return_val = ftm_gsm_enh_debug_rx_wakeup_cmd( sub_id, &cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details );
      break;

    case FTM_GSM_TX_WAKEUP:
      FTM_GSM_ENH_DBG_CMD_MSG( "Tx wakeup" );
      cmd_return_val = ftm_gsm_enh_debug_tx_wakeup_cmd( sub_id, &cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details );
      break;

    case FTM_GSM_RX_SLEEP:
      FTM_GSM_ENH_DBG_CMD_MSG( "Rx sleep" );
      cmd_return_val = ftm_gsm_enh_debug_rx_sleep_cmd( sub_id, &cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details );
      break;

    case FTM_GSM_TX_SLEEP:
      FTM_GSM_ENH_DBG_CMD_MSG( "Tx sleep" );
      cmd_return_val = ftm_gsm_enh_debug_tx_sleep_cmd( sub_id, &cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details );
      break;

    case FTM_GSM_CMD_PROC_RX_BURST:
      FTM_GSM_ENH_DBG_CMD_MSG( "Rx burst" );
      cmd_return_val = ftm_gsm_enh_debug_rx_burst_cmd( sub_id, 
                                                       &cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details,
                                                       &cmd_ptr->ftm_gsm_params.ftm_gsm_rx_details );
      break;

    case FTM_GSM_CMD_PROC_SET_TX_BAND:
      FTM_GSM_ENH_DBG_CMD_MSG( "Tx band" );
      cmd_return_val = ftm_gsm_enh_debug_set_tx_band_cmd( sub_id,
                                                          &cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details,
                                                          &cmd_ptr->ftm_gsm_params.ftm_gsm_tx_details );
      break;

    case FTM_GSM_CMD_PROC_TX_BURST:
      FTM_GSM_ENH_DBG_CMD_MSG( "Tx burst" );
      cmd_return_val = ftm_gsm_enh_debug_tx_burst_cmd( sub_id, 
                                                       &cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details,
                                                       &cmd_ptr->ftm_gsm_params.ftm_gsm_tx_details );
      break;

    case FTM_GSM_CMD_PROC_IDLE_FRAME_PROC:
      FTM_GSM_ENH_DBG_CMD_MSG( "idle frame processing" );
      break;

    case FTM_GSM_CMD_PROC_SET_ANTENNA:
      FTM_GSM_ENH_DBG_CMD_MSG( "set antenna" );
      ftm_gsm_enh_debug_set_antenna_req( sub_id, 
                                         &cmd_ptr->ftm_gsm_params.ftm_gsm_antenna_details );
      break;

    case FTM_GSM_CMD_PROC_CM_ENTER:
      FTM_GSM_ENH_DBG_CMD_MSG( "cm enter" );
      break;

    case FTM_GSM_CMD_PROC_CM_EXIT:
      FTM_GSM_ENH_DBG_CMD_MSG( "cm exit" );
      break;

    case FTM_GSM_CMD_PROC_SET_SAR_LIMIT:
      FTM_GSM_ENH_DBG_CMD_MSG( "set SAR limit" );
      break;

    case FTM_GSM_CMD_PROC_SET_VBATT_LIMIT:
      FTM_GSM_ENH_DBG_CMD_MSG( "set VBATT limit" );
      break;

    case FTM_GSM_CMD_INLINE_MEAS_INIT_ENTER_PARAMS:
      FTM_GSM_ENH_DBG_CMD_MSG( "meas init enter params" );
      break;

    case FTM_GSM_CMD_INLINE_MEAS_INIT_SETUP_PARAMS:
      FTM_GSM_ENH_DBG_CMD_MSG( "meas init setup params" );
      break;

    case FTM_GSM_CMD_INLINE_MEAS_INIT_EXIT_PARAMS:
      FTM_GSM_ENH_DBG_CMD_MSG( "meas init exit params" );
      break;

    case FTM_GSM_CMD_INLINE_MEAS_INIT_RF_HEADER:
      FTM_GSM_ENH_DBG_CMD_MSG( "meas init RF header" );
      break;

    case FTM_GSM_CMD_INLINE_MEAS_INIT_PARAMS:
      FTM_GSM_ENH_DBG_CMD_MSG( "meas init params" );
      break;

    case FTM_GSM_CMD_INLINE_MEAS_ENTER:
      FTM_GSM_ENH_DBG_CMD_MSG( "meas enter" );
      break;

    case FTM_GSM_CMD_INLINE_MEAS_EXIT:
      FTM_GSM_ENH_DBG_CMD_MSG( "meas exit" );
      break;

    case FTM_GSM_CMD_INLINE_MEAS_BUILD_SCRIPTS:
      FTM_GSM_ENH_DBG_CMD_MSG( "meas build scripts" );
      break;

    case FTM_GSM_CMD_INLINE_MEAS_GET_IRAT_INFO:
      FTM_GSM_ENH_DBG_CMD_MSG( "meas get i-RAT info" );
      break;

    case FTM_GSM_CMD_SUB_SM_TEST_INIT:
      FTM_GSM_ENH_DBG_CMD_MSG( "sub state machine init" );
      (void)rfgsm_core_intf_sm_init_sub();
      break;

    case FTM_GSM_CMD_SUB_SM_TEST_DESTROY:
      FTM_GSM_ENH_DBG_CMD_MSG( "sub state machine destroy" );
      (void)rfgsm_core_intf_sm_destroy_sub();
      break;

    case FTM_GSM_CMD_SUB_SM_TEST_DRIVE:
      FTM_GSM_ENH_DBG_CMD_MSG( "sub state machine drive" );
      rfgsm_core_intf_sm_drive_sub( sub_id, (RfgsmCoreSmSubInputType)cmd_ptr->ftm_gsm_cmn_params.state_machine_input );
      break;

    case FTM_GSM_CMD_RX_SM_TEST_INIT:
      FTM_GSM_ENH_DBG_CMD_MSG( "rx state machine init" );
      (void)rfgsm_core_intf_sm_init_rx( cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details.primary_device );
      break;

    case FTM_GSM_CMD_RX_SM_TEST_DESTROY:
      FTM_GSM_ENH_DBG_CMD_MSG( "rx state machine destroy" );
      (void)rfgsm_core_intf_sm_destroy_rx( cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details.primary_device );
      break;

    case FTM_GSM_CMD_RX_SM_TEST_DRIVE:
      FTM_GSM_ENH_DBG_CMD_MSG( "rx state machine drive" );
      rfgsm_core_intf_sm_drive_rx( cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details.primary_device, 
                                   (RfgsmCoreSmRxInputType)cmd_ptr->ftm_gsm_cmn_params.state_machine_input );
      break;

    case FTM_GSM_CMD_TX_SM_TEST_INIT:
      FTM_GSM_ENH_DBG_CMD_MSG( "tx state machine init" );
      (void)rfgsm_core_intf_sm_init_tx( cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details.primary_device );
      break;

    case FTM_GSM_CMD_TX_SM_TEST_DESTROY:
      FTM_GSM_ENH_DBG_CMD_MSG( "tx state machine destroy" );
      (void)rfgsm_core_intf_sm_destroy_tx( cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details.primary_device );
      break;

    case FTM_GSM_CMD_TX_SM_TEST_DRIVE:
      FTM_GSM_ENH_DBG_CMD_MSG( "tx state machine drive" );
      rfgsm_core_intf_sm_drive_tx( cmd_ptr->ftm_gsm_cmn_params.ftm_gsm_cmn_details.primary_device, 
                                   (RfgsmCoreSmTxInputType)cmd_ptr->ftm_gsm_cmn_params.state_machine_input );
      break;

    case FTM_GSM_RFLM_GET_TX_ALPHA:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM get Tx alpha" );
      in_vars.tx_alpha_in_vars.rflm_gsm_intf_idx = cmd_ptr->ftm_gsm_params.ftm_gsm_rflm_details.intf_idx;
      (void)rflm_gsm_get_tx_alpha( txlm_handle[sub_id], 
                                   &in_vars.tx_alpha_in_vars, 
                                   &out_vars.tx_alpha_out_vars );
      cmd_return_val = (uint32)out_vars.tx_alpha_out_vars;
      break;

    case FTM_GSM_RFLM_GET_RX_ALPHA:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM get Rx alpha" );
      in_vars.rx_alpha_in_vars.rflm_gsm_intf_idx = cmd_ptr->ftm_gsm_params.ftm_gsm_rflm_details.intf_idx;
      (void)rflm_gsm_get_rx_alpha_v2( rxlm_handle_prx[sub_id], 
                                      &in_vars.rx_alpha_in_vars, 
                                      &out_vars.rx_alpha_out_vars );
      cmd_return_val = ( out_vars.rx_alpha_out_vars.idle_mode_rx_alpha << 16 ) | 
                       ( out_vars.rx_alpha_out_vars.rx_alpha );
      break;

    case FTM_GSM_RFLM_GET_RF_GAIN:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM get RF gain" );
      break;

    case FTM_GSM_RFLM_GET_KV_DATA:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM get Tx KV data" );
      break;

    case FTM_GSM_RFLM_GET_RSB_DATA:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM get Rx RSB data" );
      break;

    case FTM_GSM_RFLM_GET_LIF_DATA:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM get Rx LIF data" );
      break;

    case FTM_GSM_RFLM_REG_CB:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM register FW callback" );
      break;

    case FTM_GSM_RFLM_DEREG_CB:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM deregister FW callback" );
      break;

    case FTM_GSM_RFLM_PROG_TXFE:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM prog TxFE" );
      break;

    case FTM_GSM_RFLM_TX_FRAME_CONFIG:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM TxFE frame config" );
      break;

    case FTM_GSM_RFLM_START:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM start" );
      break;

    case FTM_GSM_RFLM_RELEASE:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM release" );
      break;

    case FTM_GSM_RFLM_PROG_CCS_RX_WAKEUP:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM CCS Rx Wakeup" );
      break;

    case FTM_GSM_RFLM_PROG_CCS_TX_WAKEUP:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM CCS Tx Wakeup" );
      break;

    case FTM_GSM_RFLM_PROG_CCS_RX:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM CCS Rx" );
      break;

    case FTM_GSM_RFLM_PROG_CCS_TX:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM CCS Tx" );
      break;

    case FTM_GSM_RFLM_PROG_CCS_TX_BAND:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM CCS Tx band" );
      break;

    case FTM_GSM_RFLM_PROG_CCS_THERM:
      FTM_GSM_ENH_DBG_CMD_MSG( "RFLM CCS Tx therm read" );
      break;

    case FTM_GSM_RFLM_PRINT_EVENT_DETAILS:
      FTM_GSM_ENH_DBG_CMD_MSG( "print DM event details" );
      break;

    case FTM_GSM_RFLM_PRINT_CCS_TQ_DATA:
      FTM_GSM_ENH_DBG_CMD_MSG( "print CCS TQ data" );
      break;

    case FTM_GSM_CORE_SET_DEBUG_FLAGS:
      FTM_GSM_ENH_DBG_CMD_MSG( "set debug flags" );
      break;

    default:
      MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, 
             "ftm_gsm_enh_debug_dispatch() unhandled cmd ID %d", 
             cmd_ptr->ftm_hdr.cmd_id);
      break;
  }

  /* Default common response packet */
  result_buff->generic_result.error_code = FTM_SUCCESS;
  result_buff->generic_result.generic_result = cmd_return_val;
  rsp_headers.ftm_hdr.cmd_rsp_pkt_size += sizeof(ftm_gsm_enh_debug_generic_result_type);
  /*************************************/
  
  /* Create response packet */
  /* Copy common header information from the request to the response */
  rsp_headers.diag_hdr             = cmd_ptr->diag_hdr;
  rsp_headers.ftm_hdr.cmd_id       = cmd_ptr->ftm_hdr.cmd_id;
  rsp_headers.ftm_hdr.cmd_data_len = cmd_ptr->ftm_hdr.cmd_data_len;

  /* At this point the response packet size equals the size of the headers plus
     the size of the fixed return parameters. If the response contains extra data
     whose size is determined by a parameter in the request, then we add it here. */
  rsp_pkt = ftmdiag_create_new_pkt( rsp_headers.ftm_hdr.cmd_rsp_pkt_size );

  if ( rsp_pkt.pkt_payload != NULL )
  {
    /* copy header information */
    memscpy((void*)rsp_pkt.pkt_payload,
           rsp_headers.ftm_hdr.cmd_rsp_pkt_size,
           (void*)&rsp_headers,
           sizeof(ftm_composite_cmd_header_type));

    /* copy fixed return parameters */
    memscpy((void*)((byte*)rsp_pkt.pkt_payload + sizeof(ftm_composite_cmd_header_type)),
           (rsp_headers.ftm_hdr.cmd_rsp_pkt_size - sizeof(ftm_composite_cmd_header_type)),
           (void*)result_buff,
           (rsp_headers.ftm_hdr.cmd_rsp_pkt_size - sizeof(ftm_composite_cmd_header_type)));
  }
  else
  {
    MSG_2( MSG_SSID_FTM, MSG_LEGACY_ERROR,
           "ftm_gsm_enh_debug_dispatch() failed to allocate memory for rsp_pkt sub %d, cmd ID %d",
           sub_id, cmd_ptr->ftm_hdr.cmd_id );
  }

  /* Free the memory allocated for the result buff */
  if( result_buff != NULL )
  {
    ftm_free( result_buff );
  }

  return rsp_pkt;
}



#endif /* FEATURE_GSM */
#endif /* FEATURE_FACTORY_TESTMODE */
