/*
  @file
  rflm_autopin.c

  @brief
  This file contains prototypes and definitions used by RFLM AutoPin.
*/

/*=============================================================================
 
  Copyright (c) 2015 - 2020 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this document are confidential 
  and proprietary information of Qualcomm Technologies Incorporated and all rights therein are 
  expressly reserved. By accepting this material the recipient agrees that this material and the 
  information contained therein are held in confidence and in trust and will not be used, copied, 
  reproduced in whole or in part, nor its contents revealed in any manner to others without the 
  express written permission of Qualcomm Technologies Incorporated.
 
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rflm.mpss/2.11/autopin/src/rflm_autopin.c#1 $

when       who    what, where, why
--------   ---    -------------------------------------------------------------
03/28/20   pvsr   Increas AutoPin buffers to align with Tx handles
06/14/16   ag     Make sure to reset autopin glag only once in cb
                  Move F3s in Autopin cb APIs from Diag to Trace
05/30/16   ag     Remove featurizaion to skip apin in case of high env scale back off
05/28/16   wxl    Check autopin_enable_flag in FED 
05/11/16   wxl    Add API to get Autopin disable bitmask from NV 
04/27/16   qzh    Update RFSW-FW autopin update result interface
04/27/16   qzh    Update log packet after FW release
04/15/16   qzh    Fix the pout compensation sign issue.
04/13/16   qzh    Fix the rounding issue of auto-pin/pout calc
04/13/16   qzh    Update code not to use float when pin/pout calculation from LUT
04/07/16   qzh    Fix pin accum issue for one-shot autopin
04/07/16   qzh    Fix validation check limit issue for one-shot autopin
03/17/16   zhh    move the 0x18fb logging to the autopin_proc_result API
03/07/16   zhh    Added new API to calculate the online pin misalign and delta pout
03/08/16   qzh    Add WTR3925 AMAM autopin Txagc related changes
02/18/16   qzh    WTR3925 Autopin port from Thor.
02/01/16   whz    Log packet support
02/01/16   vs     Copy peak avg power from FW
01/07/16   vs     Enable Autopin across W/1x/TDS/LTE techs
12/31/15   vs     Remove Apin run check based on Envscale
12/30/15   ak     Disable autopin for WCDMA
12/24/15   vr     Autopin check-in_2
12/22/15   am     Disable Autopin for LTE temporarily
12/22/15   am     Enable Autopin for LTE
12/09/15   kma    Update REFLOG PAR to 32 bits
12/03/15   whz    Remove unncessary check in run_decsion
11/19/15   ag     Remove counter in order to schedule autopin for W every time
11/05/15   whz    AutoPin on TA: online support
11/03/15   kma    Featurize for TA modem
10/30/15   kma    Added autopin FW msg
10/29/15   whz    Remove timers completely
10/28/15   whz    Stop increasing Pin accum when RGI is max
10/21/15   whz    Version 2 log packets
                  Optimize log context ID usage
10/14/15   whz    Remove timers as we schedule AutoPin every time
09/24/15   kma    Enable CDMA autopin
09/21/15   whz    Update the definition of Log Packets again
09/17/15   kma    Enabled debug msg in diag for critical error 
                  Removed TDS check for ENV scaling value
09/10/15   aa     Enable WCDMA autopin
09/09/15   aa     autopin run deecision initialization fix
09/03/15   whz    Update the definition of Log Packets
09/03/15   whz    Enable TDSCDMA AutoPin
08/25/15   cdb    Enable LTE AutoPin
08/25/15   cdb    Featurization update
08/20/15   cdb    Add env_scale backoff criteria to autopin run decision
07/16/15   cdb    Add EFS support 
07/16/15   cdb    Abstract Thor/AMAM specific processing 
06/30/15   cdb    Add override controls
06/25/15   cdb    Remove mutex, add error checking
06/23/15   whz    Do not check log_status when doing log packet allocation
06/10/15   whz    Provide buffer to store FBRx IQ
06/10/15   cdb    Update FW API
06/08/15   whz    Add new API to return the latest Pin accum
06/05/15   whz    Don't include timer.h for FW standalone build
06/04/15   whz    Initial version

===========================================================================*/

#include "rflm.h"
#include "rflm_diag_log.h"
#include "rflm_autopin.h"
#include "rflm_dm_api.h"
#include "rflm_diag.h"
#include "rflm_wcdma_msg.h"
#include "rflm_wcdma_sw_api.h"
#include "rflm_tds_msg_proc.h"
#include "rflm_autopin_log.h"
#include "rflm_c2k_msg.h"
#ifdef FEATURE_RF_HAS_WTR2965
#include "rflm_cmn_msg.h"
#endif
#include <qurt.h>
//#ifdef FEATURE_RF_HAS_WTR3925
#include "fw_psamp_autopin_intf.h"
#include "fw_psamp_volterra_intf.h"
//#endif
#if defined(FEATURE_RF_AUTOPIN) && (RFLM_FEATURE_BUILD_MODE!=RFLM_FEATURE_BUILD_FW_LIB)
#include "rfcommon_autopin_db.h"
#endif
#include "rflm_txagc_api.h"

#define RFLM_AUTOPIN_DB100_TO_DB10_ROUNDING(value)  ((value > 0)? ((value+5)/10) : ((value-5)/10))

//#ifdef FEATURE_RF_HAS_WTR2965
fw_autopin_proc rflm_autopin_fw_autopin_fn = NULL;
//#endif
static rflm_autopin_dm_template_t rflm_autopin_dm_bufs[RFLM_AUTOPIN_MAX_BUF];

boolean rflm_autopin_in_progress = FALSE;

static int32 RFLM_AUTOPIN_TRK_THRESH[RFLM_MAX_TECH];

static uint8 RFLM_AUTOPIN_STEP_SIZE[RFLM_MAX_TECH];

uint32 rflm_autopin_seq_nr = 0;

static int rflm_autopin_log_context_id[2] = { -1, -1 };
//#ifdef FEATURE_RF_HAS_WTR2965
rflm_handle_tx_t rflm_autopin_current_active_tx_handle = 0xFFFFFFFF;
//#endif

/* Check autopin_enable_flag in rflm_dm */

