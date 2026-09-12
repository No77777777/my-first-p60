/*===========================================================================

               M E A S    S P E C I F I C   T E S T  F U N C T I O N S

DESCRIPTION
   This file contains implementations for MEAS specific test functions

   Copyright (c) 2009 - 2013 by Qualcomm Technologies Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


  $Header: //components/rel/rfa.mpss/4.11/rf/meas/ftm/src/ftm_meas.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/20/15   wwl     Add support for W/T 2 1X QTA
05/20/15   nv      Added changes to get RXAGC value during FTM IRAT testing
05/13/15   nv      Adding exit_meas support for Half scripts
05/01/15   rmb     Porting latest changes from dev branch into mainline.
04/29/15   gvn     Correct the device id of div3
04/28/15   nv      Mainline the code under feature RF_HAS_MEAS_INTF_TH_2_0
04/23/15   rmb     Call handle based cleanup of WCDMA mdsp data before handle de-allocation.
04/17/15   nv      Added changes for Half scripts and NULL2X scripts
04/08/15   rmb     Put ftm_meas.h outside feature flag so that flag definition can be imported.
04/08/14   rmb     Merge Tips of Dev branch ont mainline.
04/07/15   rmb     Remove reference to unused structure.
04/07/15   nv      Merging the changes from dev branch on to Mailine
04/06/15   rmb     Merge back meas interface changes onto Mainline with feature flag
03/09/15   zhw     Initialize param v2 after defined in FTM iRAT.
02/26/15   zhw     Hook up existing FTM iRAT test functionality with param v2
08/05/14   aw      fixed device_id typo
04/29/14   ac      nbr support
04/18/14   rmb     Change ENUM type for Core AGC to MDSP AGC.
02/27/14   rmb     First phase FTM IRAT checkin for G2W.
02/13/14   dw      [1]Change AGC command function to support multiple device
                   [2]Code cleanup
12/30/13   pl      put FTM IRAT tuning within a flag
12/13/13   rmb     Fix Compiler Error.
12/12/13   rmb     Add FTM W2W IRAT support for Bolt.
12/11/13   pl      RE-enable exit_tech and enter_tech in xtoy_test_y and
                   x_to_y_test_x respectively
12/04/13   pl      Create handle for WCDMA also
12/04/13   pl      Populate Handle meta properly
11/26/13   nrk     Fixed compiler warnings
08/13/13   pl      Cleanup unused interfaces
08/13/13   pl      Added interface to log and retrieve IRAT script info
05/09/13   aa      updated W2W FTM IRAT for new FTM framework
05/08/13   pl      Fix RXLM allocation when aborting
04/23/13   gvn     Added Abort Support
02/06/13   pl      Don't allocate RXLM buffer for WCDMA, since WCDMA allocates for itself
02/04/13   pl      update to common IRAT header
01/22/13   pl      Implement code review comments
01/20/13   pl      FMT IRAT cleanup
01/20/13   pl      FTM LTE-CA IRAT support
11/08/12   pl      Consolidate function pointer table
10/31/12   pl      adding MCPM meas support into FTM IRAT
10/24/12   pl      remove obsolete functions
10/24/12   pl      Update ftm framework interface to clarify usage
10/22/12   pl      Update mdsp interface to become re-enterant
10/19/12   pl      fix bug on passing RXLM buffers
10/17/12   pl      enable execution of RXLM buffers
10/17/12   ka      Added support for RFFE writes to W2W test
10/16/12   ka      Fixed data type and warnings
10/16/12   ka      Added W2W FTM test
10/13/12   pl      Enable using RFSW to execute IRAT buffers
10/09/12   pl      FTM IRAT framework revamp
10/09/12   pl      FTM IRAT cleanup
10/05/12   aa      Remove  obsolete wtow measurement
10/02/12   pl      Remove obsolete functions
10/01/12   sbm     Remove  rfd_grfc_engine_to_grfc_out() usage.
10/01/12   sb      Made changes to add rfm device to rf device interface and uppler layer APIs
09/15/12   lp      Resolved compilation errors with FEATURE_LTE disabled
08/09/12   php     Add support for new FW CA API
08/08/12   kb      Updated access to FW shared memory location to work with new FW interface.
07/31/12   qzh     Rfdevice interface change to use rfm_device instead of path
07/29/12   jfc     Include "rfd_legacy_inc" since code still references RFD
07/29/12   jfc     Fix error in merge
07/20/12   sr      Made changes to pass the RFM_DEVICE and rf_buffer to rfc_gsm
07/02/12   kb      updated API functions to support script buffers
06/21/12   pl      Integrate with scripting device interface
06/20/12   shb     Updated GSM device APIs to accept script buffer
06/17/12   pl      update to use CA supported device interface
09/05/12   aro     Added RF-FW Sync callback parameter in HDR MDSP Interface
09/05/12   aro     Added RF-FW Sync callback parameter in MDSP Interface
05/14/12   sar     Removed rtr8600 device.
04/19/12   jfc     Remove unused RFD includes
04/18/12   zg      Fixed a GSM crash in TDS->GSM iRAT.
04/03/12   aro     callback function for HDR MDSP Rx interfaces
04/02/12   aro     Added CB as argument in Rx / Tx MDSP interfaces
04/02/12   aro     Added CB as argument in Rx MDSP interfaces
03/17/12   jfc     Removed clkrgm_modem.h inclusion as it has been deprecated
02/16/12   zg/jyu  Added FTM MEAS support for TDS<->GSM, TDD LTE->TDS for both
                   tune-away and tune-back (RF execute GRFC/SSBI script)
02/15/12   sar     Mainlined RF_GRFC_COMMON_DESIGN_INTERIM_FEATURE.
02/03/12   av      IRAT cleanup phase 1
12/05/11   sar     Fixed featurization for non-lte build.
11/17/11   aro     Enabled voting for CDMA FW State config and MCPM config
11/17/11   aro     Added new MCPM configure function with voting mechanism
11/17/11   aro     Replaced the FW state config function with new FTM func
11/16/11   swb     Remove additional deprecated clk_regime function calls
11/10/11   aro     Upadated FW state config interface to pass semaphore to
                   wait on for blocking calls
10/21/11   swb     Remove deprecated clk_regime function calls
09/26/11   dw      Klocwork fixes
08/25/11   whc     Remove all references to legacy RFC structure and related API
08/18/11   Saul    IRAT updates/cleanup.
08/03/11   Saul    IRAT update. Now configuring flags so that FW writes scripts.
07/25/11   Saul    IRAT updates.
                   - Now sending pilot_meas_cfg message.
                   - GRFC start delta sent in cx8 units.
                   - Override of debug flags done in FTM meas APIs.
07/23/11   Saul    Filled-in L->1x/do test functionality.
07/19/11   whc     Replace IRAT GRFC mask APIs with GRFC script APIs
07/12/11   vb      rfc cleanup and rfc interface changes to suit the new auto-gen
                   of rf card files
07/06/11   tnt     Call new function for GRFC mask act/deact
07/05/11   vrb     Replacing direct RTR function call with dev intf function
07/01/11   vrb     Replacing direct calls to rfgsm_ds with wrapper functions
05/10/11   sar     Remove hdrsrch.h for rfa/api cleanup.
04/20/11   pl      LTE FTM refactoring to remove global variables
04/06/11   pl      Fixed Compiler warnings
04/05/11   tnt     Fix compiler error
04/05/11   dw      GtoW/LtoW RxlM support
04/04/11   bmg     Removed direct coupling with RTR8600
04/01/10   dw      Fix WCDMA band type mismatch in tune device API
04/01/11   dw      Fix warnings
03/24/11   pl      replace clk configuration with MCPM
03/21/11   pl      fix warnings
03/24/11   vrb     Compiler Warning Fixes
03/24/11   tnt     L2W, L2DO Irat integration
03/17/11   tnt     Replace INO with mempool IQ on Nikel
03/12/11   zg      Wrapped hdrsrch.h with FEATURE_HDR.
03/11/11   zg      Compiler warning fix.
03/07/11   jyu     Added Level 1&2 RF FTM IRAT test
03/07/11   tnt     Integrate with new LM and RFC arch
03/03/11   pl      fix compiler error
03/01/11   zg      Cleaned up bbrx hack from ftm_meas_ltodo_test_do().
02/28/11   zg      Initial L->DO irat FTM support
02/24/11   tnt     Merge to MDM9K PLF110
02/16/11   jyu     Added FTM L2L Interfreq IRAT test support
02/10/11   jyu     Added LTE INO support to IRAT FTM
02/07/11   cri     Featurize RFC dependent definitions
02/07/11   av      FTM IRAT support. Corrected some issues and add support for returning
                   script sizes.
02/03/11   bmg     Removed hard-coded GRFC configuration that isn't modem
                   independent.
01/27/11   dw      Cleanup legacy WCDMA RFC interface call
01/25/11   pl      Featurize Genesis Specific HWIO code
01/25/11   pl      Replace deprecated clk.h with DALSys.h
01/05/11   pl      remove compiler warnings
01/03/11   pl      integrate with RFC LTE class
11/09/10   lcl     Remove compiler warning
10/20/10   jyu     Fix compile warning
10/20/10   jyu     Update interface per FW change for rxagc_start_lna_state
10/06/10   jyu     Added new APIs to support IRAT FTM test
10/01/10   kguo    Support integration with fw in FTM for L2W IRAT test
10/19/10   vb      All device functions are now accessed thru device interface functions
09/28/10   kguo    Cleaned up buf_index_cleanup and buf_index_startup in xtol
                   and ltow test to be consistent with new data structure
09/22/10   tnt     Remove hardcoded GRFC setting, setting are readding from RFC files
09/20/10   tnt     Fix compiler error in ftm_meas_xtol_test_lte()
09/15/10   pl/jyu  W2L FTM FW integration with proper sequence
09/10/10   nsree   Included "ftm_gsm_rfprot.h" and "rfgsm_core_util.h" to avoid
           compiler warnings involved with IRAT. Added default enumeration case,
           and fixed incompatible pointer type warning.
09/14/10   av      LtoG support
08/26/10   kguo    Use feature FEATURE_LTOX_OPTIMIZED_IRAT for L2W test to avoid
                   compiler error in HB/HC build
08/24/10   kguo    Updated Ltow test
                   Mainline feature FEATURE_LTOX_OPTIMIZED_IRAT
08/23/10   pl      FW integration for IRAT xTOL testcase
08/18/10   pl      Added script-based IRAT functions
08/10/10   pl      Adding support for Stop scripts
08/10/10   pl      Remove non-ship header file header file
08/10/10   pl      Remove unused header file
08/09/10   kguo    Remove unused header file and updated ltoW test with new data structure
08/04/10   ka      Remove warnings
07/29/10   kguo    Featured ltow test with FEATURE_LTOX_OPTIMIZED_IRAT.
07/28/10   kguo    Added new test features for ltow test
07/27/10   pl      Remove warning
07/27/10   pl      Added test for x2DO IRAT
07/27/10   pl      Added test for x2L IRAT
06/29/10   pl      Use MDSP layer to abstract FW from RF IRAT module
06/22/10   kguo    Changed mwmory interface with L1 from pointer to index
06/10/10   kguo    Added test for LtoW
06/08/10   lcl     Added G2W and W2G support.
03/24/10   ka      Restore wtow
02/25/10   ka      Temporarily remove wtow
01/06/10   sar     Renamed ftm_task.h to rf_test_task_v.h.
12/17/09   sr      removed the rtr8600_gsm.h include file.
11/25/09   ntn     code cleanup
12/02/09   ckl     Added W2W test function and removed warning.
10/26/09   ac      added path for tune_to_chan_fp
10/21/09   sr      new device driver interface changes.
09/15/09   jps     Initial version

=============================================================================*/
#include "ftmicap.h"

