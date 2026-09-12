
/*!
  @file rfgsm_cmd_processing.c 

  @brief
  This file dispatches and sends responses for any REQ that GSM RF driver receives.

*/

/*===========================================================================

  Copyright (c) 2012 - 2016 by Qualcomm Technologies Incorporated. 
  All Rights Reserved.

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================
Copyright (c) 2012 - 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

/*===========================================================================


                EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/rf/mc/src/rfgsm_cmd_processing.c#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $ 2

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/07/19   mpa     Adding Histogram Profiling
12/15/16   kpa     DOnt clear the ASDIV settings for active RFM DEV
10/03/16   kpa     Clear the ADIV params in Rx sleep
08/30/16   kpa     CR_1060886: Fix memory leak caused by unnecessarily allocation of tx trans WTR script
04/29/16   hkm     RFHW information in F3 messages during START GSM
03/08/16   dsm     AsDiv with max power back off for SAR consideration
12/22/15   sas     Changes for fix ASDIV antenna switch issue
11/27/15   hkm     TXRXSplit support changes
11/10/15   tws     Add FBRX VSWR support
11/03/15   bp      Stop further processing of therm_read_rsp if temp. 
                   compensation is stopped through GSM Tx Disable.
09/10/15   sc      Deprecate core handle cmd_buff log and use cmd_proc log
08/21/15   sc      Deprecate subscription 3 reducing memory footprint
08/18/15   ggs     Missing functionality from Split Rx/Tx merge: 
                   Pass back Tx power in  dBm100 to GL1 in Tx confirmation
08/12/15   sc      Add split Rx/Tx state machines - part 1 (deprecation)
08/05/15   ec      Remove device usage from CM enter\exit processing
07/29/15   rc      Add power logging support
07/27/15   hkm     Providing the CCS exec time based on the op_mode request
07/15/15   cpm     FR27546: MTPL accounted for the different antennas
07/14/15   sw      Check for NULL pointer and fix compiler error
07/07/15   zlg/sw  AsDiv with max power back off for SAR consideration
07/03/15   sc      Fix LIF and enable optimised Rx burst processing
                   Temporarily compiled out old functionality
07/02/15   ec      Retrieve SAWless support table from MC layer at start
06/24/15   jb/sc   Add FTM command proc API calls to enh debug dispatch
06/19/15   sc      Update and enable optmised Rx burst and feature guard
06/16/15   zbz     Return Tx/Rx wake up CCS time profiling to L1
06/10/15   sw/cs   Remove unwanted device F3 in CM Enter
05/28/15   sc      Enable thermistor and vbatt reads
05/19/15   sc      Move Rx burst checks and logic from command proc to mc
05/15/15   sc      Correct task number for sub 2 logging
05/12/15   zgl     Changes for FR 24616: Algorithm to mitigate VBatt droop (down to 2.5V) via TX backoff
05/07/15   sc      Implement command processing log
04/20/15   sc      Populate RF sequence number passed from L1 in RFLM DM 
04/17/15   ggs     Pass back Tx power in  dBm100 to GL1 in Tx confirmation
04/16/15   sc      Deprecate legacy call-flow APIs for memory savings
04/09/15   piy     Fixed bug in Device status read feature for Mon Burst
04/09/15   sc      Remove device assumtions for split Rx/Tx
04/07/15   sc      Use correct confirmation message type for GSM stop
03/25/15   sc      Add interface to MC for Start/Stop Rx/Tx Wakeup/Sleep
03/11/15   sc      Add Rx/Tx Wakeup/Sleep interface
03/02/15   piy     Added Time Profiling for Wakeup, Sleep, Rx & Tx burst via debug NV
02/18/15   sw      Introduce hooks for new GRF state machine
02/17/15   sg      Add FEATURE_TABASCO_MODEM featurization
02/05/15   ec      Move logging under sawless logging flag
01/30/15   sw      Added NULL condition for ASD respponse payload data 
01/30/15   sw      GL1->RF handshake mechanism to handle ASD response error cases
01/27/15   sc      Force crash if RFM device mode is not GSM during sleep
01/23/15   sw      Return wakeup and enter modes to GL1 (for IMRxD)
12/17/14   sw      Set variable set_antenna_position to default state if requesetd position
                   is already set in set_antenna_req
12/10/14   sc      Correct message format for error handler
12/02/14   sml     Read WTR or RF front end devices status in between Rx/Tx 
                   burst along with GRFC status
11/25/14   sc      Err Fatal on bad input from GL1
11/25/14   sw      Disable therm read during WTR dedicated hopping
11/18/14   sw/sc   Deprecate legacy state machines and move cmd buff out of rfa
11/12/14   tsr     Fix bug for Idle mode RXD
10/21/14   sc      Reduce code execution limiting use of get qsym API
10/06/14   sc      Deprecate unused functionality
10/10/14   tsr     Removed hardcoding of GLI lower CCS prio flag to false
09/30/14   tsr     GSM IDLE Mode RXD support
09/30/14   tws     Remove the wakeup callback function pointer
09/19/14   sc      Force RxLM update for every wakeup
09/11/14   sml     Adding start API Pcycle to know about pre-emption done to RF task
09/03/14   tws     Replace rfgsm_rfm_status_type with rfa_rf_gsm_cmd_outcome_t
08/19/14   zbz     Add RF GSM CCS execution time profiling
08/11/14   shb     Fixed offtarget compiler error
08/11/14   sw      Print the sub ID as part of the error message (improved logging)
08/11/14   sw      GL1 only expects ASD callback. Do not send Antenna CNF message
08/07/14   ggs     Add PRX and DRX request result to enter mode and wakeup 
                   confirmation messages
07/30/14   cj      Added changes for Tx sharing feature
07/28/14   hoh     Detect failure case for invalid Tx device passed from L1
07/28/14   ec      Re-enable burst metrics processing with corrected API
07/21/14   sw      Replace gas_id with sub_id in the ASD callback function
07/14/14   ec      Comment out burst metrics processing until API can be corrected
07/14/14   ec      Fix critical static analysis error due to out of bounds array reference
07/14/14   ec      Create and register interface to receive burst metrics from GL1
07/12/14   sw      Replace RFGC_T_ERR macro with a function (to reduce memory footprint)
07/09/14   sw      Introduce new macro to either error fatal or print message depending on nv bit
07/01/14   tsr     DR-DSDS: Lower RX CCS event priority when GSM sub is in Idle mode
07/01/14   jr      Enable therm read on sub-2
06/30/14   ec      Mainline Rx Diversity
06/30/14   sc      Do not place rfm mode to parked on exit mode
06/27/14   sc      RxLM and dev ID per sub and minimised call-flow for RxLM update logic
06/26/14   sw      Fix small bug where incorrect task_id is printed
06/18/14   sw      Remove calls to internal geran function gl1_get_FN() from RF
                   (port CR609471 from Dime)
06/12/14   sw      Use GL1 api to map sub_id to as_id
06/11/14   sc/ggs  Add flag to check if rxlm settings update is required during
                   wake up
06/10/14   hk      Edge core parameter from GL1.
09/06/14   sw      Port from Dime - GSM RF needs to send a command response to GL1 indicating that asdiv switch is already in requested position (CR640557)
06/06/14   ec      Do not update RFM state to parked when exiting GSM
06/06/14   sw      Fix ASD KW error + ASD code optimisation
06/02/14   hk      Adding wakeup call back to L1.
05/28/14   sw      Move therm read from enter mode to tx enable
05/20/14   sw      Fix OFT compiler errors
05/15/14   sw      Return enter mode and wakeup execution times to GL1
05/02/14   sc      Default as_id to same as sub_id preventing g_view multiSIM crash
02/12/14   sw      Move prepare therm read settings from enter mode to TX enable
04/24/14   sw      Small correction to ASD F3 message
04/23/14   tsr     KW Fixes
04/16/14   tsr     GFW/Interface update for GSM RX Diversity
04/16/14   sb      IP2 cal Interface and functionality changes
04/09/14   sc      Manage rfm_mode state machine
04/07/14   ggs     KW fixes
03/31/14   tsr     Added support for handling combined Enter Mode for PRX and DRX
03/26/14   tsr     Add support to query rfm device for TX capability
03/26/14   sb      Add support for IP2 cal for Dime PM
03/25/14   sc      Use subscription from GL1
03/24/14   sc      Remove dynamic task priority manipulation
03/12/14   tsr     Added RXD support for RX burst 
03/12/14   sw      ASD tidyup
03/07/14   sw      Fix OFT compilation error (no need for UMID featurisation)
03/04/14   sw      Introduce ASD message router definitions and remove GFW framework
02/28/14   sc      Pass core data through mc to core layers
02/28/14   sw      Therm read error handling
02/26/14   sc      Pass core data through cmd processing and mc layer
02/20/14   tsr     GSM RX Diversity
02/20/14   sc      Remove call to rfm_wakeup and call mc directly
02/20/14   ec	   Feature guard therm read response structure for development build compilation
02/11/14   sw      Remove therm read Triton featurisation
30/01/14   sc      Give cnf required control to GL1 and correct Rx enter callflow
01/28/14   sw      Block registration of RFLM RSP UMIDS for all RF SUBS
12/10/13   sc      Switch to use get timing info from enter mode and cm enter
11/05/13   ec	   Remove empty parameters from F3 macros to save memory
10/15/13   sc      Mainline L1-RF interface changes
09/20/13   par     Update RF task priority setting
08/16/13   ec	   Fix incorrect Task ID
08/16/13   ec	   Fix error in GTA Exit Mode; response UMID set incorrectly for tasks 2 and 3;
                   wrong return value in success case; incorrect type used for sizing;
                   request type missing from union
08/08/13   sml     Fixing KW warning
07/30/13   pl      Enable new therm read interface
07/30/13   sml     Adding ASD RSP from GFW
07/29/13   pl      use FEATURE_RF_ASDIV instead of FEATURE_GSM_ASD_RSP
07/29/13   sml     Generic RFSW-FW interaction to generate an ACK for a CCS event
07/29/13   sml/svi ASD support for GSM tech
07/29/13   tws     Initial support for RX diversity commands
07/29/13   tws     Use new RF_TASK_1 for GSM.
07/24/13   ec      Map rf_tasks to subscriptions at msg registration. Pass rf_task
                   into relevant MC layer functions
07/22/13   sc      Check triple buffer ID passed from L1
07/23/13   ec	   Do not always return in case of NULL pointer, to preserve dispatcher call-flow
07/22/13   ec	   Use new F3s/Null pointer checks
07/17/13   sc      Merge Vreg On/Off to Enter/Exit Wakeup/Sleep and merge get timing
07/16/13   sc      Move mc/core functionality out of dispatcher and into mc/core layer
07/15/13   sc      Add struct and enum that only used by this module to remove header
07/12/13   sc      Improve the dispatch variable checking and error handling
07/12/13   av      GTA EXIT mode
07/10/13   sc      Clean up result variables and add definitions for dispatcher verifications
07/10/13   sc      Use msg ptr instead of echo response
07/10/13   sml     Fixing off-target error
07/09/13   sml     Block registration of GFW RSP UMIDS for 2 SUBS
06/28/13   ec      Fix incomplete porting of GSM Tune Away (GTA) enter mode
06/28/13   ec      Log the rf_task context commands are executing in.
06/25/13   ec      Make changes to support Therm Read Cancellation
06/20/13   ec      Fix compiler error on single sim targets
06/20/13   ec      Add support for GSM Tune Away (GTA) enter mode
05/30/13   tws     Add support for GSM sub 3
05/30/13   tws     Make the GFW queue task specific.
04/09/13   sc      Handle return from mc exit mode
04/08/13   sc      Reassign the core RXLM variable during wakeup from GL1
04/09/13   tsr     Added RF APPS->RF Task MSGR framework for temp comp to make temp comp data thread safe
03/18/13   svi     Replaced memcpy() by safer memscpy().
03/12/13   tsr     Added to tag to each therm read event handle to identify the successful retrieval of that handle
03/05/13   sb      Call Vbatt read/Vbatt comp APIs in online mode
02/26/13   sc      Register the REQ cmds for CM enter/exit with RF task
02/25/13   sc      Added cm enter/exit to init/de-init iRAT during gl1 cm init/terminate 
02/15/13   sc      Call rfgsm_mc_exit_mode
02/11/13   sc      Clean up F3s adding meaningful details and add logging
02/11/13   sc      Add GSM mode set for enter standalone and exit standalone
02/07/13   sc      Fix compiler warnings due to rfm_exit_mode check-in
02/06/13   sc      Call rfm_exit_mode to ensure previous tech is cleaned up
02/04/13   tsr     Fix Klockwork Errors from previous check-in.
01/31/13   tsr     Added framework for recieving therm read response from GFW
01/31/13   sc      Added cnf required flag. Temp default TRUE avoiding coordinated check-in
01/29/13   sc      Dynamically adjust the RF Task Priority
01/22/13   av      Removing the contraint that each rf_task processes messages based on rfm_device
01/11/13   aka     Added support for simultaneous Tx SAR
01/10/13   av      Create new REQ and CNF ids for subscriber 2 and handle them in rf_Task_2
01/09/13   sr      Made changes to run vbatt/temp comp in rf app task context.
01/07/12   sc      Fix not setting Tx enable result to success
01/03/12   sc      Added RF<->FW msgr interface CMD/RSP and framework
12/18/12   sc      Added therm read event generation framework
12/18/12   sc      Remove unused tx dynamic ptr
12/17/12   sc      Fix compiler warning due to unused variable in exit mode
12/14/12   sc      Remove unrequired iRAT req and add exit mode framework
12/11/12   sc      Ensure error handling of NULL ptr from L1 for Set Tx band
12/07/12   tws     Fix Klocwork errors.
12/06/12   tws     Fix Klocwork errors.
12/05/12   aka     Removed some hardcoding for device id
11/28/12   av      Enabling the rf_task to process only sub0 and rf_task_2 to process to ony sub1
                   enabling it now as gl1 has fixed passing the right rfm_device to rf in all the messages.
11/26/12   aka     Compiler warning fixes
11/26/12   aka     Changes to use a path specific gsm handle
11/23/12   tws     Remove deprecated GERAN includes.
11/21/12   av      Added rfm_device parameters to mc/core/mdsp/msm layers.
11/06/12   sc      Cleanup unrequired features/definitions
10/12/12   sc      Added support for iRAT startup cleanup RF Task implementation
10/10/12   sc      Move all Rx burst mc API calls to new prepare Rx CCS event mc API
10/05/12   av      Expanded rf task architecture to include more tasks. 
10/04/12   sc      Move to new RF Task dispatcher method to avoid UMID REX conflicts
10/03/12   tsr     First cut of RF time profile.
08/28/12   ggs     Remove unnecessary wrong device ID F3 messages  
08/24/12   sb/tsr  Made changes to clear the NON SBIs only during band change and SBIs during setup TX/RX burst
08/16/12   sc      Added bringup debug messages and code 
08/08/12   sc      Check device from GL1 before passing to RFGSM MC APIs
08/07/12   sc      Clearing seperate Rx and Tx scripts for CCS buffer tracability
08/06/12   sc      Added clearing of rf hal buffers before use
08/06/12   sc      Edit API to update CCS header FW vars
08/02/12   sc      Add num slots to setup Tx burst API
07/26/12   sr      Changes to APIS to take rfm_dev as parameter.
07/24/12   sc      Added RFM_DEVICE to the payloads
07/06/12   sc      Edit API parameters to support CCS buffer format
07/06/12   sc      Remove unrequired num transactions init functionality
06/27/12   sc      Remove no longer required parameter from Set Tx Band function call
06/07/12   sc      Cleaned up dispatcher verification and results
06/06/12   sc      Remove RF Manager calls and replace with RF Main Control calls
04/26/12   sc      Added shared memory interface support, integrating buffer ptrs
04/24/12   ggs     Replaced FEATURE_RF_HAS_GSM with FEATURE_GSM
03/20/12   sc      Edited dispatch functions' process-flow
03/13/12   ggs     Fixed typo in rfgsm_dispatch_outcome_t
02/27/12   ggs     Removed Set Tx power request. This will be added as part 
                   of the set TX burst payload.
                   Added Tx power control buffer to enter mode confirmation message.
02/13/12   sc      Removed dispatch init/de-init prototypes
02/08/12   ggs     Updated header comments info and response parameters
01/27/12   sc      Initial version
  

===========================================================================*/

/*===========================================================================

                  INCLUDE FILES

===========================================================================*/
#include "amssassert.h"
#include "appmgr.h"
#include "rex.h"
#include "msgr.h"
#include "rf_cap.h" /*Contains FEATURE_RF_TASK */
#include "rfa_msgr.h"
#include "rf_task.h"
#include "rf_cmd_interface.h"
#include "msgr_rex.h"
#include "rf_cmd_dispatcher.h"
#include "rfm.h"
#include "rfm_types.h"
#include "rfmeas_types.h"
#include "stringl.h"
#include "rfcom.h"
#include "rfgsm_data_manager.h"
#ifdef FEATURE_GSM
/*Need to place gsm specific msgr headers here*/
#include "rfgsm_mc.h"
#include "rfgsm_mc_rx.h"
#include "rfgsm_mc_tx.h"
#include "rfgsm_core.h"
#include "rfgsm_core_log.h"
#include "rf_hal_buffer.h"
#include "rfm_internal.h"
#include "gtmrs.h"
#include "qurt.h"
#include "rflm_gsm_msg.h"
#endif
#include "rf_task_common_services.h"
#include "rflm_gsm_dm.h"
#include "gl1_hw.h"
#include "rfgsm_nv_cmn.h"
#include "rfcommon_core_utils.h"
#include "rfcommon_nv_mm.h"

#ifdef FEATURE_RF_ASDIV
#include "modem_mem.h"
#include "rfcommon_asdiv_manager.h"
#endif

#include "geran_test_mode_api.h"
#include "rfgsm_core_intf.h"

#ifdef FEATURE_RF_POWER_REPORTING
#include "rflm_diag_power_rfa.h"
#endif


/*HISTOGRAM logging*/

#include "prof_hist.h"



/*HISTOGRAM logging*/

extern prof_hist_log_packet_handle_t gsm_rf_hist_packet_handle;
extern prof_hist_param_t rx_gsm_wakeup_build_scripts_hist;
extern prof_hist_param_t rx_gsm_sleep_build_scripts_hist;
extern prof_hist_param_t tx_gsm_wakeup_build_scripts_hist;
extern prof_hist_param_t tx_gsm_sleep_build_scripts_hist;
extern prof_hist_param_t tx_gsm_burst_build_scripts_hist;
extern prof_hist_param_t rx_gsm_mon_build_scripts_hist;
extern prof_hist_param_t rx_gsm_prog_ccs_scripts_hist;
extern prof_hist_param_t tx_gsm_prog_ccs_scripts_hist;
extern prof_hist_param_t rx_gsm_prx_burst_build_scripts_hist;
extern prof_hist_param_t rx_gsm_prx_drx_burst_build_scripts_hist;






/*===========================================================================

                                DEFINITIONS

===========================================================================*/
/*!
  @brief
  If set by FTM disaptcher the confirmation messages are sent back to
  FTM command proc dispatcher.
*/
extern boolean ftm_gsm_enh_debug_msgr_client;



/*! @brief Union of all RF receiving msgs that RF task can receive */
typedef union
{
  rfa_rf_gsm_start_gsm_req_s                rfa_rf_gsm_start_gsm;
  rfa_rf_gsm_stop_gsm_req_s                 rfa_rf_gsm_stop_gsm;
  rfa_rf_gsm_wakeup_req_s                   rfa_rf_gsm_wakeup;
  rfa_rf_gsm_rx_wakeup_req_s                rfa_rf_gsm_rx_wakeup;
  rfa_rf_gsm_tx_wakeup_req_s                rfa_rf_gsm_tx_wakeup;
  rfa_rf_gsm_sleep_req_s                    rfa_rf_gsm_sleep;
  rfa_rf_gsm_rx_sleep_req_s                 rfa_rf_gsm_rx_sleep;
  rfa_rf_gsm_tx_sleep_req_s                 rfa_rf_gsm_tx_sleep;
  rfa_rf_gsm_rx_burst_req_s                 rfa_rf_gsm_rx_burst;
  rfa_rf_gsm_tx_enable_req_s                rfa_rf_gsm_tx_enable;
  rfa_rf_gsm_tx_disable_req_s               rfa_rf_gsm_tx_disable;
  rfa_rf_gsm_set_tx_band_req_s              rfa_rf_gsm_set_tx_band;
  rfa_rf_gsm_tx_burst_req_s                 rfa_rf_gsm_tx_burst;
  rfa_rf_gsm_idle_frame_processing_req_s    rfa_rf_gsm_idle_frame_processing;
  rfa_rf_gsm_cm_enter_req_s                 rfa_rf_gsm_cm_enter;
  rfa_rf_gsm_cm_exit_req_s                  rfa_rf_gsm_cm_exit;
  rfa_rf_gsm_update_temp_comp_req_s         rfa_rf_gsm_update_temp_comp;
#ifdef FEATURE_RF_ASDIV
  rfa_rf_gsm_set_antenna_req_s              rfa_rf_gsm_set_antenna; 
#endif
  rfa_rf_gsm_process_burst_metrics_req_s    rfa_rf_gsm_process_burst_metrics;
  rfa_rf_gsm_enter_mode_req_s               rfa_rf_gsm_enter_mode;
  rfa_rf_gsm_exit_mode_req_s                rfa_rf_gsm_exit_mode;
} rfgsm_msg_u;