boolean rflm_autopin_is_enabled(rflm_tech_id_t tech, rflm_handle_tx_t handle)
{
#if defined(FEATURE_RF_AUTOPIN) && (RFLM_FEATURE_BUILD_MODE!=RFLM_FEATURE_BUILD_FW_LIB)
  boolean autopin_enable_flag = FALSE;
  rflm_autopin_dm_template_t *autopin_template = NULL;

  switch (tech)
    {
      case RFLM_TECH_WCDMA:
      case RFLM_TECH_TDSCDMA:
      case RFLM_TECH_1X:
      case RFLM_TECH_HDR:
      case RFLM_TECH_LTE:
        autopin_template = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(handle);
        if(NULL != autopin_template)
        {
          autopin_enable_flag = autopin_template->autopin_ctl.autopin_enable_flag;
        }
        else
        {
          autopin_enable_flag = FALSE;
        }
        break;

      default:
        break;
    }
  //RFLM_DIAG_MSG_HIGH("handle %d, autopin_enable_flag %d",handle, autopin_enable_flag);
  return autopin_enable_flag;
#else
  return FALSE;
#endif
}

/* Get run decision in RFLM
 * Used in Tech's txagc_update or like API
 */
rflm_err_t rflm_autopin_get_run_decision( rflm_handle_tx_t handle,
                                          rflm_autopin_get_run_decision_in_t  *in,
                                          rflm_autopin_get_run_decision_out_t *out )
{
  rflm_autopin_dm_template_t *autopin_data_ptr;
  autopin_data_ptr = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(handle);

  /* Init to FALSE */
  out->run_autopin = FALSE;

  if(EXPECT(autopin_data_ptr == NULL,FALSE))
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_run_decision: No valid DM buf ptr with Tx handle %d",
                          handle);

    return RFLM_ERR_API_FAILED;
  }

  if (autopin_data_ptr->override_ctl.debug_active && autopin_data_ptr->override_ctl.pin_run_freeze)
  {
	return RFLM_ERR_NONE;
  }

  if(EXPECT((autopin_data_ptr->autopin_ctl.init_done != TRUE),FALSE))
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_run_decision: ERROR: Tech %d, Tx handle %d. Reason: Init done %d",
						  autopin_data_ptr->autopin_ctl.tech, 
						  handle, 
						  autopin_data_ptr->autopin_ctl.init_done);
    return RFLM_ERR_API_FAILED ;

  }
  
  if (EXPECT((rflm_autopin_in_progress == TRUE),FALSE))
  {
    RFLM_DIAG_TRACE_HIGH(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_run_decision: No, in progress already");
	return RFLM_ERR_NONE;
  }

   if ((in->xpt_mode == RFLM_TXAGC_EPT) || (in->xpt_mode == RFLM_TXAGC_ET))
   {
     
      /* get the env_scale backoff from tx agc */
      autopin_data_ptr->autopin_ctl.env_scale_backoff = rflm_autopin_get_env_scale_backoff(handle);
      
      if (autopin_data_ptr->autopin_ctl.env_scale_backoff < RFLM_AUTOPIN_ENV_SCALE_BACKOFF_LIMIT)

      {
      rflm_autopin_in_progress = TRUE;
      autopin_data_ptr->autopin_ctl.handle = handle; /* currently running caller */
      rflm_autopin_current_active_tx_handle = handle;
      autopin_data_ptr->autopin_ctl.pa_state = in->pa_state;
      out->run_autopin = TRUE;
                      
      rflm_autopin_seq_nr++;	  
                      
       RFLM_DIAG_TRACE_HIGH(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, 
                            "rflm_autopin_get_run_decision: Yes, Tech %d, Tx handle %d, PA state %d",
                            autopin_data_ptr->autopin_ctl.tech, 
                            handle, 
                            in->pa_state);
   }

      else
      {
        //RFLM_DIAG_MSG_HIGH("rflm_autopin_get_run_decision: No, Tech %d, Tx handle %d, PA state %d. Reason: env scale backoff %d",
        //                 autopin_data_ptr->autopin_ctl.tech, handle, in->pa_state, autopin_data_ptr->autopin_ctl.env_scale_backoff);      
        RFLM_DIAG_TRACE_HIGH(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_run_decision: No, Tech %d, Tx handle %d, PA state %d. Reason: env scale backoff %d",
                               autopin_data_ptr->autopin_ctl.tech, handle, in->pa_state, autopin_data_ptr->autopin_ctl.env_scale_backoff);      
      }

   }
   else
   {
       RFLM_DIAG_TRACE_HIGH(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, 
	                   "rflm_autopin_get_run_decision: No, Tech %d, Tx handle %d, PA state %d. Reason: xpt_mode %d",
                            autopin_data_ptr->autopin_ctl.tech, 
                            handle, 
                            in->pa_state, 
                            in->xpt_mode);
   }

  if (out->run_autopin == TRUE)
  {
    rflm_autopin_seq_nr++;
  }

#ifdef FEATURE_RF_HAS_WTR3925
  autopin_data_ptr->autopin_ctl.run_decision = out->run_autopin;
#else
   rflm_autopin_logging_control_info(autopin_data_ptr, out->run_autopin, rflm_autopin_seq_nr);
#endif

  return RFLM_ERR_NONE;
}