#ifdef FTM_HAS_IRAT
#include "rfm_mode_types.h"
#include "ftm_meas.h"
#include "rxlm_intf.h"
#include "rf_hal_sbi.h"
#include "rf_hal_rffe.h"
#include "rfm_meas.h"
#include "rfmeas_ftm_mdsp.h"
#include "mcpm_api.h"
#include "rflm_time.h"
#include "rflm_dm_api.h"
#include "rfm_wcdma.h"

/*============================================================================
             LOCAL/EXTERNAL VARIABLES
=============================================================================*/

typedef enum
{
 HALF_SCRIPT_NBR_X =0,
 HALF_SCRIPT_NBR_Y,
 HALF_SCRIPT_NBR_MAX
}ftm_meas_half_script_neighbour_index_enum_type;


typedef struct
{
  ftm_meas_test_level_enum_type test_level;
  lm_handle_type           tgt_rxlm_ant0_idx;
  lm_handle_type           tgt_rxlm_ant1_idx;
  ftm_meas_func_tbl_type   *func_tbl_ptr[RFM_NUM_MODES];
  lm_handle_type           half_script_nbr_rxlm_ant0_idx[HALF_SCRIPT_NBR_MAX];
  lm_handle_type           half_script_nbr_rxlm_ant1_idx[HALF_SCRIPT_NBR_MAX];
} ftm_meas_state_type;
ftm_meas_state_type ftm_meas_state;

typedef struct
{
  uint16 num_trx;
  uint16 num_trx_task;
  uint16 num_non_trx;
  uint16 num_non_trx_task;
  uint16 execute_time_us;
} ftm_meas_irat_script_info_type;
ftm_meas_irat_script_info_type ftm_meas_irat_info[2];

void ftm_meas_clock_setup(rfm_meas_generic_params_type *test_param_ptr, boolean tune_away_flag);
void ftm_meas_allocate_lm_buffer(rfm_meas_generic_params_type *test_param_ptr);
void ftm_meas_deallocate_lm_buffer(rfm_meas_generic_params_type *test_param_ptr);
void ftm_meas_allocate_lm_buffer_for_half_script(rfm_meas_generic_params_type *test_param_ptr,ftm_meas_half_script_neighbour_index_enum_type nbr_index);
void ftm_meas_deallocate_lm_buffer_for_half_script(rfm_meas_generic_params_type *test_param_ptr,ftm_meas_half_script_neighbour_index_enum_type nbr_index);

boolean ftm_irat_tune = TRUE;

/*===========================================================================
             FUNCTION IMPLEMENTATIONS
===========================================================================*/

/*===========================================================================

  FUNCTION: ftm_meas_clock_setup

===========================================================================*/
/*!
    @brief
    This function is called to setup MCPM clock and power mgmt state for
    IRAT measurement

    @return
    None.
*/
/*=========================================================================*/
void ftm_meas_clock_setup(rfm_meas_generic_params_type *test_param_ptr, boolean tune_away_flag)
{
  boolean update_ok_flag = FALSE;
  mcpm_request_type mcpm_req = MCPM_1X_PARMS_UPDATE_REQ;
  mcpm_request_parms_type mcpm_param;

  if(test_param_ptr == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "Null input pointer");
    return;
  }

  /* check which srouce tech we need to update */
  switch(test_param_ptr->src_tech)
  {
     case RFCOM_1X_MODE:
       mcpm_req = MCPM_1X_PARMS_UPDATE_REQ;
       break;
     case RFCOM_1XEVDO_MODE:
       mcpm_req = MCPM_DO_PARMS_UPDATE_REQ;
       break;
     case RFCOM_GSM_MODE:
       mcpm_req = MCPM_GERAN_PARMS_UPDATE_REQ;
       break;
     case RFCOM_WCDMA_MODE:
       mcpm_req = MCPM_WCDMA_PARMS_UPDATE_REQ;
       break;
     case RFCOM_LTE_MODE:
       mcpm_req = MCPM_LTE_PARMS_UPDATE_REQ;
       break;
     case RFCOM_TDSCDMA_MODE:
       mcpm_req = MCPM_TDSCDMA_PARMS_UPDATE_REQ;
       break;
     default:
       MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "Unknow src_tech %d",test_param_ptr->src_tech);
       return;
  }

  /* Now check which neighbor we are searching for */
  mcpm_param.update_info = MCPM_NEIGHBOR_MEAS_UPDATE;
  switch(mcpm_req)
  {
     case MCPM_1X_PARMS_UPDATE_REQ:
       if(test_param_ptr->tgt_tech == RFCOM_LTE_MODE)
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_1X2L_MEAS_START : MCPM_1X2L_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else
       {
         MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,
               "Unsupport IRAT pair, src_tech %d tgt_tech %d",
               test_param_ptr->src_tech, test_param_ptr->tgt_tech);
         // do nothing
       }
       break;

     case MCPM_DO_PARMS_UPDATE_REQ:
       if(test_param_ptr->tgt_tech == RFCOM_LTE_MODE)
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_DO2L_MEAS_START : MCPM_DO2L_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else
       {
         MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,
               "Unsupport IRAT pair, src_tech %d tgt_tech %d",
               test_param_ptr->src_tech, test_param_ptr->tgt_tech);
         // do nothing
       }
       break;

     case MCPM_GERAN_PARMS_UPDATE_REQ:
       if(test_param_ptr->tgt_tech == RFCOM_LTE_MODE)
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_G2L_MEAS_START : MCPM_G2L_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else if (test_param_ptr->tgt_tech == RFCOM_WCDMA_MODE)
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_G2W_MEAS_START : MCPM_G2W_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else if (test_param_ptr->tgt_tech == RFCOM_TDSCDMA_MODE)
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_G2TDS_MEAS_START : MCPM_G2TDS_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else
       {
         MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,
               "Unsupport IRAT pair, src_tech %d tgt_tech %d",
               test_param_ptr->src_tech, test_param_ptr->tgt_tech);
         // do nothing
       }
       break;

     case MCPM_WCDMA_PARMS_UPDATE_REQ:
       if(test_param_ptr->tgt_tech == RFCOM_LTE_MODE)
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_W2L_MEAS_START : MCPM_W2L_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else if (test_param_ptr->tgt_tech == RFCOM_GSM_MODE)
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_W2G_MEAS_START : MCPM_W2G_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else if (test_param_ptr->tgt_tech == RFCOM_WCDMA_MODE)
       {
         // Nothing to do if it is W2W
       }
       else if((test_param_ptr->tgt_tech == RFCOM_1XEVDO_MODE) ||
               (test_param_ptr->tgt_tech == RFCOM_1X_MODE))
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_W21XDO_MEAS_START: MCPM_W21XDO_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else
       {
         MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,
               "Unsupport IRAT pair, src_tech %d tgt_tech %d",
               test_param_ptr->src_tech, test_param_ptr->tgt_tech);
         // do nothing
       }
       break;

     case MCPM_LTE_PARMS_UPDATE_REQ:
       if(test_param_ptr->tgt_tech == RFCOM_LTE_MODE)
       {
         // Nothing to do if it is L2L
       }
       else if (test_param_ptr->tgt_tech == RFCOM_GSM_MODE)
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_L2G_MEAS_START : MCPM_L2G_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else if (test_param_ptr->tgt_tech == RFCOM_WCDMA_MODE)
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_L2W_MEAS_START : MCPM_L2W_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else if((test_param_ptr->tgt_tech == RFCOM_1XEVDO_MODE) ||
               (test_param_ptr->tgt_tech == RFCOM_1X_MODE))
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_L21XDO_MEAS_START : MCPM_L21XDO_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else
       {
         MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,
               "Unsupport IRAT pair, src_tech %d tgt_tech %d",
               test_param_ptr->src_tech, test_param_ptr->tgt_tech);
         // do nothing
       }
       break;

     case MCPM_TDSCDMA_PARMS_UPDATE_REQ:
       if(test_param_ptr->tgt_tech == RFCOM_GSM_MODE)
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_TDSCDMA2G_MEAS_START : MCPM_TDSCDMA2G_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else if (test_param_ptr->tgt_tech == RFCOM_LTE_MODE)
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_TDSCDMA2L_MEAS_START : MCPM_TDSCDMA2L_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else if((test_param_ptr->tgt_tech == RFCOM_1XEVDO_MODE) ||
               (test_param_ptr->tgt_tech == RFCOM_1X_MODE))
       {
         mcpm_param.neighbor_meas = (tune_away_flag == TRUE)? MCPM_TDSCDMA21XDO_MEAS_START: MCPM_TDSCDMA21XDO_MEAS_STOP;
         update_ok_flag = TRUE;
       }
       else
       {
         MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,
               "Unsupport IRAT pair, src_tech %d tgt_tech %d",
               test_param_ptr->src_tech, test_param_ptr->tgt_tech);
         // do nothing
       }
       break;

     default:
       MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "Unknown mcpm update %d", mcpm_req);
       break;
  }

  if(update_ok_flag == TRUE)
  {
    MCPM_Config_Modem(mcpm_req, &mcpm_param);
  }
}