/*! @brief Outcome of the dispatch. 
 *  @details The different outcomes of the dispatch to RF task used by GSM MC
 *           Dispatcher
 */ 
typedef enum
{
  RFGSM_DISPATCH_SUCCESSFUL=1,
  RFGSM_DISPATCH_UNSUCCESSFUL=2,
  RFGSM_DISPATCH_NULL_PTR=3,
  RFGSM_DISPATCH_INVALID_PARAM =4,
  RFGSM_DISPATCH_INVALID_DEVICE = 5,
  RFGSM_DISPATCH_NULL_INPUT_ARG = 6,
  RFGSM_DISPATCH_STATE_MACHINE_ERR = 7,
  RFGSM_DISPATCH_INVALID_RFM_MODE = 8,
  RFGSM_DISPATCH_DEVICE_NOT_RX_CAPABLE = 9,
  RFGSM_DISPATCH_DEVICE_NOT_TX_CAPABLE = 10,
  RFGSM_DISPATCH_MAX
}rfgsm_dispatch_outcome_t;

/* RFLM interface variables */
typedef union
{
  msgr_hdr_struct_type   hdr;
  rflm_gsm_therm_read_rsp_t rflm_gsm_therm_read_rsp; 
  rflm_gsm_asd_rsp_t        rflm_gsm_asd_rsp;
  rflm_gsm_device_status_read_rsp_t rflm_gsm_device_status_read_rsp;
} rfgsm_rflm_msg_u;

typedef struct ALIGN(8)
{
  q_link_type     link;
  rfgsm_rflm_msg_u msg;
} rfgsm_rflm_msgr_t;

#define RFGSM_RFLM_MSGR_QUEUE_SIZE 6
q_type  rfgsm_rflm_cmd_intf_msgr_q[RF_MAX_TASK];
q_type  rfgsm_rflm_cmd_intf_msgr_free_q[RF_MAX_TASK];
rfgsm_rflm_msgr_t  rfgsm_rflm_msgr[RF_MAX_TASK][RFGSM_RFLM_MSGR_QUEUE_SIZE];



/*!==========================================================================*/
/*! @var rfgsm_umid_req_list                                                 */
/*! @brief This array holds the request (REQ) command list used to register  */
/*!        the whole list with MSGR for subscription 1                       */
static msgr_umid_type rfgsm_umid_req_list[] = 
{
  RFA_RF_GSM_START_GSM_REQ,
  RFA_RF_GSM_STOP_GSM_REQ,
  RFA_RF_GSM_WAKEUP_REQ,
  RFA_RF_GSM_RX_WAKEUP_REQ,
  RFA_RF_GSM_TX_WAKEUP_REQ,
  RFA_RF_GSM_SLEEP_REQ,
  RFA_RF_GSM_RX_SLEEP_REQ,
  RFA_RF_GSM_TX_SLEEP_REQ,
  RFA_RF_GSM_RX_BURST_REQ,
  RFA_RF_GSM_TX_ENABLE_REQ,
  RFA_RF_GSM_TX_DISABLE_REQ,
  RFA_RF_GSM_SET_TX_BAND_REQ,
  RFA_RF_GSM_TX_BURST_REQ,
  RFA_RF_GSM_IDLE_FRAME_PROCESSING_REQ,
  RFA_RF_GSM_SET_VBATT_LIMIT_IND,
  RFA_RF_GSM_CM_ENTER_REQ,
  RFA_RF_GSM_CM_EXIT_REQ,
  RFA_RF_GSM_SET_SAR_LIMIT_IND,
  RFA_RF_GSM_UPDATE_TEMP_COMP_REQ,
#ifdef FEATURE_RF_ASDIV
  RFA_RF_GSM_SET_ANTENNA_REQ, 
#endif
  RFA_RF_GSM_START_IP2_CAL_REQ,
  RFA_RF_GSM_INIT_MSM_IP2_CAL_REQ,
  RFA_RF_GSM_PROCESS_BURST_METRICS_REQ,
  RFA_RF_GSM_ENTER_MODE_REQ,
  RFA_RF_GSM_EXIT_MODE_REQ,
};



/*!==========================================================================*/
/*! @var rfgsm_umid_req_sub2_list                                            */
/*! @brief This array holds the request (REQ) command list used to register  */
/*!        the whole list with MSGR for subscription 2                       */
static msgr_umid_type rfgsm_umid_req_sub2_list[] = 
{
  RFA_RF_GSM_START_GSM_SUB2_REQ,
  RFA_RF_GSM_STOP_GSM_SUB2_REQ,
  RFA_RF_GSM_WAKEUP_SUB2_REQ,
  RFA_RF_GSM_RX_WAKEUP_SUB2_REQ,
  RFA_RF_GSM_TX_WAKEUP_SUB2_REQ,
  RFA_RF_GSM_SLEEP_SUB2_REQ,
  RFA_RF_GSM_RX_SLEEP_SUB2_REQ,
  RFA_RF_GSM_TX_SLEEP_SUB2_REQ,
  RFA_RF_GSM_RX_BURST_SUB2_REQ,
  RFA_RF_GSM_TX_ENABLE_SUB2_REQ,
  RFA_RF_GSM_TX_DISABLE_SUB2_REQ,
  RFA_RF_GSM_SET_TX_BAND_SUB2_REQ,
  RFA_RF_GSM_TX_BURST_SUB2_REQ,
  RFA_RF_GSM_IDLE_FRAME_PROCESSING_SUB2_REQ,
  RFA_RF_GSM_SET_VBATT_LIMIT_SUB2_IND,
  RFA_RF_GSM_CM_ENTER_SUB2_REQ,
  RFA_RF_GSM_CM_EXIT_SUB2_REQ,
  RFA_RF_GSM_SET_SAR_LIMIT_SUB2_IND,
  RFA_RF_GSM_UPDATE_TEMP_COMP_SUB2_REQ,
#ifdef FEATURE_RF_ASDIV
  RFA_RF_GSM_SET_ANTENNA_SUB2_REQ,
#endif
  RFA_RF_GSM_PROCESS_BURST_METRICS_SUB2_REQ,
  RFA_RF_GSM_ENTER_MODE_SUB2_REQ,
  RFA_RF_GSM_EXIT_MODE_SUB2_REQ,
};



static msgr_umid_type rfgsm_rflm_rsp_umids[] =
{
  RFLM_GSM_THERM_READ_RSP,    
  RFLM_GSM_ASD_RSP,
  RFLM_GSM_DEVICE_STATUS_READ_RSP,
};

static msgr_umid_type rfgsm_rflm_rsp_sub2_umids[] =
{
  RFLM_GSM_THERM_READ_SUB2_RSP,
  RFLM_GSM_ASD_SUB2_RSP,
  RFLM_GSM_DEVICE_STATUS_READ_SUB2_RSP,
};



/*===========================================================================

                               Macros

=============================================================================*/
/*! @brief Definitions of the RF GSM L1 message count for the mailboxes.
*/
#define RF_GSM_REQ_CNT(x) \
    (sizeof(x)/sizeof(msgr_umid_type))

/*! @brief Definitions of the RF GSM GFW message count for the mailboxes.
*/
#define RF_GSM_RFLM_RSP_CNT(x) \
    (sizeof(x)/sizeof(msgr_umid_type))



/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/
rfgsm_dispatch_outcome_t rfgsm_req_dispatch(rfgsm_msg_u* msg_ptr);
rfgsm_dispatch_outcome_t rfgsm_req_dispatch_2(rfgsm_msg_u* msg_ptr);