rflm_err_t rflm_autopin_update_proc_result( rflm_handle_tx_t handle,
                                            rflm_autopin_proc_results_t *results )
{
  rflm_autopin_dm_template_t *autopin_data_ptr;
  int32 meas_error;
  rflm_err_t update_result;
  
  /* Error checks on the handle */

  autopin_data_ptr = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(handle);

  if (autopin_data_ptr == NULL)
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,
                          "rflm_autopin_update_proc_result: No valid DM buf ptr with Tx handle %d", handle); 
    rflm_autopin_in_progress = FALSE;
    return RFLM_ERR_API_FAILED;
  }

  if (autopin_data_ptr->autopin_ctl.handle != handle) 
  {
    RFLM_DIAG_MSG_HIGH("rflm_autopin_update_proc_result: Tx Handle mismatch, handle %d, expected %d", handle, autopin_data_ptr->autopin_ctl.handle); 
    rflm_autopin_in_progress = FALSE;
    return RFLM_ERR_API_FAILED;
  }

  /* Error checks on the results */

  if (results == NULL) {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_update_proc_result: handle %d, results NULL", handle);
    rflm_autopin_in_progress = FALSE;
    return RFLM_ERR_API_FAILED;
  }

  results->seq_nr = rflm_autopin_seq_nr;

  if (results->valid_result == FALSE)
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_update_proc_result: No valid result received", 0);
    rflm_autopin_in_progress = FALSE;
    return RFLM_ERR_API_FAILED;
  }

  meas_error = results->meas_error;

  rflm_dm_set_autopin_data(handle,
                           RFLM_AUTOPIN_UPDATE_PROC_RESULTS,
                           (void *)results);

  if (autopin_data_ptr->override_ctl.debug_active && autopin_data_ptr->override_ctl.pin_accum_freeze)
  {
    rflm_autopin_in_progress = FALSE;
    return RFLM_ERR_NONE;
  }

  /* positive meas error requires a reduction of the Pin gain
   * negative meas error requires an increase of the Pin gain
   * the pin_accum represents the amount of Pin error accumulated
   * positive Pin error accum requires a reduction of the Pin gain
   * negative Pin error accum requires an increase of the Pin gain */

  if(RFLM_AUTOPIN_AMAM_ONESHOT == autopin_data_ptr->autopin_ctl.proc_type)
  {
    update_result = rflm_autopin_update_one_shot_delta_pin_pout(handle, meas_error);

    if(update_result == RFLM_ERR_API_FAILED)
    {
      RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_update_proc_result:" 
                                                           "One shot delta pin pout() returns FAIL!");
      rflm_autopin_in_progress = FALSE;
      return RFLM_ERR_NONE;
    }
	
  }
  else if(RFLM_AUTOPIN_AMAM_LEGACY == autopin_data_ptr->autopin_ctl.proc_type)
  {
    update_result = rflm_autopin_update_legacy_pin_accum( handle, meas_error);

    if(update_result == RFLM_ERR_API_FAILED)
    {
      RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_update_proc_result:" 
                                                           "Legacy pin accum() returns FAIL!");
      rflm_autopin_in_progress = FALSE;
      return RFLM_ERR_NONE;
    }
  }
  else
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_update_proc_result: Invalid proc_type %d !",
                                                          autopin_data_ptr->autopin_ctl.proc_type);
    rflm_autopin_in_progress = FALSE;
    return RFLM_ERR_API_FAILED;
  }

  switch (autopin_data_ptr->autopin_ctl.tech)
  {
  case RFLM_TECH_WCDMA:
    rflm_wcdma_update_pin(handle,
                          autopin_data_ptr->autopin_ctl.pa_state,
                          autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state]);
    break;

  case RFLM_TECH_TDSCDMA:
    rflm_tds_msgr_send_autopin_trigger(handle,
                                       autopin_data_ptr->autopin_ctl.pa_state,
                                       autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state]);
    break;

  case RFLM_TECH_1X:
  case RFLM_TECH_HDR:
#ifdef FEATURE_RF_HAS_WTR3925
    rflm_cdma_update_pin(handle,
                         autopin_data_ptr->autopin_ctl.pa_state,
                      autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state]);
#endif
    break;

  case RFLM_TECH_LTE:
    break;

  default:
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,
                          "rflm_autopin_update_proc_result: Tech %d is not supported", autopin_data_ptr->autopin_ctl.tech);
    break;
  }
  rflm_autopin_in_progress = FALSE;

  /* flush the log packet 0x18FB*/
  /* if the delta pout update success, then set the status to TRUE*/
  if(RFLM_ERR_NONE == update_result)
  {
    results->status = TRUE;
  }
  else
  {
    results->status = FALSE;
  }

#ifdef FEATURE_RF_HAS_WTR3925
  rflm_autopin_logging_dm_info(autopin_data_ptr, rflm_autopin_seq_nr, results);
#endif
  //RFLM_DIAG_MSG_HIGH("rflm_autopin_update_proc_result: handle %d, meas_error %d, pin_accum %d, pa_state %d", 
  //                   handle, 
  //                   results->meas_error,
  //                   autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state],      
  //                   autopin_data_ptr->autopin_ctl.pa_state);

  RFLM_DIAG_TRACE_HIGH(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,
                       "rflm_autopin_update_proc_result: handle %d, meas_error %d, pin_accum %d, pa_state %d", 
                       handle, 
                       results->meas_error,
                       autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state],      
                       autopin_data_ptr->autopin_ctl.pa_state);
  
  return RFLM_ERR_NONE;
} /* rflm_autopin_update_proc_result */


rflm_err_t rflm_autopin_get_latest_pin(rflm_handle_tx_t handle, uint8 pa_state, rflm_db10_t *pin)
{
  rflm_autopin_dm_template_t *autopin_data_ptr;

  rflm_tech_id_t tech = RFLM_MAX_TECH;

  autopin_data_ptr = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(handle);

  if (autopin_data_ptr == NULL)
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_latest_pin: No valid DM buf ptr with Tx handle %d",
                       handle);

    return RFLM_ERR_API_FAILED;
  }

  if (pa_state >= RFLM_CMN_RXAGC_MAX_LNA_STATES)
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_latest_pin: invalid PA state %d",
                       pa_state);

    return RFLM_ERR_API_FAILED;
  }

  tech = autopin_data_ptr->autopin_ctl.tech;
  
  if (rflm_autopin_is_enabled(tech, handle) == FALSE)
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_latest_pin: AutoPin not enabled on Tech %d with Tx handle %d",
                          tech, handle);

    return RFLM_ERR_API_FAILED;
  }

  *pin = autopin_data_ptr->autopin_ctl.pin_accum[pa_state];

  return RFLM_ERR_NONE;
}