/*===========================================================================

  FUNCTION: ftm_meas_allocate_lm_buffer

===========================================================================*/
/*!
    @brief
    This function is called to allocate LM buffer for IRAT testing in FTM mode.
    It should be called at the very beginning of IRAT testing.

    @return
    None.
*/
/*=========================================================================*/
void ftm_meas_allocate_lm_buffer(rfm_meas_generic_params_type *test_param_ptr)
{
  lm_status_type lm_status = LM_SUCCESS;
  lm_tech_type   lm_tech = LM_TECH_MAX;
  rflm_dm_handle_meta_s handle_meta;
  boolean dm_status = TRUE;

  if (test_param_ptr == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_FATAL,"NULL pointer in ftm_meas_allocate_lm_buffer()");
    return ;
  }

  if(test_param_ptr->tgt_tech >= RFM_NUM_MODES)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"Invalid target tech %d", test_param_ptr->tgt_tech);
    return ;
  }

  /* Set Target RXLM buffer first */
  switch(test_param_ptr->tgt_tech)
  {
    case RFCOM_1X_MODE:
      lm_tech = LM_1X;
      break;
    case RFCOM_GSM_MODE:
      lm_tech = LM_GSM;
      break;
    case RFCOM_1XEVDO_MODE:
      lm_tech = LM_HDR;
      break;
    case RFCOM_LTE_MODE:
      lm_tech = LM_LTE;
      break;
    case RFCOM_TDSCDMA_MODE:
      lm_tech = LM_TDSCDMA;
      break;
  case RFCOM_WCDMA_MODE:
      lm_tech = LM_UMTS;
      break;
    default:
      MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"Target Mode %d FTM IRAT doesn't support",
            test_param_ptr->tgt_tech );
      break;
  }
  if(lm_tech < LM_TECH_MAX)
  {
    if (ftm_meas_state.tgt_rxlm_ant0_idx < RXLM_MAX_BUFFERS)
    {
      /* Already allocated, no need to allocate again */
      lm_status = LM_SUCCESS;
      test_param_ptr->tgt_prx_rxlm_buf_idx = ftm_meas_state.tgt_rxlm_ant0_idx;
    }
    else
    {
      lm_status = rxlm_allocate_buffer(RXLM_CHAIN_0,lm_tech,&(test_param_ptr->tgt_prx_rxlm_buf_idx));
    }
    if (lm_status != LM_SUCCESS)
    {
      MSG_2( MSG_SSID_RF, MSG_LEGACY_FATAL,"Cannot allocate tech %d tune-away RXLM buffer for Ant- status %d",lm_tech,lm_status);
    }
    else
    {
      ftm_meas_state.tgt_rxlm_ant0_idx = test_param_ptr->tgt_prx_rxlm_buf_idx;
      dm_status = rflm_dm_get_handle_info(test_param_ptr->tgt_prx_rxlm_buf_idx, &handle_meta);
      if (dm_status == TRUE)
      {
//        handle_meta.type = RFLM_IRAT_HANDLE;
        handle_meta.device_id = test_param_ptr->tgt_device;
        (void)rflm_dm_set_handle_info(test_param_ptr->tgt_prx_rxlm_buf_idx, &handle_meta);
      }
    }
    if (test_param_ptr->tgt_div_en == 1)
    {
      if (ftm_meas_state.tgt_rxlm_ant1_idx < RXLM_MAX_BUFFERS)
      {
        /* Already allocated, no need to allocate again */
        lm_status = LM_SUCCESS;
        test_param_ptr->tgt_drx_rxlm_buf_idx = ftm_meas_state.tgt_rxlm_ant1_idx;
      }
      else
      {
        lm_status = rxlm_allocate_buffer(RXLM_CHAIN_1,lm_tech,&(test_param_ptr->tgt_drx_rxlm_buf_idx));
      }
      if (lm_status != LM_SUCCESS)
      {
        MSG_2( MSG_SSID_RF, MSG_LEGACY_FATAL,"Cannot allocate tech %d tune-away RXLM buffer for Ant-1 status %d",lm_tech,lm_status);
      }
      else
      {
        ftm_meas_state.tgt_rxlm_ant1_idx = test_param_ptr->tgt_drx_rxlm_buf_idx;
        dm_status = rflm_dm_get_handle_info(test_param_ptr->tgt_drx_rxlm_buf_idx, &handle_meta);
        if (dm_status == TRUE)
        {
//          handle_meta.type = RFLM_IRAT_HANDLE;
          handle_meta.device_id = test_param_ptr->tgt_div_device;
          (void)rflm_dm_set_handle_info(test_param_ptr->tgt_drx_rxlm_buf_idx, &handle_meta);
        }
      }
    }
    else
    {
      test_param_ptr->tgt_drx_rxlm_buf_idx = RXLM_MAX_BUFFERS;
    }
  }
}



/*===========================================================================

  FUNCTION: ftm_meas_deallocate_lm_buffer

===========================================================================*/
/*!
    @brief
    This function is called to deallocate LM buffer for IRAT testing in FTM mode.

    @return
    None.
*/
/*=========================================================================*/
void ftm_meas_deallocate_lm_buffer(rfm_meas_generic_params_type *test_param_ptr)
{
  lm_status_type lm_status;
  lm_tech_type   lm_tech = LM_TECH_MAX;
  rfm_api_status_t wcdma_cleanup_status = RFM_WCDMA_ERROR_FAILURE;

  if (test_param_ptr == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_FATAL,"NULL pointer in ftm_meas_allocate_lm_buffer()");
    return ;
  }

  if(test_param_ptr->tgt_tech >= RFM_NUM_MODES)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"Invalid target tech %d", test_param_ptr->tgt_tech);
    return ;
  }
  /* Set Target RXLM buffer first */
  switch(test_param_ptr->tgt_tech)
  {
    case RFCOM_1X_MODE:
      lm_tech = LM_1X;
      break;
    case RFCOM_GSM_MODE:
      lm_tech = LM_GSM;
      break;
    case RFCOM_1XEVDO_MODE:
      lm_tech = LM_HDR;
      break;
    case RFCOM_WCDMA_MODE:
      lm_tech = LM_UMTS;
      break;
    case RFCOM_LTE_MODE:
      lm_tech = LM_LTE;
      break;
    case RFCOM_TDSCDMA_MODE:
      lm_tech = LM_TDSCDMA;
      break;
    default:
      MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"Target Mode %d FTM IRAT doesn't support",
            test_param_ptr->tgt_tech );
      break;
  }
  if(lm_tech < LM_TECH_MAX)
  {
    /* Call RFWCDMA MDSP data cleanup before de-allocate handle */
    if (lm_tech == LM_UMTS)
    {
      wcdma_cleanup_status = rfm_wcdma_handle_cleanup(test_param_ptr->tgt_prx_rxlm_buf_idx);

      if (wcdma_cleanup_status!=RFM_WCDMA_HEALTHY_STATE)
      {
        RF_MSG_1(RF_ERROR,  "ftm_meas_deallocate_lm_buffer: Handle cleanup failed for RFLM handle %d", test_param_ptr->tgt_prx_rxlm_buf_idx);
      }
    } /* if (lm_tech == LM_UMTS) */

    lm_status = rxlm_deallocate_buffer(test_param_ptr->tgt_prx_rxlm_buf_idx);
    if (lm_status != LM_SUCCESS)
    {
      MSG_2( MSG_SSID_RF, MSG_LEGACY_FATAL,"Cannot deallocate tech %d tune-away RXLM buffer for Ant-0 status %d",lm_tech,lm_status);
    }
    else
    {
      test_param_ptr->tgt_prx_rxlm_buf_idx = RXLM_MAX_BUFFERS;
      ftm_meas_state.tgt_rxlm_ant0_idx = RXLM_MAX_BUFFERS;
    }
    if (test_param_ptr->tgt_div_en == 1)
    {
      /* Call RFWCDMA MDSP data cleanup before de-allocate handle */
      if (lm_tech == LM_UMTS)
      {
        wcdma_cleanup_status = rfm_wcdma_handle_cleanup(test_param_ptr->tgt_drx_rxlm_buf_idx);

        if (wcdma_cleanup_status!=RFM_WCDMA_HEALTHY_STATE)
        {
          RF_MSG_1(RF_ERROR,  "ftm_meas_deallocate_lm_buffer: Handle cleanup failed for RFLM handle %d", test_param_ptr->tgt_drx_rxlm_buf_idx);
        }
      } /* if (lm_tech == LM_UMTS) */

      lm_status = rxlm_deallocate_buffer(test_param_ptr->tgt_drx_rxlm_buf_idx);
      if (lm_status != LM_SUCCESS)
      {
        MSG_2( MSG_SSID_RF, MSG_LEGACY_FATAL,"Cannot deallocate tech %d tune-away RXLM buffer for Ant-1 status %d",lm_tech,lm_status);
      }
      else
      {
        test_param_ptr->tgt_drx_rxlm_buf_idx = RXLM_MAX_BUFFERS;
        ftm_meas_state.tgt_rxlm_ant1_idx = RXLM_MAX_BUFFERS;
      }
    }
    else
    {
      test_param_ptr->tgt_drx_rxlm_buf_idx = RXLM_MAX_BUFFERS;
    }
  }
}
/*===========================================================================

  FUNCTION: ftm_meas_allocate_lm_buffer_for_half_script

===========================================================================*/
/*!
    @brief
    This function is called to allocate LM buffer for Half script testing in FTM mode.

    @return
    None.
*/
/*=========================================================================*/
void ftm_meas_allocate_lm_buffer_for_half_script(rfm_meas_generic_params_type *test_param_ptr,ftm_meas_half_script_neighbour_index_enum_type nbr_index)
{
  lm_status_type lm_status = LM_SUCCESS;
  lm_tech_type   lm_tech = LM_TECH_MAX;
  rflm_dm_handle_meta_s handle_meta;
  boolean dm_status = TRUE;

  if (test_param_ptr == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_FATAL,"NULL pointer in ftm_meas_allocate_lm_buffer()");
    return ;
  }

  if(test_param_ptr->tgt_tech >= RFM_NUM_MODES)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"Invalid target tech %d", test_param_ptr->tgt_tech);
    return ;
  }

  /* Set Target RXLM buffer first */
  switch(test_param_ptr->tgt_tech)
  {
    case RFCOM_1X_MODE:
      lm_tech = LM_1X;
      break;
    case RFCOM_GSM_MODE:
      lm_tech = LM_GSM;
      break;
    case RFCOM_1XEVDO_MODE:
      lm_tech = LM_HDR;
      break;
    case RFCOM_LTE_MODE:
      lm_tech = LM_LTE;
      break;
    case RFCOM_TDSCDMA_MODE:
      lm_tech = LM_TDSCDMA;
      break;
  case RFCOM_WCDMA_MODE:
      lm_tech = LM_UMTS;
      break;
    default:
      MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"Target Mode %d FTM IRAT doesn't support",
            test_param_ptr->tgt_tech );
      break;
  }
  if(lm_tech < LM_TECH_MAX)
  {
    if (ftm_meas_state.half_script_nbr_rxlm_ant0_idx[nbr_index] < RXLM_MAX_BUFFERS)
    {
      /* Already allocated, no need to allocate again */
      lm_status = LM_SUCCESS;
      test_param_ptr->tgt_prx_rxlm_buf_idx = ftm_meas_state.half_script_nbr_rxlm_ant0_idx[nbr_index];
    }
    else
    {
      lm_status = rxlm_allocate_buffer(RXLM_CHAIN_0,lm_tech,&(test_param_ptr->tgt_prx_rxlm_buf_idx));
    }
    if (lm_status != LM_SUCCESS)
    {
      MSG_2( MSG_SSID_RF, MSG_LEGACY_FATAL,"Cannot allocate tech %d tune-away RXLM buffer for Ant- status %d",lm_tech,lm_status);
    }
    else
    {
      ftm_meas_state.half_script_nbr_rxlm_ant0_idx[nbr_index] = test_param_ptr->tgt_prx_rxlm_buf_idx;
      dm_status = rflm_dm_get_handle_info(test_param_ptr->tgt_prx_rxlm_buf_idx, &handle_meta);
      if (dm_status == TRUE)
      {
//        handle_meta.type = RFLM_IRAT_HANDLE;
        handle_meta.device_id = test_param_ptr->tgt_device;
        (void)rflm_dm_set_handle_info(test_param_ptr->tgt_prx_rxlm_buf_idx, &handle_meta);
      }
    }
    if (test_param_ptr->tgt_div_en == 1)
    {
      if (ftm_meas_state.half_script_nbr_rxlm_ant1_idx[nbr_index] < RXLM_MAX_BUFFERS)
      {
        /* Already allocated, no need to allocate again */
        lm_status = LM_SUCCESS;
        test_param_ptr->tgt_drx_rxlm_buf_idx = ftm_meas_state.half_script_nbr_rxlm_ant1_idx[nbr_index];
      }
      else
      {
        lm_status = rxlm_allocate_buffer(RXLM_CHAIN_1,lm_tech,&(test_param_ptr->tgt_drx_rxlm_buf_idx));
      }
      if (lm_status != LM_SUCCESS)
      {
        MSG_2( MSG_SSID_RF, MSG_LEGACY_FATAL,"Cannot allocate tech %d tune-away RXLM buffer for Ant-1 status %d",lm_tech,lm_status);
      }
      else
      {
        ftm_meas_state.half_script_nbr_rxlm_ant1_idx[nbr_index] = test_param_ptr->tgt_drx_rxlm_buf_idx;
        dm_status = rflm_dm_get_handle_info(test_param_ptr->tgt_drx_rxlm_buf_idx, &handle_meta);
        if (dm_status == TRUE)
        {
//          handle_meta.type = RFLM_IRAT_HANDLE;
          handle_meta.device_id = test_param_ptr->tgt_div_device;
          (void)rflm_dm_set_handle_info(test_param_ptr->tgt_drx_rxlm_buf_idx, &handle_meta);
        }
      }
    }
    else
    {
      test_param_ptr->tgt_drx_rxlm_buf_idx = RXLM_MAX_BUFFERS;
    }
  }
}