/*! @brief Function prototypes for the dispatching functionality.          */
rfgsm_dispatch_outcome_t rfgsm_dispatch_start_gsm_req( 
           rfa_rf_gsm_start_gsm_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_stop_gsm_req( 
           rfa_rf_gsm_stop_gsm_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_rx_wakeup_req( 
           rfa_rf_gsm_rx_wakeup_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_tx_wakeup_req( 
           rfa_rf_gsm_tx_wakeup_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_rx_sleep_req( 
           rfa_rf_gsm_rx_sleep_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_tx_sleep_req( 
           rfa_rf_gsm_tx_sleep_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_rx_burst_req( 
           rfa_rf_gsm_rx_burst_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_set_tx_band_req( 
           rfa_rf_gsm_set_tx_band_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_tx_burst_req( 
           rfa_rf_gsm_tx_burst_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_idle_frame_processing_req( 
           rfa_rf_gsm_idle_frame_processing_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_sar_limit_ind( 
           rfa_rf_gsm_set_sar_limit_ind_s* rfgsm_msg_ptr, rf_task_num_type rf_task );

rfgsm_dispatch_outcome_t rfgsm_dispatch_vbatt_limit_ind( 
           rfa_rf_gsm_set_vbatt_limit_ind_s* rfgsm_msg_ptr, rf_task_num_type rf_task );		//process MCS vbatt_limit setting request


rfgsm_dispatch_outcome_t rfgsm_dispatch_therm_read_rsp( 
           rfgsm_rflm_msg_u* rfgsm_msg_ptr, rf_task_num_type rf_task );

rfgsm_dispatch_outcome_t rfgsm_dispatch_device_status_read_rsp( rfgsm_rflm_msg_u* rfgsm_msg_ptr,
                                                                rf_task_num_type rf_task );

#ifdef FEATURE_RF_ASDIV
rfgsm_dispatch_outcome_t rfgsm_dispatch_asd_read_rsp( rfgsm_rflm_msg_u* rfgsm_msg_ptr,rf_task_num_type rf_task );
#endif
rfgsm_dispatch_outcome_t rfgsm_dispatch_cm_enter_req( 
           rfa_rf_gsm_cm_enter_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_cm_exit_req( 
           rfa_rf_gsm_cm_exit_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

rfgsm_dispatch_outcome_t rfgsm_dispatch_update_temp_comp_req( 
           rfa_rf_gsm_update_temp_comp_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );

boolean rfgsm_dispatch_check_device( rfcom_device_enum_type device, uint32 umid );

#ifdef FEATURE_RF_ASDIV
rfgsm_dispatch_outcome_t rfgsm_dispatch_set_antenna_req(
           rfa_rf_gsm_set_antenna_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );
#endif

rfgsm_dispatch_outcome_t rfgsm_dispatch_process_burst_metrics( 
            rfa_rf_gsm_process_burst_metrics_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf );



/*===========================================================================

                      FUNCTIONS

===========================================================================*/
/*===========================================================================
  FUNCTION:  rfgsm_send_msgr_cnf()
===========================================================================*/
/*! @fn rfgsm_send_msgr_cnf(msgr_hdr_struct_type* msg_ptr, uint32 umid, uint32 size){}
 *  @brief This function is called by any function that requires to send a 
 *  confirmation (CNF) MSG.
 *  @param msg_ptr a pointer to message router header structure type
 *  @param umid a uint32 of the Unique Message ID
 *  @param size the payload size
 *  @return errno_enum_type.
 */
errno_enum_type rfgsm_send_msgr_cnf( msgr_hdr_struct_type* msg_ptr, 
                                     uint32 umid, 
                                     uint32 size )
{
  errno_enum_type msgr_send_status = E_SUCCESS;
  
  if( msg_ptr == NULL)
  {
    MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, "MSGR Send NULL Message UMID 0x%08x",umid);
    return (E_BAD_ADDRESS);
  }

  /* If FTM enh debug dispatch mode */
  if( ftm_gsm_enh_debug_msgr_client )
  {
    /* Clear the current tech module data */
    umid = umid & 0x0000FFFF;

    /* Set FTM tech module data */
    umid = umid | (MSGR_RFA_RF_GSM_FTM<<16);
  }

  /* Send the CNF MSG back here */
  msgr_init_hdr( msg_ptr, MSGR_RFA_RF_GSM, umid );
    
  msgr_send_status = msgr_send( msg_ptr, size );
  return msgr_send_status;
}


/*===========================================================================
FUNCTION  rfgsm_send_msgr_rf_fw_intf_cmd
===========================================================================*/
errno_enum_type rfgsm_send_msgr_rf_fw_intf_cmd( msgr_hdr_struct_type* msg_ptr, 
                                                uint32 umid, 
                                                uint32 size )
{
  errno_enum_type msgr_send_status = E_SUCCESS;

  if( msg_ptr == NULL)
  {
    MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, "MSGR Send NULL Message UMID 0x%X, error_no %d",umid);
    return (E_BAD_ADDRESS);
  }

  /* initialise msgr header */
  msgr_init_hdr( msg_ptr, MSGR_GERAN_FW, umid);

  /* send msgr message to GFW */
  msgr_send_status = msgr_send( msg_ptr, size );

  return msgr_send_status;
}



/*===========================================================================
FUNCTION  rfgsm_dispatch_check_device
===========================================================================*/
/*! @fn rfgsm_dispatch_check_device
 *  @brief This funtion checks the validity of the device ID passed from L1
 *  @param device - rfm device ID
 *  @param umid - a uint32 of the Unique Message ID
 *  @return boolean - Pass or Fail indication
 */
boolean rfgsm_dispatch_check_device( rfcom_device_enum_type rfm_dev, uint32 umid )
{
  /* Check the device ID passed to RF is valid */
  if( rfm_dev >= RFM_MAX_WAN_DEVICES )
  {
    RFGC_MSG_1(MSG_LEGACY_ERROR,"Device invalid for UMID 0x%x", umid);
    return FALSE;
  }

  return TRUE;
}



/*===========================================================================

  FUNCTION:  rfgsm_error_handler

===========================================================================*/
static void rfgsm_error_handler( rfgsm_dispatch_outcome_t dispatch_result, 
                                 uint32 umid, 
                                 rf_task_num_type rf_task,
                                 rfcom_device_enum_type rfm_dev )
{
  /* Note: that rf_task is used to determine which subscription the call was */
  /*       for because it is a 1-2-1 mapping.                                */

  /* Display meaningful error depending on dispatch result enum */
  switch( dispatch_result )
  {
    case RFGSM_DISPATCH_INVALID_DEVICE:
      ERR_FATAL("GL1 passed RFGSM invalid device ID %d for UMID 0x%08x on sub %d", rfm_dev, umid, rf_task);
      break;

    case RFGSM_DISPATCH_NULL_INPUT_ARG:
      ERR_FATAL("GL1 passed RFGSM NULL input pointer for UMID 0x%08x on sub %d device ID %d", umid, rf_task, rfm_dev);
      break;
    
    case RFGSM_DISPATCH_INVALID_PARAM:
      ERR_FATAL("GL1 passed RFGSM invalid input args for UMID 0x%08x on sub %d device ID %d", umid, rf_task, rfm_dev);
      break;

    case RFGSM_DISPATCH_STATE_MACHINE_ERR:
      ERR_FATAL("GL1 called RF in an invalid sequence with UMID 0x%08x on sub %d device ID %d", umid, rf_task, rfm_dev);
      break;

    case RFGSM_DISPATCH_INVALID_RFM_MODE:
      ERR_FATAL("RFM device not GSM mode for device ID %d during UMID 0x%08x on sub %d", umid, rf_task, rfm_dev);
      break;

    default:
      MSG_4(MSG_SSID_RF, MSG_LEGACY_ERROR, 
            "GL1->RFGSM dispatch UMID 0x%08x unhandled error %d on sub %d device ID %d", 
            umid, dispatch_result, rf_task, rfm_dev);
      break;
  }
}  



/*===========================================================================
  FUNCTION:  rfgsm_cmd_proc_get_gl1_data()
===========================================================================*/
/*! @fn rfgsm_cmd_proc_get_gl1_data
 *  @brief This function gets the Access Stratum ID, current qs, current SW Fn
 *  @param msg_ptr a pointer to message router header structure type
 *  @param sub_id - subscription passed from GL1 or MCS
 *  @param as_id  - Access stratum returned from GL1
 *  @param start_time_qs - current qs returned from GL1
 *  @param frame_number - current SW frame number returned from GL1
 *  @return void
 */
static inline void rfgsm_cmd_proc_get_gl1_data( uint8 sub_id,
                                                sys_modem_as_id_e_type * as_id,
                                                uint32 * start_time_qs,
                                                uint32 * frame_number )
{
  /* Get Access Stratum from GERAN Access Stratum */
  *as_id = gl1_hw_rf_map_rf_sub_id_to_as_id( sub_id );

  /* Get the current frame-relative qs time 0-4999 */
  *start_time_qs = geran_test_mode_api_get_qsym_count( *as_id );

  /* Get the current GERAN SW frame number */
  *frame_number = geran_test_mode_api_get_FN( *as_id );
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_start_gsm_req

===========================================================================*/
/*! @fn rfgsm_dispatch_start_gsm_req( rfa_rf_gsm_start_gsm_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the initialisation of RF GSM variables for
 *         the subscription including allocating RFLM DM memory
 *  @param rfgsm_msg_ptr a pointer to the start GSM structure variable payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_start_gsm_req( rfa_rf_gsm_start_gsm_req_s* rfgsm_msg_ptr, 
                                                       rf_task_num_type rf_task,
                                                       uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_start_gsm_cnf_s    rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /*To get the HWID of the RF card*/
   rfcommon_nv_tbl_type* common_nv_tbl_ptr = NULL;
  
   /*----------------------Print out HWID of RF card-----------------*/
   common_nv_tbl_ptr = rfcommon_nv_get_tbl_ptr();
   if (common_nv_tbl_ptr != NULL) {
	  MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, 
			"HWID of the RF Card is %d", 
			common_nv_tbl_ptr->rf_hw_config);
   }
   else
   {
	  MSG(MSG_SSID_RF, MSG_LEGACY_ERROR,
		  "Common NV table pointer is NULL. Cannot get HWID of RF Card");
   }

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_start_gsm_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Start GSM API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_START_GSM_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  RFGC_T_MSG( MSG_LEGACY_HIGH, "RF GSM Cmd Proc Start GSM");

  /* Execute Start GSM functionality */
  if( rfgsm_mc_start_gsm( rfgsm_msg_ptr->sub_id, rfgsm_resp_msg.sawless_sup ) == RFGSM_MC_SUCCESS )
  {
    cmd_result = RFA_RF_GSM_SUCCESS;
  }

  /* Log the end of the Start GSM API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_START_GSM_END, rfgsm_msg_ptr->sub_id, 0, 0 );

  RFGC_T_MSG_4( MSG_LEGACY_HIGH,
                "RF GSM Cmd Proc Start GSM start time USTMR 0x%08x, start time qs=%d, duration pcycles=%d, duration us=%d",
                start_ustmr, start_time_qs, stop_pcycle-start_pcycle, duration_us );

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                      =   rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.exec_time_us             =   duration_us;
    rfgsm_resp_msg.req_result               =   cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                            umid_cnf,
                                            sizeof(rfa_rf_gsm_start_gsm_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RFGSM Start GSM MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_stop_gsm_req

===========================================================================*/
/*! @fn rfgsm_dispatch_stop_gsm_req( rfa_rf_gsm_stop_gsm_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the de-init of RF GSM variables for
 *         the subscription including deallocating RFLM DM memory
 *  @param rfgsm_msg_ptr a pointer to the start GSM structure variable payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_stop_gsm_req( rfa_rf_gsm_stop_gsm_req_s* rfgsm_msg_ptr, 
                                                      rf_task_num_type rf_task,
                                                      uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_stop_gsm_cnf_s     rfgsm_resp_msg;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  rfm_device_enum_type           rfm_dev = RFM_DEVICE_0;
  errno_enum_type               msgr_send_status = E_FAILURE;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_stop_gsm_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Stop GSM API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_STOP_GSM_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  RFGC_T_MSG( MSG_LEGACY_HIGH, "RF GSM Cmd Proc Stop GSM");

  #ifdef FEATURE_RF_ASDIV

  /*cleaning up ASDIV parameters of rfgsm core handles as part of RF GSM stop*/

  for(rfm_dev = RFM_DEVICE_0;rfm_dev < RFM_MAX_WAN_DEVICES; rfm_dev++)
     {
       rfgsm_core_handle_ptr = NULL;

       if(rfgsm_core_get_device_rx_capability(rfm_dev))
       {
         rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
       }

       if(rfgsm_core_handle_ptr != NULL)
       {
       if(rfgsm_core_handle_ptr->no_active_subs)
         {
           RFGC_T_MSG_2(MSG_LEGACY_ERROR, "asdiv configuation set to default on device  %d last requested antenna position %d",
                                           rfm_dev,
                                           rfgsm_core_handle_ptr->set_antenna_position);
  
           rfgsm_core_handle_ptr->set_antenna_position = (uint8)RF_INVALID_VALUE_8BIT;
           rfgsm_core_handle_ptr->asd_rsp = FALSE;
           rfgsm_core_handle_ptr->configure_asd_scripts = FALSE;
           rfgsm_core_handle_ptr->set_prev_ant_pos = FALSE;
         }
       }
     }

  #endif

  /* Execute Stop GSM functionality */
  if( rfgsm_mc_stop_gsm( rfgsm_msg_ptr->sub_id ) == RFGSM_MC_SUCCESS )
  {
    cmd_result = RFA_RF_GSM_SUCCESS;
  }

  /* Log the end of the Stop GSM API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_STOP_GSM_END, rfgsm_msg_ptr->sub_id, 0, 0);

  RFGC_T_MSG_4( MSG_LEGACY_HIGH,
                "RF GSM Cmd Proc Stop GSM start time USTMR 0x%08x, start time qs=%d, duration pcycles=%d, duration us=%d",
                start_ustmr, start_time_qs, stop_pcycle-start_pcycle, duration_us );

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                      =   rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.exec_time_us             =   duration_us;
    rfgsm_resp_msg.req_result               =   cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                            umid_cnf,
                                            sizeof(rfa_rf_gsm_stop_gsm_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RFGSM Stop GSM MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_rx_wakeup_req

===========================================================================*/
/*! @fn rfgsm_dispatch_rx_wakeup_req( rfa_rf_gsm_rx_wakeup_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the waking up of the RF hardware from 
 *  sleep mode, setting the hardware to the same band it was in before sleep.
 *  @param rfgsm_msg_ptr a pointer to the wakeup structure variable payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_rx_wakeup_req( rfa_rf_gsm_rx_wakeup_req_s* rfgsm_msg_ptr, 
                                                       rf_task_num_type rf_task,
                                                       uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_cmd_outcome_t      prx_device_cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_cmd_outcome_t      drx_device_cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_rx_wakeup_cnf_s    rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();
  uint32 ccs_exe_time_us = 0;

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_rx_wakeup_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Rx Wakeup API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_RX_WAKEUP_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  RFGC_T_MSG_6( MSG_LEGACY_HIGH, 
                "RF GSM Cmd Proc Rx Wakeup, PRx dev=%d, DRx dev=%d, PRx RxLM=%d, DRx RxLM=%d, op_mode=%d, cnf=%d",
                 rfgsm_msg_ptr->prx_device,
                 rfgsm_msg_ptr->drx_device,
                 rfgsm_msg_ptr->rxlm_buf_index.prx_idx,
                 rfgsm_msg_ptr->rxlm_buf_index.drx_idx,
                 rfgsm_msg_ptr->op_mode,
                 rfgsm_msg_ptr->cnf_required );

  /* Execute Rx Wakeup functionality */
  if( rfgsm_mc_rx_wakeup( rfgsm_msg_ptr->sub_id,
                          rfgsm_msg_ptr->prx_device,
                          rfgsm_msg_ptr->drx_device,
                          rfgsm_msg_ptr->op_mode,
                          &rfgsm_msg_ptr->rxlm_buf_index,
                          rfgsm_msg_ptr->gfw_buff_ptr,
                          rfgsm_msg_ptr->timing_info ) == RFGSM_MC_SUCCESS )
  {
    cmd_result = RFA_RF_GSM_SUCCESS;
  }

  #ifdef FEATURE_RF_POWER_REPORTING
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  RF_MSG_4(RF_MED, "rfgsm_cmd_processing.c prx device %d drx device %d , prx handle %d, drx handle %d",
           rfgsm_msg_ptr->prx_device,
           rfgsm_msg_ptr->drx_device,
           rfgsm_msg_ptr->rxlm_buf_index.prx_idx,
           rfgsm_msg_ptr->rxlm_buf_index.drx_idx);
  #endif
  if( rfgsm_msg_ptr->op_mode == RFGSM_PRX_OP_MODE ||
    rfgsm_msg_ptr->op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    rflm_dm_handle_meta_s handle_meta;
    rflm_dm_get_handle_info(  rfgsm_msg_ptr->rxlm_buf_index.prx_idx, &handle_meta);
    handle_meta.devide_id = rfgsm_msg_ptr->prx_device;
    handle_meta.device_id = rfgsm_msg_ptr->prx_device;
    rflm_dm_set_handle_info(  rfgsm_msg_ptr->rxlm_buf_index.prx_idx, &handle_meta );

    rflm_diag_power_log_trx_dynamic_event(RFLM_DIAG_POWER_RFA_RX_HANDLE, rfgsm_msg_ptr->rxlm_buf_index.prx_idx, RFLM_DIAG_POWER_RFA_STATE_ON);
  }

  if ( rfgsm_msg_ptr->op_mode == RFGSM_DRX_OP_MODE ||
      rfgsm_msg_ptr->op_mode == RFGSM_PRX_DRX_OP_MODE ) 
  {

    rflm_dm_handle_meta_s handle_meta;
    rflm_dm_get_handle_info(  rfgsm_msg_ptr->rxlm_buf_index.drx_idx, &handle_meta);
    handle_meta.devide_id = rfgsm_msg_ptr->drx_device;
    handle_meta.device_id = rfgsm_msg_ptr->drx_device;
    rflm_dm_set_handle_info(  rfgsm_msg_ptr->rxlm_buf_index.drx_idx, &handle_meta );

    rflm_diag_power_log_trx_dynamic_event(RFLM_DIAG_POWER_RFA_RX_HANDLE, rfgsm_msg_ptr->rxlm_buf_index.drx_idx, RFLM_DIAG_POWER_RFA_STATE_ON);
  }
  #endif
  
  switch( rfgsm_msg_ptr->op_mode )
  {
  case RFGSM_PRX_DRX_OP_MODE:                     
    ccs_exe_time_us = rfgsm_mc_get_wakeup_ccs_exe_us(rfgsm_msg_ptr->prx_device)+ rfgsm_mc_get_wakeup_ccs_exe_us(rfgsm_msg_ptr->drx_device);
    break;
  case RFGSM_DRX_OP_MODE:
    ccs_exe_time_us = rfgsm_mc_get_wakeup_ccs_exe_us(rfgsm_msg_ptr->drx_device);
    break;
  case RFGSM_PRX_OP_MODE:
  default:
    ccs_exe_time_us = rfgsm_mc_get_wakeup_ccs_exe_us(rfgsm_msg_ptr->prx_device);
    break;
  }

  /* Log the end of the Rx Wakeup API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_RX_WAKEUP_END, 
                              rfgsm_msg_ptr->sub_id, 
                              (rfgsm_msg_ptr->prx_device << 24) | 
                              (rfgsm_msg_ptr->rxlm_buf_index.prx_idx << 16) | 
                              (rfgsm_msg_ptr->drx_device << 8) | 
                              rfgsm_msg_ptr->rxlm_buf_index.drx_idx, 
                              0);

  RFGC_T_MSG_4( MSG_LEGACY_HIGH,
                "RF GSM Cmd Proc Rx Wakeup start time USTMR 0x%08x, start time qs=%d, duration pcycles=%d, duration us=%d",
                start_ustmr, start_time_qs, stop_pcycle-start_pcycle, duration_us );

  /*HISTOGRAM logging*/
  
   /*updating the histogram for rx */
   prof_hist_update(&rx_gsm_wakeup_build_scripts_hist, duration_us);
				

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                      =   rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.prx_device               =   rfgsm_msg_ptr->prx_device;
    rfgsm_resp_msg.drx_device               =   rfgsm_msg_ptr->drx_device;
    rfgsm_resp_msg.op_mode                  =   rfgsm_msg_ptr->op_mode;
    rfgsm_resp_msg.gfw_buff_ptr             =   rfgsm_msg_ptr->gfw_buff_ptr;
    rfgsm_resp_msg.timing_info              =   rfgsm_msg_ptr->timing_info;
    rfgsm_resp_msg.rxlm_buf_index.prx_idx   =   rfgsm_msg_ptr->rxlm_buf_index.prx_idx;
    rfgsm_resp_msg.rxlm_buf_index.drx_idx   =   rfgsm_msg_ptr->rxlm_buf_index.drx_idx;
    rfgsm_resp_msg.exec_time_us             =   duration_us;
    rfgsm_resp_msg.prx_device_req_result    =   prx_device_cmd_result;
    rfgsm_resp_msg.drx_device_req_result    =   drx_device_cmd_result;
    rfgsm_resp_msg.req_result               =   cmd_result;
    rfgsm_resp_msg.ccs_time_us              =   ccs_exe_time_us;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                            umid_cnf,
                                            sizeof(rfa_rf_gsm_rx_wakeup_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RFGSM Rx Wakeup MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_tx_wakeup_req

===========================================================================*/
/*! @fn rfgsm_dispatch_tx_wakeup_req( rfa_rf_gsm_tx_wakeup_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the waking up of the RF hardware from 
 *  sleep mode, setting the hardware to the same band it was in before sleep.
 *  @param rfgsm_msg_ptr a pointer to the wakeup structure variable payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_tx_wakeup_req( rfa_rf_gsm_tx_wakeup_req_s* rfgsm_msg_ptr, 
                                                       rf_task_num_type rf_task,
                                                       uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_tx_wakeup_cnf_s    rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();
  uint32 ccs_exe_time_us = 0;

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_tx_wakeup_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Tx Wakeup API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_TX_WAKEUP_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  RFGC_T_MSG_3( MSG_LEGACY_HIGH, 
                "RF GSM Cmd Proc Tx Wakeup, Tx dev=%d, TxLM=%d, cnf=%d",
                 rfgsm_msg_ptr->tx_device,
                 rfgsm_msg_ptr->txlm_buf_index,
                 rfgsm_msg_ptr->cnf_required );

  /* Execute Tx Wakeup functionality */
  if( rfgsm_mc_tx_wakeup( rfgsm_msg_ptr->sub_id,
                          rfgsm_msg_ptr->tx_device,
                          rfgsm_msg_ptr->txlm_buf_index,
                          rfgsm_msg_ptr->gfw_buff_ptr,
                          rfgsm_msg_ptr->therm_read_buff_ptr,
                          rfgsm_msg_ptr->timing_info ) == RFGSM_MC_SUCCESS )
  {
    cmd_result = RFA_RF_GSM_SUCCESS;
  }
  
  ccs_exe_time_us = rfgsm_mc_get_wakeup_ccs_exe_us(rfgsm_msg_ptr->tx_device);
 
  /* Log the end of the Tx Wakeup API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_TX_WAKEUP_END, 
                              rfgsm_msg_ptr->sub_id, 
                              (rfgsm_msg_ptr->tx_device << 8) | rfgsm_msg_ptr->txlm_buf_index, 
                              0);




              /*HISTOGRAM logging*/

  
  /*updating the histogram for tx */
  prof_hist_update(&tx_gsm_wakeup_build_scripts_hist, duration_us);

  RFGC_T_MSG_4( MSG_LEGACY_HIGH,
                "RF GSM Cmd Proc Tx Wakeup start time USTMR 0x%08x, start time qs=%d, duration pcycles=%d, duration us=%d",
                start_ustmr, start_time_qs, stop_pcycle-start_pcycle, duration_us );

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                      =   rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.tx_device                =   rfgsm_msg_ptr->tx_device;
    rfgsm_resp_msg.txlm_buf_index           =   rfgsm_msg_ptr->txlm_buf_index;
    rfgsm_resp_msg.gfw_buff_ptr             =   rfgsm_msg_ptr->gfw_buff_ptr;
    rfgsm_resp_msg.timing_info              =   rfgsm_msg_ptr->timing_info;
    rfgsm_resp_msg.exec_time_us             =   duration_us;
    rfgsm_resp_msg.req_result               =   cmd_result;
    rfgsm_resp_msg.ccs_time_us              =   ccs_exe_time_us;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                            umid_cnf,
                                            sizeof(rfa_rf_gsm_tx_wakeup_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RFGSM Tx Wakeup MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }
  
  #ifdef FEATURE_RF_POWER_REPORTING
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  RFGC_T_MSG_1( MSG_LEGACY_HIGH, "rfgsm_dispatch_tx_wakeup_req handle = %d device = %d", rfgsm_msg_ptr->txlm_buf_index, rfgsm_msg_ptr->tx_device );
  #endif
  /* associate handle with device */
  rflm_dm_handle_meta_s handle_meta;
  rflm_dm_get_handle_info(  rfgsm_msg_ptr->txlm_buf_index, &handle_meta);
  handle_meta.devide_id = rfgsm_msg_ptr->tx_device;
  handle_meta.device_id = rfgsm_msg_ptr->tx_device;
  rflm_dm_set_handle_info( rfgsm_msg_ptr->txlm_buf_index , &handle_meta );

  rflm_diag_power_log_trx_dynamic_event(RFLM_DIAG_POWER_RFA_TX_HANDLE, rfgsm_msg_ptr->txlm_buf_index, RFLM_DIAG_POWER_RFA_STATE_ENABLE);
  #endif 
  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_rx_sleep_req

===========================================================================*/
/*! @fn rfgsm_dispatch_rx_sleep_req( rfa_rf_gsm_rx_sleep_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the placing of the RF hardware to sleep
 *  for GSM mode. 
 *  @param rfgsm_msg_ptr a pointer to the sleep structure variable payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_rx_sleep_req( rfa_rf_gsm_rx_sleep_req_s* rfgsm_msg_ptr, 
                                                       rf_task_num_type rf_task,
                                                       uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_cmd_outcome_t      prx_device_cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_cmd_outcome_t      drx_device_cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_rx_sleep_cnf_s     rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_rx_sleep_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Rx Sleep API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_RX_SLEEP_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  RFGC_T_MSG_4( MSG_LEGACY_HIGH, 
                "RF GSM Cmd Proc Rx Sleep, PRx dev=%d, DRx dev=%d, op_mode=%d, cnf=%d",
                 rfgsm_msg_ptr->prx_device,
                 rfgsm_msg_ptr->drx_device,
                 rfgsm_msg_ptr->op_mode,
                 rfgsm_msg_ptr->cnf_required );

  /* Execute Rx Sleep functionality */
  if( rfgsm_mc_rx_sleep( rfgsm_msg_ptr->sub_id,
                         rfgsm_msg_ptr->prx_device,
                         rfgsm_msg_ptr->drx_device,
                         rfgsm_msg_ptr->op_mode ) == RFGSM_MC_SUCCESS )
  {
    cmd_result = RFA_RF_GSM_SUCCESS;
  }

  /* Log the end of the Rx Sleep API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_RX_SLEEP_END, 
                              rfgsm_msg_ptr->sub_id, 
                              (rfgsm_msg_ptr->prx_device << 16) | 
                              (rfgsm_msg_ptr->drx_device << 8) | 
                              rfgsm_msg_ptr->op_mode, 
                              0);

  RFGC_T_MSG_4( MSG_LEGACY_HIGH,
                "RF GSM Cmd Proc Rx Sleep start time USTMR 0x%08x, start time qs=%d, duration pcycles=%d, duration us=%d",
                start_ustmr, start_time_qs, stop_pcycle-start_pcycle, duration_us );

#ifdef FEATURE_RF_ASDIV

  /*cleaning up ASDIV parameters of rfgsm core handles of PRx device as part of PRx sleep*/
    rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfgsm_msg_ptr->prx_device);
        
    if(rfgsm_core_handle_ptr != NULL)
    {    
      if(rfgsm_core_handle_ptr->no_active_subs)
      {    
        rfgsm_core_handle_ptr->set_antenna_position = (uint8)RF_INVALID_VALUE_8BIT;
        rfgsm_core_handle_ptr->asd_rsp = FALSE;
        rfgsm_core_handle_ptr->configure_asd_scripts = FALSE;
        rfgsm_core_handle_ptr->set_prev_ant_pos = FALSE;
      }
    }

#endif


 
      /*HISTOGRAM logging*/

  /*updating the histogram for rx */
  prof_hist_update(&rx_gsm_sleep_build_scripts_hist, duration_us);



  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                      =   rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.prx_device               =   rfgsm_msg_ptr->prx_device;
    rfgsm_resp_msg.drx_device               =   rfgsm_msg_ptr->drx_device;
    rfgsm_resp_msg.op_mode                  =   rfgsm_msg_ptr->op_mode;
    rfgsm_resp_msg.exec_time_us             =   duration_us;
    rfgsm_resp_msg.prx_device_req_result    =   prx_device_cmd_result;
    rfgsm_resp_msg.drx_device_req_result    =   drx_device_cmd_result;
    rfgsm_resp_msg.req_result               =   cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                            umid_cnf,
                                            sizeof(rfa_rf_gsm_rx_sleep_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RFGSM Rx Sleep MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_tx_sleep_req

===========================================================================*/
/*! @fn rfgsm_dispatch_tx_sleep_req( rfa_rf_gsm_tx_sleep_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the placing of the RF hardware to sleep
 *  for GSM mode. 
 *  @param rfgsm_msg_ptr a pointer to the sleep structure variable payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_tx_sleep_req( rfa_rf_gsm_tx_sleep_req_s* rfgsm_msg_ptr, 
                                                      rf_task_num_type rf_task,
                                                      uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_tx_sleep_cnf_s     rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_tx_sleep_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Tx Sleep API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_TX_SLEEP_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  RFGC_T_MSG_2( MSG_LEGACY_HIGH, 
                "RF GSM Cmd Proc Tx Sleep, Tx dev=%d, cnf=%d",
                 rfgsm_msg_ptr->tx_device,
                 rfgsm_msg_ptr->cnf_required );

  /* Execute Tx Sleep functionality */
  if( rfgsm_mc_tx_sleep( rfgsm_msg_ptr->sub_id,
                         rfgsm_msg_ptr->tx_device ) == RFGSM_MC_SUCCESS )
  {
    cmd_result = RFA_RF_GSM_SUCCESS;
  }

  /* Log the end of the Tx Sleep API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_TX_SLEEP_END, 
                              rfgsm_msg_ptr->sub_id, 
                              rfgsm_msg_ptr->tx_device, 
                              0);


   /*HISTOGRAM logging*/
  
  /*updating the histogram for tx */
  prof_hist_update(&tx_gsm_sleep_build_scripts_hist, duration_us);
  

  RFGC_T_MSG_4( MSG_LEGACY_HIGH,
                "RF GSM Cmd Proc Tx Sleep start time USTMR 0x%08x, start time qs=%d, duration pcycles=%d, duration us=%d",
                start_ustmr, start_time_qs, stop_pcycle-start_pcycle, duration_us );

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                      =   rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.tx_device                =   rfgsm_msg_ptr->tx_device;
    rfgsm_resp_msg.exec_time_us             =   duration_us;
    rfgsm_resp_msg.req_result               =   cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                            umid_cnf,
                                            sizeof(rfa_rf_gsm_tx_sleep_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RFGSM Tx Sleep MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }
  
  #ifdef FEATURE_RF_POWER_REPORTING
  rfgsm_core_handle_type* core_data = rfgsm_core_handle_get( rfgsm_msg_ptr->tx_device );
  rflm_diag_power_log_trx_dynamic_event(RFLM_DIAG_POWER_RFA_TX_HANDLE, core_data->txlm_buf_index, RFLM_DIAG_POWER_RFA_STATE_DISABLE);
  #endif 
  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_rx_burst_req

===========================================================================*/
/*! @fn rfgsm_dispatch_rx_burst_req( rfa_rf_gsm_rx_burst_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the Rx burst request, handling all
 *  the GSM Rx burst information, based on type of Rx burst, for a single
 *  frame.
 *  @param rfgsm_msg_ptr a pointer to the rx_burst structure variable payload
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_rx_burst_req( rfa_rf_gsm_rx_burst_req_s* rfgsm_msg_ptr, 
                                                      rf_task_num_type rf_task,
                                                      uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_rx_burst_cnf_s     rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;
  rfgsm_mc_status_type          result = RFGSM_MC_ERROR;
  rfgsm_mc_rx_data_type         rx_data;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_rx_burst_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr->rx_burst_params, "rx_burst_params from GL1 is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Rx burst API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_RX_BEGIN, rfgsm_msg_ptr->sub_id, 
                     (rfgsm_msg_ptr->device << 12) | 
                     (rfgsm_msg_ptr->rx_diversity_device << 8) | 
                     (rfgsm_msg_ptr->enable_rx_diversity << 4) | 
                     rfgsm_msg_ptr->rx_burst_params->num_rx_bursts, 
                     rfgsm_msg_ptr->rx_burst_params->rx_chan_params[0].rf_sequence_number);

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  if (RF_MON_BURST == rfgsm_msg_ptr->rx_burst_params->rx_chan_params[0].burst_type)
    {
     RFGC_T_MSG_6( MSG_LEGACY_HIGH, 
                "RF GSM Cmd Proc Rx Mon Pwr prx_dev=%d, drx_dev=%d, RxD enabled=%d, num bursts=%d, GL1 lower ccs prio=%d, Slot0 RF seq num=%d",
                rfgsm_msg_ptr->device,
                rfgsm_msg_ptr->rx_diversity_device,
                rfgsm_msg_ptr->enable_rx_diversity,
                rfgsm_msg_ptr->rx_burst_params->num_rx_bursts,
                rfgsm_msg_ptr->rx_burst_params->gl1_lower_rx_ccs_prio,
                rfgsm_msg_ptr->rx_burst_params->rx_chan_params[0].rf_sequence_number );
     }
   else     
     {
      RFGC_T_MSG_6( MSG_LEGACY_HIGH, 
                "RF GSM Cmd Proc Rx Burst prx_dev=%d, drx_dev=%d, RxD enabled=%d, num bursts=%d, GL1 lower ccs prio=%d, Slot0 RF seq num=%d",
                rfgsm_msg_ptr->device,
                rfgsm_msg_ptr->rx_diversity_device,
                rfgsm_msg_ptr->enable_rx_diversity,
                rfgsm_msg_ptr->rx_burst_params->num_rx_bursts,
                rfgsm_msg_ptr->rx_burst_params->gl1_lower_rx_ccs_prio,
                rfgsm_msg_ptr->rx_burst_params->rx_chan_params[0].rf_sequence_number );
     }
 
  /* Assign all Rx data and execute Rx burst processing */
  rx_data.sub_id          = rfgsm_msg_ptr->sub_id;
  rx_data.prx_device      = rfgsm_msg_ptr->device;
  rx_data.drx_device      = rfgsm_msg_ptr->rx_diversity_device;
  rx_data.op_mode         = (rfgsm_msg_ptr->enable_rx_diversity == FALSE) ? RFGSM_PRX_OP_MODE : RFGSM_PRX_DRX_OP_MODE;
  rx_data.buffer_id       = rfgsm_msg_ptr->buffer_id;
  rx_data.rx_burst_params = rfgsm_msg_ptr->rx_burst_params;

  result = rfgsm_mc_rx( &rx_data );

  if( result == RFGSM_MC_SUCCESS )
  {
    cmd_result = RFA_RF_GSM_SUCCESS;
  }
  else
  {
    RFGC_T_MSG_1(MSG_LEGACY_ERROR, "PRx prepare bursts failed with result %d", result);
  }

  /* Log the end of the Rx burst API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_RX_END, rfgsm_msg_ptr->sub_id, 
                              (rfgsm_msg_ptr->device << 24) | 
                              (rfgsm_msg_ptr->rx_diversity_device << 16) | 
                              (rfgsm_msg_ptr->enable_rx_diversity << 8) | 
                     rfgsm_msg_ptr->rx_burst_params->num_rx_bursts, 
                              rfgsm_msg_ptr->rx_burst_params->rx_chan_params[0].rf_sequence_number);


 
  /*HISTOGRAM logging*/
  
  
   if (RFGSM_RX_MON_BURST == rfgsm_msg_ptr->rx_burst_params->rx_chan_params[0].rx_burst_type)
   {
	 /*updating the histogram for rx */
	 prof_hist_update(&rx_gsm_mon_build_scripts_hist, duration_us);
  
	 if (prof_hist_log_periodic(&gsm_rf_hist_packet_handle))
	 {
	   prof_hist_clear(&rx_gsm_prog_ccs_scripts_hist);
	   prof_hist_clear(&tx_gsm_prog_ccs_scripts_hist);
	   prof_hist_clear(&rx_gsm_wakeup_build_scripts_hist);
	   prof_hist_clear(&rx_gsm_sleep_build_scripts_hist);
	   prof_hist_clear(&tx_gsm_wakeup_build_scripts_hist);
	   prof_hist_clear(&tx_gsm_sleep_build_scripts_hist);
	   prof_hist_clear(&tx_gsm_burst_build_scripts_hist);
	   prof_hist_clear(&rx_gsm_prx_burst_build_scripts_hist);
	   prof_hist_clear(&rx_gsm_prx_drx_burst_build_scripts_hist);
	   prof_hist_clear(&rx_gsm_mon_build_scripts_hist);
	 }
   }
   else
   {
  
	 if(FALSE == rfgsm_msg_ptr->enable_rx_diversity)
	 {
	   /*updating the histogram for prx only*/
	   prof_hist_update(&rx_gsm_prx_burst_build_scripts_hist, duration_us);
  
	   if (prof_hist_log_periodic(&gsm_rf_hist_packet_handle))
	   {
		 prof_hist_clear(&rx_gsm_prog_ccs_scripts_hist);
		 prof_hist_clear(&tx_gsm_prog_ccs_scripts_hist);
		 prof_hist_clear(&rx_gsm_wakeup_build_scripts_hist);
		 prof_hist_clear(&rx_gsm_sleep_build_scripts_hist);
		 prof_hist_clear(&tx_gsm_wakeup_build_scripts_hist);
		 prof_hist_clear(&tx_gsm_sleep_build_scripts_hist);
		 prof_hist_clear(&tx_gsm_burst_build_scripts_hist);
		 prof_hist_clear(&rx_gsm_prx_burst_build_scripts_hist);
		 prof_hist_clear(&rx_gsm_prx_drx_burst_build_scripts_hist);
		 prof_hist_clear(&rx_gsm_mon_build_scripts_hist);
	   }
	 }
	 else
	 {
	   /*updating the histogram for prx+drx */
	   prof_hist_update(&rx_gsm_prx_drx_burst_build_scripts_hist, duration_us);
  
	   if (prof_hist_log_periodic(&gsm_rf_hist_packet_handle))
	   {
		 prof_hist_clear(&rx_gsm_prog_ccs_scripts_hist);
		 prof_hist_clear(&tx_gsm_prog_ccs_scripts_hist);
		 prof_hist_clear(&rx_gsm_wakeup_build_scripts_hist);
		 prof_hist_clear(&rx_gsm_sleep_build_scripts_hist);
		 prof_hist_clear(&tx_gsm_wakeup_build_scripts_hist);
		 prof_hist_clear(&tx_gsm_sleep_build_scripts_hist);
		 prof_hist_clear(&tx_gsm_burst_build_scripts_hist);
		 prof_hist_clear(&rx_gsm_prx_burst_build_scripts_hist);
		 prof_hist_clear(&rx_gsm_prx_drx_burst_build_scripts_hist);
		 prof_hist_clear(&rx_gsm_mon_build_scripts_hist);
	   }
	 }
   }


  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                 =   rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.device              =   rfgsm_msg_ptr->device;
    rfgsm_resp_msg.rx_diversity_device = rfgsm_msg_ptr->rx_diversity_device;
    rfgsm_resp_msg.enable_rx_diversity = rfgsm_msg_ptr->enable_rx_diversity;
    rfgsm_resp_msg.cnf_required        =   rfgsm_msg_ptr->cnf_required;
    rfgsm_resp_msg.rx_burst_params     =   rfgsm_msg_ptr->rx_burst_params;
    rfgsm_resp_msg.exec_time_us        =   duration_us;
    rfgsm_resp_msg.req_result          =   cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                           umid_cnf,
                                           sizeof(rfa_rf_gsm_rx_burst_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RFGSM Rx Burst MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_set_tx_band_req

===========================================================================*/
/*! @fn rfgsm_dispatch_set_tx_band_req( rfa_rf_gsm_set_tx_band_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the initialisation of the hardware for 
 *  the specified Tx band. 
 *  @param rfgsm_msg_ptr a pointer to the set_tx_band structure variable 
 *  payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_set_tx_band_req( rfa_rf_gsm_set_tx_band_req_s* rfgsm_msg_ptr, 
                                                         rf_task_num_type rf_task,
                                                         uint32 umid_cnf)
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_set_tx_band_cnf_s  rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_data_type          rfgsm_core_data;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfgsm_dispatch_set_tx_band_req is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Tx Band API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_TX_BAND_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  if( !rfgsm_dispatch_check_device( rfgsm_msg_ptr->device, rfgsm_msg_ptr->hdr.id ) || !rfgsm_core_get_device_tx_capability(rfgsm_msg_ptr->device) )
  {
    dispatch_result = RFGSM_DISPATCH_INVALID_DEVICE;
  }
  else
  {
    rfgsm_core_handle_ptr = rfgsm_core_handle_get( rfgsm_msg_ptr->device );

    if( rfgsm_core_handle_ptr == NULL )
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR,
            "rfgsm_core_handle_ptr is NULL for device %d!", rfgsm_msg_ptr->device);
      dispatch_result = RFGSM_DISPATCH_NULL_PTR;
    }
  }

  if( dispatch_result == RFGSM_DISPATCH_SUCCESSFUL )
  {
    if( rfgsm_msg_ptr->sbi_set_tx_band_buff_ptr == NULL )
    {
      RFGC_T_MSG(MSG_LEGACY_ERROR, 
          "RF GSM Enter Mode dispatcher failed L1 payload variable verification");
      dispatch_result = RFGSM_DISPATCH_NULL_PTR;
    }
  }

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* Verify payload parameters. */
  if( dispatch_result == RFGSM_DISPATCH_SUCCESSFUL )
  {
    RFGC_T_MSG_4( MSG_LEGACY_HIGH,
                  "RF GSM Cmd Proc Tx Band, dev_id=%d, tx band=%d, shared_mem_ptr=0x%x, cnf=%d",
                  rfgsm_msg_ptr->device, 
                  rfgsm_msg_ptr->band, 
                  rfgsm_msg_ptr->sbi_set_tx_band_buff_ptr,
                  rfgsm_msg_ptr->cnf_required );

    /* Set the core data */
    rfgsm_core_data.rf_task = rf_task;
    rfgsm_core_data.rfm_dev = rfgsm_msg_ptr->device;
    rfgsm_core_data.sub_id = rfgsm_msg_ptr->sub_id;
    rfgsm_core_data.as_id = gl1_hw_rf_map_rf_sub_id_to_as_id(rfgsm_msg_ptr->sub_id);
    rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
    rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

    if( rfgsm_mc_set_tx_band(&rfgsm_core_data, 
                             rfgsm_msg_ptr->band, 
                             rfgsm_msg_ptr->sbi_set_tx_band_buff_ptr) == RFGSM_MC_SUCCESS )
    {
      cmd_result = RFA_RF_GSM_SUCCESS;
    }
  }
  else /* dispatch_result != RFGSM_DISPATCH_SUCCESSFUL */
  {    
    RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RF GSM Set Tx band dispatcher failed variable verification for device %d", rfgsm_msg_ptr->device);

    /* Handle dispatch errors. */
    rfgsm_error_handler( dispatch_result, rfgsm_msg_ptr->hdr.id, rf_task, rfgsm_msg_ptr->device );
  }

  /* Log the end of the Tx Band API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_TX_BAND_END, 
                              rfgsm_msg_ptr->sub_id, 
                              (rfgsm_msg_ptr->device << 8) | 
                              rfgsm_msg_ptr->band, 
                              0);

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                        =   rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.device                     =   rfgsm_msg_ptr->device;
    rfgsm_resp_msg.cnf_required               =   rfgsm_msg_ptr->cnf_required;
    rfgsm_resp_msg.band                       =   rfgsm_msg_ptr->band;
    rfgsm_resp_msg.sbi_set_tx_band_buff_ptr   =   rfgsm_msg_ptr->sbi_set_tx_band_buff_ptr;
    rfgsm_resp_msg.exec_time_us               =   duration_us;
    rfgsm_resp_msg.req_result                 =   cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                            umid_cnf,
                                            sizeof(rfa_rf_gsm_set_tx_band_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RFGSM Set Tx Band MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_tx_burst_req

===========================================================================*/
/*! @fn rfgsm_dispatch_tx_burst_req( rfa_rf_gsm_tx_burst_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the Tx burst request, handling all
 *  the GSM Tx burst information for a single frame.
 *  @param rfgsm_msg_ptr a pointer to the tx_burst structure variable payload
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_tx_burst_req( rfa_rf_gsm_tx_burst_req_s* rfgsm_msg_ptr, 
                                                      rf_task_num_type rf_task,
                                                      uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_tx_burst_cnf_s     rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_data_type          rfgsm_core_data;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */  
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_tx_burst_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr->tx_burst_params, "tx_burst_params from GL1 is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Tx Burst API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_TX_BEGIN, 
                               rfgsm_msg_ptr->sub_id, 
                     rfgsm_msg_ptr->tx_burst_params->num_tx_slots, 
                               rfgsm_msg_ptr->tx_burst_params->tx_chan_params.rf_sequence_number);

  if( !rfgsm_dispatch_check_device( rfgsm_msg_ptr->device, rfgsm_msg_ptr->hdr.id ) || !rfgsm_core_get_device_tx_capability(rfgsm_msg_ptr->device) )
  {
    dispatch_result = RFGSM_DISPATCH_INVALID_DEVICE;
  }
  else
  {
    rfgsm_core_handle_ptr = rfgsm_core_handle_get( rfgsm_msg_ptr->device );

    if( rfgsm_core_handle_ptr == NULL )
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR,
            "rfgsm_core_handle_ptr is NULL for device %d!", rfgsm_msg_ptr->device);
      dispatch_result = RFGSM_DISPATCH_NULL_PTR;
    }
  }

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* Verify payload parameters. */
  if( dispatch_result == RFGSM_DISPATCH_SUCCESSFUL )
  {
    RFGC_T_MSG_7( MSG_LEGACY_HIGH, 
                  "RF GSM Cmd Proc Tx Burst, dev_id=%d, num slots=%d, band=%d, channel=%d, freq err=%d, shared_mem_ptr=0x%x, RF seq num=%d",
                  rfgsm_msg_ptr->device, 
                  rfgsm_msg_ptr->tx_burst_params->num_tx_slots,
                  rfgsm_msg_ptr->tx_burst_params->tx_chan_params.band, 
                  rfgsm_msg_ptr->tx_burst_params->tx_chan_params.arfcn,
                  rfgsm_msg_ptr->tx_burst_params->tx_chan_params.freq_err,
                  rfgsm_msg_ptr->tx_burst_params->tx_burst_event_ptr,
                  rfgsm_msg_ptr->tx_burst_params->tx_chan_params.rf_sequence_number );

    /* Set the core data */
    rfgsm_core_data.rf_task = rf_task;
    rfgsm_core_data.rfm_dev = rfgsm_msg_ptr->device;
    rfgsm_core_data.sub_id = rfgsm_msg_ptr->sub_id;
    rfgsm_core_data.as_id = gl1_hw_rf_map_rf_sub_id_to_as_id(rfgsm_msg_ptr->sub_id);
    rfgsm_core_data.triple_buffer_id = rfgsm_msg_ptr->buffer_id;
    rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

    // this MTPL direct access to the rfgsm_core_handler has been prefered for efficiency
    if(rfgsm_msg_ptr->cnf_required)
    {
      rfgsm_core_handle_ptr->mtpl_update_req = TRUE;
    }
    
    if( rfgsm_mc_process_tx_burst( &rfgsm_core_data,
                                   rfgsm_msg_ptr->tx_burst_params ) == RFGSM_MC_SUCCESS )
    {
      cmd_result = RFA_RF_GSM_SUCCESS;
    }
  }
  else /* dispatch_result != RFGSM_DISPATCH_SUCCESSFUL */
  {
    RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RF GSM Tx Burst dispatcher failed variable verification for device %d", rfgsm_msg_ptr->device);

    /* Handle dispatch errors. */   
    rfgsm_error_handler( dispatch_result, rfgsm_msg_ptr->hdr.id, rf_task, rfgsm_msg_ptr->device );
  }

  /* Log the end of the Tx Burst API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_TX_END, 
                              rfgsm_msg_ptr->sub_id,
                              (rfgsm_msg_ptr->tx_burst_params->tx_chan_params.band << 16) | 
                              (rfgsm_msg_ptr->tx_burst_params->num_tx_slots << 8) | 
                              rfgsm_msg_ptr->device, 
                              rfgsm_msg_ptr->tx_burst_params->tx_chan_params.rf_sequence_number);

	 /*HISTOGRAM logging*/


  /*updating the histogram for tx */
  prof_hist_update(&tx_gsm_burst_build_scripts_hist, duration_us);
							  

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                 =   rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.device              =   rfgsm_msg_ptr->device;
    rfgsm_resp_msg.cnf_required        =   rfgsm_msg_ptr->cnf_required;
    rfgsm_resp_msg.tx_burst_params     =   rfgsm_msg_ptr->tx_burst_params;
    rfgsm_resp_msg.vswr_meas_requested =   rfgsm_core_dm_vswr_get_meas_requested(rfgsm_msg_ptr->sub_id);
    rfgsm_resp_msg.exec_time_us        =   duration_us;
    rfgsm_resp_msg.req_result          =   cmd_result;

    if( !rfgsm_mc_update_tx_power_in_response_message( &rfgsm_core_data, rfgsm_resp_msg.tx_pwr_in_dBm100 ) )
    {
      RFGC_T_MSG( MSG_LEGACY_ERROR, 
                  "rfgsm_mc_update_tx_power_in_response_message failed");
    }

    // Since the core-tx-agc-ctl might not have been triggered
    // (ie. GL1 requests a burst CNF after core-tx-agc-ctl has been updated...),
    // then the core_handler needs to maintain its own MTPL table.
    // then also this CNF MTPL shall only contain valid data, as agreed with GL1
    // (a value of -1 still means an antenna MTPL measure not present).
    // therefore this memory block copy is thought to be an efficient mean for this asynchronous data transfer
    memscpy(rfgsm_resp_msg.mtpl_dBm100,
            sizeof(rfgsm_resp_msg.mtpl_dBm100),
            rfgsm_core_handle_ptr->mtpl,
            sizeof(rfgsm_core_handle_ptr->mtpl));
    
    /*!@todo remove this */
    if (rfgsm_resp_msg.vswr_meas_requested)
    {
      RF_MSG(RF_HIGH, "VSWR meas requested sent to GL1");
    }

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                            umid_cnf,
                                            sizeof(rfa_rf_gsm_tx_burst_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1(MSG_LEGACY_ERROR, "RFGSM Tx Burst MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_idle_frame_processing_req

===========================================================================*/
/*! @fn rfgsm_dispatch_idle_frame_processing_req( rfa_rf_gsm_idle_frame_processing_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the triggering of the idle frame 
 *  functionality i.e therm read.
 *  @param rfgsm_msg_ptr a pointer to the idle_frame_processing structure 
 *  variable payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_idle_frame_processing_req( rfa_rf_gsm_idle_frame_processing_req_s* rfgsm_msg_ptr, 
                                                                   rf_task_num_type rf_task,
                                                                   uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_idle_frame_processing_cnf_s rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_data_type          rfgsm_core_data;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_idle_frame_processing_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Idle Frame Proc API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_IDLE_FRAME_PROC_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  if( !rfgsm_dispatch_check_device( rfgsm_msg_ptr->device, rfgsm_msg_ptr->hdr.id ) )
  {
    dispatch_result = RFGSM_DISPATCH_INVALID_DEVICE;
  }
  else
  {
    rfgsm_core_handle_ptr = rfgsm_core_handle_get( rfgsm_msg_ptr->device );

    if( rfgsm_core_handle_ptr == NULL )
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR,
            "rfgsm_core_handle_ptr is NULL for device %d!", rfgsm_msg_ptr->device);
      dispatch_result = RFGSM_DISPATCH_NULL_PTR;
    }
  }

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* Verify payload parameters. */
  if( dispatch_result == RFGSM_DISPATCH_SUCCESSFUL )
  {
    RFGC_T_MSG_2( MSG_LEGACY_HIGH, 
                  "RF GSM Cmd Proc Idle Frame Proc, dev_id=%d, cnf=%d", 
                  rfgsm_msg_ptr->device,
                  rfgsm_msg_ptr->cnf_required );

    /* Set the core data */
    rfgsm_core_data.rf_task = rf_task;
    rfgsm_core_data.rfm_dev = rfgsm_msg_ptr->device;
    rfgsm_core_data.sub_id = rfgsm_msg_ptr->sub_id;
    rfgsm_core_data.as_id = gl1_hw_rf_map_rf_sub_id_to_as_id(rfgsm_msg_ptr->sub_id);
    rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
    rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

    if( rfgsm_mc_do_idle_frame_processing(&rfgsm_core_data) == RFGSM_MC_SUCCESS )
    {
      cmd_result = RFA_RF_GSM_SUCCESS;
    }
  }
  else /* dispatch_result != RFGSM_DISPATCH_SUCCESSFUL */
  {
    RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RF GSM Idle frame proc dispatcher failed variable verification for device %d", rfgsm_msg_ptr->device);

    /* Handle dispatch errors. */
    rfgsm_error_handler( dispatch_result, rfgsm_msg_ptr->hdr.id, rf_task, rfgsm_msg_ptr->device );
  }

  /* Log the end of the Idle Frame Proc API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_IDLE_FRAME_PROC_END, rfgsm_msg_ptr->sub_id, rfgsm_msg_ptr->device, 0);

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr          =     rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.device       =     rfgsm_msg_ptr->device;
    rfgsm_resp_msg.cnf_required =     rfgsm_msg_ptr->cnf_required;
    rfgsm_resp_msg.exec_time_us =     duration_us;
    rfgsm_resp_msg.req_result   =     cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                           umid_cnf,
                                           sizeof(rfa_rf_gsm_idle_frame_processing_cnf_s));
    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1(MSG_LEGACY_ERROR, "RFGSM Idle Frame Processing MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_therm_read_rsp

===========================================================================*/
/*! @fn rfgsm_dispatch_therm_read_req( rfgsm_rflm_msg_u* rfgsm_msg_ptr )
 *  @brief This function dispatches the processing of the thermistor read data
 *  @param rfgsm_rflm_msg_u a pointer to the therm_read variable payload.
 *  @param rf_task_enum_type RF task number which the message is received on
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_therm_read_rsp( rfgsm_rflm_msg_u* rfgsm_msg_ptr,
                                                        rf_task_num_type rf_task )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      rsp_cnf = RFGSM_DISPATCH_UNSUCCESSFUL;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  rfcom_device_enum_type        device = RFM_INVALID_DEVICE;
  uint8                         event_handle_tag = 0;
  uint8                         therm_read_ok;
  rfgsm_core_data_type          rfgsm_core_data;
  rflm_gsm_resp_status          status;
  
  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();
  uint8 sub_id = (rf_task == RF_TASK_2) ? RFLM_GSM_SUBSCRIPTION_2 : RFLM_GSM_DEFAULT_SUBSCRIPTION;

  /* Verify payload ptr is not NULL */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfgsm_msg_ptr is NULL", RFGSM_DISPATCH_NULL_PTR );

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Therm Read Response API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_THERM_READ_RESPONSE_BEGIN, sub_id, 0, 0);

  /* Process payload */
  device =(rfcom_device_enum_type)((rfgsm_msg_ptr->rflm_gsm_therm_read_rsp.payload>>16) & 0xFFFF );
  event_handle_tag = (uint8)( rfgsm_msg_ptr->rflm_gsm_therm_read_rsp.payload & 0xFF );
  status = rfgsm_msg_ptr->rflm_gsm_therm_read_rsp.status;

  RFGC_T_MSG_3( MSG_LEGACY_HIGH, 
                "RF GSM RSP Proc therm read, dev_id=%d, therm event_handle tag %d, status %d",
                                      device, 
                                      event_handle_tag,
                status );

  /* At this point, status should ONLY be one of the following:     
     RFLM_GSM_STATUS_OK     (successful)
     RFLM_GSM_STATUS_ERROR  (no appropriate status set by GFW - this should never happen!)
     RFLM_GSM_STATUS_THERM_READ_ABORTED (cancellation e.g. when GFW is going to sleep)
     RFLM_GSM_STATUS_THERM_READ_TIMEOUT (GFW was not able to schedule therm read within 4 frames)
  */
  /* @todo Replace ASSERT with appropriate error handling once code is mature */
  ASSERT( (status != RFLM_GSM_STATUS_DEFAULT) || (status != RFLM_GSM_STATUS_MAX) || (status != RFLM_GSM_STATUS_ERROR) );

  /* Therm read OK => process therm read response and do the house-keeping */
  /* Therm read NOK => only do the house-keeping */
  therm_read_ok = (status == RFLM_GSM_STATUS_OK) ? TRUE : FALSE;

  /* Get the handle*/
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(device);
  
  if (rfgsm_core_handle_ptr == NULL)
  {
    RFGC_T_MSG_1(MSG_LEGACY_ERROR,"rfgsm_core_handle_ptr is NULL!", 0);
    rsp_cnf = RFGSM_DISPATCH_NULL_PTR;
    return rsp_cnf;
  }

  /* Set the core data */
  rfgsm_core_data.rf_task = rf_task;
  rfgsm_core_data.rfm_dev = device;
  rfgsm_core_data.sub_id = sub_id;
  rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
  rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

  if( rfgsm_mc_process_therm_read_rsp(&rfgsm_core_data, 
                                      event_handle_tag, 
                                      therm_read_ok) == RFGSM_MC_SUCCESS )
  {
    rsp_cnf = RFGSM_DISPATCH_SUCCESSFUL;
  }

  /* Log the end of Therm Read Reponse API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_THERM_READ_RESPONSE_END, 
                              sub_id, 
                              (status << 16 ) | (event_handle_tag << 8) | device, 
                              0);

  return(rsp_cnf);
}

/*===========================================================================

  FUNCTION:  rfgsm_dispatch_device_status_read_rsp

===========================================================================*/
/*! @fn rfgsm_dispatch_device_status_read_rsp( rfgsm_rflm_msg_u* rfgsm_msg_ptr )
 *  @brief This function dispatches the processing of the Device Status read data
 *  @param rfgsm_rflm_msg_u a pointer to the de variable payload.
 *  @param rf_task_enum_type RF task number which the message is received on
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_device_status_read_rsp( rfgsm_rflm_msg_u* rfgsm_msg_ptr,
                                                        rf_task_num_type rf_task )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      rsp_cnf = RFGSM_DISPATCH_UNSUCCESSFUL;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  void                          *event_handle;
  rfcom_device_enum_type        device = RFM_INVALID_DEVICE;
  uint32                        buffer_id;
  uint8                         sub_id;
  rf_burst_type                 burst_type;
  rfgsm_core_dm_script_type 	script_type;
  uint8                         event_tag;
  uint8                         burst_num;
    
  /* Verify payload ptr is not NULL */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfgsm_msg_ptr is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Process payload */
  device =(rfcom_device_enum_type)((rfgsm_msg_ptr->rflm_gsm_device_status_read_rsp.payload>>28) & 0x0F );
  script_type = (rfgsm_core_dm_script_type)((rfgsm_msg_ptr->rflm_gsm_device_status_read_rsp.payload>>24) & 0x0F );
  event_tag = (uint8)((rfgsm_msg_ptr->rflm_gsm_device_status_read_rsp.payload>>16) & 0xFF );
  buffer_id = (uint8)((rfgsm_msg_ptr->rflm_gsm_device_status_read_rsp.payload>>12) & 0x0F );
  sub_id = (uint8)((rfgsm_msg_ptr->rflm_gsm_device_status_read_rsp.payload>>8) & 0x0F );
  burst_num = (uint8)((rfgsm_msg_ptr->rflm_gsm_device_status_read_rsp.payload>>4) & 0x0F );
  burst_type = (uint8)(rfgsm_msg_ptr->rflm_gsm_device_status_read_rsp.payload & 0x0F );

  if( device >= RFM_MAX_WAN_DEVICES )
  {
    RFGC_T_MSG_1(MSG_LEGACY_ERROR,"Device id is wrong. Its %d", device);
    rsp_cnf = RFGSM_DISPATCH_INVALID_PARAM;
	return rsp_cnf;
  }

  /* Check the burst buffer ID is valid */
  if((buffer_id >= RFLM_GSM_TRIPLE_BUFFER) && 
      (buffer_id != RFLM_GSM_BUFFER_ID_NOT_REQUIRED))
  {
    RFGC_T_MSG_1(MSG_LEGACY_ERROR, "RF GSM core DM burst_buffer_id %d invalid", buffer_id);
    rsp_cnf = RFGSM_DISPATCH_INVALID_PARAM;
	return rsp_cnf;
  }
  
  /* Get the handle*/
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(device);
  
  if (rfgsm_core_handle_ptr == NULL)
  {
    RFGC_T_MSG_1(MSG_LEGACY_ERROR,"rfgsm_core_handle_ptr is NULL! for device %d", device);
    rsp_cnf = RFGSM_DISPATCH_NULL_PTR;
    return rsp_cnf;
  }
  
  switch ( script_type )
  {
    case RFLM_GSM_TX_STOP_SCRIPT:
      if(rfgsm_mc_process_device_status_read_rsp(sub_id, buffer_id, burst_type,0, event_tag, burst_num) == RFGSM_MC_SUCCESS)
      {
        rsp_cnf = RFGSM_DISPATCH_SUCCESSFUL;
      }
      break;

    case RFLM_GSM_MON_STOP_SCRIPT:  
    case RFLM_GSM_RX_STOP_SCRIPT:
      if(rfgsm_mc_process_device_status_read_rsp(sub_id, buffer_id, burst_type,RFLM_GSM_SYNC_RX_EVENT, event_tag, burst_num) == RFGSM_MC_SUCCESS)
      {
        rsp_cnf = RFGSM_DISPATCH_SUCCESSFUL;
      } 
      break;

    case RFLM_GSM_RX_ACQ_STOP_SCRIPT:
      if(rfgsm_mc_process_device_status_read_rsp(sub_id, buffer_id, burst_type,RFLM_GSM_SCH_OR_ACQ_RX_EVENT, event_tag, burst_num) == RFGSM_MC_SUCCESS)
      {
        rsp_cnf = RFGSM_DISPATCH_SUCCESSFUL;
      } 
      break;

    case RFLM_GSM_RX_PCH_STOP_SCRIPT:
      if(rfgsm_mc_process_device_status_read_rsp(sub_id, buffer_id, burst_type,RFLM_GSM_PCH_RX_EVENT, event_tag, burst_num) == RFGSM_MC_SUCCESS)
      {
        rsp_cnf = RFGSM_DISPATCH_SUCCESSFUL;
      } 
      break;

    default:
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "Invalid Script Type for Device Status Read. Its %d ", script_type);
	  rsp_cnf = RFGSM_DISPATCH_INVALID_PARAM;
      break;
  }
  
  return(rsp_cnf);
}

#ifdef FEATURE_RF_ASDIV
/*===========================================================================

  FUNCTION:  rfgsm_dispatch_asd_read_rsp

===========================================================================*/
/*! @fn rfgsm_dispatch_asd_read_req( rfgsm_rflm_msg_u* rfgsm_msg_ptr )
 *  @brief This function dispatches the processing of the ASD read data
 *  @param rfgsm_msg_ptr a pointer to the ASD_read structure variable
 *  payload.
 *  @param rf_task_enum_type RF task number which the message is received on
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_asd_read_rsp( rfgsm_rflm_msg_u* rfgsm_msg_ptr, 
                                                      rf_task_num_type rf_task )
{
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      rsp_cnf = RFGSM_DISPATCH_UNSUCCESSFUL;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_handle_type        *rfgsm_tx_core_handle_ptr = NULL;
  rfcom_device_enum_type        device = RFM_INVALID_DEVICE ;
  uint8                         ant_postion;
  rfgsm_core_data_type          rfgsm_core_data;
  rfgsm_core_asd_switch_event_info_type *asd_info;
  rfcom_device_enum_type        tx_device = RFM_INVALID_DEVICE;
  sys_band_class_e_type sys_band = SYS_BAND_CLASS_MAX;
  rfcom_band_type_u u_gsm_band;
  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* Verify payload ptr is not NULL */
  if( rfgsm_msg_ptr == NULL )
  {
    RFGC_T_MSG(MSG_LEGACY_ERROR, "RF GSM RSP Proc ASD Read: Pointer argument into rfgsm_msg_ptr is NULL");
    rsp_cnf = RFGSM_DISPATCH_NULL_PTR;
    return rsp_cnf;
  }

  /* Process the payload */
  asd_info = (rfgsm_core_asd_switch_event_info_type *) rfgsm_msg_ptr->rflm_gsm_asd_rsp.payload;
  if( asd_info == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "RF GSM RSP Proc ASD Read: payload data is NULL");
    return rsp_cnf;
  }
  device = asd_info->rfm_dev;
  ant_postion = asd_info->set_antenna_position;

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( asd_info->sub_id, &as_id, &start_time_qs, &frame_number );

  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_ASD_READ_RSP_BEGIN, asd_info->sub_id, 0, 0);

  MSG_3(MSG_SSID_RF, MSG_LEGACY_HIGH, "RF GSM RSP Proc ASD Read Response, "
                                      "dev_id=%d, umid=0x%x, ant_position=%d",
                                      device, rfgsm_msg_ptr->hdr.id, ant_postion);

  /* Get the handle*/
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(device);
  
  if (rfgsm_core_handle_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_core_handle_ptr is NULL!", 0);
    return rsp_cnf;
  }

  /*asdiv response error case timer clear */
  rfgsm_mc_asd_rsp_check_stop(device);
  
  /* Set the core data */
  rfgsm_core_data.rf_task = rf_task;
  rfgsm_core_data.rfm_dev = device;
  rfgsm_core_data.sub_id = asd_info->sub_id;
  rfgsm_core_data.as_id = as_id;
  rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
  rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

  /* Check if swiched antenna based SAR backoff needs to be applied */
  /* If YES, next TX burst prepare shall update sar backoff limit   */

  //Get TX based on preferred device instead of associated device.
  u_gsm_band.gsm_band = rfgsm_core_handle_ptr->rx_band;
  sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_EGSM_MODE, u_gsm_band);
  tx_device = rfc_common_get_preferred_tx_device(sys_band);

  /* Get the handle for tx device */
  rfgsm_tx_core_handle_ptr = rfgsm_core_handle_get( tx_device );

  if ( rfgsm_tx_core_handle_ptr != NULL &&
       rfgsm_tx_core_handle_ptr->rfgsm_tx_nv_tbl != NULL && 
       rfgsm_tx_core_handle_ptr->rfgsm_tx_nv_tbl->tx_static_nv_data_ptr != NULL )

  {
    if( rfgsm_tx_core_handle_ptr->rfgsm_tx_nv_tbl->tx_static_nv_data_ptr->use_switched_antenna_sar_lut )
    {
      rfgsm_tx_core_handle_ptr->tx_profile_update_required = TRUE;
    }
  }
  else
  {
    MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_core_handle_ptr/rfgsm_tx_nv_tbl/tx_static_nv_data_ptr for tx device %d is NULL!", tx_device );
  }

  /* Process the response from RFLM. RFLM only sends repsonse back when the RX start  
    event (and antenna switch) has been processed by CCS */
  if( rfgsm_mc_process_asd_read_rsp(&rfgsm_core_data, ant_postion) == RFGSM_MC_SUCCESS )
  {
    rsp_cnf = RFGSM_DISPATCH_SUCCESSFUL;
  }

  /* Antenna switch has occurred => call the GL1 callback function to confirm this */
  /* The callback function in turn informs TRM the switch has occurred */
  if (asd_info->l1_cb != NULL) 
  {
    rfa_rf_gsm_set_antenna_callback_s l1_cb_data;

    /* Populate the L1 callback data */
    l1_cb_data.req_result = (rfgsm_msg_ptr->rflm_gsm_asd_rsp.status==RFLM_GSM_STATUS_OK)?RFA_RF_GSM_SUCCESS:RFA_RF_GSM_EXECUTION_ERROR;
    l1_cb_data.ant_pos = ant_postion;
    l1_cb_data.device = device;
    l1_cb_data.sub_id = asd_info->sub_id;

    /* Call the L1 callback function */
    asd_info->l1_cb( &l1_cb_data );
  }

  /*check if swiched antenna based SAR backoff needs to be applied, if YES, next TX burst prepare shall update sar backoff limit*/
  if ( rfgsm_core_handle_ptr->rfgsm_tx_nv_tbl != NULL && 
       rfgsm_core_handle_ptr->rfgsm_tx_nv_tbl->tx_static_nv_data_ptr != NULL )

  {
    if( rfgsm_core_handle_ptr->rfgsm_tx_nv_tbl->tx_static_nv_data_ptr->use_switched_antenna_sar_lut )
    {
      rfgsm_core_handle_ptr->tx_profile_update_required = TRUE;
    }
  }
  else
  {
    MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_tx_nv_tbl/tx_static_nv_data_ptr for device %d is NULL!", device );
  }

  /* Log the end of ASD Read Response API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_ASD_READ_RSP_END, 
                              asd_info->sub_id, 
                              (ant_postion << 8) | device, 
                              0);

  return(rsp_cnf);
}
#endif



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_update_temp_comp_req

===========================================================================*/
/*! @fn rfgsm_dispatch_update_temp_comp_req( rfa_rf_gsm_update_temp_comp_req_s* rfgsm_msg_ptr, uint32 umid_cnf )
 *  @brief This function dispatches the processing of temp comp update request
 *  @param rfgsm_msg_ptr a pointer to the temp comp update structure variable
 *  payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_update_temp_comp_req( rfa_rf_gsm_update_temp_comp_req_s* rfgsm_msg_ptr,
                                                              rf_task_num_type rf_task,
                                                              uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_update_temp_comp_cnf_s rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_data_type          rfgsm_core_data;

rfgsm_core_handle_ptr = rfgsm_core_handle_get( rfgsm_msg_ptr->device );
  
  if( rfgsm_core_handle_ptr == NULL )
  {
    RFGC_T_MSG_1( MSG_LEGACY_ERROR, "rfgsm_core_handle_ptr is NULL for device %d!", rfgsm_msg_ptr->device);
    dispatch_result = RFGSM_DISPATCH_NULL_PTR;
    return dispatch_result;
  }

  /* Check if Tempcomp was stopped*/
  if(FALSE == rfgsm_core_handle_ptr->rfgsm_core_temp_comp_started)
  {
    RFGC_T_MSG_1( MSG_LEGACY_HIGH, "[dev %d] Tempcomp is stopped already, not updating temp_comp data any more ", rfgsm_msg_ptr->device);
    dispatch_result = RFGSM_DISPATCH_UNSUCCESSFUL;
    return dispatch_result;
  }

  rf_task_num_type rfgsm_rf_task = rfgsm_core_get_task_id_from_sub_id(rfgsm_msg_ptr->sub_id);

  if ( RF_TASK_1 == rfgsm_rf_task)
  {
    if (rfgsm_core_handle_ptr->therm_read.fw_therm_read_umid != rfgsm_sub_to_therm_umid_map[RF_TASK_1].therm_read_umid)/*GERAN_FW_RFM_THERM_READ_CMD*/
    {
      RFGC_T_MSG_2( MSG_LEGACY_HIGH,"Not updating temp comp data because of mismatch between UMID [0x%x] and SUB_ID [%d] ",
                                                  rfgsm_core_handle_ptr->therm_read.fw_therm_read_umid, rfgsm_msg_ptr->sub_id);
      dispatch_result = RFGSM_DISPATCH_STATE_MACHINE_ERR;
    }
  }

  if ( RF_TASK_2 == rfgsm_rf_task)
  {
    if (rfgsm_core_handle_ptr->therm_read.fw_therm_read_umid != rfgsm_sub_to_therm_umid_map[RF_TASK_2].therm_read_umid)/*GERAN_FW2_RFM_THERM_READ_CMD)*/
    {
      RFGC_T_MSG_2( MSG_LEGACY_HIGH,"Not updating temp comp data because of mismatch between UMID [0x%x] and SUB_ID [%d] ",
                                                  rfgsm_core_handle_ptr->therm_read.fw_therm_read_umid, rfgsm_msg_ptr->sub_id);
      dispatch_result = RFGSM_DISPATCH_STATE_MACHINE_ERR;
    }
  }

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_update_temp_comp_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the start of the temp compensation request */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_UPDATE_TEMP_COMP_REQ_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  RFGC_T_MSG_1( MSG_LEGACY_HIGH, 
                "RF GSM Cmd update temp comp flag Proc, dev_id=%d", 
                rfgsm_msg_ptr->device );

  if( !rfgsm_dispatch_check_device( rfgsm_msg_ptr->device, rfgsm_msg_ptr->hdr.id ) )
  {
    dispatch_result = RFGSM_DISPATCH_INVALID_DEVICE;
  }


  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* Verify payload parameters. */
  if( dispatch_result == RFGSM_DISPATCH_SUCCESSFUL )
  {
    /* Triple buffer should be zero unless burst event */
    rfgsm_core_handle_ptr->buffer_id = 0;

    /* Set the core data */
    rfgsm_core_data.rf_task = rf_task;
    rfgsm_core_data.rfm_dev = rfgsm_msg_ptr->device;
    rfgsm_core_data.sub_id = rfgsm_msg_ptr->sub_id;
    rfgsm_core_data.as_id = as_id;
    rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
    rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

    if( rfgsm_mc_update_temp_comp_req(&rfgsm_core_data, 
                                      rfgsm_msg_ptr->temp_comp_scaled_value, 
                                      rfgsm_msg_ptr->temp_bin_16) == RFGSM_MC_SUCCESS )
    {
      cmd_result = RFA_RF_GSM_SUCCESS;
    }
  }
  else /* dispatch_result != RFGSM_DISPATCH_SUCCESSFUL */
  {
    RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RF GSM Update temp comp dispatcher failed variable verification for device %d", rfgsm_msg_ptr->device);

    /* Handle dispatch errors. */
    rfgsm_error_handler( dispatch_result, rfgsm_msg_ptr->hdr.id, rf_task, rfgsm_msg_ptr->device );
  }

  /* Log the end of Update Temp Comp Request API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_UPDATE_TEMP_COMP_REQ_END, rfgsm_msg_ptr->sub_id, rfgsm_msg_ptr->device, 0);

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                      =     rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.device                   =     rfgsm_msg_ptr->device;
    rfgsm_resp_msg.cnf_required             =     rfgsm_msg_ptr->cnf_required;
    rfgsm_resp_msg.req_result               =     cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                           umid_cnf,
                                           sizeof(rfa_rf_gsm_update_temp_comp_cnf_s));
    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1(MSG_LEGACY_ERROR, "RFGSM update Temp Comp cnf UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }
   
  return(dispatch_result);
}



#ifdef FEATURE_RF_ASDIV
/*===========================================================================

  FUNCTION:  rfgsm_dispatch_set_antenna_req

===========================================================================*/
/*! @fn rfgsm_dispatch_set_antenna_req(rfa_rf_gsm_set_antenna_req_s* rfgsm_msg_ptr, uint32 umid_cnf)
 *  @brief This function dispatches the processing of set antenna request
 *  @param rfgsm_msg_ptr a pointer to the set antenna structure variable
 *  payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_set_antenna_req
( 
  rfa_rf_gsm_set_antenna_req_s* rfgsm_msg_ptr,
  rf_task_num_type rf_task,
  uint32 umid_cnf
)
{
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_set_antenna_cnf_s  rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_data_type          rfgsm_core_data;
  rfcom_device_enum_type        device;
  msgr_hdr_struct_type          hdr;
  boolean                       cnf_required;
  uint8                         ant_pos;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  /* ----------------------------------------------------------------------- */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_set_antenna_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);
 
  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the Set Antenna Req API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_SET_ANTENNA_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  device = rfgsm_msg_ptr->device;
  hdr = rfgsm_msg_ptr->hdr;
  cnf_required = rfgsm_msg_ptr->cnf_required;
  ant_pos = rfgsm_msg_ptr->ant_pos;

  RFGC_T_MSG_3(MSG_LEGACY_HIGH, "RF GSM Cmd Set Antenna Request, "
    "dev_id=%d, umid=0x%x ant_pos=%d", device, hdr.id, ant_pos);

  if( !rfgsm_dispatch_check_device( device, hdr.id ) )
  {
    dispatch_result = RFGSM_DISPATCH_INVALID_DEVICE;
  }
  else 
  {
    rfgsm_core_handle_ptr = rfgsm_core_handle_get( device );
    if( rfgsm_core_handle_ptr == NULL )
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR, "rfgsm_core_handle_ptr is NULL for device %d!", device);
      dispatch_result = RFGSM_DISPATCH_UNSUCCESSFUL;
    }
  }

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* ----------------------------------------------------------------------- */
  if (dispatch_result == RFGSM_DISPATCH_SUCCESSFUL)
  {
    // Check to see if the antenna is already in the requested position
    if( rfcommon_asdiv_get_current_position(device) == ant_pos )
    {
      RFGC_T_MSG_1( MSG_LEGACY_HIGH, "Requested switch pos %d already set", ant_pos );
      rfgsm_core_handle_ptr->set_antenna_position = (uint8)RF_INVALID_VALUE_8BIT;

      if ( rfgsm_msg_ptr->l1_cb_func != NULL)
      {
        // Antenna is already in the requested position
        rfa_rf_gsm_set_antenna_callback_s l1_cb_data;
                         
        // Populate the L1 callback data
        l1_cb_data.req_result = RFA_RF_GSM_SUCCESS;
        l1_cb_data.device     = device;
        l1_cb_data.ant_pos    = ant_pos;
        l1_cb_data.sub_id     = rfgsm_msg_ptr->sub_id;

        // Call the L1 callback function 
        rfgsm_msg_ptr->l1_cb_func( &l1_cb_data ); 

        // Set result to SUCCESS for CNF message to GL1
        cmd_result = RFA_RF_GSM_SUCCESS;
      }
      else
      {
        RFGC_T_MSG( MSG_LEGACY_ERROR, "L1 callback function ptr is NULL" );
      }
    }
    else
    {
      /* These will be stored in event hdr and returned by RFLM so store them away */
      rfgsm_core_handle_ptr->asd_l1_cb_func = rfgsm_msg_ptr->l1_cb_func; 
      rfgsm_core_handle_ptr->asd_sub_id = rfgsm_msg_ptr->sub_id;

      /* Set the core data */
      rfgsm_core_data.rf_task = rf_task;
      rfgsm_core_data.rfm_dev = device;
      rfgsm_core_data.sub_id = rfgsm_msg_ptr->sub_id;
      rfgsm_core_data.as_id = gl1_hw_rf_map_rf_sub_id_to_as_id(rfgsm_msg_ptr->sub_id);
      rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
      rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

      if( rfgsm_mc_set_antenna_req(&rfgsm_core_data, ant_pos) == RFGSM_MC_SUCCESS )
      {
        cmd_result = RFA_RF_GSM_SUCCESS;
      }
    }
  }
  else
  {
    RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RF GSM Set Antenna Request dispatcher failed variable verification for device %d", device);

    /* Handle dispatch errors. */
    rfgsm_error_handler( dispatch_result, rfgsm_msg_ptr->hdr.id, rf_task, rfgsm_msg_ptr->device );
  }

  /* Log the end of Set Antenna API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_SET_ANTENNA_END, 
                              rfgsm_msg_ptr->sub_id, 
                              (ant_pos << 8) | device, 
                              0);

  return(dispatch_result);
}
#endif



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_populate_ip2_cal_script

===========================================================================*/
/*! @fn rfgsm_dispatch_build_ip2_cal_script( rfa_rf_gsm_start_ip2_cal_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf )
 *  @brief This function dispatches the processing of populate ip2 cal script
 *  @param rfgsm_msg_ptr a pointer to the ip2 cal structure variable
 *  payload.
 *  @param rf_task
 *  @param umid_cnf
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */

rfgsm_dispatch_outcome_t rfgsm_dispatch_build_ip2_cal_script
( 
  rfa_rf_gsm_start_ip2_cal_req_s* rfgsm_msg_ptr,
  rf_task_num_type rf_task,
  uint32 umid_cnf
)
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_res = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_start_ip2_cal_cnf_s  rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_data_type          rfgsm_core_data;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfgsm_dispatch_build_ip2_cal_script is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the IP2 cal script API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_BUILD_IP2_CAL_SCRIPT_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, 
        "RF GSM Cmd start IP2 cal, dev_id=%d, umid=0x%x", 
        rfgsm_msg_ptr->device, rfgsm_msg_ptr->hdr.id);

  /* Get the handle and device*/
  if(!rfgsm_dispatch_check_device( rfgsm_msg_ptr->device, rfgsm_msg_ptr->hdr.id ))
  {
    RFGC_T_MSG_1(MSG_LEGACY_ERROR,"Device ID passed is greater than or equal to %d ", RFM_MAX_WAN_DEVICES);
    dispatch_res = RFGSM_DISPATCH_INVALID_DEVICE;
  }
  else
  {
    rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfgsm_msg_ptr->device);

    if (rfgsm_core_handle_ptr == NULL)
    {
      RFGC_T_MSG_1(MSG_LEGACY_ERROR,"rfgsm_core_handle_ptr is NULL!", 0);
      dispatch_res = RFGSM_DISPATCH_NULL_PTR;
    }
  }

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* Verify payload parameters. */

  if( dispatch_res == RFGSM_DISPATCH_SUCCESSFUL )
  {
      /* Set the core data */
      rfgsm_core_data.rf_task = rf_task;
      rfgsm_core_data.rfm_dev = rfgsm_msg_ptr->device;
      rfgsm_core_data.sub_id = RFLM_GSM_DEFAULT_SUBSCRIPTION; /*! @todo rfgsm_msg_ptr->sub_id; */
      rfgsm_core_data.as_id = gl1_hw_rf_map_rf_sub_id_to_as_id(rfgsm_core_data.sub_id);
      rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
      rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

      if ( rfgsm_mc_build_ip2_cal_script(&rfgsm_core_data) )
      {
        cmd_result = RFA_RF_GSM_SUCCESS;
      }
  }
  else /* dispatch_res != RFGSM_DISPATCH_SUCCESSFUL  */
  {
    RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RF GSM build IP2 cal script dispatcher failed variable verification for device %d", rfgsm_msg_ptr->device);

    /* Handle dispatch errors. */
    rfgsm_error_handler( dispatch_res, rfgsm_msg_ptr->hdr.id, rf_task, rfgsm_msg_ptr->device );
  }

  /* Log the end of IP2 Calibration script API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_BUILD_IP2_CAL_SCRIPT_END, rfgsm_msg_ptr->sub_id, rfgsm_msg_ptr->device, 0);

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {

    /* Echo REQ message info to CNF message info for functionality usage. */
    rfgsm_resp_msg.hdr                      =     rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.device                   =     rfgsm_msg_ptr->device;
    rfgsm_resp_msg.cnf_required             =     rfgsm_msg_ptr->cnf_required;
    rfgsm_resp_msg.req_result               =     cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                           umid_cnf,
                                           sizeof(rfa_rf_gsm_start_ip2_cal_cnf_s));
    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1(MSG_LEGACY_ERROR, "RFGSM START IP2 CAL cnf UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return dispatch_res;

}