rflm_err_t rflm_autopin_get_latest_pout(rflm_handle_tx_t handle, rflm_db10_t *pout)
{
  rflm_autopin_dm_template_t *autopin_data_ptr;

  rflm_tech_id_t tech = RFLM_MAX_TECH;

  autopin_data_ptr = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(handle);

  if (autopin_data_ptr == NULL)
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_latest_pout: No valid DM buf ptr with Tx handle %d",
                       handle);

    return RFLM_ERR_API_FAILED;
  }

  tech = autopin_data_ptr->autopin_ctl.tech;
  
  if (rflm_autopin_is_enabled(tech, handle) == FALSE)
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_latest_pout: AutoPin not enabled on Tech %d with Tx handle %d",
                          tech, handle);

    return RFLM_ERR_API_FAILED;
  }

  *pout = -autopin_data_ptr->autopin_ctl.pout_delta;

  return RFLM_ERR_NONE;
}


rflm_err_t rflm_autopin_set_data(void *autopin_data,
                                 rflm_autopin_cmd_e cmd,
                                 const void *payload)
{
  rflm_err_t ret_val = RFLM_ERR_API_FAILED;

  rflm_autopin_dm_template_t *autopin_ptr;
  
  if (autopin_data == NULL)
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_set_data: Null pointer for AutoPin buffer");
    return ret_val;
  }

  autopin_ptr = (rflm_autopin_dm_template_t *)autopin_data;

  switch (cmd)
  {
  case RFLM_AUTOPIN_UPDATE_CTL_DATA:
    if (payload != NULL)
    {
      memscpy(&(autopin_ptr->autopin_ctl),
              sizeof(rflm_autopin_ctl_data_t),
              (rflm_autopin_ctl_data_t *)payload,
              sizeof(rflm_autopin_ctl_data_t));
    }

    ret_val = RFLM_ERR_NONE;
    
    break;
    
  case RFLM_AUTOPIN_UPDATE_PROC_RESULTS:
    if (payload != NULL)
    {
      memscpy(&(autopin_ptr->results_log),
              sizeof(rflm_autopin_proc_results_t),
              (rflm_autopin_proc_results_t *)payload,
              sizeof(rflm_autopin_proc_results_t));
    }

    ret_val = RFLM_ERR_NONE;
    
    break;
    
  default:
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,
                          "rflm_autopin_set_data: unsupported command %d ", (int16)cmd); 
    ret_val = RFLM_ERR_API_FAILED;
  }

  if ( ret_val != RFLM_ERR_NONE )
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_set_data: API failed for command %d", (int16)cmd); 
  }
    
  return ret_val;
}


rflm_err_t rflm_autopin_get_data(void *autopin_data,
                                 rflm_autopin_cmd_e cmd,
                                 void *payload)
{
  rflm_err_t ret_val = RFLM_ERR_API_FAILED;

  rflm_autopin_dm_template_t *autopin_ptr;

  if ( autopin_data == NULL ) 
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_data: Null pointer for AutoPin buffer");
    return ret_val;
  }

  autopin_ptr = (rflm_autopin_dm_template_t *)autopin_data;

  switch (cmd)
  {
  case RFLM_AUTOPIN_GET_CTL_DATA:
    if (payload != NULL)
    {
      memscpy(payload,
              sizeof(rflm_autopin_ctl_data_t),
              &(autopin_ptr->autopin_ctl),
              sizeof(rflm_autopin_ctl_data_t));
    }

    ret_val = RFLM_ERR_NONE;
    
    break;
    
  case RFLM_AUTOPIN_GET_PROC_RESULTS:
    if (payload != NULL)
    {
      memscpy(payload,
              sizeof(rflm_autopin_proc_results_t),
              &(autopin_ptr->results_log),
              sizeof(rflm_autopin_proc_results_t));
    }

    ret_val = RFLM_ERR_NONE;
    
    break;

  default:
    RFLM_DIAG_TRACE_ERROR (RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_data: unsupported command %d ", (int16)cmd); 
    ret_val = RFLM_ERR_API_FAILED;
  }

  if ( ret_val != RFLM_ERR_NONE )
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_data: API failed for command %d", (int16)cmd); 
  }

  return ret_val;
}


rflm_err_t rflm_autopin_dup_data(void *new_autopin_data,
                                 void *old_autopin_data)
{
  memscpy(new_autopin_data,
          sizeof(rflm_autopin_dm_template_t),
          old_autopin_data,
          sizeof(rflm_autopin_dm_template_t));

  return RFLM_ERR_NONE;
}


void rflm_autopin_init_buf(rflm_autopin_dm_template_t *template_ptr)
{
  rflm_autopin_cleanup_buf(template_ptr);
}


void rflm_autopin_cleanup_buf(rflm_autopin_dm_template_t *template_ptr)
{
  memset(template_ptr, 0, sizeof(rflm_autopin_dm_template_t));

  template_ptr->autopin_ctl.tech = RFLM_MAX_TECH;

  template_ptr->autopin_ctl.handle = 0xFFFFFFFF;

#ifdef FEATURE_RF_HAS_WTR3925
  template_ptr->autopin_ctl.logging_dm_info_context_id = rflm_autopin_log_context_id[0];
  template_ptr->autopin_ctl.logging_iq_capture_context_id = rflm_autopin_log_context_id[1];
#endif
}