/*===========================================================================

  FUNCTION: ftm_meas_deallocate_lm_buffer

===========================================================================*/
/*!
    @brief
    This function is called to deallocate LM buffer for Half script testing in FTM mode.
    It should be called at the very beginning of IRAT testing.

    @return
    None.
*/
/*=========================================================================*/
void ftm_meas_deallocate_lm_buffer_for_half_script(rfm_meas_generic_params_type *test_param_ptr,ftm_meas_half_script_neighbour_index_enum_type nbr_index)
{
  lm_status_type lm_status;
  lm_tech_type   lm_tech = LM_TECH_MAX;
  rfm_api_status_t wcdma_cleanup_status;

  if (test_param_ptr == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_FATAL,"NULL pointer in ftm_meas_allocate_lm_buffer()");
    return ;
  }

  if(test_param_ptr->src_tech >= RFM_NUM_MODES)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"Invalid target tech %d", test_param_ptr->src_tech);
    return ;
  }
  /* Set Target RXLM buffer first */
  switch(test_param_ptr->src_tech)
  {
    case RFCOM_1X_MODE:
      lm_tech = LM_1X;
      break;
    case RFCOM_GSM_MODE:
      lm_tech = LM_GSM;
      break;
    case RFCOM_1XEVDO_MODE:
      lm_tech = LM_HDR;
      break;
    case RFCOM_WCDMA_MODE:
      lm_tech = LM_UMTS;
      break;
    case RFCOM_LTE_MODE:
      lm_tech = LM_LTE;
      break;
    case RFCOM_TDSCDMA_MODE:
      lm_tech = LM_TDSCDMA;
      break;
    default:
      MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL,"Target Mode %d FTM IRAT doesn't support",
            test_param_ptr->src_tech );
      break;
  }
  if(lm_tech < LM_TECH_MAX)
  {
    /* Call RFWCDMA MDSP data cleanup before de-allocate handle */
    if (lm_tech == LM_UMTS)
    {
      wcdma_cleanup_status =
        rfm_wcdma_handle_cleanup(test_param_ptr->src_ca_path0_prx_rxlm_buf_idx);

      if (wcdma_cleanup_status!=RFM_WCDMA_HEALTHY_STATE)
      {
        RF_MSG_1(RF_ERROR,  "ftm_meas_deallocate_lm_buffer: Handle cleanup failed for RFLM handle %d", test_param_ptr->src_ca_path0_prx_rxlm_buf_idx);
      }
    } /* if (lm_tech == LM_UMTS) */

    lm_status = rxlm_deallocate_buffer(test_param_ptr->src_ca_path0_prx_rxlm_buf_idx);
    if (lm_status != LM_SUCCESS)
    {
      MSG_2( MSG_SSID_RF, MSG_LEGACY_FATAL,"Cannot deallocate tech %d tune-away RXLM buffer for Ant-0 status %d",lm_tech,lm_status);
    }
    else
    {
      test_param_ptr->src_ca_path0_prx_rxlm_buf_idx = RXLM_MAX_BUFFERS;
      ftm_meas_state.half_script_nbr_rxlm_ant0_idx[nbr_index] = RXLM_MAX_BUFFERS;
    }
    if (test_param_ptr->src_ca_path0_div_en == 1)
    {
      /* Call RFWCDMA MDSP data cleanup before de-allocate handle */
      if (lm_tech == LM_UMTS)
      {
        wcdma_cleanup_status = rfm_wcdma_handle_cleanup(test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx);

        if (wcdma_cleanup_status!=RFM_WCDMA_HEALTHY_STATE)
        {
          RF_MSG_1(RF_ERROR,  "ftm_meas_deallocate_lm_buffer: Handle cleanup failed for RFLM handle %d", test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx);
        }
      } /* if (lm_tech == LM_UMTS) */

      lm_status = rxlm_deallocate_buffer(test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx);
      if (lm_status != LM_SUCCESS)
      {
        MSG_2( MSG_SSID_RF, MSG_LEGACY_FATAL,"Cannot deallocate tech %d tune-away RXLM buffer for Ant-1 status %d",lm_tech,lm_status);
      }
      else
      {
        test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx = RXLM_MAX_BUFFERS;
        ftm_meas_state.half_script_nbr_rxlm_ant1_idx[nbr_index] = RXLM_MAX_BUFFERS;
      }
    }
    else
    {
      test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx = RXLM_MAX_BUFFERS;
    }
  }
}

/*===========================================================================

  FUNCTION: ftm_meas_xtoy_test_y

===========================================================================*/
/*!
    @brief
    This function acts the dispatcher for IRAT FTM tests. According to the input
    source/target technology, the corrsponding IRAT FTM API for that combination
    will be called.

    @return
    None.
*/
/*=========================================================================*/
void ftm_meas_xtoy_test_y(rfm_meas_generic_params_type *test_param_ptr)
{
  ftm_meas_func_tbl_type *target_func_tbl_ptr = NULL;
  ftm_meas_func_tbl_type *source_func_tbl_ptr = NULL;
  boolean status = FALSE;
  rfm_meas_enter_param_type enter_param;
  rfm_meas_setup_param_type setup_param;
  rfm_meas_exit_param_type exit_param;

  rfm_meas_common_param_type common_meas_params;

  rfm_meas_common_init_params(&common_meas_params);

  /* Input parameter checking */
  if (test_param_ptr == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_FATAL,"NULL pointer in ftm_meas_xtoy_test_y()");
    return ;
  }

  if( (test_param_ptr->src_tech >= RFM_NUM_MODES) ||
      (test_param_ptr->tgt_tech >= RFM_NUM_MODES))
  {
    MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Invalid ftm_meas_xtoy_test_y for source tech = %d and target tech =%d",
           test_param_ptr->test_level, test_param_ptr->src_tech, test_param_ptr->tgt_tech);
    return;
  }

  /* Check for module registration */
  if((ftm_meas_state.func_tbl_ptr[test_param_ptr->tgt_tech] == NULL) ||
     (ftm_meas_state.func_tbl_ptr[test_param_ptr->src_tech] == NULL) )
  {
    MSG_4( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Function table not registered for src_tech %d (tbl 0x%X) or target_tech %d (tbl 0x%X)",
           test_param_ptr->src_tech, ftm_meas_state.func_tbl_ptr[test_param_ptr->src_tech],
           test_param_ptr->tgt_tech, ftm_meas_state.func_tbl_ptr[test_param_ptr->tgt_tech]);
    return;
  }
  target_func_tbl_ptr = ftm_meas_state.func_tbl_ptr[test_param_ptr->tgt_tech];
  source_func_tbl_ptr = ftm_meas_state.func_tbl_ptr[test_param_ptr->src_tech];

  /* Determine Test mode */
  if(test_param_ptr->test_level < FTM_MEAS_LEVEL_MAX)
  {
    // Level I test: RF exercises scripts
    ftm_meas_state.test_level = test_param_ptr->test_level;
  }
  else
  {
    MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Level %d not supported in this command ftm_meas_xtoy_test_y for source tech = %d and target tech =%d",
           test_param_ptr->test_level, test_param_ptr->src_tech, test_param_ptr->tgt_tech);
    return;
  }

  /* Allocate RXLM buffer for Target Tech since RXLM buffer is not allocated yet */
  ftm_meas_allocate_lm_buffer(test_param_ptr);
  if(source_func_tbl_ptr->get_src_rxlm_buffer_fp!= NULL)
  {
    status = source_func_tbl_ptr->get_src_rxlm_buffer_fp(test_param_ptr);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed getting source RXLM buffer for Tech %d",test_param_ptr->src_tech);
      return;
    }
  }

  if(source_func_tbl_ptr->get_src_txlm_buffer_fp!= NULL)
  {
    status = source_func_tbl_ptr->get_src_txlm_buffer_fp(test_param_ptr);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed getting source RXLM buffer for Tech %d",test_param_ptr->src_tech);
      return;
    }
  }

  /* Setup Clocks */
  ftm_meas_clock_setup(test_param_ptr, TRUE);

  common_meas_params.script_type = RFM_MEAS_PROGRAM_STARTUP;

  /* Get tech params */
  if(source_func_tbl_ptr->get_tech_param_fp!=NULL)
  {
    common_meas_params.source_param = source_func_tbl_ptr->get_tech_param_fp(test_param_ptr,TRUE);
  }
  if(target_func_tbl_ptr->get_tech_param_fp!=NULL)
  {
    common_meas_params.target_param = target_func_tbl_ptr->get_tech_param_fp(test_param_ptr,FALSE);
  }

  common_meas_params.header.source_tech = test_param_ptr->src_tech;

    /* Fill up CAR 0 parameters */
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].band = test_param_ptr->src_ca_path0_band;
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_ptr->src_ca_path0_device;
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_ptr->src_ca_path0_prx_rxlm_buf_idx;
  if( test_param_ptr->src_ca_path0_div_en == TRUE )
  {
    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_ptr->src_ca_path0_div_device_id1;
    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx;

    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX2].device = test_param_ptr->src_ca_path0_div_device_id2;
    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX2].lm_handle = test_param_ptr->src_ca_path0_drx_device_2_rxlm_buf_idx;

    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX3].device = test_param_ptr->src_ca_path0_div_device_id3;
    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX3].lm_handle = test_param_ptr->src_ca_path0_drx_device_3_rxlm_buf_idx;
  }
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].tx_dev_handle[0].device    = test_param_ptr->src_ca_path0_tx_device;
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].tx_dev_handle[0].lm_handle = test_param_ptr->src_ca_path0_txlm_buf_idx;




  /*Start assuming that we have to initialize only one source device*/
  common_meas_params.header.num_source_item = 1;

  /* Copy CAR 1 Data if it is enabled and increment num_source_item value based on it*/
  if (test_param_ptr->src_ca_path1_flag_enable == TRUE)
  {
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].band = test_param_ptr->src_ca_path1_band;
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_ptr->src_ca_path1_device;
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_ptr->src_ca_path1_prx_rxlm_buf_idx;
    if( test_param_ptr->src_ca_path1_div_en == TRUE)
    {
      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_ptr->src_ca_path1_div_device_id1;
      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx;

      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX2].device = test_param_ptr->src_ca_path1_div_device_id2;
      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX2].lm_handle = test_param_ptr->src_ca_path1_drx_device_2_rxlm_buf_idx;

      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX3].device = test_param_ptr->src_ca_path1_div_device_id3;
      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX3].lm_handle = test_param_ptr->src_ca_path1_drx_device_3_rxlm_buf_idx;
    }

   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].tx_dev_handle[0].device    = test_param_ptr->src_ca_path1_tx_device;
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].tx_dev_handle[0].lm_handle = test_param_ptr->src_ca_path1_txlm_buf_idx;
	/*Increase one as one more carrier got enabled*/
	common_meas_params.header.num_source_item++;
  }

  /* Copy CA PATH 2 Data only if both CA PATH 1 and CA PATH  2 are enabled and increment num_source_item value based on it*/
  if ((test_param_ptr->src_ca_path2_flag_enable == TRUE) && (test_param_ptr->src_ca_path1_flag_enable == TRUE))
  {
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].band = test_param_ptr->src_ca_path2_band;
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_ptr->src_ca_path2_device;
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_ptr->src_ca_path2_prx_rxlm_buf_idx;
   if( test_param_ptr->src_ca_path2_div_en == TRUE)
   {
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_ptr->src_ca_path2_div_device_id1;
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_ptr->src_ca_path2_drx_device_1_rxlm_buf_idx;

    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX2].device = test_param_ptr->src_ca_path2_div_device_id2;
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX2].lm_handle = test_param_ptr->src_ca_path2_drx_device_2_rxlm_buf_idx;

    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX3].device = test_param_ptr->src_ca_path2_div_device_id3;
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX3].lm_handle = test_param_ptr->src_ca_path2_drx_device_3_rxlm_buf_idx;
   }
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].tx_dev_handle[0].device    = test_param_ptr->src_ca_path2_tx_device;
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].tx_dev_handle[0].lm_handle = test_param_ptr->src_ca_path2_txlm_buf_idx;
   /*Increase one as one more carrier got enabled*/
	common_meas_params.header.num_source_item++;
  }


  common_meas_params.header.target_tech = test_param_ptr->tgt_tech;

  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].band = test_param_ptr->tgt_band;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_ptr->tgt_device;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_ptr->tgt_prx_rxlm_buf_idx;

  if( test_param_ptr->tgt_div_en == TRUE )
  {
    common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_ptr->tgt_div_device;
    common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_ptr->tgt_drx_rxlm_buf_idx;
  }

  /*As of now, we dont expect TGT tech to have multiple CA Paths*/
  common_meas_params.header.num_target_item = 1;

  /* Enter Meas */
  enter_param.enter_params = &common_meas_params;

  if(target_func_tbl_ptr->enter_meas_fp != NULL)
  {
    status = target_func_tbl_ptr->enter_meas_fp(&enter_param);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed ENTER_MEAS for Tech %d",test_param_ptr->tgt_tech);
      return;
    }
  }

  /* Build script */
  setup_param.setup_params = &common_meas_params;

  if(target_func_tbl_ptr->build_script_fp != NULL)
  {
    status = target_func_tbl_ptr->build_script_fp(&setup_param);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed BUILD_SCRIPT for Tech %d",test_param_ptr->tgt_tech);
      return;
    }
  }

  if (ftm_irat_tune == TRUE)
  {
    /* If abort is called */
    if (test_param_ptr->test_level == FTM_MEAS_TEST_LEVEL_ABORT)
    {
      /* exit script*/
      exit_param.is_measurement_gap_aborted = TRUE;

      exit_param.exit_params = &common_meas_params;

      if (target_func_tbl_ptr->exit_meas_fp != NULL)
      {
        status = target_func_tbl_ptr->exit_meas_fp(&exit_param);
      }
    }
    else /* No Abort so continue with tune away */
    {
      /* Exit Source tech */
      if (source_func_tbl_ptr->meas_exit_tech_fp != NULL )
      {
        status = source_func_tbl_ptr->meas_exit_tech_fp(test_param_ptr, TRUE);
        if(status == FALSE)
        {
          MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
              "Failed Exiting Tech %d in IRAT Tune-Away",test_param_ptr->src_tech);
          return;
        }
      }

      /* Enter Target Tech */
      if (target_func_tbl_ptr->meas_enter_tech_fp != NULL )
      {
        status = target_func_tbl_ptr->meas_enter_tech_fp(test_param_ptr, FALSE);
        if(status == FALSE)
        {
          MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
              "Failed Entering Tech %d in IRAT Tune-Away",test_param_ptr->tgt_tech);
          return;
        }
      }
    }
  }
}