/*===========================================================================

  FUNCTION:  rfgsm_dispatch_init_msm_ip2_cal

===========================================================================*/
/*! @fn rfgsm_dispatch_init_msm_ip2_cal( rfa_rf_gsm_ip2_cal_msm_req_s* rfgsm_msg_ptr, rf_task_num_type rf_task, uint32 umid_cnf )
 *  @brief This function dispatches the processing of populate ip2 cal script
 *  @param rfgsm_msg_ptr a pointer to the ip2 cal structure variable
 *  payload.
 *  @param rf_task
 *  @param umid_cnf
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */

rfgsm_dispatch_outcome_t rfgsm_dispatch_init_msm_ip2_cal
( 
  rfa_rf_gsm_ip2_cal_msm_req_s* rfgsm_msg_ptr,
  rf_task_num_type rf_task,
  uint32 umid_cnf
)
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_res = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_ip2_cal_msm_cnf_s  rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;
  rfgsm_core_handle_type        *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_data_type          rfgsm_core_data;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfgsm_dispatch_init_msm_ip2_cal is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Log the beginning of the MSM IP2 cal API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_INIT_MSM_IP2_CAL_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, 
        "RF GSM Cmd start init IP2 cal msm, dev_id=%d, umid=0x%x", 
        rfgsm_msg_ptr->device, rfgsm_msg_ptr->hdr.id);

  /* Get the handle and device*/
  if(!rfgsm_dispatch_check_device( rfgsm_msg_ptr->device, rfgsm_msg_ptr->hdr.id ))
  {
    RFGC_T_MSG_1(MSG_LEGACY_ERROR,"Device ID passed is greater than or equal to %d ", RFM_MAX_WAN_DEVICES);
    dispatch_res = RFGSM_DISPATCH_INVALID_DEVICE;
  }
  else
  {
     /* Get the handle*/
     rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfgsm_msg_ptr->device);
      
     if (rfgsm_core_handle_ptr == NULL)
     {
        RFGC_T_MSG_1(MSG_LEGACY_ERROR,"rfgsm_core_handle_ptr is NULL!", 0);
        dispatch_res = RFGSM_DISPATCH_NULL_PTR;
     }
  }

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* Verify payload parameters. */

  if( dispatch_res == RFGSM_DISPATCH_SUCCESSFUL )
  {
    /* Set the core data */
    rfgsm_core_data.rf_task = rf_task;
    rfgsm_core_data.rfm_dev = rfgsm_msg_ptr->device;
    rfgsm_core_data.sub_id = RFLM_GSM_DEFAULT_SUBSCRIPTION; /*! @todo rfgsm_msg_ptr->sub_id; */
    rfgsm_core_data.as_id = as_id;
    rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
    rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

    if( rfgsm_mc_msm_init_ip2_cal(&rfgsm_core_data ,rfgsm_msg_ptr->ip2_rxlm_buffer_idx,rfgsm_msg_ptr->ip2_txlm_buffer_idx) )
    {
       cmd_result = RFA_RF_GSM_SUCCESS;
    }
  }
  else /* dispatch_res != RFGSM_DISPATCH_SUCCESSFUL */
  {
    RFGC_T_MSG_1( MSG_LEGACY_ERROR, "RF GSM init msm ip2 cal dispatcher failed variable verification for device %d", rfgsm_msg_ptr->device);

    /* Handle dispatch errors. */
    rfgsm_error_handler( dispatch_res, rfgsm_msg_ptr->hdr.id, rf_task, rfgsm_msg_ptr->device );
  }

  /* Log the end of Init MSM IP2 Calibration API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_INIT_MSM_IP2_CAL_END, rfgsm_msg_ptr->sub_id, rfgsm_msg_ptr->device, 0);

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for functionality usage. */
    rfgsm_resp_msg.hdr                      =     rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.device                   =     rfgsm_msg_ptr->device;
    rfgsm_resp_msg.cnf_required             =     rfgsm_msg_ptr->cnf_required;
    rfgsm_resp_msg.req_result   =     cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                           umid_cnf,
                                           sizeof(rfa_rf_gsm_ip2_cal_msm_cnf_s));
    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1(MSG_LEGACY_ERROR, "RFGSM INIT MSM IP2 CAL cnf UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return dispatch_res;

}