rflm_autopin_dm_template_t *rflm_autopin_assign_buf()
{
  int i;
  
  rflm_autopin_dm_template_t *autopin_template = NULL;

  for (i = 0; i < RFLM_AUTOPIN_MAX_BUF; i++)
  {
    if (rflm_autopin_dm_bufs[i].autopin_ctl.buf_in_use == FALSE)
    {
      autopin_template = &rflm_autopin_dm_bufs[i];
      rflm_autopin_init_buf(autopin_template);
      autopin_template->autopin_ctl.buf_in_use = TRUE;

      /* If we enable QXDM check here, we cannot enable QXDM AutoPin logpacket on the fly.
       * That is, we have to power cycle to let context ID be allocated.
       * if (log_status((uint16)RFLM_LOG_PACKET_AUTOPIN_DM_INFO_ID) == TRUE)
       * if (log_status((uint16)RFLM_LOG_PACKET_AUTOPIN_IQ_CAPTURE_ID) == TRUE)*/

      if (rflm_autopin_log_context_id[0] == -1)
      {
        autopin_template->autopin_ctl.logging_dm_info_context_id =
          rflm_diag_allocate_context(RFLM_LOG_PACKET_AUTOPIN_DM_INFO_ID,
                                     0,
                                     4);

        if (rflm_diag_validate_context(autopin_template->autopin_ctl.logging_dm_info_context_id) != TRUE)
        {
#ifdef FEATURE_RF_HAS_WTR3925
          rflm_autopin_log_context_id[0] = -1;
          autopin_template->autopin_ctl.logging_dm_info_context_id = -1;
#endif
          RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_assign_buf: failed to allocate context id", RFLM_LOG_PACKET_AUTOPIN_DM_INFO_ID);
        }
        else
        {
          rflm_autopin_log_context_id[0] = autopin_template->autopin_ctl.logging_dm_info_context_id;
        }
      }
      else
      {
        autopin_template->autopin_ctl.logging_dm_info_context_id = rflm_autopin_log_context_id[0];
      }
#ifdef FEATURE_RF_HAS_WTR3925
      if (log_status((uint16)RFLM_LOG_PACKET_AUTOPIN_IQ_CAPTURE_ID) == TRUE)
      {
#endif
      if (rflm_autopin_log_context_id[1] == -1)
      {
        autopin_template->autopin_ctl.logging_iq_capture_context_id =
          rflm_diag_allocate_context(RFLM_LOG_PACKET_AUTOPIN_IQ_CAPTURE_ID,
                                     0,
                                     4);

        if (rflm_diag_validate_context(autopin_template->autopin_ctl.logging_iq_capture_context_id) != TRUE)
        {
#ifdef FEATURE_RF_HAS_WTR3925
            rflm_autopin_log_context_id[1] = -1;
            autopin_template->autopin_ctl.logging_iq_capture_context_id = -1;
#endif
          RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_assign_buf: failed to allocate context id", RFLM_LOG_PACKET_AUTOPIN_IQ_CAPTURE_ID);
        }
        else
        {
          rflm_autopin_log_context_id[1] = autopin_template->autopin_ctl.logging_iq_capture_context_id;
        }
      }
      else
      {
        autopin_template->autopin_ctl.logging_iq_capture_context_id = rflm_autopin_log_context_id[1];
      }
#ifdef FEATURE_RF_HAS_WTR3925
      }
      else
      {
        RFLM_DIAG_MSG_ERROR("AutoPin Logging: Auto Pin IQ captures log packet 0x18FC is disabled in qxdm");
      }
#endif
      break;
    }
  }

  return autopin_template;
}


boolean rflm_autopin_release_buf(rflm_autopin_dm_template_t *autopin_template)
{
  int8 i = 0;
  boolean fully_released = FALSE;

  if (autopin_template->autopin_ctl.buf_in_use == TRUE)
  {
    autopin_template->autopin_ctl.buf_in_use = FALSE;
  }

  for (i = 0; i < 4; i++)
  {
    if (rflm_autopin_dm_bufs[i].autopin_ctl.buf_in_use == TRUE)
    {
      break;
    }
    fully_released = TRUE;
  }

  if (fully_released)
  {
    if(rflm_diag_validate_context(autopin_template->autopin_ctl.logging_dm_info_context_id) == TRUE)
    {
      rflm_diag_deallocate_context(autopin_template->autopin_ctl.logging_dm_info_context_id);
      rflm_autopin_log_context_id[0] = -1;
#ifdef FEATURE_RF_HAS_WTR3925
      autopin_template->autopin_ctl.logging_dm_info_context_id = -1;
#endif
    }

    if (rflm_diag_validate_context(autopin_template->autopin_ctl.logging_iq_capture_context_id) == TRUE)
    {
      rflm_diag_deallocate_context(autopin_template->autopin_ctl.logging_iq_capture_context_id);
      rflm_autopin_log_context_id[1] = -1;
#ifdef FEATURE_RF_HAS_WTR3925
      autopin_template->autopin_ctl.logging_iq_capture_context_id = -1;
#endif
    }
  }

  return TRUE;
}

void rflm_autopin_set_params(rflm_tech_id_t tech, int32 trk_thresh, uint8 step_size)
{
  if (tech < RFLM_MAX_TECH)
  {
    RFLM_AUTOPIN_TRK_THRESH[tech] = trk_thresh;
    RFLM_AUTOPIN_STEP_SIZE[tech] = step_size;
  }  
}

rflm_err_t rflm_autopin_update_env_scale(rflm_handle_tx_t handle, uint32 env_scale_backoff)
{
  rflm_autopin_dm_template_t *autopin_data_ptr;
  
  autopin_data_ptr = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(handle);
  
  if (autopin_data_ptr!=NULL)
  {
    autopin_data_ptr->autopin_ctl.env_scale_backoff = env_scale_backoff;
    return RFLM_ERR_NONE;
  }
  else
  {
    return RFLM_ERR_API_FAILED;
  }
}

uint32 rflm_autopin_get_env_scale_backoff(rflm_handle_tx_t tx_handle)
{
  /*! @brief  get txagc_template ptr from handle_id */
  rflm_txagc_event_template_t *txagc_template;
  rflm_txagc_params_t *txagc_param_pt;
  
  txagc_template = (rflm_txagc_event_template_t*) rflm_dm_get_tx_agc_ptr( (rflm_dm_handle_id_t) tx_handle);
  ASSERT(txagc_template != NULL );
  txagc_param_pt = &(txagc_template->params);
  
  return txagc_param_pt->txagc_meta[txagc_param_pt->current_param_idx].env_scale;
}