/*===========================================================================

  FUNCTION: ftm_meas_xtoy_test_x

===========================================================================*/
/*!
    @brief
    This function acts the dispatcher for IRAT FTM tests. According to the input
    source/target technology, the corrsponding IRAT FTM API for that combination
    will be called.

    @return
    None.
*/
/*=========================================================================*/
void ftm_meas_xtoy_test_x(rfm_meas_generic_params_type *test_param_ptr)
{
  ftm_meas_func_tbl_type *target_func_tbl_ptr = NULL;
  ftm_meas_func_tbl_type *source_func_tbl_ptr = NULL;
  boolean status = FALSE;
  rfm_meas_exit_param_type exit_param;

  rfm_meas_common_param_type common_meas_params;

  rfm_meas_common_init_params(&common_meas_params);

  /* Input parameter checking */
  if (test_param_ptr == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_FATAL,"NULL pointer in ftm_meas_xtoy_test_x()");
    return ;
  }

  if( (test_param_ptr->src_tech >= RFM_NUM_MODES) ||
      (test_param_ptr->tgt_tech >= RFM_NUM_MODES))
  {
    MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Invalid ftm_meas_xtoy_test_x for source tech = %d and target tech =%d",
           test_param_ptr->test_level, test_param_ptr->src_tech, test_param_ptr->tgt_tech);
    return;
  }

  /* Check for module registration */
  if((ftm_meas_state.func_tbl_ptr[test_param_ptr->tgt_tech] == NULL) ||
     (ftm_meas_state.func_tbl_ptr[test_param_ptr->src_tech] == NULL) )
  {
    MSG_4( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Function table not registered for src_tech %d (tbl 0x%X) or target_tech %d (tbl 0x%X)",
           test_param_ptr->src_tech, ftm_meas_state.func_tbl_ptr[test_param_ptr->src_tech],
           test_param_ptr->tgt_tech, ftm_meas_state.func_tbl_ptr[test_param_ptr->tgt_tech]);
    return;
  }
  target_func_tbl_ptr = ftm_meas_state.func_tbl_ptr[test_param_ptr->tgt_tech];
  source_func_tbl_ptr = ftm_meas_state.func_tbl_ptr[test_param_ptr->src_tech];

  /* Get the source RXLM buffers */
  if(source_func_tbl_ptr->get_src_rxlm_buffer_fp!= NULL)
  {
    status = source_func_tbl_ptr->get_src_rxlm_buffer_fp(test_param_ptr);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed getting source RXLM buffer for Tech %d",test_param_ptr->src_tech);
      return;
    }
  }

  /* Get the source TXLM buffers */
  if(source_func_tbl_ptr->get_src_txlm_buffer_fp!= NULL)
  {
    status = source_func_tbl_ptr->get_src_txlm_buffer_fp(test_param_ptr);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed getting source RXLM buffer for Tech %d",test_param_ptr->src_tech);
      return;
    }
  }

  /* Get the Taget RXLM buffers from state machine stored xtoy_test_y()*/
  test_param_ptr->tgt_prx_rxlm_buf_idx = ftm_meas_state.tgt_rxlm_ant0_idx;
  test_param_ptr->tgt_drx_rxlm_buf_idx = ftm_meas_state.tgt_rxlm_ant1_idx;

  if (ftm_irat_tune == TRUE)
  {
    /* Exit Target tech */
    if (target_func_tbl_ptr->meas_exit_tech_fp != NULL )
    {
      status = target_func_tbl_ptr->meas_exit_tech_fp(test_param_ptr, FALSE);
      if(status == FALSE)
      {
        MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
            "Failed Exiting Tech %d in IRAT Tune-Back",test_param_ptr->tgt_tech);
        return;
      }
    }

    /* Enter Source tech */
    if (source_func_tbl_ptr->meas_enter_tech_fp != NULL )
    {
      status = source_func_tbl_ptr->meas_enter_tech_fp(test_param_ptr, TRUE);
      if(status == FALSE)
      {
        MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
            "Failed Entering Tech %d in IRAT Tune-Back",test_param_ptr->src_tech);
        return;
      }
    }
  }

  common_meas_params.script_type = RFM_MEAS_PROGRAM_CLEANUP;

  if(source_func_tbl_ptr->get_tech_param_fp!=NULL)
  {
    common_meas_params.source_param = source_func_tbl_ptr->get_tech_param_fp(test_param_ptr,TRUE);
  }
  if(target_func_tbl_ptr->get_tech_param_fp!=NULL)
  {
    common_meas_params.target_param = target_func_tbl_ptr->get_tech_param_fp(test_param_ptr,FALSE);
  }

  common_meas_params.header.source_tech = test_param_ptr->src_tech;

    /* Fill up CAR 0 parameters */
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].band = test_param_ptr->src_ca_path0_band;
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_ptr->src_ca_path0_device;
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_ptr->src_ca_path0_prx_rxlm_buf_idx;
  if( test_param_ptr->src_ca_path0_div_en == TRUE )
  {
    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_ptr->src_ca_path0_div_device_id1;
    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx;

    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX2].device = test_param_ptr->src_ca_path0_div_device_id2;
    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX2].lm_handle = test_param_ptr->src_ca_path0_drx_device_2_rxlm_buf_idx;

    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX3].device = test_param_ptr->src_ca_path0_div_device_id2;
    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX3].lm_handle = test_param_ptr->src_ca_path0_drx_device_3_rxlm_buf_idx;
  }
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].tx_dev_handle[0].device    = test_param_ptr->src_ca_path0_tx_device;
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].tx_dev_handle[0].lm_handle = test_param_ptr->src_ca_path0_txlm_buf_idx;




  /*Start assuming that we have to initialize only one source device*/
  common_meas_params.header.num_source_item = 1;

  /* Copy CAR 1 Data if it is enabled and increment num_source_item value based on it*/
  if (test_param_ptr->src_ca_path1_flag_enable == TRUE)
  {
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].band = test_param_ptr->src_ca_path1_band;
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_ptr->src_ca_path1_device;
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_ptr->src_ca_path1_prx_rxlm_buf_idx;
    if( test_param_ptr->src_ca_path1_div_en == TRUE)
    {
      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_ptr->src_ca_path1_div_device_id1;
      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx;

      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX2].device = test_param_ptr->src_ca_path1_div_device_id2;
      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX2].lm_handle = test_param_ptr->src_ca_path1_drx_device_2_rxlm_buf_idx;

      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX3].device = test_param_ptr->src_ca_path1_div_device_id3;
      common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].rx_dev_handle[DEV_HANDLE_DRX3].lm_handle = test_param_ptr->src_ca_path1_drx_device_3_rxlm_buf_idx;
    }

   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].tx_dev_handle[0].device    = test_param_ptr->src_ca_path1_tx_device;
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_1].tx_dev_handle[0].lm_handle = test_param_ptr->src_ca_path1_txlm_buf_idx;
	/*Increase one as one more carrier got enabled*/
	common_meas_params.header.num_source_item++;
  }

  /* Copy CA PATH 2 Data only if both CA PATH 1 and CA PATH  2 are enabled and increment num_source_item value based on it*/
  if ((test_param_ptr->src_ca_path2_flag_enable == TRUE) && (test_param_ptr->src_ca_path1_flag_enable == TRUE))
  {
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].band = test_param_ptr->src_ca_path2_band;
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_ptr->src_ca_path2_device;
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_ptr->src_ca_path2_prx_rxlm_buf_idx;
   if( test_param_ptr->src_ca_path2_div_en == TRUE)
   {
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_ptr->src_ca_path2_div_device_id1;
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_ptr->src_ca_path2_drx_device_1_rxlm_buf_idx;

    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX2].device = test_param_ptr->src_ca_path2_div_device_id2;
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX2].lm_handle = test_param_ptr->src_ca_path2_drx_device_2_rxlm_buf_idx;

    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX3].device = test_param_ptr->src_ca_path2_div_device_id2;
    common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].rx_dev_handle[DEV_HANDLE_DRX3].lm_handle = test_param_ptr->src_ca_path2_drx_device_3_rxlm_buf_idx;
   }
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].tx_dev_handle[0].device    = test_param_ptr->src_ca_path2_tx_device;
   common_meas_params.header.source_param_v2[RFM_MEAS_SCELL_2].tx_dev_handle[0].lm_handle = test_param_ptr->src_ca_path2_txlm_buf_idx;
   /*Increase one as one more carrier got enabled*/
	common_meas_params.header.num_source_item++;
  }


  common_meas_params.header.target_tech = test_param_ptr->tgt_tech;

  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].band = test_param_ptr->tgt_band;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_ptr->tgt_device;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_ptr->tgt_prx_rxlm_buf_idx;

  if( test_param_ptr->tgt_div_en == TRUE )
  {
    common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_ptr->tgt_div_device;
    common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_ptr->tgt_drx_rxlm_buf_idx;
  }
  /*As of now, we dont expect TGT tech to have multiple CA Paths*/
  common_meas_params.header.num_target_item = 1;

  /* Exit Meas */
  exit_param.is_measurement_gap_aborted = FALSE;
  exit_param.exit_params = &common_meas_params;

  if(target_func_tbl_ptr->exit_meas_fp != NULL)
  {
    status = target_func_tbl_ptr->exit_meas_fp(&exit_param);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed EXIT_MEAS for Tech %d",test_param_ptr->tgt_tech);
      return;
    }
  }

  /* Set back to Original Clocks */
  ftm_meas_clock_setup(test_param_ptr, FALSE);

  ftm_meas_deallocate_lm_buffer(test_param_ptr);
}
/*===========================================================================

  FUNCTION: ftm_meas_half_script

===========================================================================*/
/*!
    @brief
    This function acts the dispatcher for FTM IRAT HALF SCRIPT EXIT

    @param test_param_half_script_ptr - Half script parameters in FTM MEAS GENERIC Format

    @half_script_type - type of half script

    @return
    None.
*/
/*=========================================================================*/