/*===========================================================================

  FUNCTION:  rfgsm_dispatch_sar_limit_ind

===========================================================================*/
/*! @fn rfgsm_dispatch_sar_limit_ind( rfa_rf_gsm_set_sar_limit_ind_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the SAR limit update received from MCS
 *  @param rfgsm_msg_ptr a pointer to the sar limit structure variable
 *  payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_sar_limit_ind( rfa_rf_gsm_set_sar_limit_ind_s* rfgsm_msg_ptr,
                                                       rf_task_num_type rf_task )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t dispatch_result = RFGSM_DISPATCH_UNSUCCESSFUL;
  uint32                   index = 0;

  rfa_rf_gsm_sar_limit_params_type sar_limit_params;
  int16                    sar_limit_gmsk[RFGSM_MAX_TX_SLOTS_PER_FRAME];
  int16                    sar_limit_8psk[RFGSM_MAX_TX_SLOTS_PER_FRAME];

  rfgsm_core_handle_type   *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_data_type     rfgsm_core_data;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();
  uint8 sub_id = (rf_task == RF_TASK_1) ? RFLM_GSM_SUBSCRIPTION_1 : RFLM_GSM_SUBSCRIPTION_2;

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfgsm_dispatch_sar_limit_ind is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the SAR indication API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_SAR_LIMIT_IND_BEGIN, sub_id, 0, 0);

  sar_limit_params = rfgsm_msg_ptr->rfa_rf_gsm_sar_limit_params;

  /* Validate device from MCS */
  if(sar_limit_params.device >= RFM_MAX_WAN_DEVICES)
  {
    RFGC_T_MSG_1(MSG_LEGACY_ERROR, "RF Device %d from MCS is not as expected", sar_limit_params.device);

    /* Default to device 0 if MCS fail to pass valid device */
    /* @TODO is this correct? */
    sar_limit_params.device = RFCOM_TRANSCEIVER_0;
  }

  /* Get the handle*/
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(sar_limit_params.device);
  RF_CMD_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_handle_ptr is NULL!", RFGSM_DISPATCH_NULL_PTR);

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* Copy GMSK and 8PSK modulation t xpwr limts into local structures*/
  for (index = 0; index < RFGSM_MAX_TX_SLOTS_PER_FRAME; index++)
  {
    sar_limit_gmsk[index] = sar_limit_params.sar_tx_limit[0][index];
    sar_limit_8psk[index] = sar_limit_params.sar_tx_limit[1][index];
  }

  /* Set the core data */
  rfgsm_core_data.rf_task = rf_task;
  rfgsm_core_data.rfm_dev = sar_limit_params.device;
  rfgsm_core_data.sub_id = sub_id;
  rfgsm_core_data.as_id = as_id;
  rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
  rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

  if( rfgsm_mc_set_simult_sar_limit(&rfgsm_core_data, sar_limit_gmsk, sar_limit_8psk) == RFGSM_MC_SUCCESS )
  {
    dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  }

  /* Log the end of SAR Limit Indication API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_SAR_LIMIT_IND_END, sub_id, sar_limit_params.device, 0);

  return(dispatch_result);
}

/*===========================================================================

  FUNCTION:  rfgsm_dispatch_vbatt_limit_ind

===========================================================================*/
/*! @fn rfgsm_dispatch_vbatt_limit_ind( rfa_rf_gsm_set_vbatt_limit_ind_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the VBATT limit update received from MCS
 *  @param rfgsm_msg_ptr a pointer to the vbatt limit structure variable
 *  payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_vbatt_limit_ind( rfa_rf_gsm_set_vbatt_limit_ind_s* rfgsm_msg_ptr,
                                                       rf_task_num_type rf_task )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t dispatch_result = RFGSM_DISPATCH_UNSUCCESSFUL;
  uint32                   index = 0;

  rfa_rf_gsm_vbatt_limit_params_type vbatt_limit_params;
  int16                    vbatt_limit_gmsk[RFGSM_MAX_TX_SLOTS_PER_FRAME];
  int16                    vbatt_limit_8psk[RFGSM_MAX_TX_SLOTS_PER_FRAME];

  rfgsm_core_handle_type   *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_data_type     rfgsm_core_data;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* Retrieve sub_id from task as not passed in MSGR payload */
  uint8 sub_id = (rf_task == RF_TASK_1) ? RFLM_GSM_SUBSCRIPTION_1 : RFLM_GSM_SUBSCRIPTION_2;

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfgsm_dispatch_vbatt_limit_ind is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the VBATT indication API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_VBATT_LIMIT_IND_BEGIN, sub_id, 0, 0);

  /*@TODO this is doing a structure copy, why can't we just use the msg payload? */
  vbatt_limit_params = rfgsm_msg_ptr->rfa_rf_gsm_vbatt_limit_params;

  /* Get the handle */
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(vbatt_limit_params.device);
  
  RF_CMD_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_handle_ptr is NULL!", RFGSM_DISPATCH_NULL_PTR);

  if(vbatt_limit_params.device >= RFM_MAX_WAN_DEVICES)
  {
    RFGC_T_MSG_1(MSG_LEGACY_ERROR,
          "RF Device %d from MCS is not as expected", vbatt_limit_params.device);
    vbatt_limit_params.device = RFCOM_TRANSCEIVER_0;
  }

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* Copy GMSK and 8PSK modulation t xpwr limts into local structures*/
  for (index = 0; index < RFGSM_MAX_TX_SLOTS_PER_FRAME; index++)
  {
    vbatt_limit_gmsk[index] = vbatt_limit_params.vbatt_tx_limit[0][index];
    vbatt_limit_8psk[index] = vbatt_limit_params.vbatt_tx_limit[1][index];
  }

  /* Set the core data */
  rfgsm_core_data.rf_task = rf_task;
  rfgsm_core_data.rfm_dev = vbatt_limit_params.device;
  rfgsm_core_data.sub_id = sub_id;
  rfgsm_core_data.as_id = as_id;
  rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
  rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

  if( rfgsm_mc_set_simult_vbatt_limit(&rfgsm_core_data, vbatt_limit_gmsk, vbatt_limit_8psk) == RFGSM_MC_SUCCESS )
  {
    dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  }

  /* Log the end of the VBATT Limit Indication API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_VBATT_LIMIT_IND_END, sub_id, vbatt_limit_params.device, 0);

  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_cm_enter_req

===========================================================================*/
/*! @fn rfgsm_dispatch_cm_enter_req( rfa_rf_gsm_cm_enter_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the initialisation of the RF Driver for 
 *  a specific radio transceiver for GSM.
 *  @param rfgsm_msg_ptr a pointer to the exit_mode structure variable
 *  payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_cm_enter_req( rfa_rf_gsm_cm_enter_req_s* rfgsm_msg_ptr, 
                                                      rf_task_num_type rf_task,
                                                      uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_cm_enter_cnf_s     rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;
  rfgsm_core_data_type          rfgsm_core_data;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_cm_enter_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the CM Enter API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_CM_ENTER_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* Verify payload parameters. */
  RFGC_T_MSG_1( MSG_LEGACY_HIGH, 
                "RF GSM Cmd Proc CM Enter cnf=%d", 
                  rfgsm_msg_ptr->cnf_required );

    /* Set the core data */
    rfgsm_core_data.rf_task = rf_task;
    rfgsm_core_data.sub_id = rfgsm_msg_ptr->sub_id;
    rfgsm_core_data.as_id = gl1_hw_rf_map_rf_sub_id_to_as_id(rfgsm_msg_ptr->sub_id);
    rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;

    if( rfgsm_mc_cm_enter( &rfgsm_core_data, 
                           rfgsm_msg_ptr->timing_info ) == RFGSM_MC_SUCCESS )
    {
      cmd_result = RFA_RF_GSM_SUCCESS;
    }

    /* Log the end of the CM Enter API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_CM_ENTER_END, rfgsm_msg_ptr->sub_id, 0, 0);

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                      =     rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.cnf_required             =     rfgsm_msg_ptr->cnf_required;
    rfgsm_resp_msg.exec_time_us             =     duration_us;
    rfgsm_resp_msg.req_result               =     cmd_result;

    if ( rfgsm_mc_retrieve_sawless_support(rfgsm_resp_msg.sawless_sup) == FALSE )
    {
      RFGC_T_MSG(MSG_LEGACY_ERROR, "Failed to retrieve SAWless status for logical devices during CM enter mode!");
    }

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                            umid_cnf,
                                            sizeof(rfa_rf_gsm_cm_enter_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1(MSG_LEGACY_ERROR, "RFGSM Compressed Mode Enter MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_cm_exit_req

===========================================================================*/
/*! @fn rfgsm_dispatch_cm_exit_req( rfa_rf_gsm_cm_exit_req_s* rfgsm_msg_ptr )
 *  @brief This function dispatches the initialisation of the RF Driver for 
 *  a specific radio transceiver for GSM.
 *  @param rfgsm_msg_ptr a pointer to the exit_mode structure variable
 *  payload.
 *  @return rfgsm_dispatch_outcome_t enum variable describing the success/
 *  failure of the function.
 */