boolean rflm_autopin_is_rgi_max(rflm_handle_tx_t tx_handle)
{
  rflm_txagc_event_template_t *txagc_template = NULL;
  rflm_txagc_params_t *txagc_param_ptr = NULL;
  rflm_txagc_static_t *txagc_static_data_ptr = NULL;
  uint8 row_idx_const;
  rflm_txagc_pa_state_data_t pa_state;
  uint16 rgi_value_current;
  uint16 rgi_value_next;
  uint32 iq_gain;
  
  txagc_template = (rflm_txagc_event_template_t*) rflm_dm_get_tx_agc_ptr((rflm_dm_handle_id_t)tx_handle);

  if ( txagc_template == NULL ) 
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_is_rgi_max: No valid TxAGC DM buffer ptr with Tx handle %d", tx_handle);
    return TRUE;  // the intention here is to avoid Pin accum update when something wrong
  }

  txagc_param_ptr = &txagc_template->params;

  row_idx_const = txagc_param_ptr->txagc_meta[txagc_param_ptr->current_param_idx].row_idx_const;
  pa_state = txagc_param_ptr->txagc_meta[txagc_param_ptr->current_param_idx].pa_state;
  iq_gain = txagc_param_ptr->txagc_meta[txagc_param_ptr->current_param_idx].iq_gain;

  txagc_static_data_ptr = &txagc_template->txagc_static_data;

  rgi_value_current = txagc_static_data_ptr->tx_lin_static[pa_state].txagc_static_lut_script[row_idx_const].rgi_val;
  rgi_value_next = (row_idx_const == RFLM_TXAGC_TX_LIN_APT_SIZE-1) ? rgi_value_current : txagc_static_data_ptr->tx_lin_static[pa_state].txagc_static_lut_script[row_idx_const+1].rgi_val;

  RFLM_DIAG_MSG_HIGH("rflm_autopin_is_rgi_max: handle %d, RGI index %d, RGI value %d, IQ gain backoff %d", 
                     tx_handle,
                     row_idx_const,
                     rgi_value_current,
                     iq_gain);

  if (rgi_value_current == rgi_value_next
      && iq_gain == 0)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  RFLM API called by FW_PSAMP to deliver AMAM processing result to AutoPin. 
 
  @details
  This function implement the API, and abstracts AMAM specific handling from 
  generic AutoPin results processing. API from Thor, will change to new API 
  rflm_autopin_update_fw_volterra_proc_result() after FW integrated.
 
  @param pout
  FW_PSAMP's amam processing output. Structure type defined by FW_PSAMP
 
  @return
  void
 
*/
void rflm_autopin_update_fw_amam_proc_result(fw_psamp_autopin_result_s * pout)
{
#ifdef FEATURE_RF_HAS_WTR3925
  rflm_autopin_proc_results_t results;
  if (pout != NULL)
  {
    rflm_autopin_logging_iq_capture(pout, rflm_autopin_seq_nr);

    results.valid_result = (pout->status == 0)&&(pout->corr_snr >= FWPSAMP_CORR_SNR_THRESHOLD);
    if (!results.valid_result)
    {
      RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_update_fw_amam_proc_result: FW result not valid, status %d corr_snr %d",pout->status, pout->corr_snr);
      //RFLM_DIAG_TRACE_HIGH(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_update_fw_amam_proc_result: FW result not valid, status %d corr_snr %d",
      //                     pout->status, pout->corr_snr);
    }

    results.meas_error = pout->amam_err;
    results.ss_gain_db = pout->ss_gain_db;
    results.reflog_max_idx = pout->reflog_max_idx;
    memscpy(results.kernal_weights_real,
            FW_PSAMP_VOLTERRA_MAX_KERNELS*sizeof(int16),
            pout->kernal_weights_real,
            FW_PSAMP_VOLTERRA_MAX_KERNELS*sizeof(int16));
    memscpy(results.kernal_weights_imag,
            FW_PSAMP_VOLTERRA_MAX_KERNELS*sizeof(int16),
            pout->kernal_weights_imag,
            FW_PSAMP_VOLTERRA_MAX_KERNELS*sizeof(int16));
    if (RFLM_ERR_NONE != rflm_autopin_update_proc_result( (rflm_handle_tx_t)pout->handle, &results ))
    {
      //ASSERT(1==0);
    }
  }
  else
  {
    //RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,
    //                     "rflm_autopin_update_fw_amam_proc_result: NULL ptr");
   RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_update_fw_amam_proc_result: NULL ptr");
    //ASSERT(1==0);
  }
#endif   
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  RFLM API called by FW_PSAMP to deliver AMAM processing result to AutoPin
 
  @details
  This function implement the API, and abstracts AMAM specific handling from 
  generic AutoPin results processing. 
 
  @param pout
  FW_PSAMP's amam processing output. Structure type defined by FW_PSAMP
 
  @return
  void
 
*/

void rflm_autopin_update_fw_volterra_proc_result(fw_psamp_volterra_job_s *pout)
{
#ifdef FEATURE_RF_HAS_WTR3925
  rflm_autopin_proc_results_t results;

  if (pout != NULL)
  {
    rflm_autopin_logging_iq_capture_volterra(pout, rflm_autopin_seq_nr);

    results.valid_result = (pout->result.status == 0)&&(pout->result.corr_snr >= FWPSAMP_CORR_SNR_THRESHOLD);
    if (!results.valid_result)
    {
      RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_update_fw_amam_proc_result: FW result not valid, status %d corr_snr %d",pout->result.status, pout->result.corr_snr);
      //RFLM_DIAG_TRACE_HIGH(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_update_fw_amam_proc_result: FW result not valid, status %d corr_snr %d",
      //                     pout->status, pout->corr_snr);
    }

    results.meas_error = pout->result.apin_err;
    results.ss_gain_db = pout->result.rx_gain_low;
    results.reflog_max_idx = pout->result.tx_max;
    for (uint32 i = 0; i < FW_PSAMP_VOLTERRA_MAX_KERNELS; i++)
    {
      results.kernal_weights_real[i] = pout->result.row0_weight[i].I;
      results.kernal_weights_imag[i] = pout->result.row0_weight[i].Q;
    }
    if (RFLM_ERR_NONE != rflm_autopin_update_proc_result( (rflm_handle_tx_t)pout->param.handle, &results ))
    {
      //ASSERT(1==0);
    }
  }
  else
  {
    //RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,
    //                     "rflm_autopin_update_fw_amam_proc_result: NULL ptr");
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_update_fw_amam_proc_result: NULL ptr");
    rflm_autopin_in_progress = FALSE;
	//ASSERT(1==0);
  }
#endif  

}



void rflm_autopin_update_fw_par_proc_result(fw_autopin_output_params_t *pout)
{
#ifdef FEATURE_RF_HAS_WTR2965

  errno_enum_type err;
  rflm_cmn_autopin_rsp_t msg;

  msgr_init_hdr(&msg.msg_hdr, MSGR_RFLM_CMN, RFLM_CMN_AUTOPIN_RSP);
  msg.msg_rsp.par = pout->result.par;
  msg.msg_rsp.valid_par = pout->result.valid_result;
  msg.msg_rsp.peak_avg_pwr = pout->result.peak_avg_pwr;
  msg.msg_rsp.mean_pwr = pout->result.mean_pwr;
  
  err = msgr_send (&msg.msg_hdr, sizeof(msg));

  RFLM_DIAG_TRACE_MED ( RFLM_DIAG_MSG_SSID_WCDMA, "rflm_autopin_update_fw_par_proc_result");

  if( err != E_SUCCESS )
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "msgr_send error: %d", err);
  }