void ftm_meas_half_script(rfm_meas_generic_params_type *test_param_half_script_ptr,rfm_meas_half_script_type half_script_type)
{
  ftm_meas_func_tbl_type *target_func_tbl_ptr = NULL;
  ftm_meas_func_tbl_type *source_func_tbl_ptr = NULL;
  boolean status = FALSE;
  rfm_meas_enter_param_type enter_param;
  rfm_meas_setup_param_type setup_param;
  rfm_meas_exit_param_type exit_param;
  ftm_meas_half_script_neighbour_index_enum_type nbr_idx;


  rfm_meas_common_param_type common_meas_params;

  /*Initialize common_meas_params parameters */
  rfm_meas_common_init_params(&common_meas_params);

  /* Input parameter checking */
  if (test_param_half_script_ptr == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_FATAL,"NULL pointer in ftm_meas_xtoy_test_y()");
    return ;
  }

  /* Sanity checks: Check if SRC Tech  and TGT Tech are valid*/
  if( (test_param_half_script_ptr->src_tech >= RFM_NUM_MODES) ||
      (test_param_half_script_ptr->tgt_tech >= RFM_NUM_MODES))
  {
    MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Invalid ftm_meas_half_script_start for source tech = %d and target tech =%d",
           test_param_half_script_ptr->test_level, test_param_half_script_ptr->src_tech, test_param_half_script_ptr->tgt_tech);
    return;
  }

  /* Check for module registration */
  if((ftm_meas_state.func_tbl_ptr[test_param_half_script_ptr->tgt_tech] == NULL) ||
     (ftm_meas_state.func_tbl_ptr[test_param_half_script_ptr->src_tech] == NULL) )
  {
    MSG_4( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Function table no registered for src_tech %d (tbl 0x%X) or target_tech %d (tbl 0x%X)",
           test_param_half_script_ptr->src_tech, ftm_meas_state.func_tbl_ptr[test_param_half_script_ptr->src_tech],
           test_param_half_script_ptr->tgt_tech, ftm_meas_state.func_tbl_ptr[test_param_half_script_ptr->tgt_tech]);
    return;
  }

  /* update func tbl ptr*/
  target_func_tbl_ptr = ftm_meas_state.func_tbl_ptr[test_param_half_script_ptr->tgt_tech];
  source_func_tbl_ptr = ftm_meas_state.func_tbl_ptr[test_param_half_script_ptr->src_tech];

  /* Determine Test mode */
  if(test_param_half_script_ptr->test_level < FTM_MEAS_LEVEL_MAX)
  {
    // Level I test: RF exercises scripts
    ftm_meas_state.test_level = test_param_half_script_ptr->test_level;
  }
  else
  {
    MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Level %d not supported in this command for source tech = %d and target tech =%d",
           test_param_half_script_ptr->test_level, test_param_half_script_ptr->src_tech, test_param_half_script_ptr->tgt_tech);
    return;
  }


  /*TGT LM buffer allocation*/
  switch(half_script_type)
  {
   /*For neighbour to home case, we need to use the existing LM script buffers*/
   case RFM_HALF_SCRIPT_NBR_X_2_HOMEFREQ:
   case RFM_HALF_SCRIPT_NBR_Y_2_HOMEFREQ:
    test_param_half_script_ptr->tgt_prx_rxlm_buf_idx = ftm_meas_state.tgt_rxlm_ant0_idx;
	if(test_param_half_script_ptr->tgt_div_en ==1)
 	 test_param_half_script_ptr->tgt_drx_rxlm_buf_idx = ftm_meas_state.tgt_rxlm_ant1_idx;
	break;

   /*When we are moving to NBR X, we need to allocate buffer for X*/
   case RFM_HALF_SCRIPT_HOMEFREQ_2_NBR_X:
   case RFM_HALF_SCRIPT_NBR_Y_2_NBR_X:
    ftm_meas_allocate_lm_buffer_for_half_script(test_param_half_script_ptr,HALF_SCRIPT_NBR_X);
	break;

   /*When we are moving to NBR Y, we need to allocate buffer for Y*/
   case RFM_HALF_SCRIPT_HOMEFREQ_2_NBR_Y:
   case RFM_HALF_SCRIPT_NBR_X_2_NBR_Y:
   	ftm_meas_allocate_lm_buffer_for_half_script(test_param_half_script_ptr,HALF_SCRIPT_NBR_Y);
	break;

   default:
      MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL," wrong type of half_script_type %d",
            half_script_type);
    break;
  }


  switch(half_script_type)
  {
   /*For home to x cases, we need to use the existing LM script buffers for source*/
   /*This LM buffer has been allocated during tuneaway, i.e. target tech in Tune away*/
   case RFM_HALF_SCRIPT_HOMEFREQ_2_NBR_X:
   case RFM_HALF_SCRIPT_HOMEFREQ_2_NBR_Y:
    test_param_half_script_ptr->src_ca_path0_prx_rxlm_buf_idx = ftm_meas_state.tgt_rxlm_ant0_idx;
	if(test_param_half_script_ptr->src_ca_path0_div_en == 1)
 	 test_param_half_script_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx = ftm_meas_state.tgt_rxlm_ant1_idx;
	break;

   /*When we are moving from NBR X, we need to use the already allocated buffer for X*/
   case RFM_HALF_SCRIPT_NBR_X_2_HOMEFREQ:
   case RFM_HALF_SCRIPT_NBR_X_2_NBR_Y:
    test_param_half_script_ptr->src_ca_path0_prx_rxlm_buf_idx = ftm_meas_state.half_script_nbr_rxlm_ant0_idx[HALF_SCRIPT_NBR_X];
	if(test_param_half_script_ptr->src_ca_path0_div_en == 1)
 	 test_param_half_script_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx = ftm_meas_state.half_script_nbr_rxlm_ant1_idx[HALF_SCRIPT_NBR_X];
	break;

   /*When we are moving from NBR Y, we need to use the already allocated buffer for Y*/
   case RFM_HALF_SCRIPT_NBR_Y_2_HOMEFREQ:
   case RFM_HALF_SCRIPT_NBR_Y_2_NBR_X:
    test_param_half_script_ptr->src_ca_path0_prx_rxlm_buf_idx = ftm_meas_state.half_script_nbr_rxlm_ant0_idx[HALF_SCRIPT_NBR_Y];
	if(test_param_half_script_ptr->src_ca_path0_div_en == 1)
 	 test_param_half_script_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx = ftm_meas_state.half_script_nbr_rxlm_ant1_idx[HALF_SCRIPT_NBR_Y];
	break;

   default:
      MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL," wrong type of half_script_type %d",
            half_script_type);
    break;
 }

  /* Setup Clocks */
  ftm_meas_clock_setup(test_param_half_script_ptr, TRUE);

  /*Update the script type*/
  common_meas_params.script_type = RFM_MEAS_PROGRAM_TUNEAWAY_HALF_SCRIPT;

  /* Get tech params */
  if(source_func_tbl_ptr->get_tech_param_fp!=NULL)
  {
    common_meas_params.source_param = source_func_tbl_ptr->get_tech_param_fp(test_param_half_script_ptr,TRUE);
  }
  if(target_func_tbl_ptr->get_tech_param_fp!=NULL)
  {
    common_meas_params.target_param = target_func_tbl_ptr->get_tech_param_fp(test_param_half_script_ptr,FALSE);
  }

  common_meas_params.header.source_tech = test_param_half_script_ptr->src_tech;

    /* Fill up CAR 0 parameters */
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].band = test_param_half_script_ptr->src_ca_path0_band;
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_half_script_ptr->src_ca_path0_device;
  common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_half_script_ptr->src_ca_path0_prx_rxlm_buf_idx;
  if( test_param_half_script_ptr->src_ca_path0_div_en == TRUE )
  {
    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_half_script_ptr->src_ca_path0_div_device_id1;
    common_meas_params.header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_half_script_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx;
  }

  /*Start assuming that we have to initialize only one source device*/
  common_meas_params.header.num_source_item = 1;

  common_meas_params.header.target_tech = test_param_half_script_ptr->tgt_tech;

  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].band = test_param_half_script_ptr->tgt_band;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_half_script_ptr->tgt_device;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_half_script_ptr->tgt_prx_rxlm_buf_idx;

  if( test_param_half_script_ptr->tgt_div_en == TRUE )
  {
    common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_half_script_ptr->tgt_div_device;
    common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_half_script_ptr->tgt_drx_rxlm_buf_idx;
  }

  /*As of now, we dont expect TGT tech to have multiple CA Paths*/
  common_meas_params.header.num_target_item = 1;

  /* Enter Meas */
  enter_param.enter_params = &common_meas_params;

  if(target_func_tbl_ptr->enter_meas_fp != NULL)
  {
    status = target_func_tbl_ptr->enter_meas_fp(&enter_param);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed ENTER_MEAS for Tech %d",test_param_half_script_ptr->tgt_tech);
      return;
    }
  }

  /* Build script */
  setup_param.setup_params = &common_meas_params;

  if(target_func_tbl_ptr->build_script_fp != NULL)
  {
    status = target_func_tbl_ptr->build_script_fp(&setup_param);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed BUILD_SCRIPT for Tech %d",test_param_half_script_ptr->tgt_tech);
      return;
    }
  }

  if (ftm_irat_tune == TRUE)
  {
   /* Exit Source tech */
   if (source_func_tbl_ptr->meas_exit_tech_fp != NULL )
   {
    status = source_func_tbl_ptr->meas_exit_tech_fp(test_param_half_script_ptr, TRUE);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
           "Failed Exiting Tech %d in IRAT Tune-Away",test_param_half_script_ptr->src_tech);
      return;
    }
   }

   /* Enter Target Tech */
   if (target_func_tbl_ptr->meas_enter_tech_fp != NULL )
   {
    status = target_func_tbl_ptr->meas_enter_tech_fp(test_param_half_script_ptr, FALSE);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
           "Failed Entering Tech %d in IRAT Tune-Away",test_param_half_script_ptr->tgt_tech);
     return;
    }
   }
  }

  /* Exit Meas - Rewrite Target parameters with source parameters*/
  /* This is required as per the current architecture: we need to cleanup source tech during Half script*/
  /* execution. But Half script APIs in rfm layer will clean only target tech. Hence, we need to overwrite  */
  /* target parameters with source parameters*/
  /* This piece of code might change once half script architecture is finalized from WL1*/
  rfm_meas_common_init_params(&common_meas_params);
  if(target_func_tbl_ptr->get_tech_param_fp!=NULL)
  {
   common_meas_params.target_param = target_func_tbl_ptr->get_tech_param_fp(test_param_half_script_ptr,TRUE);
  }
  common_meas_params.header.target_tech = test_param_half_script_ptr->src_tech;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].band = test_param_half_script_ptr->src_ca_path0_band;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_half_script_ptr->src_ca_path0_device;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_half_script_ptr->src_ca_path0_prx_rxlm_buf_idx;
  if( test_param_half_script_ptr->src_ca_path0_div_en == TRUE )
  {
   common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_half_script_ptr->src_ca_path0_div_device_id1;
   common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_half_script_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx;
  }
  common_meas_params.header.num_target_item = 1;

  exit_param.is_measurement_gap_aborted = FALSE;
  exit_param.exit_params = &common_meas_params;

  if(target_func_tbl_ptr->exit_meas_fp != NULL)
  {
   status = target_func_tbl_ptr->exit_meas_fp(&exit_param);
   if(status == FALSE)
   {
     MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed EXIT_MEAS for Tech %d",test_param_half_script_ptr->tgt_tech);
    return;
   }
  }

  switch(half_script_type)
  {
   case RFM_HALF_SCRIPT_NBR_X_2_HOMEFREQ:
   case RFM_HALF_SCRIPT_NBR_Y_2_HOMEFREQ:

   /*Since we are returning to Home Freq, we need to set back the clocks*/
   ftm_meas_clock_setup(test_param_half_script_ptr, FALSE);

   /*We need to deallocate the buffer of current neighbour - X or Y*/
   nbr_idx = (half_script_type == RFM_HALF_SCRIPT_NBR_X_2_HOMEFREQ)? HALF_SCRIPT_NBR_X: HALF_SCRIPT_NBR_Y;
   ftm_meas_deallocate_lm_buffer_for_half_script(test_param_half_script_ptr,nbr_idx);

   break;

   case RFM_HALF_SCRIPT_NBR_X_2_NBR_Y:
   	/*We need to deallocate the buffer of current neighbour - X*/
    ftm_meas_deallocate_lm_buffer_for_half_script(test_param_half_script_ptr,HALF_SCRIPT_NBR_X);
   break;

   case RFM_HALF_SCRIPT_NBR_Y_2_NBR_X:
   	/*We need to deallocate the buffer of current neighbour - Y*/
    ftm_meas_deallocate_lm_buffer_for_half_script(test_param_half_script_ptr,HALF_SCRIPT_NBR_Y);
   break;

   /*No need to anything for all these cases*/
   case RFM_HALF_SCRIPT_HOMEFREQ_2_NBR_X:
   case RFM_HALF_SCRIPT_HOMEFREQ_2_NBR_Y:
   break;

   default:
	MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL," wrong type of half_script_type %d",
		  half_script_type);
   break;
  }
}
/*===========================================================================

  FUNCTION: ftm_meas_null2x_start

===========================================================================*/
/*!
    @brief
    This function acts the dispatcher for IRAT FTM NULL2X START

    @param - test_param_null2x_ptr - NULL2X script parameters in FTM MEAS GENERIC Format

    @return
    None.
*/
/*=========================================================================*/
void ftm_meas_null2x_start(rfm_meas_generic_params_type *test_param_null2x_ptr)
{
  ftm_meas_func_tbl_type *target_func_tbl_ptr = NULL;

  boolean status = FALSE;
  rfm_meas_enter_param_type enter_param;
  rfm_meas_setup_param_type setup_param;
  rfm_meas_exit_param_type exit_param;

  rfm_meas_common_param_type common_meas_params;

  rfm_meas_common_init_params(&common_meas_params);

  /* Check if input ptr is NULL */
  if (test_param_null2x_ptr == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_FATAL,"NULL pointer in ftm_meas_xtoy_test_y()");
    return ;
  }

  /* Make sure that targer tech is proper*/
  if(test_param_null2x_ptr->tgt_tech >= RFM_NUM_MODES)
  {
    MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Invalid ftm_meas_null2x_start for target tech =%d",test_param_null2x_ptr->tgt_tech,
           0,0);
    return;
  }

  /* Check for module registration */
  if(ftm_meas_state.func_tbl_ptr[test_param_null2x_ptr->tgt_tech] == NULL)
  {
    MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Function table no registered for target_tech %d (tbl 0x%X)",
           test_param_null2x_ptr->tgt_tech, ftm_meas_state.func_tbl_ptr[test_param_null2x_ptr->tgt_tech],0);
    return;
  }

  /*Assign func tbl ptr*/
  target_func_tbl_ptr = ftm_meas_state.func_tbl_ptr[test_param_null2x_ptr->tgt_tech];

  /* Determine Test mode */
  if (test_param_null2x_ptr->test_level < FTM_MEAS_LEVEL_MAX)
  {
    // Level I test: RF exercises scripts
    ftm_meas_state.test_level = test_param_null2x_ptr->test_level;
  }
  else
  {
    MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Level %d not supported in this command ftm_meas_null2x_start for target tech =%d",
            test_param_null2x_ptr->test_level,test_param_null2x_ptr->tgt_tech,0);
    return;
  }

  /* Allocate RXLM buffer for Target Tech since RXLM buffer is not allocated yet */
  ftm_meas_allocate_lm_buffer(test_param_null2x_ptr);

  common_meas_params.script_type = RFM_MEAS_PROGRAM_NULL2X;

  /* Get tech params */
  if(target_func_tbl_ptr->get_tech_param_fp!=NULL)
  {
    common_meas_params.target_param = target_func_tbl_ptr->get_tech_param_fp(test_param_null2x_ptr,FALSE);
  }

  common_meas_params.header.target_tech = test_param_null2x_ptr->tgt_tech;

  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].band = test_param_null2x_ptr->tgt_band;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_null2x_ptr->tgt_device;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_null2x_ptr->tgt_prx_rxlm_buf_idx;

  if( test_param_null2x_ptr->tgt_div_en == TRUE )
  {
    common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_null2x_ptr->tgt_div_device;
    common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_null2x_ptr->tgt_drx_rxlm_buf_idx;
  }

  /*As of now, we dont expect TGT tech to have multiple CA Paths*/
  common_meas_params.header.num_target_item = 1;

  /* Enter Meas */
  enter_param.enter_params = &common_meas_params;

  if(target_func_tbl_ptr->enter_meas_fp != NULL)
  {
    status = target_func_tbl_ptr->enter_meas_fp(&enter_param);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed ENTER_MEAS for Tech %d",test_param_null2x_ptr->tgt_tech);
      return;
    }
  }

  /* Build script */
  setup_param.setup_params = &common_meas_params;

  if(target_func_tbl_ptr->build_script_fp != NULL)
  {
    status = target_func_tbl_ptr->build_script_fp(&setup_param);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed BUILD_SCRIPT for Tech %d",test_param_null2x_ptr->tgt_tech);
      return;
    }
  }

  if (ftm_irat_tune == TRUE)
  {
    /* If abort is called */
    if (test_param_null2x_ptr->test_level == FTM_MEAS_TEST_LEVEL_ABORT)
    {
      /* exit script*/
      exit_param.is_measurement_gap_aborted = TRUE;

      exit_param.exit_params = &common_meas_params;

      if (target_func_tbl_ptr->exit_meas_fp != NULL)
      {
        status = target_func_tbl_ptr->exit_meas_fp(&exit_param);
      }
    }
    else /* No Abort so continue with tune away */
    {
      /* Enter Target Tech */
      if (target_func_tbl_ptr->meas_enter_tech_fp != NULL )
      {
        status = target_func_tbl_ptr->meas_enter_tech_fp(test_param_null2x_ptr, FALSE);
        if(status == FALSE)
        {
          MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
              "Failed Entering Tech %d in IRAT Tune-Away",test_param_null2x_ptr->tgt_tech);
          return;
        }
      }
    }
  }
}