rfgsm_dispatch_outcome_t rfgsm_dispatch_cm_exit_req( rfa_rf_gsm_cm_exit_req_s* rfgsm_msg_ptr, 
                                                     rf_task_num_type rf_task,
                                                     uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t      dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t      cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_cm_exit_cnf_s      rfgsm_resp_msg;
  errno_enum_type               msgr_send_status = E_FAILURE;
  rfgsm_core_data_type          rfgsm_core_data;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_cm_exit_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the CM Exit API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_CM_EXIT_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* Verify payload parameters. */

  RFGC_T_MSG_1( MSG_LEGACY_HIGH, 
                "RF GSM Cmd Proc CM Exit, cnf=%d", 
                  rfgsm_msg_ptr->cnf_required );

    /* Set the core data */
    rfgsm_core_data.rf_task = rf_task;
    rfgsm_core_data.sub_id = rfgsm_msg_ptr->sub_id;
    rfgsm_core_data.as_id = gl1_hw_rf_map_rf_sub_id_to_as_id(rfgsm_msg_ptr->sub_id);
    rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;

    if( rfgsm_mc_cm_exit(&rfgsm_core_data) == RFGSM_MC_SUCCESS )
    {
      cmd_result = RFA_RF_GSM_SUCCESS;
    }

    /* Log the end of the CM Exit API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_CM_EXIT_END, rfgsm_msg_ptr->sub_id, 0, 0);

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                      =     rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.cnf_required             =     rfgsm_msg_ptr->cnf_required;
    rfgsm_resp_msg.exec_time_us             =     duration_us;
    rfgsm_resp_msg.req_result               =     cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                            umid_cnf,
                                            sizeof(rfa_rf_gsm_cm_exit_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1(MSG_LEGACY_ERROR, "RFGSM Compressed Mode Exit MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return(dispatch_result);
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_process_burst_metrics

===========================================================================*/
rfgsm_dispatch_outcome_t rfgsm_dispatch_process_burst_metrics( rfa_rf_gsm_process_burst_metrics_req_s* rfgsm_msg_ptr, 
                                                         rf_task_num_type rf_task, 
                                                         uint32 umid_cnf )
{
  /* ----------------------------------------------------------------------- */
  /* Initialise Response and Result Variables */
  rfgsm_dispatch_outcome_t          dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfa_rf_gsm_cmd_outcome_t          cmd_result = RFA_RF_GSM_EXECUTION_ERROR;
  rfa_rf_gsm_process_burst_metrics_cnf_s  rfgsm_resp_msg;
  errno_enum_type                   msgr_send_status = E_FAILURE;
  rfgsm_core_handle_type            *rfgsm_core_handle_ptr = NULL;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* ----------------------------------------------------------------------- */
  /* Check common msgr msg ptr, device id , and rfgsm core handle validity   */
  RF_CMD_NULL_CHECK_RTN( rfgsm_msg_ptr, "Pointer argument into rfa_rf_gsm_process_burst_metrics_req_s is NULL", RFGSM_DISPATCH_NULL_PTR);

  /* Get GERAN Access Stratum, current QS, and current SW frame number */
  rfgsm_cmd_proc_get_gl1_data( rfgsm_msg_ptr->sub_id, &as_id, &start_time_qs, &frame_number );

  /* Log the beginning of the bursat metrics API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_CMD_BURST_METRICS_BEGIN, rfgsm_msg_ptr->sub_id, 0, 0);

  if( !rfgsm_dispatch_check_device( rfgsm_msg_ptr->device, rfgsm_msg_ptr->hdr.id ) )
  {
    dispatch_result = RFGSM_DISPATCH_INVALID_DEVICE;
  }
  else
  {
    rfgsm_core_handle_ptr = rfgsm_core_handle_get( rfgsm_msg_ptr->device );

    if( rfgsm_core_handle_ptr == NULL )
    {
      RFGC_T_MSG_1( MSG_LEGACY_ERROR,
            "rfgsm_core_handle_ptr is NULL for device %d!", rfgsm_msg_ptr->device);
      dispatch_result = RFGSM_DISPATCH_NULL_PTR;
    }
  }

  /* ----------------------------------------------------------------------- */
  /* Execution                                                               */
  /* Verify payload parameters. */
  if( dispatch_result != RFGSM_DISPATCH_SUCCESSFUL )
  {
    RFGC_T_MSG(MSG_LEGACY_ERROR, "RF GSM Process Metrics dispatcher failed variable verification");
  }
  else
  {
    if( rfgsm_core_handle_ptr->sawless_linearity_control.enable_logging == TRUE )
    {
      RFGC_T_MSG_2( MSG_LEGACY_HIGH, 
                    "RF GSM Cmd Proc Process SAWless Metrics, dev_id=%d, cnf=%d", 
                    rfgsm_msg_ptr->device,
                    rfgsm_msg_ptr->cnf_required );
    }

    if( rfgsm_msg_ptr->num_metrics_entries > RFGSM_MAX_METRICS_ENTRIES )
    {
      RFGC_T_MSG_2(MSG_LEGACY_ERROR, 
                   "RF GSM Process Metrics: too many entries passed! Max: %d, Passed: %d", 
                   rfgsm_msg_ptr->num_metrics_entries, 
                   RFGSM_MAX_METRICS_ENTRIES);
      cmd_result = RFA_RF_GSM_L1_PARAM_ERROR;
    }
    else
    {
        if ( (rfgsm_mc_process_burst_metrics(rfgsm_msg_ptr->device, rfgsm_msg_ptr->metrics, rfgsm_msg_ptr->num_metrics_entries)) == TRUE )
        {
          cmd_result = RFA_RF_GSM_SUCCESS;
        }
    }
  }

    /* Log the end of the Burst Metrics API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_CMD_BURST_METRICS_END, 
                              rfgsm_msg_ptr->sub_id, 
                              (rfgsm_msg_ptr->num_metrics_entries << 8) | rfgsm_msg_ptr->device, 
                              0);

  /* ----------------------------------------------------------------------- */
  /* Response                                                                */
  if(rfgsm_msg_ptr->cnf_required)
  {
    /* Echo REQ message info to CNF message info for message confirmation. */
    rfgsm_resp_msg.hdr                      =     rfgsm_msg_ptr->hdr;
    rfgsm_resp_msg.device                   =     rfgsm_msg_ptr->device;
    rfgsm_resp_msg.cnf_required             =     rfgsm_msg_ptr->cnf_required;
    rfgsm_resp_msg.req_result               =     cmd_result;

    /* Send a confirmation message */
    msgr_send_status = rfgsm_send_msgr_cnf((msgr_hdr_struct_type *)&rfgsm_resp_msg,
                                            umid_cnf,
                                            sizeof(rfa_rf_gsm_process_burst_metrics_cnf_s));

    if (msgr_send_status != E_SUCCESS)
    {
      RFGC_T_MSG_1(MSG_LEGACY_ERROR, "RFGSM Process Metrics MSGR Send UNSUCCESSFUL, error_no %d",msgr_send_status);
    }
  }

  return(dispatch_result);

}

/*===========================================================================

  FUNCTION:  rfgsm_l1_msgr_register

===========================================================================*/
/*!
  @fn boolean rfgsm_msgr_register( msgr_client_t *client_id, msgr_id_t id )
  @brief This function is used by RF Task to register for commands sent by the
         L1 task via the message router. This function is called from
         rf_task_msgr_register function which is called from RF Task itself
  @param client_id a pointer to the RF task control block to regsiter
  @param id the RF TASK command signal RF_CMD_Q_SIG flagging the TASK that there
            are commands waiting in the queue
  @return boolean TRUE if all registrations were successful, FALSE otherwise
*/
/*=========================================================================*/
boolean rfgsm_msgr_register( msgr_client_t *client_id, msgr_id_t id )
{
  boolean         rc         = TRUE;
  errno_enum_type ret_status = E_SUCCESS;

  /* Register for the RF GSM L1 Commands that the RF task is interested in receiving */
  ret_status = msgr_register_block(MSGR_RFA_RF_GSM,
                                   client_id,
                                   id,
                                   rfgsm_umid_req_list,
                                   RF_GSM_REQ_CNT(rfgsm_umid_req_list));


  if (ret_status != E_SUCCESS)
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "rfgsm_l1_msgr_register: Unable to register for RF GSM L1 message router register block." );
    rc = FALSE;
  }

  return(rc);
}