#endif
  
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  RFLM API called by calculate the pin_misalign and pout_delta
 
  @details
  This function implement the API, Tech RFLM module will use this API to get the pin misalign
  and pout_delta according to the gain compress or expand value. Below is the algorithm,

   This function computes
   ((Y[i] - Y[i-1])/(Gain_comp[i] - Gain_comp[i-1])*(gain_comp - Gain_comp[i-1]) 
   + Gain_comp[i-1] for an input value gain_comp which falls between the points
   [Gain_comp[i-1], Gain_comp[i]] to get pin_misalign.

   After get the current pin_misalign, we calculate a new pin accumulate:
   pin_accum = pin_accum_previous + pin_misalign. Then set the pin_accum as the 
   new Gain_comp to get the delta Pout with the same equation
   
   During the calculation of the pin_misalign, the table Y should be 
   pin_pout_lut.lut[dpd_idx].pin_misalign. When we calculate delta Pout, the table Y 
   is the pin_pout_lut.lut[dpd_idx].pout_delta.

   Table pin_pout_lut is stored in the rflm_dm.handle_pool.buf[handle_id].autopin_ptr, 
   we can get it the with the Tx handle. It contains all the gain_comp/pin_misalign/
   pout_delta tables.
 
 @param params
  rflm_handle_tx_handle hanle  the current tx handle to indexing the autopin_ptr
  int32 gain_comp the gain compress or expand value of previous IQ capture
 
  @return
  rflm_err_t: indicate the calc is done or not
 
*/
rflm_err_t 
rflm_autopin_update_one_shot_delta_pin_pout
(
  rflm_handle_tx_t handle, 
  int32 gain_comp
)
{
  rflm_autopin_dm_template_t *autopin_data_ptr;
  uint8 dpd_idx;
  uint8 lp_var;
  int32 pin_misalign = 0;
  int32 pout_delta = 0;
  int32 pin_accum = 0;
  int32 gain_prev = 0;
  int32 gain_next = 0;
  int32 intep_prev = 0;
  int32 intep_next = 0;
  int8 temp_db10 = 0;

  rflm_tech_id_t tech = RFLM_MAX_TECH;
  rflm_txagc_event_template_t *txagc_template;

  autopin_data_ptr = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(handle);
  txagc_template = (rflm_txagc_event_template_t *)rflm_dm_get_tx_agc_ptr( (rflm_dm_handle_id_t) handle);

  /* for the type of calibrate data is int8, need convert the input gain_comp to int8 first*/
  if(((gain_comp / 10) > 127) || ((gain_comp / 10) < -127))
  {
     RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_get_delta_pin_pout:Input gain_comp %d is invalid!",
                       gain_comp);

    return RFLM_ERR_API_FAILED;;
  }

  /* parameters check */
  if (NULL == autopin_data_ptr)
  {
     RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_get_delta_pin_pout: No valid DM buf ptr with Tx handle %d",
                       handle);

    return RFLM_ERR_API_FAILED;
  }

  if (NULL == txagc_template)
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_get_delta_pin_pout: No valid TxAgc Template for Tx handle %d",
                       handle);

    return RFLM_ERR_API_FAILED;
  }

  tech = autopin_data_ptr->autopin_ctl.tech;

  /* get the dpd_idx from autopin DM*/
  dpd_idx = txagc_template->txagc_ctl_data.dpd_idx;

  if (FALSE == rflm_autopin_is_enabled(tech, handle))
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_delta_pin_pout: AutoPin not enabled on Tech %d with Tx handle %d",
                          tech, handle);

    return RFLM_ERR_API_FAILED;
  }

  if(RFLM_AUTOPIN_NUM_DPD <= dpd_idx)
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_delta_pin_pout: dpd_idx %d should not exceed %d",
                          dpd_idx, RFLM_AUTOPIN_NUM_DPD);

    return RFLM_ERR_API_FAILED;
  }

  if(TRUE == autopin_data_ptr->pin_pout_lut.valid[dpd_idx])
  {
    if(dpd_idx == autopin_data_ptr->pin_pout_lut.lut[dpd_idx].dpd_idx)
    {
      for(lp_var = 0; lp_var < autopin_data_ptr->pin_pout_lut.lut[dpd_idx].num_pin; lp_var++)
      {
        if(gain_comp <= (autopin_data_ptr->pin_pout_lut.lut[dpd_idx].gain_comp[lp_var] * 10))
        {
          /* if the gain_comp is less than the first gain_comp, then capping it to the first gain_comp*/
          if(0 == lp_var)
          {
            pin_misalign = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pin_misalign[lp_var] * 10);
          }
          else
          {
            gain_prev = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].gain_comp[lp_var - 1] * 10);
            gain_next = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].gain_comp[lp_var] * 10);
            intep_prev = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pin_misalign[lp_var - 1] * 10);
            intep_next = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pin_misalign[lp_var] * 10);

            /* lookup the delta pout based one the gain_comp */
            /*Ideally it should be pin_misalign = intep_prev +  (gain_comp - gain_prev) * (intep_next - intep_prev)  / (gain_next - gain_prev)
              Due to it is init not float, need to code like below to protect the accuracy with rounding*/
            pin_misalign = (int32)(intep_prev + 
                                  (((gain_comp - gain_prev) * (intep_next - intep_prev)*2 + (gain_next - gain_prev)) / ((gain_next - gain_prev)*2)));
          }
          break;
        }
        /* if didn't find the right gain_comp when the lp_var goes to the highest, then use the max value of the LUT*/
        else if(lp_var == (autopin_data_ptr->pin_pout_lut.lut[dpd_idx].num_pin - 1))
        {
          pin_misalign = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pin_misalign[lp_var] * 10);
        }
      }
      pin_accum = (int32)(autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state] * 10) + pin_misalign;
      /* lookup the delta pout based on the pin_accum */
      for(lp_var = 0; lp_var < autopin_data_ptr->pin_pout_lut.lut[dpd_idx].num_pin; lp_var++)
      {
        if(pin_accum <= (autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pin_misalign[lp_var] * 10))
        {
          if(0 == lp_var)
          {
            /* capping both pin_accum and pout_delta when pin_misalign is lower than the first pin_misalign*/
            pin_accum = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pin_misalign[lp_var] * 10);
            pout_delta = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pout_delta[lp_var] * 10);
          }
          else
          {
            gain_prev = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pin_misalign[lp_var - 1] * 10);
            gain_next = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pin_misalign[lp_var] * 10);
            intep_prev = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pout_delta[lp_var - 1] * 10);
            intep_next = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pout_delta[lp_var] * 10);

            /* interp the pout_delta with rounding */
            /*Ideally it should be pout_delta = intep_prev +  (pin_accum - gain_prev) * (intep_next - intep_prev)  / (gain_next - gain_prev)
              Due to it is init not float, need to code like below to protect the accuracy with rounding*/
            pout_delta= (int32)(intep_prev + 
                                  (((pin_accum - gain_prev) * (intep_next - intep_prev) * 2 + (gain_next - gain_prev)) / ((gain_next - gain_prev)*2)));
          }
          break;
        }
        /* if didn't find the right gain_comp when the lp_var goes to the highest, then use the max value of the LUT*/
        else if(lp_var == (autopin_data_ptr->pin_pout_lut.lut[dpd_idx].num_pin - 1))
        {
          /* capping both pin_accum and pout_delta when pin_misalign is higher than max pin_misalign*/
          pin_accum = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pin_misalign[lp_var] * 10);
          pout_delta = (int32)(autopin_data_ptr->pin_pout_lut.lut[dpd_idx].pout_delta[lp_var] * 10);
        }
      }
    }
    else
    {
       RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN, "rflm_autopin_get_delta_pin_pout: Input dpd_idx %d not match Lut dpd_idx %d",
                          dpd_idx, autopin_data_ptr->pin_pout_lut.lut[dpd_idx].dpd_idx);

      return RFLM_ERR_API_FAILED;
    }
  }
  else
  {
    RFLM_DIAG_MSG_HIGH( "rflm_autopin_get_delta_pin_pout: the Lut is invalid for dpd_idx %d",
                          dpd_idx);

    return RFLM_ERR_API_FAILED;
  }

  /* convert the db100 unit to db10 with rounding*/
  if(autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state] < RFLM_AUTOPIN_MAX_PIN_OFFSET)
  {
    autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state] = (rflm_db10_t)RFLM_AUTOPIN_DB100_TO_DB10_ROUNDING(pin_accum);
    autopin_data_ptr->autopin_ctl.pout_delta = (rflm_db10_t)RFLM_AUTOPIN_DB100_TO_DB10_ROUNDING(pout_delta);
    autopin_data_ptr->autopin_ctl.gain_comp_exp = (int8)RFLM_AUTOPIN_DB100_TO_DB10_ROUNDING(gain_comp);
    autopin_data_ptr->autopin_ctl.one_shot_pin = (int8)RFLM_AUTOPIN_DB100_TO_DB10_ROUNDING(pin_misalign);
  }

  return RFLM_ERR_NONE;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  RFLM API called by calculate the pin_accum for legacy autoPin
 
  @details
  This function implement the API, Tech RFLM module will use this API to get the pin misalign
  and pout_delta according to the gain compress or expand value
 
  @param params
  rflm_handle_tx_handle hanle  the current tx handle to indexing the autopin_ptr
  int32 gain_comp the indication of increase or decrease the pin_accum comes from FW
 
  @return
  rflm_err_t: indicate the calc is done or not
 