/*===========================================================================

  FUNCTION: ftm_meas_null2x_stop

===========================================================================*/
/*!
    @brief
    This function acts the dispatcher for IRAT FTM NULL2X STOP

    @param - test_param_null2x_ptr - NULL2X script parameters in FTM MEAS GENERIC Format

    @return
    None.
*/
/*=========================================================================*/
void ftm_meas_null2x_stop(rfm_meas_generic_params_type *test_param_null2x_ptr)
{
  ftm_meas_func_tbl_type *target_func_tbl_ptr = NULL;
  boolean status = FALSE;
  rfm_meas_exit_param_type exit_param;

  rfm_meas_common_param_type common_meas_params;

  rfm_meas_common_init_params(&common_meas_params);


  /* Input parameter checking */
  if(test_param_null2x_ptr->tgt_tech >= RFM_NUM_MODES)
  {
    MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Invalid ftm_meas_null2x_stop for target tech =%d",test_param_null2x_ptr->tgt_tech,
           0,0);
    return;
  }

  /* Check for module registration */
  if(ftm_meas_state.func_tbl_ptr[test_param_null2x_ptr->tgt_tech] == NULL)
  {
    MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "Function table no registered for  target_tech %d (tbl 0x%X)",
           test_param_null2x_ptr->tgt_tech, ftm_meas_state.func_tbl_ptr[test_param_null2x_ptr->tgt_tech],0);
    return;
  }

  /* Check for module registration */
  if(ftm_meas_state.func_tbl_ptr[test_param_null2x_ptr->tgt_tech] == NULL)
  {
    MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,
		 "Function table no registered for target_tech %d (tbl 0x%X)",
		 test_param_null2x_ptr->tgt_tech, ftm_meas_state.func_tbl_ptr[test_param_null2x_ptr->tgt_tech],0);
	 return;
  }


  target_func_tbl_ptr = ftm_meas_state.func_tbl_ptr[test_param_null2x_ptr->tgt_tech];

  /* Get the Taget RXLM buffers from state machine stored xtoy_test_y()*/
  test_param_null2x_ptr->tgt_prx_rxlm_buf_idx = ftm_meas_state.tgt_rxlm_ant0_idx;
  test_param_null2x_ptr->tgt_drx_rxlm_buf_idx = ftm_meas_state.tgt_rxlm_ant1_idx;

  if (ftm_irat_tune == TRUE)
  {
    /* Exit Target tech */
    if (target_func_tbl_ptr->meas_exit_tech_fp != NULL )
    {
      status = target_func_tbl_ptr->meas_exit_tech_fp(test_param_null2x_ptr, FALSE);
      if(status == FALSE)
      {
        MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
            "Failed Exiting Tech %d in IRAT Tune-Back",test_param_null2x_ptr->tgt_tech);
        return;
      }
    }
  }

  common_meas_params.script_type = RFM_MEAS_PROGRAM_NULL2X;

  if(target_func_tbl_ptr->get_tech_param_fp!=NULL)
  {
    common_meas_params.target_param = target_func_tbl_ptr->get_tech_param_fp(test_param_null2x_ptr,FALSE);
  }

  common_meas_params.header.target_tech = test_param_null2x_ptr->tgt_tech;

  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].band = test_param_null2x_ptr->tgt_band;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device = test_param_null2x_ptr->tgt_device;
  common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = test_param_null2x_ptr->tgt_prx_rxlm_buf_idx;

  if( test_param_null2x_ptr->tgt_div_en == TRUE )
  {
    common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].device = test_param_null2x_ptr->tgt_div_device;
    common_meas_params.header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_DRX1].lm_handle = test_param_null2x_ptr->tgt_drx_rxlm_buf_idx;
  }
  /*As of now, we dont expect TGT tech to have multiple CA Paths*/
  common_meas_params.header.num_target_item = 1;

  /* Exit Meas */
  exit_param.is_measurement_gap_aborted = FALSE;
  exit_param.exit_params = &common_meas_params;

  if(target_func_tbl_ptr->exit_meas_fp != NULL)
  {
    status = target_func_tbl_ptr->exit_meas_fp(&exit_param);
    if(status == FALSE)
    {
      MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
          "Failed EXIT_MEAS for Tech %d",test_param_null2x_ptr->tgt_tech);
      return;
    }
  }

  /* Commenting clock setup API for now. Tech teams have to evaluate their requirement and come back*/
  #if 0
  ftm_meas_clock_setup(test_param_null2x_ptr, FALSE);
  #endif

  ftm_meas_deallocate_lm_buffer(test_param_null2x_ptr);
}