/*===========================================================================

  FUNCTION:  rfgsm_msgr_register_sub2

===========================================================================*/
/*!
  @fn boolean rfgsm_msgr_register( msgr_client_t *client_id, msgr_id_t id )
  @brief This function is used by RF Task to register for commands sent by the
         L1 task via the message router. This function is called from
         rf_task_msgr_register function which is called from RF Task itself
  @param client_id a pointer to the RF task control block to regsiter
  @param id the RF TASK command signal RF_CMD_Q_SIG flagging the TASK that there
            are commands waiting in the queue
  @return boolean TRUE if all registrations were successful, FALSE otherwise
*/
/*=========================================================================*/
boolean rfgsm_msgr_register_sub2( msgr_client_t *client_id, msgr_id_t id )
{
  boolean         rc         = TRUE;
  errno_enum_type ret_status = E_SUCCESS;

  /* Register for the RF GSM L1 Commands that the RF task is interested in receiving */
  ret_status = msgr_register_block(MSGR_RFA_RF_GSM,
                                   client_id,
                                   id,
                                   rfgsm_umid_req_sub2_list,
                                   RF_GSM_REQ_CNT(rfgsm_umid_req_sub2_list));
  if (ret_status != E_SUCCESS)
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "rfgsm_l1_msgr_register: Unable to register for RF GSM L1 message router register block." );
    rc = FALSE;
  }

  return(rc);
}



/*===========================================================================

  FUNCTION:  rfgsm_msgr_deregister

===========================================================================*/
/*!
  @fn boolean rfgsm_msgr_register( msgr_client_t *client_id, msgr_id_t id )
  @brief This function is used by RF Task to de-register commands sent by the
         L1 task via the message router. This function is called from
         rf_task_msgr_register function which is called from RF Task itself
  @param client_id a pointer to the RF task control block to de-regsiter
  @return boolean TRUE if all de-registrations were successful, FALSE otherwise
*/
/*=========================================================================*/
boolean rfgsm_msgr_deregister( msgr_client_t *client_id )
{
  boolean         rc         = TRUE;
  errno_enum_type ret_status = E_SUCCESS;

  /* Deregister all the RF GSM L1 message RF task has registered for. */
  ret_status = msgr_deregister_block(MSGR_RFA_RF_GSM,
                                     client_id,
                                     rfgsm_umid_req_list,
                                     RF_GSM_REQ_CNT(rfgsm_umid_req_list));

  if (ret_status != E_SUCCESS)
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "rfgsm_msgr_deregister: Unable to deregister RF message router register block." );
    rc = FALSE;
  }
  return(rc);
}

/*===========================================================================

  FUNCTION:  rfgsm_msgr_deregister_sub2

===========================================================================*/
/*!
  @fn boolean rfgsm_msgr_register( msgr_client_t *client_id, msgr_id_t id )
  @brief This function is used by RF Task to de-register commands sent by the
         L1 task via the message router. This function is called from
         rf_task_msgr_register function which is called from RF Task itself
  @param client_id a pointer to the RF task control block to de-regsiter
  @return boolean TRUE if all de-registrations were successful, FALSE otherwise
*/
/*=========================================================================*/
boolean rfgsm_msgr_deregister_sub2( msgr_client_t *client_id )
{
  boolean         rc         = TRUE;
  errno_enum_type ret_status = E_SUCCESS;

  /* Deregister all the RF GSM L1 message RF task has registered for. */
  ret_status = msgr_deregister_block(MSGR_RFA_RF_GSM,
                                     client_id,
                                     rfgsm_umid_req_sub2_list,
                                     RF_GSM_REQ_CNT(rfgsm_umid_req_sub2_list));
  if (ret_status != E_SUCCESS)
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "rfgsm_msgr_deregister: Unable to deregister RF message router register block." );
    rc = FALSE;
  }
  return(rc);
}



/*===========================================================================

  FUNCTION:  rfgsm_req_dispatcher

===========================================================================*/
/*!
  @brief This is the common command handler for rf gsm RFM commands
  @param req_ptr A pointer to a structure containing tcb and msgr details
  @return void
*/
/*=========================================================================*/
void rfgsm_req_dispatcher( rf_cmd_type* req_ptr )
{
  rfgsm_dispatch_outcome_t dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfgsm_msg_u rfgsm_msgr_req;
  uint8 * cpy_dest;
  void * cpy_src;
  size_t cpy_size;

  if( req_ptr != NULL )
  {
    /*Limit memcpy message copy size to max allowable size*/
    cpy_size = MIN(sizeof(rfgsm_msg_u)-sizeof(rf_msgr_hdr_struct_type), RF_MAX_CMD_PAYLOAD_SIZE);

    cpy_src = (void *) &req_ptr->msgr_hdr;
    cpy_dest = (uint8 *) &rfgsm_msgr_req;

    /*Copy the MSGR request packet header */
    memscpy( (void *)cpy_dest, sizeof(rfgsm_msg_u), cpy_src, sizeof(rf_msgr_hdr_struct_type));

    /* Set the destination and source pointers to the payload */
    cpy_src = (void *) &req_ptr->payload;
    cpy_dest += sizeof(rf_msgr_hdr_struct_type);

    /* Copy the payload */
    memscpy( (void *)cpy_dest, (sizeof(rfgsm_msg_u)-sizeof(rf_msgr_hdr_struct_type)), cpy_src, cpy_size );

      /* Dispatch the message */
      dispatch_result = rfgsm_req_dispatch(&rfgsm_msgr_req);

      if (dispatch_result != RFGSM_DISPATCH_SUCCESSFUL)
      {
        MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, "GSM command received from rf command dispatcher. GSM Command"
              "processor could not process the command. Exited with error code"
              "%d", dispatch_result );
        dispatch_result = RFGSM_DISPATCH_UNSUCCESSFUL;
      }
  }
}


/*===========================================================================

  FUNCTION:  rfgsm_req_dispatcher_2

===========================================================================*/
/*!
  @brief This is the common command handler for rf gsm RFM commands
  @param req_ptr A pointer to a structure containing tcb and msgr details
  @return void
*/
/*=========================================================================*/

void rfgsm_req_dispatcher_2( rf_cmd_type* req_ptr )
{
  rfgsm_dispatch_outcome_t dispatch_result = RFGSM_DISPATCH_SUCCESSFUL;
  rfgsm_msg_u rfgsm_msgr_req;
  uint8 * cpy_dest;
  void * cpy_src;
  size_t cpy_size;

  if( req_ptr != NULL )
  {
    /*Limit memcpy message copy size to max allowable size*/
    cpy_size = MIN(sizeof(rfgsm_msg_u)-sizeof(rf_msgr_hdr_struct_type), RF_MAX_CMD_PAYLOAD_SIZE);

    cpy_src = (void *) &req_ptr->msgr_hdr;
    cpy_dest = (uint8 *) &rfgsm_msgr_req;

    /*Copy the MSGR request packet header */
    memscpy( (void *)cpy_dest, sizeof(rfgsm_msg_u), cpy_src, sizeof(rf_msgr_hdr_struct_type));

    /* Set the destination and source pointers to the payload */
    cpy_src = (void *) &req_ptr->payload;
    cpy_dest += sizeof(rf_msgr_hdr_struct_type);

    /* Copy the payload */
    memscpy( (void *)cpy_dest, (sizeof(rfgsm_msg_u)-sizeof(rf_msgr_hdr_struct_type)), cpy_src, cpy_size );

      dispatch_result = rfgsm_req_dispatch_2(&rfgsm_msgr_req);

      if (dispatch_result != RFGSM_DISPATCH_SUCCESSFUL)
      {
        MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, "GSM command received from rf command dispatcher. GSM Command"
                  "processor could not process the command. Exited with error code"
                  "%d", dispatch_result );
        dispatch_result = RFGSM_DISPATCH_UNSUCCESSFUL;
      }

  }

}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_init

===========================================================================*/
/*!
  @fn void rfgsm_dispatch_init(void)
  @brief Registers the GSM module and dispatch handlers with the RF task dispatcher.
         This function is called from rfgsm_mc_init before any RF GSM L1 command is
         sent to the RF Driver Task.
*/
/*=========================================================================*/
void rfgsm_dispatch_init(void)
{
  /*Register with rf module dispatch table using RF RFA as the module.*/
  if(rf_cmd_dispatch_register_tech( RF_TASK_1, 
                                    MSGR_TECH_RFA, 
                                    MSGR_MODULE_RF_GSM, 
                                    rfgsm_req_dispatcher ) != RF_CMD_REGISTRATION_SUCCESS)
  {
     MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "rfgsm_dispatch_init:Failed MSGR_TECH_RFA / MSGR_MODULE_RF_GSM "
               "registration" );
     return;
  }
}

/*===========================================================================

  FUNCTION:  rfgsm_dispatch_init_2

===========================================================================*/
/*!
  @fn void rfgsm_dispatch_init(void)
  @brief Registers the GSM module and dispatch handlers with the RF task dispatcher.
         This function is called from rfgsm_mc_init before any RF GSM L1 command is
         sent to the RF Driver Task.
*/
/*=========================================================================*/
void rfgsm_dispatch_init_2(void)
{
  /*Register with rf module dispatch table using RF RFA as the module.*/
  if(rf_cmd_dispatch_register_tech( RF_TASK_2, 
                                    MSGR_TECH_RFA, 
                                    MSGR_MODULE_RF_GSM, 
                                    rfgsm_req_dispatcher_2 ) != RF_CMD_REGISTRATION_SUCCESS)
  {
     MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "rfgsm_dispatch_init_2:Failed MSGR_TECH_RFA / MSGR_MODULE_RF_GSM "
               "registration" );
     return;
  }
}



/*===========================================================================

  FUNCTION:  rfgsm_dispatch_deinit

===========================================================================*/
/*!
  @fn void rfgsm_dispatch_deinit(void)
  @brief This funtion has to be called whenever the RF task is killed. Frees allocated
         memory and registration tables.
  @return void
  @TODO this function is not called at the moment but need verify if required because
        rfm_init is only called once. Need to understand if is is possible to create 
        multiple instances of the same module and therefore not deregistering could 
        be a problem if the case where multiple inits occurs. (Shouldn't occur)
*/
/*=========================================================================*/
void rfgsm_dispatch_deinit(void)
{
   /*Clean up the rf command dispatch table which contains all the pointers to 
     individual command handler. The rf module dispatch table is cleaned by 
     rf_req_dispatcher when rf task is killed.
   */
   /*! @TODO the module dispatch de-registration returns a success/failure flag 
             which needs to be handled here.
   */
  rf_cmd_dispatch_deregister_tech( RF_TASK_1, MSGR_TECH_RFA, MSGR_MODULE_RF_GSM);
}