*/

rflm_err_t 
rflm_autopin_update_legacy_pin_accum
(
  rflm_handle_tx_t handle, 
  int32 gain_comp
)
{
  rflm_autopin_dm_template_t *autopin_data_ptr;
  autopin_data_ptr = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(handle);

  /* parameters check */
  if (NULL == autopin_data_ptr)
  {
    RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_update_legacy_pin: No valid DM buf ptr with Tx handle %d",
                       handle);

    return RFLM_ERR_API_FAILED;
  }
  
#ifdef FEATURE_RF_HAS_WTR3925
    gain_comp -= RFLM_AUTOPIN_AMAM_ERROR_IMBALANCE_OFFSET;
  
    if (gain_comp > -RFLM_AUTOPIN_AMAM_ERROR_DEADZONE
        && gain_comp < RFLM_AUTOPIN_AMAM_ERROR_DEADZONE)
    {
      RFLM_DIAG_TRACE_ERROR(RFLM_DIAG_MSG_SSID_CMN_AUTOPIN,"rflm_autopin_update_proc_result: meas_error %d in deadzone ", gain_comp);
      return RFLM_ERR_API_FAILED;
    }
#endif

  if (gain_comp > 0)
  {
    if (autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state] < RFLM_AUTOPIN_MAX_PIN_OFFSET
        && !rflm_autopin_is_rgi_max(handle))
    {
      autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state] += 
        (rflm_db10_t)(RFLM_AUTOPIN_STEP_SIZE[autopin_data_ptr->autopin_ctl.tech]);
    }
  }
  else if (gain_comp < 0)
  {
    if (autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state] > -RFLM_AUTOPIN_MAX_PIN_OFFSET)
    {
      autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state] -= 
        (rflm_db10_t)(RFLM_AUTOPIN_STEP_SIZE[autopin_data_ptr->autopin_ctl.tech]);
    }
  }

  if (autopin_data_ptr->override_ctl.debug_active && autopin_data_ptr->override_ctl.pin_accum_override)
  {
    autopin_data_ptr->autopin_ctl.pin_accum[autopin_data_ptr->autopin_ctl.pa_state] = autopin_data_ptr->override_ctl.pin_accum_ovr_val[autopin_data_ptr->autopin_ctl.pa_state];
  }

  return RFLM_ERR_NONE;
}