void ftm_meas_init(void)
{
   rfm_mode_enum_type mode = RFM_PARKED_MODE;
   ftm_meas_half_script_neighbour_index_enum_type nbr = HALF_SCRIPT_NBR_X;

   ftm_meas_state.test_level = FTM_MEAS_TEST_LEVEL_FULL_SCRIPT;
   ftm_meas_state.tgt_rxlm_ant0_idx = RXLM_MAX_BUFFERS;
   ftm_meas_state.tgt_rxlm_ant1_idx = RXLM_MAX_BUFFERS;
   for(nbr=HALF_SCRIPT_NBR_X; nbr < HALF_SCRIPT_NBR_MAX; nbr++)
   {
     ftm_meas_state.half_script_nbr_rxlm_ant0_idx[nbr]=RXLM_MAX_BUFFERS;
     ftm_meas_state.half_script_nbr_rxlm_ant1_idx[nbr]=RXLM_MAX_BUFFERS;
   }
   for(mode=RFM_PARKED_MODE; mode < RFM_NUM_MODES; mode++)
   {
     ftm_meas_state.func_tbl_ptr[mode]=NULL;
   }
}

boolean ftm_meas_register(rfm_mode_enum_type tech, ftm_meas_func_tbl_type *func_tbl_ptr)
{
  if(func_tbl_ptr == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_FATAL, "NULL function pointer table");
    return FALSE;
  }

  if(tech >= RFM_NUM_MODES)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL, "Invalid Tech %d",tech);
    return FALSE;
  }

  ftm_meas_state.func_tbl_ptr[tech] = func_tbl_ptr;
  return TRUE;
}

boolean ftm_meas_deregister(rfm_mode_enum_type tech)
{
  if(tech >= RFM_NUM_MODES)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_FATAL, "Invalid Tech %d",tech);
    return FALSE;
  }

  ftm_meas_state.func_tbl_ptr[tech] = NULL;
  return TRUE;
}


void ftm_meas_log_irat_script_info
(
   rfm_meas_program_script_type script_type,
   uint16 num_trx,
   uint16 num_trx_task,
   uint16 num_non_trx,
   uint16 num_non_trx_task,
   uint16 exec_time_us
)
{
  uint8 index = 0;
  if (script_type == RFM_MEAS_PROGRAM_STARTUP)
  {
    index = 0;
  }
  else if (script_type == RFM_MEAS_PROGRAM_CLEANUP)
  {
    index = 1;
  }
  else
  {
    return;
  }
  memset(&(ftm_meas_irat_info[index]),0,sizeof(ftm_meas_irat_script_info_type));
  ftm_meas_irat_info[index].num_trx = num_trx;
  ftm_meas_irat_info[index].num_trx_task = num_trx_task;
  ftm_meas_irat_info[index].num_non_trx = num_non_trx;
  ftm_meas_irat_info[index].num_non_trx_task = num_non_trx_task;
  ftm_meas_irat_info[index].execute_time_us = exec_time_us;
}

/*----------------------------------------------------------------------------*/
void ftm_meas_get_irat_script_info
(
   rfm_meas_program_script_type script_type,
   uint16 *num_trx,
   uint16 *num_trx_task,
   uint16 *num_non_trx,
   uint16 *num_non_trx_task,
   uint16 *exec_time_us
)
{
  uint8 index = 0;
  if (script_type == RFM_MEAS_PROGRAM_STARTUP)
  {
    index = 0;
  }
  else if (script_type == RFM_MEAS_PROGRAM_CLEANUP)
  {
    index = 1;
  }
  else
  {
    return;
  }

  if ((num_trx!= NULL) &&
      (num_trx_task!= NULL) &&
      (num_non_trx!= NULL) &&
      (num_non_trx_task!= NULL) &&
      (exec_time_us!= NULL))
  {
    *num_trx = ftm_meas_irat_info[index].num_trx;
    *num_trx_task = ftm_meas_irat_info[index].num_trx_task;
    *num_non_trx = ftm_meas_irat_info[index].num_non_trx;
    *num_non_trx_task = ftm_meas_irat_info[index].num_non_trx_task;
    *exec_time_us = ftm_meas_irat_info[index].execute_time_us;
  }
}

/*===========================================================================

  FUNCTION: ftm_meas_get_rxagc_results

===========================================================================*/
/*!
    @brief
    This function acts the dispatcher to get RXAGC.

    @param
    1) cell_tech - Technology whose RXAGC is requested
    2) cell_type - Cell type for which RXAGC is requested
    3) rxagc_ptr - ptr to RXAGC array which will be filled up for all carriers.
                   The default value is highest of int16 - 0x7FFF. This indicates
                   that RXAGC is disabled. We expect Tech level API to fill up
                   measured values for active carriers in dB10 and leave others
                   carriers with their default values. Tool will interpret devices
                   with default values as inactive.

    @return
    TRUE  - If function pointer for the requeted Tech is properly defined and
            all other sanity checks have passed.
    FALSE - If sanity check fails
*/
/*=========================================================================*/

boolean ftm_meas_get_rxagc_results
(
  rfm_mode_enum_type cell_tech,
  ftm_meas_cell_type cell_type,
  ftm_meas_rxagc_type *rxagc_ptr
)
{

  rfm_meas_carrier_type meas_carrier_index = 0;
  rfm_meas_rx_dev_handle_type rx_dev_index = 0;
  uint8 carrier_idx = 0;

  ftm_meas_func_tbl_type *cell_func_tbl_ptr = ftm_meas_state.func_tbl_ptr[cell_tech];

  /*Initialize all the RXAGC values to invalid value, which we decided for this case
     to be highest int16 - 0x7FFF*/
  for(meas_carrier_index = 0;meas_carrier_index < RFM_MEAS_CELL_NUM;meas_carrier_index++)
  {
   for(rx_dev_index = 0;rx_dev_index < MAX_RX_DEV_HANDLE_ITEM;rx_dev_index++)
   {
    for(carrier_idx = 0;carrier_idx < 3;carrier_idx++)
    {
     rxagc_ptr->rx_agc[meas_carrier_index][rx_dev_index][carrier_idx] = 0x7FFF;
    }
   }
  }

  if((cell_func_tbl_ptr != NULL) && (cell_func_tbl_ptr->get_meas_rxagc_fp != NULL))
  {
   cell_func_tbl_ptr->get_meas_rxagc_fp(cell_type,rxagc_ptr);
   return TRUE;
  }
  else
  {
   MSG_1(MSG_SSID_RF,MSG_LEGACY_ERROR,
  	   "Functional ptr not defined for Tech %d",cell_tech);
   return FALSE;
  }
}

#endif /* FTM_HAS_IRAT */