/*===========================================================================

  FUNCTION:  rfgsm_dispatch_deinit_2

===========================================================================*/
/*!
  @fn void rfgsm_dispatch_deinit(void)
  @brief This funtion has to be called whenever the RF task is killed. Frees allocated
         memory and registration tables.
  @return void
  @TODO this function is not called at the moment but need verify if required because
        rfm_init is only called once. Need to understand if is is possible to create 
        multiple instances of the same module and therefore not deregistering could 
        be a problem if the case where multiple inits occurs. (Shouldn't occur)
*/
/*=========================================================================*/
void rfgsm_dispatch_deinit_2(void)
{
   /*Clean up the rf command dispatch table which contains all the pointers to 
     individual command handler. The rf module dispatch table is cleaned by 
     rf_req_dispatcher when rf task is killed.
   */
   /*! @TODO the module dispatch de-registration returns a success/failure flag 
             which needs to be handled here.
   */
  rf_cmd_dispatch_deregister_tech( RF_TASK_2, MSGR_TECH_RFA, MSGR_MODULE_RF_GSM);
}



/*===========================================================================
  FUNCTION:  rfgsm_req_dispatch
===========================================================================*/
/*!
  @fn rfgsm_dispatch_outcome_t rfgsm_req_dispatch(rfgsm_msg_u* msg_ptr)
  @brief This function is the main dispatch function for all the REQs the RF
         task will receive. Once it receives a command, it basically dispatches
         it to the appropriate function again in this file. That function will
         basically send the command to the repective MC function and then once that 
         is executed, it will send back a the reponse. That function will also echo 
         the arguments of the original command.
  @param msg_ptr a pointer to the structure variable of the payload
  @return rfgsm_dispatch_outcome_t 
*/
/*=========================================================================*/
rfgsm_dispatch_outcome_t rfgsm_req_dispatch(rfgsm_msg_u* msg_ptr)
{
  rfgsm_dispatch_outcome_t req_cnf;
  msgr_hdr_s   *msgr_hdr_ptr = (msgr_hdr_s*) msg_ptr; 

  if (msg_ptr != NULL)
  {
    switch ( msgr_hdr_ptr->id )
    {
    case RFA_RF_GSM_START_GSM_REQ:
      req_cnf = rfgsm_dispatch_start_gsm_req((rfa_rf_gsm_start_gsm_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_START_GSM_CNF);
      break;
    case RFA_RF_GSM_STOP_GSM_REQ:
      req_cnf = rfgsm_dispatch_stop_gsm_req((rfa_rf_gsm_stop_gsm_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_STOP_GSM_CNF);
      break;
    case RFA_RF_GSM_RX_WAKEUP_REQ:
      req_cnf = rfgsm_dispatch_rx_wakeup_req((rfa_rf_gsm_rx_wakeup_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_RX_WAKEUP_CNF);
      break;
    case RFA_RF_GSM_TX_WAKEUP_REQ:
      req_cnf = rfgsm_dispatch_tx_wakeup_req((rfa_rf_gsm_tx_wakeup_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_TX_WAKEUP_CNF);
      break;
    case RFA_RF_GSM_RX_SLEEP_REQ:
      req_cnf = rfgsm_dispatch_rx_sleep_req((rfa_rf_gsm_rx_sleep_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_RX_SLEEP_CNF);
      break;
    case RFA_RF_GSM_TX_SLEEP_REQ:
      req_cnf = rfgsm_dispatch_tx_sleep_req((rfa_rf_gsm_tx_sleep_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_TX_SLEEP_CNF);
      break;
    case RFA_RF_GSM_RX_BURST_REQ:
      req_cnf = rfgsm_dispatch_rx_burst_req((rfa_rf_gsm_rx_burst_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_RX_BURST_CNF);
      break;
    case RFA_RF_GSM_SET_TX_BAND_REQ:
      req_cnf = rfgsm_dispatch_set_tx_band_req((rfa_rf_gsm_set_tx_band_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_SET_TX_BAND_CNF);
      break;
    case RFA_RF_GSM_TX_BURST_REQ:
      req_cnf = rfgsm_dispatch_tx_burst_req((rfa_rf_gsm_tx_burst_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_TX_BURST_CNF);
      break;
    case RFA_RF_GSM_IDLE_FRAME_PROCESSING_REQ:
      req_cnf = rfgsm_dispatch_idle_frame_processing_req((rfa_rf_gsm_idle_frame_processing_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_IDLE_FRAME_PROCESSING_CNF);
      break;
    case RFA_RF_GSM_SET_SAR_LIMIT_IND:
      req_cnf = rfgsm_dispatch_sar_limit_ind((rfa_rf_gsm_set_sar_limit_ind_s*) msg_ptr, RF_TASK_1);
      break;
	  	
    case RFA_RF_GSM_SET_VBATT_LIMIT_IND:
      req_cnf = rfgsm_dispatch_vbatt_limit_ind((rfa_rf_gsm_set_vbatt_limit_ind_s*) msg_ptr, RF_TASK_1);
      break;
	  
    case RFA_RF_GSM_CM_ENTER_REQ:
      req_cnf = rfgsm_dispatch_cm_enter_req( (rfa_rf_gsm_cm_enter_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_CM_ENTER_CNF);
      break;
    case RFA_RF_GSM_CM_EXIT_REQ:
      req_cnf = rfgsm_dispatch_cm_exit_req( (rfa_rf_gsm_cm_exit_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_CM_EXIT_CNF);
      break;
    case RFA_RF_GSM_UPDATE_TEMP_COMP_REQ:
      req_cnf = rfgsm_dispatch_update_temp_comp_req( (rfa_rf_gsm_update_temp_comp_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_UPDATE_TEMP_COMP_CNF);
      break;
    #ifdef FEATURE_RF_ASDIV
    case RFA_RF_GSM_SET_ANTENNA_REQ:
      req_cnf = rfgsm_dispatch_set_antenna_req( (rfa_rf_gsm_set_antenna_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_SET_ANTENNA_CNF);      
      break;
    #endif
    case RFA_RF_GSM_START_IP2_CAL_REQ:
      req_cnf = rfgsm_dispatch_build_ip2_cal_script((rfa_rf_gsm_start_ip2_cal_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_START_IP2_CAL_CNF);
      break;
    case RFA_RF_GSM_INIT_MSM_IP2_CAL_REQ:
      req_cnf = rfgsm_dispatch_init_msm_ip2_cal((rfa_rf_gsm_ip2_cal_msm_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_INIT_MSM_IP2_CAL_CNF);
      break;
    case RFA_RF_GSM_PROCESS_BURST_METRICS_REQ:
      req_cnf = rfgsm_dispatch_process_burst_metrics((rfa_rf_gsm_process_burst_metrics_req_s*) msg_ptr, RF_TASK_1, RFA_RF_GSM_PROCESS_BURST_METRICS_CNF);
      break;

    /* Deprecated APIs */
    case RFA_RF_GSM_TX_ENABLE_REQ:
    case RFA_RF_GSM_TX_DISABLE_REQ:
    case RFA_RF_GSM_WAKEUP_REQ:
    case RFA_RF_GSM_SLEEP_REQ:
    case RFA_RF_GSM_ENTER_MODE_REQ:
    case RFA_RF_GSM_EXIT_MODE_REQ:
      ERR_FATAL( "rfgsm_req_dispatch() deprecated API 0x%08x", msgr_hdr_ptr->id, 0, 0 );
      req_cnf = RFGSM_DISPATCH_UNSUCCESSFUL;
      break;

    default :
      MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, "Undefined GSM REQ 0x%08x for SUB1", msgr_hdr_ptr->id );
      req_cnf = RFGSM_DISPATCH_UNSUCCESSFUL;
      break;
    }
  } 
  else
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "Pointer NULL" );
    req_cnf = RFGSM_DISPATCH_NULL_PTR;
  }
  return(req_cnf);
}


/*===========================================================================*/
/*!
  @fn rfgsm_dispatch_outcome_t rfgsm_req_dispatch_2(rfgsm_msg_u* msg_ptr) for 
      task 2/subscriber2
  @brief This function is the main dispatch function for all the REQs the RF
         task will receive. Once it receives a command, it basically dispatches
         it to the appropriate function again in this file. That function will
         basically send the command to the repective MC function and then once that 
         is executed, it will send back a the reponse. That function will also echo 
         the arguments of the original command.
  @param msg_ptr a pointer to the structure variable of the payload
  @return rfgsm_dispatch_outcome_t 
*/
/*=========================================================================*/
rfgsm_dispatch_outcome_t rfgsm_req_dispatch_2(rfgsm_msg_u* msg_ptr)
{
  rfgsm_dispatch_outcome_t req_cnf;
  msgr_hdr_s   *msgr_hdr_ptr = (msgr_hdr_s*) msg_ptr; 

  if (msg_ptr != NULL)
  {
    switch ( msgr_hdr_ptr->id )
    {
    case RFA_RF_GSM_START_GSM_SUB2_REQ:
      req_cnf = rfgsm_dispatch_start_gsm_req((rfa_rf_gsm_start_gsm_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_START_GSM_SUB2_CNF);
      break;
    case RFA_RF_GSM_STOP_GSM_SUB2_REQ:
      req_cnf = rfgsm_dispatch_stop_gsm_req((rfa_rf_gsm_stop_gsm_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_STOP_GSM_SUB2_CNF);
      break;
    case RFA_RF_GSM_RX_WAKEUP_SUB2_REQ:
      req_cnf = rfgsm_dispatch_rx_wakeup_req((rfa_rf_gsm_rx_wakeup_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_RX_WAKEUP_SUB2_CNF);
      break;
    case RFA_RF_GSM_TX_WAKEUP_SUB2_REQ:
      req_cnf = rfgsm_dispatch_tx_wakeup_req((rfa_rf_gsm_tx_wakeup_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_TX_WAKEUP_SUB2_CNF);
      break;
    case RFA_RF_GSM_RX_SLEEP_SUB2_REQ:
      req_cnf = rfgsm_dispatch_rx_sleep_req((rfa_rf_gsm_rx_sleep_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_RX_SLEEP_SUB2_CNF);
      break;
    case RFA_RF_GSM_TX_SLEEP_SUB2_REQ:
      req_cnf = rfgsm_dispatch_tx_sleep_req((rfa_rf_gsm_tx_sleep_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_TX_SLEEP_SUB2_CNF);
      break;
    case RFA_RF_GSM_RX_BURST_SUB2_REQ:
      req_cnf = rfgsm_dispatch_rx_burst_req((rfa_rf_gsm_rx_burst_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_RX_BURST_SUB2_CNF);
      break;
    case RFA_RF_GSM_SET_TX_BAND_SUB2_REQ:
      req_cnf = rfgsm_dispatch_set_tx_band_req((rfa_rf_gsm_set_tx_band_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_SET_TX_BAND_SUB2_CNF);
      break;
    case RFA_RF_GSM_TX_BURST_SUB2_REQ:
      req_cnf = rfgsm_dispatch_tx_burst_req((rfa_rf_gsm_tx_burst_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_TX_BURST_SUB2_CNF);
      break;
    case RFA_RF_GSM_IDLE_FRAME_PROCESSING_SUB2_REQ:
      req_cnf = rfgsm_dispatch_idle_frame_processing_req((rfa_rf_gsm_idle_frame_processing_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_IDLE_FRAME_PROCESSING_SUB2_CNF);
      break;
    case RFA_RF_GSM_SET_SAR_LIMIT_SUB2_IND:
      req_cnf = rfgsm_dispatch_sar_limit_ind((rfa_rf_gsm_set_sar_limit_ind_s*) msg_ptr, RF_TASK_2 );
      break;

    case RFA_RF_GSM_SET_VBATT_LIMIT_SUB2_IND:
      req_cnf = rfgsm_dispatch_vbatt_limit_ind((rfa_rf_gsm_set_vbatt_limit_ind_s*) msg_ptr, RF_TASK_2 );
      break;

    case RFA_RF_GSM_CM_ENTER_SUB2_REQ:
      req_cnf = rfgsm_dispatch_cm_enter_req( (rfa_rf_gsm_cm_enter_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_CM_ENTER_SUB2_CNF);
      break;
    case RFA_RF_GSM_CM_EXIT_SUB2_REQ:
      req_cnf = rfgsm_dispatch_cm_exit_req( (rfa_rf_gsm_cm_exit_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_CM_EXIT_SUB2_CNF);
      break;
    case RFA_RF_GSM_UPDATE_TEMP_COMP_SUB2_REQ:
      req_cnf = rfgsm_dispatch_update_temp_comp_req( (rfa_rf_gsm_update_temp_comp_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_UPDATE_TEMP_COMP_SUB2_CNF);
      break;
    #ifdef FEATURE_RF_ASDIV
    case RFA_RF_GSM_SET_ANTENNA_SUB2_REQ:
      req_cnf = rfgsm_dispatch_set_antenna_req( (rfa_rf_gsm_set_antenna_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_SET_ANTENNA_SUB2_CNF);      
      break;
    #endif
    case RFA_RF_GSM_START_IP2_CAL_SUB2_REQ:
      req_cnf = RFGSM_DISPATCH_SUCCESSFUL;
      break;
    case RFA_RF_GSM_PROCESS_BURST_METRICS_SUB2_REQ:
      req_cnf = rfgsm_dispatch_process_burst_metrics((rfa_rf_gsm_process_burst_metrics_req_s*) msg_ptr, RF_TASK_2, RFA_RF_GSM_PROCESS_BURST_METRICS_SUB2_CNF);
      break;

    /* Deprecated APIs */
    case RFA_RF_GSM_TX_ENABLE_SUB2_REQ:
    case RFA_RF_GSM_TX_DISABLE_SUB2_REQ:
    case RFA_RF_GSM_WAKEUP_SUB2_REQ:
    case RFA_RF_GSM_SLEEP_SUB2_REQ:
    case RFA_RF_GSM_ENTER_MODE_SUB2_REQ:
    case RFA_RF_GSM_EXIT_MODE_SUB2_REQ:
      ERR_FATAL( "rfgsm_req_dispatch_2() deprecated API 0x%08x", msgr_hdr_ptr->id, 0, 0 );
      req_cnf = RFGSM_DISPATCH_UNSUCCESSFUL;
      break;

    default:
      MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, "Undefined GSM REQ 0x%08x for SUB2", msgr_hdr_ptr->id );
      req_cnf = RFGSM_DISPATCH_UNSUCCESSFUL;
      break;
    }
  } 
  else
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "Pointer NULL" );
    req_cnf = RFGSM_DISPATCH_NULL_PTR;
  }
  return(req_cnf);
}



/*===========================================================================
  FUNCTION:  rfgsm_rflm_rsp_dispatch
===========================================================================*/
/*!
  @fn void rfgsm_rflm_rsp_dispatch(rf_task_num_type task_num)
  @brief This function is the main dispatch function for all the RSPs the RF
         task will receive from RFLM TECH. Once it receives a command, it 
         dispatches it to the appropriate function in this file. That function 
         then sends the command to the respective MC function and then once
         that is executed, it returns a msg to free the msr router q
  @param task_num RF Task the message was received on
  @return void 
*/

void rfgsm_rflm_rsp_dispatch(rf_task_num_type task_num)
{
  rfgsm_rflm_msgr_t*  cmd_ptr  = NULL;
  rfgsm_dispatch_outcome_t result = RFGSM_DISPATCH_SUCCESSFUL;

  ASSERT(task_num < RF_MAX_TASK);

  /* get messages from the MSGR queue*/
  while ((cmd_ptr = (rfgsm_rflm_msgr_t *) q_get(&rfgsm_rflm_cmd_intf_msgr_q[task_num])) != NULL)
  {    
    switch (cmd_ptr->msg.hdr.id )
    {
    case RFLM_GSM_THERM_READ_RSP:
#ifdef FEATURE_DUAL_SIM
    case RFLM_GSM_THERM_READ_SUB2_RSP:
#endif
      result = rfgsm_dispatch_therm_read_rsp(&(cmd_ptr->msg), task_num);
     break;
#ifdef FEATURE_RF_ASDIV
    case RFLM_GSM_ASD_RSP:
#ifdef FEATURE_DUAL_SIM
    case RFLM_GSM_ASD_SUB2_RSP:
#endif
      result = rfgsm_dispatch_asd_read_rsp(&(cmd_ptr->msg), task_num);
    break;
#endif
    case RFLM_GSM_DEVICE_STATUS_READ_RSP:
#ifdef FEATURE_DUAL_SIM
    case RFLM_GSM_DEVICE_STATUS_READ_SUB2_RSP:
#endif
      result = rfgsm_dispatch_device_status_read_rsp(&(cmd_ptr->msg), task_num);
    break;

    default :
      MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "Undefined RFLM REQ" );
      result = RFGSM_DISPATCH_SUCCESSFUL;
      break;
    }
   /* Indicate error if RF Task confirmation message does not indicate success */
    if(result != RFGSM_DISPATCH_SUCCESSFUL)
    {  
      MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, "RFGSM RFLM Response Dispatch failed: %d", result );
    }

    /* Return message to the free queue */
    q_put( &rfgsm_rflm_cmd_intf_msgr_free_q[task_num], &cmd_ptr->link);
  }
  return;
}

/*===========================================================================*/
/*!
  @fn void rfgsm_initialise_rflm_msg_router_q( void )
  @brief This function initialises the msg router q to handle all the RSPs the RF
         task will receive from RFLM. A new signal is registered which will be set
         whenever there is a response cmd from RFLM
  @param task_num RF Task the message was received on
  @return void
*/
/*=========================================================================*/
void rfgsm_initialise_rflm_msg_router_q( rf_task_num_type task_num )
{
   errno_enum_type  msgr_error;
   msgr_client_t    msgr_client;
   msgr_id_t        msgr_id;

   uint32  i = 0;

   ASSERT(task_num < RF_MAX_TASK);

   (void) q_init( &rfgsm_rflm_cmd_intf_msgr_q[task_num] );
   (void) q_init( &rfgsm_rflm_cmd_intf_msgr_free_q[task_num] );

   i = 0;

   while( i < RFGSM_RFLM_MSGR_QUEUE_SIZE )
   {
     msgr_hdr_struct_type*  tmp_ptr   = (msgr_hdr_struct_type *) &(rfgsm_rflm_msgr[task_num][i].msg.hdr);
     q_link_type*           link_item_ptr = q_link( tmp_ptr, &(rfgsm_rflm_msgr[task_num][i].link) );

     q_put( &rfgsm_rflm_cmd_intf_msgr_free_q[task_num], link_item_ptr );

     i++;
   }

   /* Register the queue and free queue with the message router */
   msgr_error = msgr_client_create(&msgr_client);

   ASSERT(msgr_error == E_SUCCESS);

   msgr_error = msgr_client_add_rex_q(
                                      &msgr_client,                              //!< Message Client Handle
                                      rex_self(),                                //!< REX TCB pointer
                                      RF_RFLM_CMD_Q_SIG,                          //!< REX TCB pointer
                                      &rfgsm_rflm_cmd_intf_msgr_q[task_num],      //!< queue to place the msg
                                      &rfgsm_rflm_cmd_intf_msgr_free_q[task_num], //!< queue to get empty buffer
                                      offsetof(rfgsm_rflm_msgr_t, msg),           //!< offset to msgr_hdr
                                      MSGR_NO_OFFSET,                            //!< offset to legacy cmd_type
                                      sizeof(rfgsm_rflm_msgr_t),                  //!< max size of msgs
                                      &msgr_id                                   //!< queue identifier
                                     );

   ASSERT(msgr_error == E_SUCCESS);
      
   if ( task_num == RF_TASK_1 )
   {
     msgr_error = msgr_register_block(MSGR_RFLM_GSM, &msgr_client, msgr_id, rfgsm_rflm_rsp_umids, 
                                      (RF_GSM_RFLM_RSP_CNT(rfgsm_rflm_rsp_umids)) );
   }
      
   if ( task_num == RF_TASK_2 )
   {
     msgr_error = msgr_register_block(MSGR_RFLM_GSM, &msgr_client, msgr_id, rfgsm_rflm_rsp_sub2_umids,
                                      (RF_GSM_RFLM_RSP_CNT(rfgsm_rflm_rsp_sub2_umids)) );
   } 

   ASSERT(msgr_error == E_SUCCESS);
}
