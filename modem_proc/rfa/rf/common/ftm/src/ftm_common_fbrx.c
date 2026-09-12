/*!
  @file
  ftm_common_fbrx.c

  @brief
  This module contains interface and implementation of FTM common FBRx.
  
*/

/*==============================================================================

  Copyright (c) 2013-2020 Qualcomm Technologies Incorporated. All Rights Reserved

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

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/src/ftm_common_fbrx.c#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
01/29/18   pvsr    Return previous logged results for Single trigger FBRX in FTM-RF
07/02/16   sk      Increase timer for droop CHAR
06/30/16   sk      Fix LTE B1 FBRX cal for first channel
06/23/16   sk      Support 40 Mhz FBRX cal on Jacala
06/20/16   sk      FBRX 40 Mhz cal and FTM-RF support
05/24/16    aa     Added triple buffer safety code for fbrx results
03/24/16   sk      KW Fix
03/23/16   sk      Jacala LTE FBRX tune-away support
03/16/15    aa     Fix KW error
03/01/15    aa     Update the Tx/Rx rms and dc values in dBFS10 units
03/01/15    aa     Update the xcorr value to allow tools to detect xcorr peak index 
03/01/15    aa     Adjust autocal to use default fixed Rx scale of 2048 
01/25/16    nv 	   Add FBRx disable after FBRx sample and process
01/22/16    aa     FBRx interface changes v1 for Jacala 
10/06/15    aa     Adjust autocal algorithm to resolve saturation and qnoise issues 
10/06/15    aa     Added more FBRx ftm debug commands
09/21/15    aa     Save TX/FBRx LM buffer info in FBRx state machine
09/21/15    aa     Added suport for FBRx debug ftm command
08/26/15   sk      [Phase 1] WTR hopping - Move TX MC SM variables to hadnle based DM
08/31/15    aa     Fix FBRx DC estimate computation to match DLL 
08/13/15    aa     Update FBRx DC estimate computation 
08/13/15    aa     Fix KW error
08/07/15    aa     Added support for FBRx autocal
01/14/15    aa     Added support to return the signal level in mV10
07/22/15   aa      Fix FTM FBRx debug sample capture related errors
05/28/15   aro     Support to test ATLAS WCDMA driver in THOR baseline
04/23/15    ka     Remove hardcoded device 0 for fbrx
04/02/15    aa     Fix compiler due related to missing featurization
03/11/15    aa     Added changes to get valid tx handle from tech and remove the loop
03/02/15    kr     Code changes to avoid compilation errors in WCDMA Compile out flavor.
02/26/15    jr     Code changes to avoid compilation errors in TDS Compile out flavor.
02/26/15    rp     Code changes to avoid compilation errors in LTE Compile out flavor.
02/12/15    pk     Featurized the CDMA code in ftm_common_fbrx_get_tx_chan
01/29/15    kg     Added FTM API to return FBRX sample captures
01/29/15    aa     Added support for dynamic memory alloc to store FBRX samples
11/20/14    ka     Defensive code to protect against invalid LM handle
11/10/14    ka     Return residual DC estimate during fbrx gain cal
10/08/14    ka     Replace rfc with device api for retrieving device pointer
09/30/14    aa     Fix compiler warning
09/08/14   jmf     [SelfTest] Implement Tx Power measurement using TxAGC loop
08/26/14    aa     Added support for FBRX QRCT logging
08/11/14    aa     Update run continuous cmd to control fbrx enable/disable
08/01/14    aa     Added support for rsb comp during calv3 opcode based fbrx setup
07/29/14    JJ     Set fbrx result flag to FALSE if flag returned by uk is 0
07/22/14    ka     Return next_rsb for cal
07/22/14    ka     Ensure correct handle for get mode function
06/12/14    ka     Add support to store samples from cal failures
05/26/14    ka     Return results from single trigger cmd
05/07/14    aa     Fix KW error
04/10/14    ka     KW fix
04/07/14    ka     Update single trigger. Remove obsolete code.
03/27/14    ka     Store gain state used for cal
03/20/14    ka     Added support for storing captures during cal
02/27/14    ka     Do not disable WTR during cal
02/17/14    ka     Gain cal fix
02/13/14    ka     Added continuous run api, updated capture function
02/11/14    aa     Fix compiler warning
02/06/14    aa     Updated fbrx uK results
01/30/14    aa     Added API to get valid tx handle for fbrx
01/22/14    ka     Added fbrx set params
12/23/13    ka     Added API for calibration
11/25/13    ka     Cmd fixes, added store results
11/13/13    ka     Added FTM fbrx common test framework
10/30/13    ka     Added bridge config
10/11/13    ka     Added activate chain
09/23/13    ka     Initial version

==============================================================================*/

#include "comdef.h"
#include "ftm.h"
#include "ftm_task.h"
#include "rfcommon_fbrx_api.h"
#include "ftm_common_fbrx.h"
#include "ftm_msg.h"
#include "ftmdiag.h"
#include "rfm_internal.h" /* for rfm_get_calibration_state() */
#ifdef FEATURE_RF_FBRX
#include "rflm_api_fbrx.h"
#include "rflm_dm_api.h"
#endif
#include "rfc_card.h"
#include "rfdevice_cmn_intf.h"
#include "rflm_api_fbrx_fw_hal.h"
#include "stringl.h" /* for memscpy */
#include "fs_estdio.h"
#include "ftm_common_locks.h"
#include "rf_test_task_v.h"
#include "rfcommon_core_utils.h"
#include "modem_mem.h"
#include "rflm_defs.h"
#include "math.h"

#ifdef FEATURE_LTE
#include "rflte_state.h"
#include "rflte_ftm_mc.h"
#ifdef FEATURE_RF_HAS_WTR3925
#include "rfc_card_lte.h"
#include "rfdevice_lte_interface.h"
#include "rflm_api_dtr_rx_fw.h"
#include "rflm_dtr_rx_typedef_ag.h"
#include "rflm_api_lte.h"
#include "rxlm_intf.h"
#include "rflte_ftm_mdsp.h"
#endif
#endif

#ifndef ROUND
#define ROUND(x) (((x) > 0)?(floor((x)+0.5)):(ceil((x)-0.5)))
#endif

#ifdef FEATURE_WCDMA
#include "rfwcdma_mc.h"
#ifdef ATLAS_RF_WCDMA_PRESI
#include "rfa_wcdma_interface.h"
#include "rfm_wcdma_ftm.h"
#else
#include "rfwcdma_data.h"
#endif
#endif
#include "ftm_cdma_data.h"
#include "ftm_tdscdma_ctl.h"

#include "rf_cdma_data.h"
#include "rf_tdscdma_mc.h"

extern uint16 ftm_tdscdma_current_chan;
#ifdef FEATURE_WCDMA
#ifndef ATLAS_RF_WCDMA_PRESI
extern rfwcdma_mc_state_type rfwcdma_mc_state;
#endif
#endif

static lm_handle_type ftm_common_fbrx_test_handle = 0;
static boolean ftm_common_fbrx_test_handle_allocated = FALSE;

static uint32 fbrx_current_buffer_size = 0;
static boolean ftm_common_fbrx_debug_samples_flag = TRUE; /* indicates if fresh read*/
static int fbrx_number_of_sample_captures = 0;
static int fbrx_debug_num_pkt_processed = 0;

static ftm_common_fbrx_sample_captures_init_type fbrx_sample_capture_init_data = 
{
  0,                  /* tech */
  0,                  /* band */
  0,                  /* max_num_valid_data_buf */
  FTM_FBRX_ERR_NONE,  /* generic_err_code */
  {NULL}              /* buffer_ptr[] */
};

static uint8 ftm_common_fbrx_buf_index = 0;

static ftm_common_fbrx_log_results_data_type  *ftm_common_fbrx_log_results_data = NULL;

static ftm_common_fbrx_debug_data_type* ftm_common_fbrx_debug_sample_captures  = NULL;

static ftm_fbrx_autocal_data_init_type ftm_fbrx_autocal_data = 
{
  {0}, 
  {0},
  {0},
  {0}
};

/*============================================================================*/
/*!
  @addtogroup FTM_COMMON_FBRX
 
  @brief
  This module includes the FTM functionality for FBRx.
 
  @{
*/


/*============================================================================*/
/*!
  @name FBRx FTM Test

  @brief
  This section contains FTM test functions for common FBRx.
 
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_enter_mode
 
  @details
  This function tests the common FBRx enter mode API
 
*/
#ifdef FEATURE_RF_FBRX

ftm_rsp_pkt_type ftm_common_fbrx_enter_mode(ftm_common_pkt_type *cmd_ptr)
{
  ftm_fbrx_handle_response_type* response_ptr = NULL;
  ftm_fbrx_enter_mode_param_type* param_ptr = NULL;
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_handle_response_type));

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_handle_response_type*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_enter_mode_param_type*)&(cmd_ptr->ftm_extended_params);

  if (!ftm_common_fbrx_test_handle_allocated)
  {
    if (param_ptr != NULL)
    {
      response_ptr->status = rfcommon_fbrx_mc_enter_mode(&ftm_common_fbrx_test_handle, 
                                                          param_ptr->tech, 0);
      response_ptr->handle_id = ftm_common_fbrx_test_handle;
      ftm_common_fbrx_test_handle_allocated = TRUE;

      FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_enter_mode: result %d, handle %d", 
                 response_ptr->status, 
                 ftm_common_fbrx_test_handle );
    }
  }
  else
  {
    response_ptr->status = FALSE;
    FTM_MSG_3( FTM_ERROR, "ftm_common_fbrx_enter_mode - handle already allocated!", 0, 0, 0);
  }

  return rsp;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_init_tx
 
  @details
  This function tests the common FBRx init tx API
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_init_tx(ftm_common_pkt_type *cmd_ptr)
{
  ftm_fbrx_handle_response_type* response_ptr = NULL;
  ftm_fbrx_init_tx_param_type* param_ptr = NULL;
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_handle_response_type));
  uint32 tx_handle = -1;
  rflm_fbrx_dm_template_t* fbrx_data_ptr = NULL;

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_handle_response_type*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_init_tx_param_type*)&(cmd_ptr->ftm_extended_params);

  if (ftm_common_fbrx_test_handle_allocated)
  {
    if (param_ptr != NULL)
    {
      tx_handle = ftm_common_fbrx_get_valid_tx_handle(param_ptr->tech, param_ptr->device, param_ptr->carrier_idx );

      response_ptr->status = rfcommon_fbrx_mc_init_tx( ftm_common_fbrx_test_handle, param_ptr->cfg, tx_handle, param_ptr->tech, param_ptr->carrier_idx);
                                                       
      /* update the fbrx_mode in DM */
      fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(tx_handle);
      if (fbrx_data_ptr != NULL)
      {
        (void)rfcommon_fbrx_mc_update_mode( tx_handle, param_ptr->cfg);
      }

      response_ptr->handle_id = ftm_common_fbrx_test_handle;

      FTM_MSG_3( FTM_HIGH, "ftm_common_fbrx_init_tx: result %d, handle %d, cfg %d", 
                 response_ptr->status, 
                 ftm_common_fbrx_test_handle,
                 param_ptr->cfg );
    }
  }
  else
  {
    response_ptr->status = FALSE;
    FTM_MSG_3( FTM_ERROR, "ftm_common_fbrx_init_tx - handle not allocated!", 0, 0, 0);
  }


  return rsp;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_enable
 
  @details
  This function tests the common FBRx enable API
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_enable(ftm_common_pkt_type *cmd_ptr)
{
  ftm_fbrx_handle_response_type* response_ptr = NULL;
  ftm_fbrx_enable_param_type* param_ptr = NULL;
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_handle_response_type));

  param_ptr = param_ptr;

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_handle_response_type*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_enable_param_type*)&(cmd_ptr->ftm_extended_params);

  if (ftm_common_fbrx_test_handle_allocated)
  {
    /* Pass in the FBRx chain */
    response_ptr->status = rfcommon_fbrx_enable(ftm_common_fbrx_test_handle, param_ptr->reserved);
    response_ptr->handle_id = ftm_common_fbrx_test_handle;

    FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_enable: result %d, handle %d", 
               response_ptr->status, 
               ftm_common_fbrx_test_handle);

  }
  else
  {
    response_ptr->status = FALSE;
    FTM_MSG_3( FTM_ERROR, "ftm_common_fbrx_enable - handle not allocated!", 0, 0, 0);
  }


  return rsp;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_exit
 
  @details
  This function tests the common FBRx exit mode API
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_exit(ftm_common_pkt_type *cmd_ptr)
{
  ftm_fbrx_handle_response_type* response_ptr = NULL;
  ftm_fbrx_exit_param_type* param_ptr = NULL;
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_handle_response_type));

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_handle_response_type*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_exit_param_type*)&(cmd_ptr->ftm_extended_params);

  if (ftm_common_fbrx_test_handle_allocated)
  {
    if (param_ptr != NULL)
    {
      response_ptr->status = rfcommon_fbrx_mc_exit(ftm_common_fbrx_test_handle, param_ptr->tech);
      response_ptr->handle_id = ftm_common_fbrx_test_handle;
      ftm_common_fbrx_test_handle_allocated = FALSE;

      FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_exit: result %d, handle %d", 
                 response_ptr->status, 
                 ftm_common_fbrx_test_handle );
    }
  }
  else
  {
    response_ptr->status = FALSE;
    FTM_MSG_3( FTM_ERROR, "ftm_common_fbrx_exit - handle not allocated!", 0, 0, 0);
  }

  return rsp;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_activate_chain
 
  @details
  This function tests the common FBRx activate chain API
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_activate_chain(ftm_common_pkt_type *cmd_ptr)
{
  ftm_fbrx_handle_response_type* response_ptr = NULL;
  ftm_fbrx_activate_chain_param_type* param_ptr = NULL;
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_handle_response_type));

  param_ptr = param_ptr;

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_handle_response_type*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_activate_chain_param_type*)&(cmd_ptr->ftm_extended_params);

  if (ftm_common_fbrx_test_handle_allocated)
  {
    #ifdef FEATURE_RF_FBRX
    rflm_fbrx_activate_chain(ftm_common_fbrx_test_handle);
    #endif
    response_ptr->status = TRUE; 
    response_ptr->handle_id = ftm_common_fbrx_test_handle;

    FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_activate_chain: result %d, handle %d", 
               response_ptr->status, 
               ftm_common_fbrx_test_handle);

  }
  else
  {
    response_ptr->status = FALSE;
    FTM_MSG_3( FTM_ERROR, "ftm_common_fbrx_activate_chain - handle not allocated!", 0, 0, 0);
  }


  return rsp;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_reconfigure_conflict_chain
 
  @details
  This api recofigures the LM settings into HW based the values in the RxLM handle
  This is required to switch between Drx and FBRX RxLM settings in conflict cases
 
*/
#ifdef FEATURE_RF_HAS_WTR3925
boolean ftm_common_fbrx_reconfigure_conflict_chain(lm_handle_type rx_handle)
{
  boolean status = FALSE;
  rflm_dtr_rx_indices_struct rx_indices;
  rflm_err_t lm_config_status = RFLM_ERR_NONE;

  if(!(rflm_dm_validate_handle(rx_handle)))
  {
    FTM_MSG_1( FTM_ERROR, "ftm_common_fbrx_reconfigure_conflict_chain: Invalid handle : %d passed",
                           rx_handle);
    return status;
  }

  //rxlm_acquire_lock(handle);

  rxlm_get_buffer_content(rx_handle, (uint32)RFLM_DTR_RX_INDICES, (uint8 *)&rx_indices);

  lm_config_status = rflm_dtr_rx_configure_chain(rx_handle,&rx_indices, RFLM_DTR_RX_FULL_CFG_MASK);

  //rxlm_release_lock(handle);

  if (lm_config_status == RFLM_ERR_NONE)
  {
    status = TRUE;
  }

  return status;
}
#endif

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_bridge_config
 
  @details
  This function tests the common FBRx bridge config API
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_bridge_config(ftm_common_pkt_type *cmd_ptr)
{
  ftm_fbrx_handle_response_type* response_ptr = NULL;
  ftm_fbrx_bridge_config_param_type* param_ptr = NULL;
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_handle_response_type));

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_handle_response_type*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_bridge_config_param_type*)&(cmd_ptr->ftm_extended_params);


  if (param_ptr != NULL)
  {
    #ifdef FEATURE_RF_FBRX
    rflm_fbrx_bridge_config(param_ptr->enable);
    #endif
    response_ptr->status = TRUE;

    FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_bridge_config: result %d, enable %d", 
               response_ptr->status, 
               param_ptr->enable);
  }
  else
  {
    response_ptr->status = FALSE;
    FTM_MSG_3( FTM_ERROR, "ftm_common_fbrx_bridge_config - NULL param pointer!", 0, 0, 0);
  }


  return rsp;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_single_trigger
 
  @details
  This function triggers a single capture and processing run of the FBRx uK
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_single_trigger(ftm_common_pkt_type *cmd_ptr)
{
  ftm_fbrx_single_trigger_response_type* response_ptr = NULL;
  ftm_fbrx_single_trigger_param_type* param_ptr = NULL;
  ftm_common_fbrx_capture_input_type input;
  ftm_common_fbrx_capture_result_type output;
  uint8 i = 0, fbrx_results_read_buf_idx = 0;
  rfdevice_rxtx_common_class* dev_ptr = NULL; 
  rflm_fbrx_dm_template_t* fbrx_data_ptr;
  uint32 tx_handle = 0;
  rfcom_band_type_u band_union;
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_single_trigger_response_type));
  uint8 results_buf_idx = 0;

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_single_trigger_response_type*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_single_trigger_param_type*)&(cmd_ptr->ftm_extended_params);

  response_ptr->status = FALSE;
  
  if (param_ptr != NULL)
  {
    #ifdef FEATURE_RF_FBRX

    tx_handle = ftm_common_fbrx_get_valid_tx_handle(param_ptr->tech, param_ptr->tx_device, param_ptr->carrier_idx);
    fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(tx_handle);
    if (fbrx_data_ptr != NULL )
    {
        band_union = rfcommon_fbrx_mc_convert_uint8_to_rfcom_band((rfm_mode_enum_type)fbrx_data_ptr->fbrx_ctl.rfm_mode, 
                                                                  fbrx_data_ptr->fbrx_ctl.fbrx_band);
        dev_ptr = rfcommon_core_get_tx_device_ptr( param_ptr->tx_device, 
                                                   (rfm_mode_enum_type)fbrx_data_ptr->fbrx_ctl.rfm_mode, 
                                                   band_union);
        if (dev_ptr == NULL)
        {
			FTM_MSG( FTM_ERROR, " ftm_common_fbrx_single_trigger() : if (dev_ptr == NULL) failed");
			response_ptr->status = FALSE;
			return rsp;
        }
		input.tech = fbrx_data_ptr->fbrx_ctl.rfm_mode;
        input.band = rfcommon_fbrx_mc_convert_uint8_to_rfcom_band(input.tech, fbrx_data_ptr->fbrx_ctl.fbrx_band);
    }
	else
	{
		FTM_MSG( FTM_ERROR, " ftm_common_fbrx_single_trigger() : if (fbrx_data_ptr != NULL ) failed");
		response_ptr->status = FALSE;
		return rsp;
	}
	

    input.fbrx_gain_state = param_ptr->fbrx_gain_state;
    input.scale_rx = param_ptr->scale_rx;
    input.tx_device_id = param_ptr->tx_device;
    input.ubias = param_ptr->ubias;
    input.store_capture_data = FALSE;
	
   output = ftm_common_fbrx_capture_and_process(input);    
	
    response_ptr->status = (uint8)output.status;
    response_ptr->tx_handle = tx_handle;
    response_ptr->gain = output.gain;
    response_ptr->ls_err = output.ls_err;

	
    for (i=0;i<RFLM_FBRX_XCORR_LOG_SIZE;i++)
    {
      response_ptr->xcorr_log[i] = output.xcorr_log[i];
    }

    if (fbrx_data_ptr != NULL )
    {
      results_buf_idx = fbrx_data_ptr->fbrx_ctl.fbrx_results_buf_idx;
      /* Update xcorr peak idx at xcorr_log[0]*/
      response_ptr->xcorr_log[0] = fbrx_data_ptr->results_log[results_buf_idx].results[0].xcorr_peak_idx;
    }

    #endif /* FEATURE_RF_FBRX */
  }

    FTM_MSG_1( FTM_HIGH, "ftm_common_fbrx_single_trigger: result %d", 
               response_ptr->status);

  return rsp;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_single_trigger
 
  @details
  This function triggers a single capture and processing run of the FBRx for PA aging tools that will have rxbias/ubias override
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_single_trigger_v2(ftm_common_pkt_type *cmd_ptr)
{
  ftm_fbrx_single_trigger_response_type_v2* response_ptr = NULL;
  ftm_fbrx_single_trigger_param_type* param_ptr = NULL;
  ftm_common_fbrx_capture_input_type input;
  ftm_common_fbrx_capture_result_type output;
  uint8 i = 0, fbrx_results_read_buf_idx = 0;
  rfdevice_rxtx_common_class* dev_ptr = NULL; 
  rflm_fbrx_dm_template_t* fbrx_data_ptr;
  uint32 tx_handle = 0;
  rfcom_band_type_u band_union;
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_single_trigger_response_type_v2));
  uint8 results_buf_idx = 0;

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_single_trigger_response_type_v2*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_single_trigger_param_type*)&(cmd_ptr->ftm_extended_params);

  response_ptr->status = FALSE;
  
  if (param_ptr != NULL)
  {
    #ifdef FEATURE_RF_FBRX

    tx_handle = ftm_common_fbrx_get_valid_tx_handle(param_ptr->tech, param_ptr->tx_device, param_ptr->carrier_idx);
    fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(tx_handle);
    if (fbrx_data_ptr != NULL )
    {
        band_union = rfcommon_fbrx_mc_convert_uint8_to_rfcom_band((rfm_mode_enum_type)fbrx_data_ptr->fbrx_ctl.rfm_mode, 
                                                                  fbrx_data_ptr->fbrx_ctl.fbrx_band);
        dev_ptr = rfcommon_core_get_tx_device_ptr( param_ptr->tx_device, 
                                                   (rfm_mode_enum_type)fbrx_data_ptr->fbrx_ctl.rfm_mode, 
                                                   band_union);
        if (dev_ptr == NULL)
        {
			FTM_MSG( FTM_ERROR, " ftm_common_fbrx_single_trigger_v2() : if (dev_ptr == NULL) failed");
			response_ptr->status = FALSE;
			return rsp;
        }
		input.tech = fbrx_data_ptr->fbrx_ctl.rfm_mode;
        input.band = rfcommon_fbrx_mc_convert_uint8_to_rfcom_band(input.tech, fbrx_data_ptr->fbrx_ctl.fbrx_band);
    }
	else
	{
		FTM_MSG( FTM_ERROR, " ftm_common_fbrx_single_trigger_v2() : if (fbrx_data_ptr != NULL ) failed");
		response_ptr->status = FALSE;
		return rsp;
	}
	

    input.fbrx_gain_state = param_ptr->fbrx_gain_state;
    input.scale_rx = param_ptr->scale_rx;
    input.tx_device_id = param_ptr->tx_device;
    input.ubias = param_ptr->ubias;
    input.store_capture_data = FALSE;
	
    int16  cal_pwr;                ///< tx power at which fbrx gain cal was done                            
	uint16 cal_rxscale;           ///< rx_scale value used during fbrx gain cal
	uint8  cal_ubias;              ///< ubias used during fbrx gain cal 
	uint16 last_cal_gain;        ///< fbrx cal gain value
	int16  last_temp_comp;
	uint8   curr_ubias;
	uint16  curr_scale_rx;
	
	output = ftm_mode_fbrx_single_trigger
	(
		input,
		&cal_pwr,                 ///< tx power at which fbrx gain cal was done                            
		&cal_rxscale,            ///< rx_scale value used during fbrx gain cal
		&cal_ubias,              ///< ubias used during fbrx gain cal 
		&last_cal_gain,         ///< fbrx cal gain value
		&last_temp_comp,
		&curr_ubias,
		&curr_scale_rx		
	);
	
	response_ptr->last_cal_gain = last_cal_gain;
	response_ptr->cal_rxscale = cal_rxscale;
	response_ptr->cal_pwr = cal_pwr;
	response_ptr->last_temp_comp = last_temp_comp;
	response_ptr->cal_ubias = cal_ubias;
	response_ptr->curr_ubias = curr_ubias;
	response_ptr->curr_scale_rx = curr_scale_rx;    
	
    response_ptr->status = (uint8)output.status;
    response_ptr->tx_handle = tx_handle;
    response_ptr->gain = output.gain;
    response_ptr->ls_err = output.ls_err;

	
    for (i=0;i<RFLM_FBRX_XCORR_LOG_SIZE;i++)
    {
      response_ptr->xcorr_log[i] = output.xcorr_log[i];
    }

    if (fbrx_data_ptr != NULL )
    {
      results_buf_idx = fbrx_data_ptr->fbrx_ctl.fbrx_results_buf_idx;
      /* Update xcorr peak idx at xcorr_log[0]*/
      response_ptr->xcorr_log[0] = fbrx_data_ptr->results_log[results_buf_idx].results[0].xcorr_peak_idx;
    }

    #endif /* FEATURE_RF_FBRX */
  }

    FTM_MSG_1( FTM_HIGH, "ftm_common_fbrx_single_trigger_v2: result %d", 
               response_ptr->status);

  return rsp;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_run_continuous
 
  @details
  This function triggers FBRx to run in normal operation mode
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_run_control(ftm_common_pkt_type *cmd_ptr)
{
  uint32 handle = 0;
  ftm_fbrx_handle_response_type* response_ptr = NULL;
  ftm_fbrx_run_control_param_type* param_ptr = NULL;
  #ifdef FEATURE_RF_FBRX
  rflm_fbrx_dm_template_t *fbrx_data_ptr = NULL;
  #endif
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_handle_response_type));

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_handle_response_type*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_run_control_param_type*)&(cmd_ptr->ftm_extended_params);


  response_ptr->status = FALSE;

  if (param_ptr != NULL)
  {
    #ifdef FEATURE_RF_FBRX
    handle = ftm_common_fbrx_get_valid_tx_handle(param_ptr->tech, param_ptr->tx_device, param_ptr->carrier_idx);

    fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(handle);
    if (fbrx_data_ptr != NULL)
    {
      if (param_ptr->enable)
      {
        /* Set override values for continuous operation */
        fbrx_data_ptr->override_ctl.enable = TRUE;
        fbrx_data_ptr->override_ctl.use_count = FALSE;
        fbrx_data_ptr->override_ctl.override_mode = FALSE;

      }
      else
      {
        /* Set override values to stop operation */
        fbrx_data_ptr->override_ctl.enable = FALSE;
        fbrx_data_ptr->override_ctl.use_count = FALSE;
        fbrx_data_ptr->override_ctl.override_mode = TRUE;

      }

      response_ptr->status = TRUE;
      response_ptr->handle_id = handle;

      FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_run_control: result %d, handle %d", 
                            response_ptr->status, 
                            handle);
    }
    else
    {
      FTM_MSG_1( FTM_ERROR, "ftm_common_fbrx_run_control: Invalid Tx Handle= %d", handle);
    }

    #endif /* FEATURE_RF_FBRX */
  }

  return rsp;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_enable_rf
 
  @details
  This function tests the common FBRx enable RF dev API
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_enable_rf(ftm_common_pkt_type *cmd_ptr)
{
  ftm_fbrx_handle_response_type* response_ptr = NULL;
  ftm_fbrx_enable_rf_param_type* param_ptr = NULL;
  rfdevice_rxtx_common_class* dev_ptr = NULL; 
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_handle_response_type));
  rflm_fbrx_dm_template_t* fbrx_data_ptr;
  uint32 tx_handle = 0;
  rfcom_band_type_u band_union;

  uint8 sub_id;

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_handle_response_type*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_enable_rf_param_type*)&(cmd_ptr->ftm_extended_params);
  if (param_ptr != NULL )
  {
    tx_handle = ftm_common_fbrx_get_valid_tx_handle(param_ptr->tech, param_ptr->tx_device, param_ptr->carrier_idx );
    fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(tx_handle);
    if (fbrx_data_ptr != NULL )
    {
       band_union = rfcommon_fbrx_mc_convert_uint8_to_rfcom_band((rfm_mode_enum_type)fbrx_data_ptr->fbrx_ctl.rfm_mode, 
                                                                 fbrx_data_ptr->fbrx_ctl.fbrx_band);
       dev_ptr = rfcommon_core_get_tx_device_ptr( param_ptr->tx_device, 
                                                  (rfm_mode_enum_type)fbrx_data_ptr->fbrx_ctl.rfm_mode, 
                                                  band_union);
    }
  }

  if (param_ptr != NULL && dev_ptr != NULL)
  {
    if (param_ptr->enable)
    {
      response_ptr->status = rfcommon_fbrx_mc_enable_rf_dev(dev_ptr, param_ptr->gain_state);
      #ifdef FEATURE_RF_HAS_WTR3925
      #if defined(FEATURE_LTE)
      if ((param_ptr->tech == RFM_LTE_MODE) || (param_ptr->tech == RFM_LTE_FTM_MODE))
      {
        rfm_device_enum_type drx_device;
        rfdevice_lte_script_data_type *temp_script_data_ptr = NULL;
        temp_script_data_ptr = rflte_state_get_script_data_ptr
                               ( sub_id,
                                 RFLTE_MC_PCELL_IDX,
                                 RFLTE_MC_CARR_TYPE_TX
                               );
        if (temp_script_data_ptr != NULL)
        {
          temp_script_data_ptr->write_to_hw_flag = TRUE;
        }
        drx_device = rflte_ftm_mc_sm_get_carr_drx_device(RFLTE_MC_PCELL_IDX);
        if (rfc_lte_is_fb_device_conflict(drx_device,param_ptr->tx_device,rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX)))
        {
          rfdevice_lte_rx_disable(drx_device,
                                  rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX),
                                  temp_script_data_ptr,
                                  NULL);
        }
      }
      #endif
      #endif
    }
    else
    {
      response_ptr->status = rfcommon_fbrx_mc_disable_rf_dev(dev_ptr);
      #ifdef FEATURE_RF_HAS_WTR3925
      #if defined(FEATURE_LTE)
      if ((param_ptr->tech == RFM_LTE_MODE) || (param_ptr->tech == RFM_LTE_FTM_MODE))
      {
        rfm_device_enum_type drx_device;
        rfdevice_lte_script_data_type *temp_script_data_ptr = NULL;
        temp_script_data_ptr = rflte_state_get_script_data_ptr
                               ( sub_id,
                                 RFLTE_MC_PCELL_IDX,
                                 RFLTE_MC_CARR_TYPE_TX
                               );
        if (temp_script_data_ptr != NULL)
        {
          temp_script_data_ptr->write_to_hw_flag = TRUE;
        }
        drx_device = rflte_ftm_mc_sm_get_carr_drx_device(RFLTE_MC_PCELL_IDX);
        if (rfc_lte_is_fb_device_conflict(drx_device,param_ptr->tx_device,rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX)))
        {
          rfdevice_lte_rx_enable(drx_device,
                                 rflte_ftm_mc_sm_get_tx_band(),
                                 temp_script_data_ptr,
                                 NULL);
        }
      }
      #endif
      #endif
    }
    response_ptr->handle_id = ftm_common_fbrx_test_handle;

    FTM_MSG_4( FTM_HIGH, "ftm_common_fbrx_enable_rf: result %d, handle %d, enable %d, gain state %d", 
               response_ptr->status, 
               ftm_common_fbrx_test_handle,
               param_ptr->enable,
               param_ptr->gain_state );
  }
  else
  {
    response_ptr->status = FALSE;
    response_ptr->handle_id = tx_handle;
    FTM_MSG_3( FTM_ERROR, "ftm_common_fbrx_enable_rf - NULL param pointer!", 0, 0, 0);
  }

  return rsp;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_store_results
 
  @details
  This function writes the FBRx capture results to EFS
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_store_results(ftm_common_pkt_type *cmd_ptr)
{
  ftm_fbrx_store_results_response_type* response_ptr = NULL;
  ftm_fbrx_store_results_param_type* param_ptr = NULL;
  char iq_file_name[RFCOMMON_FBRX_MAX_FILE_LENGTH];
  EFS_EFILE *iq_fp;
  fs_size_t write_result;
  int file_close_res;
  uint8 i;
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_store_results_response_type));
  uint32 tx_handle;
  uint16 store_data_counter = 0;

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_store_results_response_type*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_store_results_param_type*)&(cmd_ptr->ftm_extended_params);

  response_ptr->status = TRUE;

  if (param_ptr != NULL)
  {

    tx_handle = ftm_common_fbrx_get_valid_tx_handle( param_ptr->tech, param_ptr->device, param_ptr->carrier_idx);

    response_ptr->handle_id = tx_handle;
    response_ptr->status = rfcommon_fbrx_msm_store_data( param_ptr->num_tx,
                                                         param_ptr->num_rx,
                                                         tx_handle,
                                                         &store_data_counter );

    response_ptr->store_data_counter = store_data_counter;
  }
    
  FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_store_results: result status =%d, tx_handle = %d " , response_ptr->status, tx_handle);

  return rsp;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  rfcommon_fbrx_mc_get_tx_chan
 
  @details
  This function returns the current TX channel for each tech 
 
*/

uint32 ftm_common_fbrx_get_tx_chan( rfm_mode_enum_type rfm_tech, rfm_device_enum_type rfm_tx_device )
{
  uint32 tx_chan = 0;
  uint32 cell_idx = 0;
  ftm_cdma_data_status_type dev_state = {0}; 
  lm_handle_type txlm_buffer_idx = RFLTE_MC_TXLM_INVALID_HANDLE;

  uint8 sub_id=0;

  switch (rfm_tech)
  {
     #ifdef FEATURE_WCDMA
     case RFM_IMT_MODE:
       {
         #ifdef ATLAS_RF_WCDMA_PRESI
         rfm_wcdma_ftm_radio_state_data_type radio_state;
         rfm_wcdma_ftm_tx_get_radio_state( rfm_tx_device, &radio_state );
         tx_chan = radio_state.channel[0];
         #else
         tx_chan = rfwcdma_data_get_curr_chan(rfm_tx_device, RFCOM_SINGLE_CARRIER);
         #endif
       }
       break;
     #endif

     #ifdef FEATURE_CDMA1X
     case RFM_1X_MODE:
     #endif /* #ifdef (FEATURE_CDMA1X) */
     #ifdef FEATURE_HDR
     case RFM_1XEVDO_MODE:
     #endif /* #ifdef (FEATURE_HDR) */
     #if defined(FEATURE_HDR)&&defined(FEATURE_CDMA1X)
     case RFM_SVDO_MODE:
     #endif /* #if defined(FEATURE_HDR)&&defined(FEATURE_CDMA1X) */ 
     #if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR)
       (void)ftm_cdma_data_get_device_status(rfm_tx_device, &dev_state); 
         tx_chan = dev_state.curr_chan;
       break;
     #endif /* #if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR) */

#if defined(FEATURE_LTE)
     case RFM_LTE_MODE:
     case RFM_LTE_FTM_MODE:
       rflte_state_get_txlm_buffer_idx(sub_id,cell_idx,&txlm_buffer_idx);
       tx_chan = rflte_state_get_curr_chan_tx( cell_idx,txlm_buffer_idx );
       break;
#endif

#ifdef FEATURE_TDSCDMA
     case RFM_TDSCDMA_MODE:
     case RFM_TDSCDMA_MODE_FTM:
       tx_chan = ftm_tdscdma_current_chan;
       break;
#endif /*FEATURE_TDSCDMA*/

     default:
       tx_chan = 0 ;
       break;
  }

  return tx_chan;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_dealloc_mem_sample_capture_buf
 
  @details
  This function de-allocates memory for the fbrx sample captures  
*/
void ftm_common_fbrx_dealloc_mem_sample_capture_buf( void )
{
  uint8 max_valid_buf = fbrx_sample_capture_init_data.max_num_valid_data_buf;
  uint8 i = 0;

  for ( i=0; i < FTM_FBRX_MAX_NUM_SC_DATA_BUF; i++ )
  {
    if ( fbrx_sample_capture_init_data.buffer_ptr[i] != NULL )
    {
      /* Memory already allocated for buffer index, deallocate memory */
      modem_mem_free((void*)fbrx_sample_capture_init_data.buffer_ptr[i], MODEM_MEM_CLIENT_RFA);
      fbrx_sample_capture_init_data.buffer_ptr[i] = NULL;
      FTM_MSG_1( FTM_HIGH, "ftm_common_fbrx_dealloc_mem_sample_capture_buf: Memory successfully de-allocated for buffer index = %d. ", i );
    }
  }

  fbrx_sample_capture_init_data.generic_err_code = FTM_FBRX_ERR_NONE;
  fbrx_sample_capture_init_data.max_num_valid_data_buf = 0;
  fbrx_sample_capture_init_data.tech = 0;
  fbrx_sample_capture_init_data.band = 0; 

} /* ftm_common_fbrx_dealloc_mem_sample_capture_buf */

/*in FTM mode, we cannot do FBRX capture(leads to crash 2% cases) with desired input so we can rely only on cal pwr, ubias, etc from QXDM logs and response packets
*/
ftm_common_fbrx_capture_result_type ftm_mode_fbrx_single_trigger
(
  ftm_common_fbrx_capture_input_type input,
  int16  *cal_pwr,                 ///< tx power at which fbrx gain cal was done                            
  uint16 *cal_rxscale,            ///< rx_scale value used during fbrx gain cal
  uint8  *cal_ubias,              ///< ubias used during fbrx gain cal 
  uint16 *last_cal_gain,         ///< fbrx cal gain value
  int16  *last_temp_comp,
  uint8  *curr_ubias,
  uint16 *curr_scale_rx
)
{
	
  uint32 handle;
  #ifdef FEATURE_RF_FBRX
  rflm_fbrx_dm_template_t *fbrx_data_ptr = NULL;
  rflm_handle_rx_t fbrx_lm_handle = -1;
  #endif
  ftm_common_fbrx_capture_result_type result = {0,0,0,0,0,0,0,{0}};
  rfdevice_rxtx_common_class* dev_ptr; 
  boolean dev_status = TRUE;
  uint8 results_buf_idx=0;
  rfm_mode_enum_type rfm_tech;
  rfcom_band_type_u rfcom_band;
  boolean ftm_rf_mode = FALSE;
  uint16  read_buf_idx = 0;
  boolean temp_tx_lin_update_flag=0;
  
  FTM_MSG( FTM_HIGH, "ftm_mode_lte_fbrx_single_trigger() ");

  uint8 sc_buf_idx = fbrx_sample_capture_init_data.max_num_valid_data_buf; /* current sample capture buffer idx */

  result.status = RFCOMMON_FBRX_ERROR;

  dev_ptr = rfcommon_core_get_tx_device_ptr((rfm_device_enum_type)input.tx_device_id, input.tech, input.band);

  if (input.fbrx_gain_state > RFLM_FBRX_NUM_GAIN_STATES-1)
  {
    input.fbrx_gain_state = RFLM_FBRX_NUM_GAIN_STATES-1;
  }

  dev_status &= rfcommon_fbrx_mc_enable_rf_dev(dev_ptr, input.fbrx_gain_state);

  #ifdef FEATURE_RF_FBRX

  #if defined(FEATURE_LTE)
  handle = ftm_common_fbrx_get_valid_tx_handle( input.tech, 
                                                (rfm_device_enum_type)input.tx_device_id,
                                                rflte_ftm_mc_sm_get_carrier_index() );
  #else 
  handle = ftm_common_fbrx_get_valid_tx_handle( input.tech, 
                                                (rfm_device_enum_type)input.tx_device_id,
                                                0 );
  #endif

  fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(handle);

  if (fbrx_data_ptr != NULL)
  {  
    rfm_tech = fbrx_data_ptr->fbrx_ctl.rfm_mode;
    rfcom_band = rfcommon_fbrx_mc_convert_uint8_to_rfcom_band(rfm_tech, fbrx_data_ptr->fbrx_ctl.fbrx_band );
    fbrx_sample_capture_init_data.tech = rfm_tech;
    fbrx_sample_capture_init_data.band = fbrx_data_ptr->fbrx_ctl.fbrx_band; 

    /* Get FBRx rxlm handle */
    fbrx_lm_handle = rfcommon_fbrx_mc_get_tech_handle( rfm_tech );

    if ((rfm_tech == RFM_LTE_MODE) || (rfm_tech == RFM_LTE_MODE))
    {
      fbrx_lm_handle = fbrx_data_ptr->fbrx_ctl.fbrx_lm_handle;
	  
      ftm_rf_mode = rflm_lte_get_ftm_rf_flag_per_cell(RFLM_LTE_MC_CELL0);
	  
      FTM_MSG_3( FTM_HIGH, "ftm_common_fbrx_capture_and_process: Configuring on Tx handle : %d, FBRX handle :  %d, carrier : %d",
                            handle,fbrx_lm_handle,rflte_ftm_mc_sm_get_carrier_index());
    }
     
	
      temp_tx_lin_update_flag = fbrx_data_ptr->fbrx_ctl.txlin_update_flag;
      fbrx_data_ptr->fbrx_ctl.txlin_update_flag = FALSE;
	  results_buf_idx = fbrx_data_ptr->fbrx_ctl.fbrx_results_buf_idx;
	  read_buf_idx = fbrx_data_ptr->exp_gain_data.read_buf_idx;
	  *curr_ubias=fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[0].ubias_new;
	  *curr_scale_rx=fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[0].rxscale_new;
	  *cal_pwr = (fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[0].cal_pwr);
	  *last_temp_comp = fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[0].last_temp_comp;
	  *cal_rxscale = (fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[0].cal_rxscale);
	  *cal_ubias = (fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[0].cal_ubias);
	  *last_cal_gain = (fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[0].last_cal_gain);
	  result.gain = fbrx_data_ptr->results_log[results_buf_idx].results[0].gain_est;
		  
	  FTM_MSG_8(FTM_HIGH, "curr_rxscale: %d, curr_ubias: %d,cal_pwr: %d,last_temp_comp: %d,cal_rxscale: %d,cal_ubias: %d,last_cal_gain: %d,est gain : %d", 
					*curr_scale_rx,
					*curr_ubias,
					*cal_pwr,
					*last_temp_comp,
					*cal_rxscale,
					*cal_ubias,
					*last_cal_gain,
					result.gain
					);

	  fbrx_data_ptr->fbrx_ctl.txlin_update_flag = temp_tx_lin_update_flag;
	  
  	  return result;
	  
  }
  else
  {
	  FTM_MSG( FTM_ERROR, " ftm_mode_lte_fbrx_single_trigger() : if (fbrx_data_ptr != NULL) failed");
  }
  #endif
  return result;
}



/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_capture_and_process
 
  @details
  Triggers a single capture and processing of FBRx samples. Intended for use
  during calibration.
 
  @param ftm_common_fbrx_capture_input_type.tx_device_id
           set to 0 for WTR0, 1 for WTR1, etc.
 
  @param ftm_common_fbrx_capture_input_type.xpt_mode - APT/EPT/ET
 
  @param ftm_common_fbrx_capture_input_type.gain_comp
           Scaling factor for processing. Depends on RB allocation for LTE.
           Constant for other techs. Use -32768 for now.
 
  @param ftm_common_fbrx_capture_input_type.fbrx_gain_state
           Set to desired FBRx gain state.
 
*/
ftm_common_fbrx_capture_result_type ftm_common_fbrx_capture_and_process
(
  ftm_common_fbrx_capture_input_type input
)
{
  uint32 handle;
  #ifdef FEATURE_RF_FBRX
  rflm_fbrx_dm_template_t *fbrx_data_ptr = NULL, *valid_data_ptr=NULL;
  rflm_handle_rx_t fbrx_lm_handle = -1;
  rfcommon_fbrx_error_type api_status = RFCOMMON_FBRX_SUCCESS;
  #endif
  ftm_common_fbrx_capture_result_type result = {0,0,0,0,0,0,0,{0}};
  rfdevice_rxtx_common_class* dev_ptr; 
  boolean dev_status = TRUE;
  uint8 i=0, results_buf_idx=0;
  boolean done = FALSE;
  uint16 counter = 0;
  int32 real_err, imag_err, complex_err;
  rfm_mode_enum_type rfm_tech;
  rfcom_band_type_u rfcom_band;

  int32 real_rx_sum = 0, imag_rx_sum = 0;
  int32 fbrx_dc_dBFS10 = 0;
  double max_rx_sum = 0;
  uint32 signal_rms_mv10 = 0;
  boolean ftm_rf_mode = FALSE;

  uint8 sub_id=0;

#ifdef FEATURE_RF_HAS_WTR3925
#if defined(FEATURE_LTE)
  rfm_device_enum_type drx_device;
  rfdevice_lte_script_data_type *temp_script_data_ptr = NULL;
  boolean lm_config_status = FALSE;
  lm_handle_type lm_config_handle;
#endif
#endif

  uint8 sc_buf_idx = fbrx_sample_capture_init_data.max_num_valid_data_buf; /* current sample capture buffer idx */

  result.status = RFCOMMON_FBRX_ERROR;

  dev_ptr = rfcommon_core_get_tx_device_ptr((rfm_device_enum_type)input.tx_device_id, input.tech, input.band);

  if (input.fbrx_gain_state > RFLM_FBRX_NUM_GAIN_STATES-1)
  {
    input.fbrx_gain_state = RFLM_FBRX_NUM_GAIN_STATES-1;
  }

  dev_status &= rfcommon_fbrx_mc_enable_rf_dev(dev_ptr, input.fbrx_gain_state);

  #ifdef FEATURE_RF_FBRX

  #if defined(FEATURE_LTE)
  handle = ftm_common_fbrx_get_valid_tx_handle( input.tech, 
                                                (rfm_device_enum_type)input.tx_device_id,
                                                rflte_ftm_mc_sm_get_carrier_index() );
  #else 
  handle = ftm_common_fbrx_get_valid_tx_handle( input.tech, 
                                                (rfm_device_enum_type)input.tx_device_id,
                                                0 );
  #endif

  fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(handle);

  if (fbrx_data_ptr != NULL)
  {  
    rfm_tech = fbrx_data_ptr->fbrx_ctl.rfm_mode;
    rfcom_band = rfcommon_fbrx_mc_convert_uint8_to_rfcom_band(rfm_tech, fbrx_data_ptr->fbrx_ctl.fbrx_band );
    fbrx_sample_capture_init_data.tech = rfm_tech;
    fbrx_sample_capture_init_data.band = fbrx_data_ptr->fbrx_ctl.fbrx_band; 

    /* Get FBRx rxlm handle */
    fbrx_lm_handle = rfcommon_fbrx_mc_get_tech_handle( rfm_tech );

    if ((rfm_tech == RFM_LTE_MODE) || (rfm_tech == RFM_LTE_MODE))
    {
      fbrx_lm_handle = fbrx_data_ptr->fbrx_ctl.fbrx_lm_handle;
	  
      ftm_rf_mode = rflm_lte_get_ftm_rf_flag_per_cell(RFLM_LTE_MC_CELL0);
	  
      FTM_MSG_3( FTM_HIGH, "ftm_common_fbrx_capture_and_process: Configuring on Tx handle : %d, FBRX handle :  %d, carrier : %d",
                            handle,fbrx_lm_handle,rflte_ftm_mc_sm_get_carrier_index());
    }

    if (ftm_rf_mode == FALSE)
    {
    if (fbrx_lm_handle != 0xFFFFFFFF)
    {
      /* Update the IQMC RSB coefficients to FBRX DM */
      api_status = rfcommon_fbrx_mc_update_rsb_coeff( input.tx_device_id,
                                                      handle, 
                                                      fbrx_lm_handle, 
                                                      rfm_tech,
                                                      rfcom_band );
    }
    else
    {
      api_status = RFCOMMON_FBRX_ERROR;
    }

    if (api_status == RFCOMMON_FBRX_ERROR)
    {
      FTM_MSG( FTM_ERROR, "ftm_common_fbrx_capture_and_process: rfcommon_fbrx_mc_update_rsb_coeff() failed");
    }
    else
    {
      api_status  = rfcommon_fbrx_msm_write_iqmc_to_hw( fbrx_lm_handle, 
                                                        fbrx_data_ptr->rsb_data[input.fbrx_gain_state].iqmc );
      if (api_status == RFCOMMON_FBRX_ERROR)
      {
        FTM_MSG( FTM_ERROR, "ftm_common_fbrx_capture_and_process: rfcommon_fbrx_msm_write_iqmc_to_hw() failed");
      }
    }

   #ifdef FEATURE_RF_HAS_WTR3925
   #if defined(FEATURE_LTE)
    
    if ((input.tech == RFM_LTE_MODE) || (input.tech == RFM_LTE_FTM_MODE))
    {
      temp_script_data_ptr = rflte_state_get_script_data_ptr
                             ( sub_id,
                               RFLTE_MC_PCELL_IDX,
                               RFLTE_MC_CARR_TYPE_TX
                             );
      if (temp_script_data_ptr != NULL)
      {
        temp_script_data_ptr->write_to_hw_flag = TRUE;
      }
      drx_device = rflte_ftm_mc_sm_get_carr_drx_device(RFLTE_MC_PCELL_IDX);
      if (rfc_lte_is_fb_device_conflict(drx_device,(rfm_device_enum_type)input.tx_device_id,rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX)))
      {
        rfdevice_lte_rx_disable(drx_device,
                                rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX),
                                temp_script_data_ptr,
                                NULL);
      }
      else
      {
        drx_device = rfc_lte_get_fb_device((rfm_device_enum_type)input.tx_device_id,rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX));
        
        rfdevice_lte_rx_disable(drx_device,
                                rflte_ftm_mc_sm_get_tx_band(),
                                temp_script_data_ptr,
                                NULL);
        
        FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_capture_and_process: Non-conflict Disabling Drx dev : %d on band : %d ",
                                   drx_device, rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_SCELL1_IDX));
      }
    }
  #endif
  #endif

    for (i=0;i<RFLM_FBRX_LTE_MAX_RUNS_PER_SF;i++)
    {
      fbrx_data_ptr->fbrx_ctl.uk_results_valid_flag[i] = FALSE;
    }
    /* Set override values for single trigger mode */
    fbrx_data_ptr->override_ctl.counter = 1;
    fbrx_data_ptr->override_ctl.enable = TRUE;
    fbrx_data_ptr->override_ctl.use_count = TRUE;
    fbrx_data_ptr->override_ctl.override_mode = TRUE;
    fbrx_data_ptr->override_ctl.scale_rx = input.scale_rx;
    fbrx_data_ptr->override_ctl.ubias = input.ubias;
    fbrx_data_ptr->override_ctl.gain_state = input.fbrx_gain_state;

    rflm_fbrx_log_sample_capture_buffer_alloc_cb( );

    }

    while (!done && counter < 250)
    {
      DALSYS_BusyWait(100);
      if (fbrx_data_ptr->fbrx_ctl.uk_results_valid_flag[0])
      {
        done = TRUE;          
      }
      counter++;
    }

    if (dev_status==TRUE && done==TRUE)
    {
      result.status = RFCOMMON_FBRX_SUCCESS;
    }
    else
    {
      result.status = RFCOMMON_FBRX_ERROR;
      FTM_MSG( FTM_ERROR, "ftm_common_fbrx_capture_and_process: Invalid FBRx capture.");
    }

    if (fbrx_data_ptr->fbrx_ctl.uk_results_valid_flag[0])
    {
      results_buf_idx = fbrx_data_ptr->fbrx_ctl.fbrx_results_buf_idx;

      result.gain = fbrx_data_ptr->results_log[results_buf_idx].results[0].gain_est;
      result.gain_imbalance = fbrx_data_ptr->results_log[results_buf_idx].results[0].gain_imbalance;

      /* Get the computed fbrx dc value in dBFS10 unit from FBRx DM*/
      fbrx_dc_dBFS10 = fbrx_data_ptr->fbrx_ctl.rx_dc_dbfs;
      FTM_MSG_1( FTM_HIGH, "ftm_common_fbrx_capture_and_process: fbrx_dc_dbfs10=%d", fbrx_dc_dBFS10);

      result.loft = fbrx_dc_dBFS10;

      result.phase_imbalance = fbrx_data_ptr->results_log[results_buf_idx].results[0].phase_imbalance;
      result.rsb = fbrx_data_ptr->results_log[results_buf_idx].results[0].next_rsb;
      result.ls_err = fbrx_data_ptr->results_log[results_buf_idx].results[0].ls_err;

      for (i=0;i<RFLM_FBRX_XCORR_LOG_SIZE;i++)
      {
        result.xcorr_log[i] = fbrx_data_ptr->results_log[results_buf_idx].results[0].xcorr_log[i];
      }
      /* Update the xcorr peak with default value of 16384 to allow tools to detect the 
         xcorr peak index value since xcorr array returned by CFW is all 0's in Jacala */
      result.xcorr_log[fbrx_data_ptr->results_log[results_buf_idx].results[0].xcorr_peak_idx] = 16384;

      if (result.status == RFCOMMON_FBRX_SUCCESS)
      {
        /* dBFS -> mV conversion formula: mV = 10^(dBFS/20) * ADC_Vpeak = 10^(dBFS/20) * 2V. */
        real_rx_sum = fbrx_data_ptr->results_log[results_buf_idx].results[0].rx_sum >> 32;
        imag_rx_sum = (fbrx_data_ptr->results_log[results_buf_idx].results[0].rx_sum << 32) >> 32;
        max_rx_sum = MAX(rflm_abs_satw(real_rx_sum), rflm_abs_satw(imag_rx_sum));
        signal_rms_mv10 = (int32)( (int64)( (max_rx_sum/fbrx_data_ptr->fbrx_ctl.nref) * 10000) >> 14 );

        //FTM_MSG_4( FTM_HIGH, "FBRx: real_rx_sum =%d, imag_rx_sum=%d, nref=%d, signal_rms_mv10=%d", 
        //                      real_rx_sum, imag_rx_sum, fbrx_data_ptr->fbrx_ctl.nref, signal_rms_mv10);

        if (signal_rms_mv10 < RFCOMMON_FBRX_SIGNAL_UNDERFLOW_THRESH)
        {
          result.status = RFCOMMON_FBRX_RMS_SIG_UNDERFLOW;
         FTM_MSG_2( FTM_HIGH, "FBRx signal_rms_mv10 (%d) < %d, Signal Underflow", 
                               signal_rms_mv10, RFCOMMON_FBRX_SIGNAL_UNDERFLOW_THRESH);
        }
        else if (signal_rms_mv10 > RFCOMMON_FBRX_SIGNAL_OVERFLOW_THRESH)
        {
          result.status = RFCOMMON_FBRX_RMS_SIG_OVERFLOW;
         FTM_MSG_2( FTM_HIGH, "FBRx signal_rms_mv10 (%d) > %d, Signal Overflow", 
                               signal_rms_mv10, RFCOMMON_FBRX_SIGNAL_OVERFLOW_THRESH);
        }
      }

      valid_data_ptr = fbrx_data_ptr;
    }

    complex_err = (int32)(result.ls_err);
    real_err = complex_err >> 16;
    imag_err = (complex_err << 16) >> 16;

    if ( (valid_data_ptr != NULL) && (sc_buf_idx < FTM_FBRX_MAX_NUM_SC_DATA_BUF) )
    {
      if (fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx] == NULL )
      {
        /* Allocate memory and update the pointer to the static variable */
        fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx] = modem_mem_alloc( sizeof(ftm_common_fbrx_debug_data_type), MODEM_MEM_CLIENT_RFA);

        if ( fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx] != NULL )
        {
          FTM_MSG_1( FTM_HIGH, "ftm_common_fbrx_capture_and_process: Saving debug capture %d", sc_buf_idx );
          FTM_MSG_1( FTM_HIGH, "FBRx gain estimate %d", result.gain );
          FTM_MSG_1( FTM_HIGH, "FBRx RSB 0x%08X", result.rsb );
          for (i=0;i<RFLM_FBRX_XCORR_LOG_SIZE;i++)
          {
            FTM_MSG_2( FTM_HIGH, "FBRx xcorr[%d] = %d", i, result.xcorr_log[i]);
          }

          fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->gain_state =  input.fbrx_gain_state;
          fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->channel = ftm_common_fbrx_get_tx_chan(valid_data_ptr->fbrx_ctl.rfm_mode,
                                                                                                      input.tx_device_id);
          fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->fbrx_mode = valid_data_ptr->fbrx_ctl.fbrx_mode;

          if( (abs(real_err) > RFCOMMON_FBRX_CAL_DEBUG_THRESH || abs(imag_err) > RFCOMMON_FBRX_CAL_DEBUG_THRESH) )
          {
            /* Bad SC */
            fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->fbrx_sample_is_good = FALSE;
          }
          else
          {
            /* Good SC */
            fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->fbrx_sample_is_good = TRUE;
          }

          fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->tx_sample_length = valid_data_ptr->fbrx_ctl.num_tx_in_samples;
          fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->rx_sample_length = valid_data_ptr->fbrx_ctl.num_rx_in_samples;

          (void)rfcommon_fbrx_msm_store_data_internal(&(fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->tx_samples[0]),
                                                      &(fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->rx_samples[0]),
                                                      valid_data_ptr->fbrx_ctl.num_tx_in_samples, 
                                                      valid_data_ptr->fbrx_ctl.num_rx_in_samples,
                                                      handle );

          fbrx_sample_capture_init_data.max_num_valid_data_buf ++ ;

        }
        else
        {
          fbrx_sample_capture_init_data.generic_err_code = FTM_FBRX_ERR_MEM_ALLOC_FAILED;
          FTM_MSG_1( FTM_ERROR, "ftm_common_fbrx_capture_and_process: Memory allocation failed for buffer index = %d ", sc_buf_idx );
        }
      }
      else
      {
        /* Memory already allocated for buffer index, deallocate memory */
        modem_mem_free((void*)fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx], MODEM_MEM_CLIENT_RFA);
        fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx] = NULL;
        fbrx_sample_capture_init_data.generic_err_code = FTM_FBRX_ERR_MEM_ALLOC_FAILED;
        FTM_MSG_1( FTM_ERROR, "ftm_common_fbrx_capture_and_process: Memory previously allocated for buffer index = %d. De-allocating memory now ", sc_buf_idx );
      }
    }
  }
  else
  {
    FTM_MSG_1( FTM_ERROR, "ftm_common_fbrx_capture_and_process: Invalid handle = %d ", handle );
  }
  
  //jcen
  FTM_MSG_1( FTM_HIGH, "ftm_common_fbrx_capture_and_process: disable the FRBx! gain state %d ", input.fbrx_gain_state );
  rfcommon_fbrx_mc_disable_rf_dev(dev_ptr);
  
  if (ftm_rf_mode == FALSE)
  {  
  #ifdef FEATURE_RF_HAS_WTR3925
  #if defined(FEATURE_LTE)
  if ((input.tech == RFM_LTE_MODE) || (input.tech == RFM_LTE_FTM_MODE))
  {
    if (temp_script_data_ptr != NULL)
    {
      temp_script_data_ptr->write_to_hw_flag = TRUE;
    }
    drx_device = rflte_ftm_mc_sm_get_carr_drx_device(RFLTE_MC_PCELL_IDX);
    if (rfc_lte_is_fb_device_conflict(drx_device,(rfm_device_enum_type)input.tx_device_id,rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX)))
    {
      rfdevice_lte_rx_enable(drx_device,
                              rflte_ftm_mc_sm_get_tx_band(),
                              temp_script_data_ptr,
                              NULL);
    }
    else
    {
      drx_device = rfc_lte_get_fb_device((rfm_device_enum_type)input.tx_device_id,rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX));
        
      rfdevice_lte_rx_enable(drx_device,
                             rflte_ftm_mc_sm_get_tx_band(),
                             temp_script_data_ptr,
                             NULL);
        
      FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_capture_and_process: Non-conflict Enabling Drx dev : %d on band : %d ",
                                 drx_device, rflte_ftm_mc_sm_get_tx_band());
    }
  }
  #endif
  #endif
  }
  
  #endif /* FEATURE_RF_FBRX */

  return result;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_lm_test
 
  @details
  This function tests LM config for FBRx
 
*/
void ftm_common_fbrx_lm_test( void )
{
  lm_handle_type handle;
  boolean fbrx_result=FALSE;
  rflm_fbrx_dm_template_t* fbrx_data_ptr;
  uint32 tx_handle = ftm_common_fbrx_get_valid_tx_handle(RFCOM_WCDMA_MODE, 0, 0);
  uint8 fbrx_chain = 3;

  fbrx_result = rfcommon_fbrx_mc_enter_mode(&handle, RFCOM_WCDMA_MODE, 0);
  FTM_MSG_2( FTM_HIGH, "rfcommon_fbrx_mc_enter_mode: result %d, handle %d", fbrx_result, handle );
  fbrx_result = rfcommon_fbrx_mc_init_tx(handle, RFCOMMON_FBRX_UMTS_SC, tx_handle, RFCOM_WCDMA_MODE, 0);
  FTM_MSG_2( FTM_HIGH, "rfcommon_fbrx_mc_init_tx: result %d, handle %d", fbrx_result, handle );

  /* update the fbrx_mode in DM for all valid tx handle in ftm mode */

  fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(tx_handle);
  if (fbrx_data_ptr != NULL)
  {
    fbrx_result = rfcommon_fbrx_mc_update_mode( tx_handle, RFCOMMON_FBRX_UMTS_SC);
  }

  fbrx_result = rfcommon_fbrx_enable(handle, fbrx_chain);
  FTM_MSG_2( FTM_HIGH, "rfcommon_fbrx_enable: result %d, handle %d", fbrx_result, handle );
  #ifdef FEATURE_RF_FBRX
  rflm_fbrx_bridge_config(TRUE);
  FTM_MSG_1( FTM_HIGH, "rflm_fbrx_bridge_config on: handle %d", handle );
  rflm_fbrx_activate_chain(handle);
  FTM_MSG_1( FTM_HIGH, "rflm_fbrx_activate_chain: handle %d", handle );
  #endif /* FEATURE_RF_FBRX */
  fbrx_result = rfcommon_fbrx_mc_exit(handle, RFCOM_WCDMA_MODE);
  FTM_MSG_2( FTM_HIGH, "rfcommon_fbrx_mc_exit: result %d, handle %d", fbrx_result, handle );
} /* ftm_common_fbrx_lm_test */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_get_valid_tx_handle
 
  @details
  This function returns a valid Tx handle for FBRX
 
*/

/*----------------------------------------------------------------------------*/
rflm_handle_tx_t  
ftm_common_fbrx_get_valid_tx_handle( rfm_mode_enum_type tech, 
                                     rfm_device_enum_type device, 
                                     uint32 carrier_idx)
{
  rflm_handle_tx_t tx_handle = -1;

  uint8 sub_id=0;
  
  switch (tech)
  {
#ifdef FEATURE_WCDMA
    case RFM_IMT_MODE:
      #ifdef ATLAS_RF_WCDMA_PRESI
      tx_handle = rf_wcdma_mc_tx_get_modem_chain(device);
      #else
      tx_handle = rfwcdma_data_get_rflm_buf_idx(device);
      #endif
      break;    
#endif

#ifdef FEATURE_CDMA1X
     case RFM_1X_MODE:
#endif /* #ifdef (FEATURE_CDMA1X) */
#ifdef FEATURE_HDR
     case RFM_1XEVDO_MODE:
#endif /* #ifdef (FEATURE_HDR) */
#if defined(FEATURE_HDR)&&defined(FEATURE_CDMA1X)
     case RFM_SVDO_MODE:
#endif /* #if defined(FEATURE_HDR)&&defined(FEATURE_CDMA1X) */ 
#if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR)
        tx_handle = rf_cdma_data_get_txlm_handle(device);
       break;
#endif /* #if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR) */

#if defined(FEATURE_LTE)
     case RFM_LTE_MODE:
     case RFM_LTE_FTM_MODE:
        (void) rflte_state_get_txlm_buffer_idx(sub_id,carrier_idx, &tx_handle);
       break;
#endif /* FEATURE_LTE */

#ifdef FEATURE_TDSCDMA
     case RFM_TDSCDMA_MODE:
     case RFM_TDSCDMA_MODE_FTM:
       tx_handle = rf_tdscdma_mc_lm_bufs.tx_buf_wrk.buf_idx;
       break;
#endif  /*FEATURE_TDSCDMA*/

     default:
       tx_handle = -1;
       FTM_MSG_1( FTM_ERROR, "ftm_common_fbrx_get_valid_tx_handle: Invalid tech = %d. ", tech );
       break;
  }

  return tx_handle;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_set_cal_state
 
  @details
  This function sets the ftm cal mode for FBRX
 
*/
void ftm_common_fbrx_set_cal_state( boolean ftm_cal_mode )
{
  rflm_fbrx_dm_template_t* fbrx_data_ptr;
  uint32 tx_handle;
  /* @todo: get valid tx handle from tech and remove the handle loop */
  for (tx_handle=0; tx_handle < RFLM_HANDLE_COUNTS; tx_handle++ )
  {
    fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(tx_handle);
    if (fbrx_data_ptr != NULL)
    {
      fbrx_data_ptr->fbrx_ctl.ftm_cal_mode = ftm_cal_mode;
    }
  }
} /* ftm_common_fbrx_set_cal_state */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_debug_get_sample_captures
 
  @details
  This function returns the sample captures
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_debug_get_sample_captures(ftm_common_pkt_type *cmd_ptr)
{
  ftm_fbrx_debug_sample_captures_response_type* response_ptr = NULL;
  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_debug_sample_captures_response_type));
  int buffer_copy_size = FTM_FBRX_CAPTURE_MAX_NUMBER_OF_BYTES_DIAG;
  int i = 0;

  response_ptr = (ftm_fbrx_debug_sample_captures_response_type*) rsp.pkt_payload;

  if (response_ptr == NULL)
  {
    return rsp; 
    FTM_MSG_HIGH("ftm_common_fbrx_debug_get_sample_captures: response ptr = NULL returned ",0, 0, 0);
  }
  else
  {
    memset(response_ptr, 0, sizeof(ftm_fbrx_debug_sample_captures_response_type));
  }

  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));  

  if (fbrx_sample_capture_init_data.max_num_valid_data_buf < 1)
  {
    /*there is nothing stored in the debug buffer. 
      return 0 size and do nothing.*/
    response_ptr->num_of_captures = 0;
    response_ptr->err_code        = FTM_FBRX_ERR_API_FAILED;
    FTM_MSG_HIGH("ftm_common_fbrx_debug_get_sample_captures: max valid data = %d, err code = %d", fbrx_sample_capture_init_data.max_num_valid_data_buf, response_ptr->err_code, 0);
  }
  else
  {
    if (ftm_common_fbrx_debug_samples_flag == TRUE)
    {
      ftm_common_fbrx_debug_sample_captures =
         modem_mem_alloc(sizeof(ftm_common_fbrx_debug_data_type) * fbrx_sample_capture_init_data.max_num_valid_data_buf, MODEM_MEM_CLIENT_RFA);

      if (ftm_common_fbrx_debug_sample_captures != NULL) 
      {
        FTM_MSG_HIGH("ftm_common_fbrx_debug_get_sample_captures: ftm_common_fbrx_debug_sample_captures memory successfully allocated for %d bufers", fbrx_sample_capture_init_data.max_num_valid_data_buf, 0, 0);
      fbrx_current_buffer_size = fbrx_sample_capture_init_data.max_num_valid_data_buf * sizeof(ftm_common_fbrx_debug_data_type);

      for (i = 0; i < fbrx_sample_capture_init_data.max_num_valid_data_buf; i++) 
      {
            memscpy(( uint8*) ftm_common_fbrx_debug_sample_captures + i*sizeof(ftm_common_fbrx_debug_data_type), 
            sizeof(ftm_common_fbrx_debug_data_type),
            fbrx_sample_capture_init_data.buffer_ptr[i],
            sizeof(ftm_common_fbrx_debug_data_type));
      }

      ftm_common_fbrx_debug_samples_flag = FALSE;
    }
      else
      {
        response_ptr->err_code  = FTM_FBRX_ERR_MEM_ALLOC_FAILED;
        FTM_MSG_HIGH( "ftm_common_fbrx_debug_get_sample_captures: ftm_common_fbrx_debug_sample_captures = NULL returned, err code = %d", 
                       response_ptr->err_code, 0, 0);
        return rsp; 
      }
    }

    response_ptr->num_of_captures = fbrx_sample_capture_init_data.max_num_valid_data_buf;
    response_ptr->version         = FTM_FBRX_SAMPLE_CAPTURE_VERSION;
    response_ptr->tech            = fbrx_sample_capture_init_data.tech;
    response_ptr->band            = fbrx_sample_capture_init_data.band;
    response_ptr->err_code        = fbrx_sample_capture_init_data.generic_err_code;

    FTM_MSG_HIGH("ftm_common_fbrx_debug_get_sample_captures: current buff size = %d ", fbrx_current_buffer_size, 0,0);

    if (fbrx_current_buffer_size > FTM_FBRX_CAPTURE_MAX_NUMBER_OF_BYTES_DIAG) 
    {
      response_ptr->is_last_packet = FALSE;
      fbrx_current_buffer_size = fbrx_current_buffer_size - FTM_FBRX_CAPTURE_MAX_NUMBER_OF_BYTES_DIAG;
      response_ptr->num_of_bytes = FTM_FBRX_CAPTURE_MAX_NUMBER_OF_BYTES_DIAG;
    } 
    else 
    {
      response_ptr->is_last_packet = TRUE;
      buffer_copy_size = fbrx_current_buffer_size;
      response_ptr->num_of_bytes = fbrx_current_buffer_size;
    }

    FTM_MSG_HIGH( "ftm_common_fbrx_debug_get_sample_captures: num_of_bytes precessed = %d, is_last_packet =%d", 
                   response_ptr->num_of_bytes, response_ptr->is_last_packet, 0 );

    memscpy(( void*) &(response_ptr->capture_data), buffer_copy_size, 
            ( uint8*) ftm_common_fbrx_debug_sample_captures + fbrx_debug_num_pkt_processed*FTM_FBRX_CAPTURE_MAX_NUMBER_OF_BYTES_DIAG,
            buffer_copy_size);

    if (response_ptr->is_last_packet == FALSE) {
      fbrx_debug_num_pkt_processed++;
    }
    else /* now clean up everything */
    {
      ftm_common_fbrx_debug_samples_flag = TRUE;
      fbrx_debug_num_pkt_processed = 0;
      fbrx_current_buffer_size = 0;
      modem_mem_free(ftm_common_fbrx_debug_sample_captures, MODEM_MEM_CLIENT_RFA);
      ftm_common_fbrx_dealloc_mem_sample_capture_buf();
    }

    FTM_MSG_HIGH( "ftm_common_fbrx_debug_get_sample_captures: num pkt processed = %d, buff size remaining = %d", 
                  fbrx_debug_num_pkt_processed, fbrx_current_buffer_size, 0 );
  }
  return rsp;
} /* ftm_common_fbrx_debug_get_sample_captures */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_get_mode
 
  @details
  This function returns the currently used FBRx mode [0-3]
 
*/
uint8 ftm_common_fbrx_get_mode( rfm_mode_enum_type tech, 
                                rfm_device_enum_type device, 
                                uint32 carrier_idx )
{
  rflm_fbrx_dm_template_t* fbrx_data_ptr;
  uint32 tx_handle;
  uint8 fbrx_mode=3;

  tx_handle = ftm_common_fbrx_get_valid_tx_handle( tech, device, carrier_idx);

  fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(tx_handle);

  if (fbrx_data_ptr != NULL)
  {      
    fbrx_mode = fbrx_data_ptr->fbrx_ctl.fbrx_mode;
  }

  return fbrx_mode;
}

/*----------------------------------------------------------------------------*/

void ftm_common_fbrx_log_results_init_func( void)
{
  ftm_log_config_table_entry  *log_config_ptr = NULL;
  log_config_ptr = ftm_log_find_log_id( FTM_LOG_FBRX_RESULTS );

  if( log_config_ptr == NULL )
  {
    FTM_MSG_ERROR("NULL pointer obtained for log id %d ",FTM_LOG_FBRX_RESULTS,0,0);
    return;
  }

  ftm_common_enter_critical_section();

  /* must allocate an internal buffer to collect the data */
  ftm_common_fbrx_log_results_data =
  (ftm_common_fbrx_log_results_data_type *)ftm_malloc( log_config_ptr->log_config.num_buffer
                                             * sizeof (ftm_common_fbrx_log_results_data_type)); /*lint !e737 suppress int to unsigned int */

  /* check that we have memory to work with */
  if(ftm_common_fbrx_log_results_data == NULL)
  {
    FTM_MSG_ERROR("ftm_common_fbrx_log_results_init_func: Unable to allocate FBRX results data buffer",0,0,0);
  }

  ftm_common_leave_critical_section();

}
/*----------------------------------------------------------------------------*/

void ftm_common_fbrx_log_results_exit_func( void)
{
  ftm_common_enter_critical_section();
  if(ftm_common_fbrx_log_results_data != NULL)
  {
    ftm_free((void*)ftm_common_fbrx_log_results_data); /* free the internal buffer */
  }
  ftm_common_leave_critical_section();
}
/*----------------------------------------------------------------------------*/

void ftm_common_fbrx_log_results_cb(int4 interval)
{
  static int buf     = 0;
  ftm_common_fbrx_log_results_t  *fbrx_log  = NULL;
  ftm_log_config_table_entry     *log_config_ptr;

  rflm_fbrx_dm_template_t* fbrx_ptr = NULL;
  uint32 tx_handle = 0;
  int32 ls_err = 0, real_lse = 0, img_lse = 0;
  uint8 buf_idx = 0;
  uint8 run_idx = 0;
  uint8 results_buf_idx = 0;

  log_config_ptr = ftm_log_find_log_id( FTM_LOG_FBRX_RESULTS);
  if(log_config_ptr == NULL)
  {
   FTM_MSG_ERROR("NULL pointer obtained for log id %d ",FTM_LOG_FBRX_RESULTS,0,0);
   return;
  }

  ftm_common_enter_critical_section();

  /* @todo: get valid tx handle from tech and remove the handle loop */
  for (tx_handle=0; tx_handle < RFLM_HANDLE_COUNTS; tx_handle++ )
  {
    fbrx_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(tx_handle);
    if (fbrx_ptr != NULL)
    {
      buf_idx = rflm_mod_wrap( fbrx_ptr->exp_gain_data.read_buf_idx, RFLM_FBRX_MAX_BUFFER_SIZE );
      results_buf_idx = fbrx_ptr->fbrx_ctl.fbrx_results_buf_idx;

      ftm_common_fbrx_log_results_data[buf].version = 1; //14

      ftm_common_fbrx_log_results_data[buf].txagc = 
          fbrx_ptr->exp_gain_data.sf_data[buf_idx].slot_data[run_idx].last_tx_agc;   

      ftm_common_fbrx_log_results_data[buf].pa = 
          fbrx_ptr->exp_gain_data.sf_data[buf_idx].slot_data[run_idx].pa_state_idx;

      ftm_common_fbrx_log_results_data[buf].gs = 
          fbrx_ptr->exp_gain_data.sf_data[buf_idx].slot_data[run_idx].fbrx_gain_state;

      ftm_common_fbrx_log_results_data[buf].gn_est = 
          fbrx_ptr->results_log[results_buf_idx].results[run_idx].gain_est;
        
      ftm_common_fbrx_log_results_data[buf].gn_err = 
          fbrx_ptr->exp_gain_data.sf_data[buf_idx].slot_data[run_idx].gain_error_dB10;  

      /* Calculate the mag of LS error from real/imag part */
      ls_err = (int32)fbrx_ptr->results_log[results_buf_idx].results[run_idx].ls_err; 
      real_lse = ls_err >> 16; 
      img_lse  = (ls_err << 16) >> 16; 
      ftm_common_fbrx_log_results_data[buf].mag_lse = FTM_COMMON_FBRX_RET_MAG_LSE(real_lse, img_lse);
                     
      ftm_common_fbrx_log_results_data[buf].cal_pwr = 
          fbrx_ptr->exp_gain_data.sf_data[buf_idx].slot_data[run_idx].cal_pwr;

      ftm_common_fbrx_log_results_data[buf].cal_gn = 
          fbrx_ptr->exp_gain_data.sf_data[buf_idx].slot_data[run_idx].last_cal_gain;

      ftm_common_fbrx_log_results_data[buf].temp_comp = 
          fbrx_ptr->exp_gain_data.sf_data[buf_idx].slot_data[run_idx].last_temp_comp;

      ftm_common_fbrx_log_results_data[buf].ubias = 
          fbrx_ptr->exp_gain_data.sf_data[buf_idx].slot_data[run_idx].ubias_new;

      ftm_common_fbrx_log_results_data[buf].rxscale = 
          fbrx_ptr->exp_gain_data.sf_data[buf_idx].slot_data[run_idx].rxscale_new;

      ftm_common_fbrx_log_results_data[buf].txscale = 
          fbrx_ptr->exp_gain_data.sf_data[buf_idx].slot_data[run_idx].txscale_new;

      ftm_common_fbrx_log_results_data[buf].tech = fbrx_ptr->fbrx_ctl.tech;

      ftm_common_fbrx_log_results_data[buf].band = fbrx_ptr->fbrx_ctl.fbrx_band;

      ftm_common_fbrx_log_results_data[buf].bandwidth = fbrx_ptr->fbrx_ctl.bandwidth;

      ftm_common_fbrx_log_results_data[buf].fbrx_mode = fbrx_ptr->fbrx_ctl.fbrx_mode;

      ftm_common_fbrx_log_results_data[buf].slot = 
          fbrx_ptr->exp_gain_data.sf_data[buf_idx].slot_data[run_idx].valid_slot_idx;

      ftm_common_fbrx_log_results_data[buf].nv = fbrx_ptr->fbrx_ctl.fbrx_nv_ctrl_status;      
    }
  }

  if(++buf >= log_config_ptr->log_config.num_buffer)
  {
   fbrx_log = (ftm_common_fbrx_log_results_t *)
               ftm_log_malloc( (uint16)FTM_LOG_FBRX_RESULTS,
                                sizeof (ftm_common_fbrx_log_results_t)+ ( log_config_ptr->log_config.num_buffer - 1 )
                                * sizeof(ftm_common_fbrx_log_results_data_type) ); /*lint !e737 suppres int to unsigned int*/
   if(fbrx_log != NULL)
   {
    /* have to copy the internal buffer to the suite_log packet, we cannot
    simply accumulate the data in the suite_log packet directly since any memory
    allocated with ftm_log_malloc must be passed to log_commit soon afterward
    (where "soon" is apparently less than tens of ms) */
    memscpy( ( void*) &( fbrx_log->data),
             sizeof(ftm_common_fbrx_log_results_data_type),
             ( void*) ftm_common_fbrx_log_results_data,
             log_config_ptr->log_config.num_buffer * sizeof( ftm_common_fbrx_log_results_data_type)); /*lint !e737 suppres int to unsigned int*/

    /* send log */
    log_commit( ( ftm_log_type*) fbrx_log );
   }

   buf = 0;
  }

  ftm_common_leave_critical_section();
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_meas_ant_pwr
 
  @details
  This function returns the average antenna power estimate for Self Test
 
  @params
  Number of averages, Pointers to populate estimated power values in dB10 and dB100.
  current FBRx Gain Stage in which measurement was made
 
  @return
  Returns TRUE on success, else FALSE
 
*/
boolean ftm_common_fbrx_meas_ant_pwr( 
   int16 * pwr_est_db10, 
   int16 * pwr_est_db100, 
   uint8 num_averages, 
   uint8 * fbrx_gs 
)
{
  boolean done = FALSE, api_status = TRUE;
  uint16 counter = 0, run_idx = 0, read_buf_idx = 0;
  uint32 handle;
  rflm_fbrx_dm_template_t* fbrx_data_ptr;
  double curr_gain_est = 0.0, cal_gain = 0.0, curr_ubias = 0.0, cal_rxscale = 0.0, curr_rxscale = 0.0, cal_ubias = 0.0;
  double pwr_offset_dbl = 0.0, cal_pwr_dbl = 0.0, pwr_est_dbl = 0.0, pwr_est_db10_dbl = 0.0, pwr_est_db10_dbl_avg = 0.0;
  int16 gain_error_db10 = 0, last_tx_agc_db10 = 0, temp_comp_db10 = 0;
  int32 pwr_est_db1000 = 0;
  uint8 avg_iter = 0;
  uint8 iter = 0;
  boolean temp_tx_lin_update_flag[RFLM_HANDLE_COUNTS] = {FALSE};
  uint8 temp_tx_lin_flag_restore_handle_idx[RFLM_HANDLE_COUNTS] = {0xFF};
  uint8 num_tx_lin_flag_restore = 0;
  uint8 results_buf_idx=0;

  /* @todo: get valid tx handle from tech and remove the handle loop */
  for (handle = 0; handle < RFLM_HANDLE_COUNTS; handle++)
  {
    fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(handle);
    if (fbrx_data_ptr != NULL)
    {
      temp_tx_lin_flag_restore_handle_idx[num_tx_lin_flag_restore] = handle;
      temp_tx_lin_update_flag[num_tx_lin_flag_restore] = fbrx_data_ptr->fbrx_ctl.txlin_update_flag;
      num_tx_lin_flag_restore++;
      fbrx_data_ptr->fbrx_ctl.txlin_update_flag = FALSE;
    }
  }


  FTM_MSG(FTM_ERROR, "Entered ftm_common_fbrx_meas_ant_pwr" );
  for (avg_iter = 0; avg_iter < num_averages; avg_iter++)
  {
    done = FALSE;
    counter = 0;
    /* @todo: get valid tx handle from tech and remove the handle loop */
    for (handle = 0; handle < RFLM_HANDLE_COUNTS; handle++)
    {
      fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(handle);
      if (fbrx_data_ptr != NULL)
      {
        for (run_idx = 0; run_idx < RFLM_FBRX_LTE_MAX_RUNS_PER_SF; run_idx++)
        {
          // Turn all valid handles' return flag to false, because we want to trigger fresh reading
          fbrx_data_ptr->fbrx_ctl.uk_results_valid_flag[run_idx] = FALSE; 
        }
      }
    }

    // Measurement should come automatically from Tx AGC loop, if it is running and the return flag
    // will be updated, wait until the flag is TRUE, max wait time is 30000 us or 30 ms
    while (!done && counter < RFCOMMON_FBRX_MAX_TXPWRMEAS_TIME_P1USEC)
    {
      DALSYS_BusyWait(100);
      /* @todo: get valid tx handle from tech and remove the handle loop */
      for (handle = 0; handle < RFLM_HANDLE_COUNTS; handle++)
      {
        fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(handle);
        if (fbrx_data_ptr != NULL)
        {
          // only check 0-th run data, because currently that has valid meas only
          if (fbrx_data_ptr->fbrx_ctl.uk_results_valid_flag[0]) 
          {
            done = TRUE;
            break; // break handle loop
          }
        }
      }
      if (done == TRUE)
      {
        break; // break counter loop
      }
      counter++;
    }

    if (done == FALSE)
    {
      FTM_MSG_1(FTM_ERROR, "ftm_common_fbrx_meas_ant_pwr: Measurement did not complete in allotted %d ms or no valid fbrx handle", 
                            RFCOMMON_FBRX_MAX_TXPWRMEAS_TIME_P1USEC/10);
      api_status =  FALSE;
    }

    if (api_status)
    {
      results_buf_idx = fbrx_data_ptr->fbrx_ctl.fbrx_results_buf_idx;
      FTM_MSG_3(FTM_HIGH, "ftm_common_fbrx_meas_ant_pwr: FBRx Handle %d has %d valid results with fbrx_gs %d", 
                handle, fbrx_data_ptr->results_log[results_buf_idx].num_valid_results,
                fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].fbrx_gain_state);
      *fbrx_gs  = fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].fbrx_gain_state;

      // Total RFLM_FBRX_MAX_BUFFER_SIZE read buffers are populated in circular fashion, need to read last valid one
      if (fbrx_data_ptr->exp_gain_data.read_buf_idx == 0)
      {
        read_buf_idx = RFLM_FBRX_MAX_BUFFER_SIZE - 1;
      }
      else
      {
        read_buf_idx = fbrx_data_ptr->exp_gain_data.read_buf_idx - 1;
      }

      run_idx = 0; // only 0-th run per SF was checked earlier and should have valid value

      gain_error_db10 = fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].gain_error_dB10;
      last_tx_agc_db10 = fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].last_tx_agc;

      // power estimate in dB10 from TxAGC update loop, 0.1dB resolution
      pwr_est_db10_dbl += (double)last_tx_agc_db10 - (double)gain_error_db10;

      FTM_MSG_3(FTM_HIGH, "ftm_common_fbrx_meas_ant_pwr: last_tx_agc_db10 (%d) - gain_error_db10 (%d) = pwr_est_db10 (%d) ",
                last_tx_agc_db10,
                gain_error_db10,
                last_tx_agc_db10 - gain_error_db10);


      // calculate power estimate in dB100 as well, 0.01dB resolution, using double-precision math
      cal_pwr_dbl = (double)(fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].cal_pwr);
      temp_comp_db10 = fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].last_temp_comp;
      cal_rxscale = (double)(fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].cal_rxscale);
      cal_ubias = (double)(fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].cal_ubias);
      cal_gain = (double)(fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].last_cal_gain);

      FTM_MSG_5(FTM_HIGH, "cal_pwr_db10: %d, temp_comp_db10: %d, cal_rx_scale: %d, cal_ubias: %d, cal_gain:%d",
                fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].cal_pwr,
                fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].last_temp_comp,
                fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].cal_rxscale,
                fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].cal_ubias,
                fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].last_cal_gain);

      curr_rxscale = (double)(fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].rxscale_new);
      curr_ubias = (double)(fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].ubias_new);
      curr_gain_est = (double)(fbrx_data_ptr->results_log[results_buf_idx].results[run_idx].gain_est);
      //curr_gain_est = (double)(fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].current_gain);

      FTM_MSG_3(FTM_HIGH, "curr_rxscale: %d, curr_ubias: %d, curr_gain_est: %d", 
                fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].rxscale_new,
                fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].ubias_new,
                fbrx_data_ptr->results_log[results_buf_idx].results[run_idx].gain_est
                //fbrx_data_ptr->exp_gain_data.sf_data[read_buf_idx].slot_data[run_idx].current_gain
                );

      // need to subraction temp_comp_db10 val
      pwr_offset_dbl = 20*log10((curr_gain_est*cal_rxscale*pow(2.0,cal_ubias))/(cal_gain*curr_rxscale*pow(2.0,curr_ubias))); 

      pwr_est_dbl = ((double)cal_pwr_dbl/10) + pwr_offset_dbl;

      pwr_est_db1000 += (int32)(pwr_est_dbl*1000) - (int32)(temp_comp_db10*100);

    }
  }

  if (api_status)
  {
    if ( pwr_est_db1000 < 0)
    {
      *pwr_est_db100 = (int16)((pwr_est_db1000-5)/(10*num_averages));
    }
    else
    {
      *pwr_est_db100 = (int16)((pwr_est_db1000+5)/(10*num_averages));
    }

    FTM_MSG_3(FTM_HIGH, "round ( pwr_est_accum_db1000: %d / 10*num_averages: %d ) = pwr_est_db100: %d", 
              pwr_est_db1000,
              num_averages,
              *pwr_est_db100);

    pwr_est_db10_dbl_avg = pwr_est_db10_dbl*10/num_averages;

    if ( ( ((int16)pwr_est_db10_dbl)% 5 == 0 ) && (((int16)pwr_est_db10_dbl) < 0))
    {
      * pwr_est_db10 = (int16)(((int16)pwr_est_db10_dbl_avg-5)/10);
    }
    else
    {
      * pwr_est_db10 = (int16)(((int16)pwr_est_db10_dbl_avg+5)/10);
    }

    FTM_MSG_3(FTM_HIGH, "round ( pwr_est_accum_db10: %d / num_averages: %d ) = pwr_est_db10: %d", 
              (int32)pwr_est_db10_dbl,
              num_averages,
              *pwr_est_db10);
  }

  for (iter = 0; iter < num_tx_lin_flag_restore; iter++)
  {
    fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(temp_tx_lin_flag_restore_handle_idx[iter]);
    if (fbrx_data_ptr != NULL)
    {
      fbrx_data_ptr->fbrx_ctl.txlin_update_flag = temp_tx_lin_update_flag[iter];
    }
  }


  return api_status;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_get_corr_setpoint
 
  @details
  This function returns the RX_RX_CONJ_CORR_SETPT for each tech
 
*/
uint16 ftm_common_fbrx_get_corr_setpoint( rfm_mode_enum_type tech)                       
{
  uint16 corr_setpoint = 8192;

  switch (tech)
  {
     #ifdef FEATURE_WCDMA
     case RFM_IMT_MODE:
       corr_setpoint = 8192;
       break;
     #endif

     #ifdef FEATURE_CDMA1X
     case RFM_1X_MODE:
     #endif /* #ifdef (FEATURE_CDMA1X) */
     #ifdef FEATURE_HDR
     case RFM_1XEVDO_MODE:
     #endif /* #ifdef (FEATURE_HDR) */
     #if defined(FEATURE_HDR)&&defined(FEATURE_CDMA1X)
     case RFM_SVDO_MODE:
     #endif /* #if defined(FEATURE_HDR)&&defined(FEATURE_CDMA1X) */ 
     #if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR)
       corr_setpoint = 8192;
       break;
     #endif /* #if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR) */

   #ifdef FEATURE_LTE
     case RFM_LTE_MODE:
     case RFM_LTE_FTM_MODE:
       corr_setpoint = 8192;
       break;
   #endif

   #ifdef FEATURE_TDSCDMA
     case RFM_TDSCDMA_MODE:
     case RFM_TDSCDMA_MODE_FTM:
       corr_setpoint = 8192;
       break;
   #endif /*FEATURE_TDSCDMA*/

     default:
       corr_setpoint = 8192;
     break;
  }

  return corr_setpoint;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_get_corr_setpoint
 
  @details
  This function returns the ftm_fbrx_autocal_type data pointer for given tech/band
 
*/
ftm_fbrx_autocal_type* 
ftm_common_fbrx_get_autocal_init_data( rflm_fbrx_dm_template_t* fbrx_data_ptr, uint8 gs_idx)
{
  ftm_fbrx_autocal_type* return_val = NULL;

  if (fbrx_data_ptr != NULL) 
  {
     uint8 band = fbrx_data_ptr->fbrx_ctl.fbrx_band;
     uint8 mode = fbrx_data_ptr->fbrx_ctl.fbrx_mode;
     rfm_mode_enum_type tech = fbrx_data_ptr->fbrx_ctl.rfm_mode;
     rfcommon_fbrx_nv_container_type c_val = fbrx_data_ptr->fbrx_ctl.container_val;

     switch (tech)
     {
        #ifdef FEATURE_WCDMA
        case RFM_IMT_MODE:
           if ( (band < RFCOM_NUM_WCDMA_BANDS) && 
                (mode < RFCOMMON_FBRX_NUM_MODES) && 
                (gs_idx < RFCOMMON_FBRX_GAIN_VS_FREQ_NUM_V2) ) 
           {
             return_val = &(ftm_fbrx_autocal_data.wcdma_data[band].fbrx_mode[mode].fb_gs[gs_idx]);
           }
          break;
        #endif

        #ifdef FEATURE_CDMA1X
        case RFM_1X_MODE:
        #endif /* #ifdef (FEATURE_CDMA1X) */
        #ifdef FEATURE_HDR
        case RFM_1XEVDO_MODE:
        #endif /* #ifdef (FEATURE_HDR) */
        #if defined(FEATURE_HDR)&&defined(FEATURE_CDMA1X)
        case RFM_SVDO_MODE:
        #endif /* #if defined(FEATURE_HDR)&&defined(FEATURE_CDMA1X) */ 
        #if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR)
           if ( (band < RFM_CDMA_MAX_BAND) && 
                (mode < RFCOMMON_FBRX_NUM_MODES) && 
                (gs_idx < RFCOMMON_FBRX_GAIN_VS_FREQ_NUM_V2) ) 
           {
             return_val = &(ftm_fbrx_autocal_data.cdma_data[band].fbrx_mode[mode].fb_gs[gs_idx]);
           }
          break;
        #endif /* #if defined(FEATURE_CDMA1X) || defined(FEATURE_HDR) */

      #ifdef FEATURE_LTE
        case RFM_LTE_MODE:
        case RFM_LTE_FTM_MODE:
           if ( (band < RFCOM_NUM_LTE_BANDS) && (c_val < RFCOMMON_FBRX_NV_CONTAINER_MAX) &&
                (mode < RFCOMMON_FBRX_NUM_MODES) && 
                (gs_idx < RFCOMMON_FBRX_GAIN_VS_FREQ_NUM_V2) )               
           {
             return_val = &(ftm_fbrx_autocal_data.lte_data[c_val][band].fbrx_mode[mode].fb_gs[gs_idx]);
           }
          break;
      #endif

      #ifdef FEATURE_TDSCDMA
        case RFM_TDSCDMA_MODE:
        case RFM_TDSCDMA_MODE_FTM:
           if ( (band < RFCOM_NUM_TDSCDMA_BANDS) && 
                (mode < RFCOMMON_FBRX_NUM_MODES) && 
                (gs_idx < RFCOMMON_FBRX_GAIN_VS_FREQ_NUM_V2) )            
           {
             return_val = &(ftm_fbrx_autocal_data.tds_data[band].fbrx_mode[mode].fb_gs[gs_idx]);
           }
          break;
      #endif /*FEATURE_TDSCDMA*/

        default:
          return_val = NULL;
        break;
     }
  }

  return return_val;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_capture_and_process_v2
 
  @details
  Triggers a single capture and processing of FBRx samples. Intended for use
  during calibration.
 
  @param ftm_common_fbrx_capture_input_type.tx_device_id
           set to 0 for WTR0, 1 for WTR1, etc.
 
  @param ftm_common_fbrx_capture_input_type.fbrx_gain_state
           Set to desired FBRx gain state.
 
*/

ftm_common_fbrx_capture_result_v2_type ftm_common_fbrx_capture_and_process_v2
(
  ftm_common_fbrx_capture_input_v2_type input
)
{
  uint32 handle;
  #ifdef FEATURE_RF_FBRX
  rflm_fbrx_dm_template_t *fbrx_data_ptr = NULL, *valid_data_ptr=NULL;
  rflm_handle_rx_t fbrx_lm_handle = -1;
  rfcommon_fbrx_error_type api_status = RFCOMMON_FBRX_SUCCESS;
  #endif
  ftm_common_fbrx_capture_result_v2_type output = {0};

  rfdevice_rxtx_common_class* dev_ptr; 
  boolean dev_status = TRUE;
  uint8 i;
  boolean done = FALSE;
  uint8 counter = 0;
  int32 real_err, imag_err, complex_err;
  rfm_mode_enum_type rfm_tech;
  rfcom_band_type_u rfcom_band;

  int32 real_rx_sum = 0, imag_rx_sum = 0;
  int32 fbrx_dc_dBFS10 = 0;
  double max_rx_sum = 0;

  uint32 signal_rms_mv10 = 0;

  uint16 xcorr_peak_val = 0;
  uint8  xcorr_peak_idx = 0;

  uint8 cur_fbrx_mode =0, cur_fbrx_band = 0;
  uint8 sc_buf_idx = fbrx_sample_capture_init_data.max_num_valid_data_buf; /* current sample capture buffer idx */
  uint32 curr_channel =0;
  output.status = RFCOMMON_FBRX_ERROR;
  uint8 results_buf_idx = 0;
  boolean ftm_rf_mode = FALSE;  

  #ifdef FEATURE_RF_HAS_WTR3925
  #if defined(FEATURE_LTE)
  rfm_device_enum_type drx_device;
  rfdevice_lte_script_data_type *temp_script_data_ptr = NULL;
  boolean lm_config_status = FALSE;
  lm_handle_type lm_config_handle;
  #endif
  #endif

  uint8 sub_id=0;

  dev_ptr = rfcommon_core_get_tx_device_ptr((rfm_device_enum_type)input.tx_device_id, input.tech, input.band);

  if (input.fbrx_gain_state > RFLM_FBRX_NUM_GAIN_STATES-1)
  {
    input.fbrx_gain_state = RFLM_FBRX_NUM_GAIN_STATES-1;
  }

  dev_status &= rfcommon_fbrx_mc_enable_rf_dev(dev_ptr, input.fbrx_gain_state);

  #ifdef FEATURE_RF_FBRX

  #if defined(FEATURE_LTE)
  handle = ftm_common_fbrx_get_valid_tx_handle( input.tech, 
                                                (rfm_device_enum_type)input.tx_device_id,
                                                rflte_ftm_mc_sm_get_carrier_index() );
  #else 
  handle = ftm_common_fbrx_get_valid_tx_handle( input.tech, 
                                                (rfm_device_enum_type)input.tx_device_id,
                                                0 );
  #endif

  fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(handle);

  if (fbrx_data_ptr != NULL)
  {   
    rfm_tech = input.tech;
    cur_fbrx_band =  fbrx_data_ptr->fbrx_ctl.fbrx_band;
    rfcom_band = rfcommon_fbrx_mc_convert_uint8_to_rfcom_band(rfm_tech, cur_fbrx_band);

    fbrx_sample_capture_init_data.tech = rfm_tech;
    fbrx_sample_capture_init_data.band = cur_fbrx_band; 
    cur_fbrx_mode = fbrx_data_ptr->fbrx_ctl.fbrx_mode;

    curr_channel = ftm_common_fbrx_get_tx_chan(rfm_tech, input.tx_device_id);
    /* Get FBRx rxlm handle */
    fbrx_lm_handle = rfcommon_fbrx_mc_get_tech_handle( rfm_tech );

    if ((rfm_tech == RFM_LTE_MODE) || (rfm_tech == RFM_LTE_MODE))
    {
      fbrx_lm_handle = fbrx_data_ptr->fbrx_ctl.fbrx_lm_handle;
	  
      ftm_rf_mode = rflm_lte_get_ftm_rf_flag_per_cell(RFLM_LTE_MC_CELL0);      
	  
      FTM_MSG_3( FTM_HIGH, "ftm_common_fbrx_capture_and_process: Configuring on Tx handle : %d, FBRX handle :  %d, carrier : %d",
                            handle,fbrx_lm_handle,rflte_ftm_mc_sm_get_carrier_index());
    }

    if (ftm_rf_mode == FALSE)
    {
    if (fbrx_lm_handle != 0xFFFFFFFF)
    {
      /* Update the IQMC RSB coefficients to FBRX DM */
      api_status = rfcommon_fbrx_mc_update_rsb_coeff( input.tx_device_id, handle, fbrx_lm_handle, rfm_tech, rfcom_band );
    }
    else
    {
      api_status = RFCOMMON_FBRX_ERROR;
    }

    if (api_status == RFCOMMON_FBRX_ERROR)
    {
      FTM_MSG( FTM_ERROR, "ftm_common_fbrx_capture_and_process_v2: rfcommon_fbrx_mc_update_rsb_coeff() failed");
    }
    else
    {
      api_status  = rfcommon_fbrx_msm_write_iqmc_to_hw( fbrx_lm_handle, 
                                                        fbrx_data_ptr->rsb_data[input.fbrx_gain_state].iqmc );
      if (api_status == RFCOMMON_FBRX_ERROR)
      {
        FTM_MSG( FTM_ERROR, "ftm_common_fbrx_capture_and_process_v2: rfcommon_fbrx_msm_write_iqmc_to_hw() failed");
      }
    }

    #ifdef FEATURE_RF_HAS_WTR3925
    #if defined(FEATURE_LTE)
    
    if ((input.tech == RFM_LTE_MODE) || (input.tech == RFM_LTE_FTM_MODE))
    {
      temp_script_data_ptr = rflte_state_get_script_data_ptr
                             ( sub_id,
                               RFLTE_MC_PCELL_IDX,
                               RFLTE_MC_CARR_TYPE_TX
                             );
      if (temp_script_data_ptr != NULL)
      {
        temp_script_data_ptr->write_to_hw_flag = TRUE;
      }
      drx_device = rflte_ftm_mc_sm_get_carr_drx_device(RFLTE_MC_PCELL_IDX);
      if (rfc_lte_is_fb_device_conflict(drx_device,(rfm_device_enum_type)input.tx_device_id,rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX)))
      {
        rfdevice_lte_rx_disable(drx_device,
                                rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX),
                                temp_script_data_ptr,
                                NULL);

      }
      else if (rflte_state_get_intra_band_ul_ca_flag(sub_id))
      {
        drx_device = rflte_ftm_mc_sm_get_carr_drx_device(RFLTE_MC_SCELL1_IDX);
        
        rfdevice_lte_rx_disable(drx_device,
                                rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_SCELL1_IDX),
                                temp_script_data_ptr,
                                NULL);
        
        FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: ULCA: Non-conflict Disabling Drx dev : %d on band : %d ",
                                   drx_device, rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_SCELL1_IDX));
      }
    }
    #endif
    #endif

    for (i=0;i<RFLM_FBRX_LTE_MAX_RUNS_PER_SF;i++)
    {
      fbrx_data_ptr->fbrx_ctl.uk_results_valid_flag[i] = FALSE;
    }
    /* Set override values for single trigger mode */
    fbrx_data_ptr->override_ctl.counter = 1;
    fbrx_data_ptr->override_ctl.enable = TRUE;
    fbrx_data_ptr->override_ctl.use_count = TRUE;
    fbrx_data_ptr->override_ctl.override_mode = TRUE;
    fbrx_data_ptr->override_ctl.gain_state = input.fbrx_gain_state;

    fbrx_data_ptr->override_ctl.scale_rx = FTM_FBRX_RXSCALE_CAL_VAL;
    fbrx_data_ptr->override_ctl.ubias = 0;  
    /* Print input params for debugging */
    FTM_MSG_7( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: FBRx action=%d,  mode=%d, gain_state=%d, tech=%d, band =%d, chan=%d, rx_scale=%d", 
                          input.fbrx_action, cur_fbrx_mode, input.fbrx_gain_state, rfm_tech, cur_fbrx_band, curr_channel, fbrx_data_ptr->override_ctl.scale_rx);


    rflm_fbrx_log_sample_capture_buffer_alloc_cb( );
    }    

    while (!done && counter < 50)
    {
      DALSYS_BusyWait(100);
      if (fbrx_data_ptr->fbrx_ctl.uk_results_valid_flag[0])
      {
        done = TRUE;          
      }
      counter++;
    }

    if (dev_status==TRUE && done==TRUE)
    {
      output.status = RFCOMMON_FBRX_SUCCESS;
    }
    else
    {
      output.status = RFCOMMON_FBRX_ERROR;
      FTM_MSG( FTM_ERROR, "ftm_common_fbrx_capture_and_process_v2: Invalid FBRx capture.");
    }

    if (fbrx_data_ptr->fbrx_ctl.uk_results_valid_flag[0])
    {
      results_buf_idx = fbrx_data_ptr->fbrx_ctl.fbrx_results_buf_idx;

      if (output.status == RFCOMMON_FBRX_SUCCESS)
      {
        /* dBFS -> mV conversion formula: mV = 10^(dBFS/20) * ADC_Vpeak = 10^(dBFS/20) * 2V. */
        real_rx_sum = fbrx_data_ptr->results_log[results_buf_idx].results[0].rx_sum >> 32;
        imag_rx_sum = (fbrx_data_ptr->results_log[results_buf_idx].results[0].rx_sum << 32) >> 32;
        max_rx_sum = MAX(rflm_abs_satw(real_rx_sum), rflm_abs_satw(imag_rx_sum));
        signal_rms_mv10 = (int32)( (int64)( (max_rx_sum/fbrx_data_ptr->fbrx_ctl.nref) * 10000) >> 14 );
      }

      if (signal_rms_mv10 < RFCOMMON_FBRX_SIGNAL_UNDERFLOW_THRESH)
      {
        output.status = RFCOMMON_FBRX_RMS_SIG_UNDERFLOW;
        FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: FBRx signal_rms_mv10 (%d) < %d, Signal Underflow",
                              signal_rms_mv10, RFCOMMON_FBRX_SIGNAL_UNDERFLOW_THRESH);
      }
      else if (signal_rms_mv10 > RFCOMMON_FBRX_SIGNAL_OVERFLOW_THRESH)
      {
        output.status = RFCOMMON_FBRX_RMS_SIG_OVERFLOW;
        FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: FBRx signal_rms_mv10 (%d) > %d, Signal Overflow", 
                              signal_rms_mv10, RFCOMMON_FBRX_SIGNAL_OVERFLOW_THRESH);
      }

      /* Get the computed fbrx dc value in dBFS10 unit from FBRx DM*/
      fbrx_dc_dBFS10 = fbrx_data_ptr->fbrx_ctl.rx_dc_dbfs;
      FTM_MSG_1( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: fbrx_dc_dbfs10=%d", fbrx_dc_dBFS10);

      /* Compute the peak xcorr values */
      for (i=0;i<RFLM_FBRX_XCORR_LOG_SIZE;i++)
      {
        output.results[0].xcorr_log[i] = fbrx_data_ptr->results_log[results_buf_idx].results[0].xcorr_log[i];
      }
      xcorr_peak_idx = fbrx_data_ptr->results_log[results_buf_idx].results[0].xcorr_peak_idx;
      xcorr_peak_val = fbrx_data_ptr->results_log[results_buf_idx].results[0].xcorr_log[xcorr_peak_idx];

      FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: xcorr_peak_val=%d, xcorr_peak_idx=%d", xcorr_peak_val, xcorr_peak_idx );

      /* assuming single captures only for now */
      /* @todo: Update results when dual capture support is added */
      output.num_of_captures = 1;
      output.results[0].gain = fbrx_data_ptr->results_log[results_buf_idx].results[0].gain_est;
      output.results[0].gain_imbalance = fbrx_data_ptr->results_log[results_buf_idx].results[0].gain_imbalance;
      output.results[0].phase_imbalance = fbrx_data_ptr->results_log[results_buf_idx].results[0].phase_imbalance;
      output.results[0].rsb = fbrx_data_ptr->results_log[results_buf_idx].results[0].next_rsb;
      output.results[0].ls_err = fbrx_data_ptr->results_log[results_buf_idx].results[0].ls_err;
      output.results[0].residual_dc = fbrx_dc_dBFS10;
      output.results[0].rxscale = fbrx_data_ptr->override_ctl.scale_rx;
      output.results[0].ubias = fbrx_data_ptr->override_ctl.ubias;

      output.results[0].fbrx_mode = cur_fbrx_mode;
      output.results[0].tx_tx_conj_corr = fbrx_data_ptr->results_log[results_buf_idx].results[0].tx_tx_conj_corr; 
      output.results[0].tx_sum = fbrx_data_ptr->results_log[results_buf_idx].results[0].tx_sum;
      output.results[0].rx_sum = fbrx_data_ptr->results_log[results_buf_idx].results[0].rx_sum;
      output.results[0].rx_rx_conj_corr = fbrx_data_ptr->results_log[results_buf_idx].results[0].rx_rx_conj_corr;

      output.results[0].rms_sig_level = signal_rms_mv10;
      /* @todo: need to check for expected power */
      output.results[0].fb_gain_state = fbrx_data_ptr->override_ctl.gain_state;

      output.results[0].xcorr_peak_val  = xcorr_peak_val;
      output.results[0].xcorr_peak_idx = xcorr_peak_idx;

      valid_data_ptr = fbrx_data_ptr;
    }

    complex_err = (int32)(output.results[0].ls_err);
    real_err = complex_err >> 16;
    imag_err = (complex_err << 16) >> 16;

    if ( (valid_data_ptr != NULL) && (sc_buf_idx < FTM_FBRX_MAX_NUM_SC_DATA_BUF) )
    {
      if (fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx] == NULL )
      {
        /* Allocate memory and update the pointer to the static variable */
        fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx] = modem_mem_alloc( sizeof(ftm_common_fbrx_debug_data_type), MODEM_MEM_CLIENT_RFA);

        if ( fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx] != NULL )
        {
          FTM_MSG_1( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: Saving debug capture %d", sc_buf_idx );
          FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: FBRx gain estimate %d, FBRx RSB 0x%08X", 
                                output.results[0].gain, output.results[0].rsb );

          for (i=0;i<RFLM_FBRX_XCORR_LOG_SIZE;i++)
          {
            FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: FBRx xcorr[%d] = %d", i, output.results[0].xcorr_log[i]);
          }

          fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->gain_state =  input.fbrx_gain_state;
          fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->channel = curr_channel;
          fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->fbrx_mode = valid_data_ptr->fbrx_ctl.fbrx_mode;

          if( (abs(real_err) > RFCOMMON_FBRX_CAL_DEBUG_THRESH || abs(imag_err) > RFCOMMON_FBRX_CAL_DEBUG_THRESH) )
          {
            /* Bad SC */
            fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->fbrx_sample_is_good = FALSE;
          }
          else
          {
            /* Good SC */
            fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->fbrx_sample_is_good = TRUE;
          }

          fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->tx_sample_length = valid_data_ptr->fbrx_ctl.num_tx_in_samples;
          fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->rx_sample_length = valid_data_ptr->fbrx_ctl.num_rx_in_samples;

          (void)rfcommon_fbrx_msm_store_data_internal(&(fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->tx_samples[0]),
                                                      &(fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx]->rx_samples[0]),
                                                      valid_data_ptr->fbrx_ctl.num_tx_in_samples, 
                                                      valid_data_ptr->fbrx_ctl.num_rx_in_samples,
                                                      handle );
          
          fbrx_sample_capture_init_data.max_num_valid_data_buf ++ ;

        }
        else
        {
          fbrx_sample_capture_init_data.generic_err_code = FTM_FBRX_ERR_MEM_ALLOC_FAILED;
          FTM_MSG_1( FTM_ERROR, "ftm_common_fbrx_capture_and_process_v2: Memory allocation failed for buffer index = %d ", sc_buf_idx );
        }
      }
      else
      {
        /* Memory already allocated for buffer index, deallocate memory */
        modem_mem_free((void*)fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx], MODEM_MEM_CLIENT_RFA);
        fbrx_sample_capture_init_data.buffer_ptr[sc_buf_idx] = NULL;
        fbrx_sample_capture_init_data.generic_err_code = FTM_FBRX_ERR_MEM_ALLOC_FAILED;
        FTM_MSG_1( FTM_ERROR, "ftm_common_fbrx_capture_and_process_v2: Memory previously allocated for buffer index = %d. De-allocating memory now ", sc_buf_idx );
      }
    }

    if (fbrx_data_ptr->results_log[results_buf_idx].results[0].gain_est > RFCOMMON_FBRX_GAIN_OVERFLOW_THRESHOLD ) 
    {
       FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: gain_est = %d > THRESHOLD =%d", 
                  fbrx_data_ptr->results_log[results_buf_idx].results[0].gain_est, RFCOMMON_FBRX_GAIN_OVERFLOW_THRESHOLD );
       output.status = RFCOMMON_FBRX_GAIN_OVERFLOW;
       return output;
    }
    else if (fbrx_data_ptr->results_log[results_buf_idx].results[0].gain_est < RFCOMMON_FBRX_GAIN_UNDERFLOW_THRESHOLD ) 
    {
       FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: gain_est = %d < THRESHOLD =%d", 
                  fbrx_data_ptr->results_log[results_buf_idx].results[0].gain_est, RFCOMMON_FBRX_GAIN_UNDERFLOW_THRESHOLD );
       output.status = RFCOMMON_FBRX_GAIN_UNDERFLOW;
       return output;
    }
  }
  else
  {
    FTM_MSG_2( FTM_ERROR, "ftm_common_fbrx_capture_and_process_v2: Invalid handle = %d or Tx Device = %d", handle, input.tx_device_id );
  }

   FTM_MSG( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: Disable FBRx");
   rfcommon_fbrx_mc_disable_rf_dev(dev_ptr);

   if (ftm_rf_mode == FALSE)
   {     
   #ifdef FEATURE_RF_HAS_WTR3925
   #if defined(FEATURE_LTE)
   if ((input.tech == RFM_LTE_MODE) || (input.tech == RFM_LTE_FTM_MODE))
   {
     if (temp_script_data_ptr != NULL)
     {
       temp_script_data_ptr->write_to_hw_flag = TRUE;
     }
     drx_device = rflte_ftm_mc_sm_get_carr_drx_device(RFLTE_MC_PCELL_IDX);
     if (rfc_lte_is_fb_device_conflict(drx_device,(rfm_device_enum_type)input.tx_device_id,rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX)))
     {
       rfdevice_lte_rx_enable(drx_device,
                              rflte_ftm_mc_sm_get_tx_band(),
                              temp_script_data_ptr,
                              NULL);
     }
     else if (rflte_state_get_intra_band_ul_ca_flag(sub_id))
     {
       drx_device = rflte_ftm_mc_sm_get_carr_drx_device(RFLTE_MC_SCELL1_IDX);
        
       rfdevice_lte_rx_enable(drx_device,
                              rflte_ftm_mc_sm_get_tx_band(),
                              temp_script_data_ptr,
                              NULL);
        
       FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_capture_and_process_v2: ULCA: Non-conflict Enabling Drx dev : %d on band : %d ",
                                 drx_device, rflte_ftm_mc_sm_get_tx_band());
     }
   }
   #endif
   #endif
   }
  #endif /* FEATURE_RF_FBRX */

  return output;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  ftm_common_fbrx_set_debug_control_condition
 
  @details
  This function allows user to change certain fbrx controls and add crash conditions for FBRx debugging purposes
 
*/
ftm_rsp_pkt_type ftm_common_fbrx_set_debug_control_condition(ftm_common_pkt_type *cmd_ptr)
{
  uint32 handle = 0;
  ftm_fbrx_handle_response_type* response_ptr = NULL;
  ftm_fbrx_debug_ctrl_param_type* param_ptr = NULL;

  #ifdef FEATURE_RF_FBRX
  rflm_fbrx_dm_template_t *fbrx_data_ptr = NULL;
  #endif

  ftm_rsp_pkt_type rsp = ftmdiag_create_new_pkt(sizeof(ftm_fbrx_handle_response_type));

  if(rsp.pkt_payload == NULL)
  {
    FTM_MSG_3( FTM_ERROR, "ftm_common_fbrx_set_debug_control_condition: Cannot allocate memory for response packet payload, cmd id = %u", cmd_ptr->ftm_hdr.cmd_id, 0, 0);
    return rsp;
  }

  response_ptr = (ftm_fbrx_handle_response_type*) rsp.pkt_payload;

  // Copy data from request to response
  memscpy(response_ptr, sizeof(ftm_composite_cmd_header_type), 
          cmd_ptr, sizeof(ftm_composite_cmd_header_type));

  param_ptr = (ftm_fbrx_debug_ctrl_param_type*)&(cmd_ptr->ftm_extended_params);

  response_ptr->status = FALSE;

  if (param_ptr != NULL)
  {
    if( param_ptr->fbrx_debug_cmd == FTM_FBRX_SET_TX_RX_DELAY )
    {
      rfcommon_fbrx_update_tx_rx_delay_override(param_ptr->fbrx_debug_val_1, param_ptr->fbrx_debug_val_2);
      FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: Override Tx Rx delay control:"
                          "override= %d, tx_rx_value=%d", 
                           param_ptr->fbrx_debug_val_1, param_ptr->fbrx_debug_val_2 );
      response_ptr->handle_id = param_ptr->fbrx_debug_val_2;
      response_ptr->status = TRUE;
    }
    else if( param_ptr->fbrx_debug_cmd == FTM_FBRX_SET_PROC_TYPE )
    {
      rfcommon_fbrx_update_proc_type_override(param_ptr->fbrx_debug_val_1, param_ptr->fbrx_debug_val_2);
      FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: Override FBRx proc type:"
                           "override= %d, fbrx_proc_type=%d", 
                            param_ptr->fbrx_debug_val_1, param_ptr->fbrx_debug_val_2 );
      response_ptr->handle_id = param_ptr->fbrx_debug_val_2;
      response_ptr->status = TRUE;
    }
    else
    {
      #ifdef FEATURE_RF_FBRX
      handle = rfcommon_fbrx_mc_get_tech_tx_lm_handle(param_ptr->rfm_tech, param_ptr->carrier_idx);

      fbrx_data_ptr = (rflm_fbrx_dm_template_t*)rflm_dm_get_fbrx_buf_ptr(handle);
      if (fbrx_data_ptr != NULL)
      {
        /* reset any previous fbrx debug thresholds */
        rflm_fbrx_init_thresh_control_data(fbrx_data_ptr);

        switch ( param_ptr->fbrx_debug_cmd )
        {
          case FTM_FBRX_SET_BSC_THRESH_CONDITION:
           /* Set bsc threshold override values*/
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.crash_en = param_ptr->crash_en;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.counter_val = param_ptr->crash_counter;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.low_txagc_thresh = param_ptr->low_txagc_thresh;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.high_txagc_thresh = param_ptr->high_txagc_thresh;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.meas_type = param_ptr->meas_type;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.run_type = param_ptr->run_type;

           fbrx_data_ptr->fbrx_debug_ctrl.bsc_data.crash_en = param_ptr->crash_en;
           fbrx_data_ptr->fbrx_debug_ctrl.bsc_data.lse_thresh = param_ptr->fbrx_debug_val_1;
           fbrx_data_ptr->fbrx_debug_ctrl.bsc_data.gain_err_thresh = param_ptr->fbrx_debug_val_2;
           fbrx_data_ptr->fbrx_debug_ctrl.bsc_data.invert_bsc = param_ptr->fbrx_debug_val_3;
           
           FTM_MSG_3( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: Set BSC threshold condition:"
                                "rfm_tech= %d, carrier_idx=%d, tx_handle=%d", 
                                 param_ptr->rfm_tech, param_ptr->carrier_idx, handle );
           FTM_MSG_6( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: Set BSC threshold condition:"
                                "crash_en=%d, counter=%d, low_txagc=%d, high_txagc=%d, meas_type=%d, run_idx_type=%d",
                                 param_ptr->crash_en, param_ptr->crash_counter, param_ptr->low_txagc_thresh, 
                                 param_ptr->high_txagc_thresh, param_ptr->meas_type, param_ptr->run_type);
           FTM_MSG_3( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: Set BSC threshold condition:"
                                "lse_thresh= %d, gain_err_thresh=%d, invert=%d", 
                                 param_ptr->fbrx_debug_val_1, param_ptr->fbrx_debug_val_2, param_ptr->fbrx_debug_val_3);

           response_ptr->status = TRUE;

           break;

          case FTM_FBRX_SET_GAIN_EST_THRESH_CONDITION:
           /* Set fbrx gain estimate threshold override values*/

           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.crash_en = param_ptr->crash_en;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.counter_val = param_ptr->crash_counter;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.low_txagc_thresh = param_ptr->low_txagc_thresh;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.high_txagc_thresh = param_ptr->high_txagc_thresh;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.meas_type = param_ptr->meas_type;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.run_type = param_ptr->run_type;

           fbrx_data_ptr->fbrx_debug_ctrl.gain_est_data.crash_en = param_ptr->crash_en;
           fbrx_data_ptr->fbrx_debug_ctrl.gain_est_data.low_gain_est_thresh = param_ptr->fbrx_debug_val_1;
           fbrx_data_ptr->fbrx_debug_ctrl.gain_est_data.high_gain_est_thresh = param_ptr->fbrx_debug_val_2;

           FTM_MSG_3( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: Set Gain Est threshold condition:"
                                "rfm_tech= %d, carrier_idx=%d, tx_handle=%d", 
                                 param_ptr->rfm_tech, param_ptr->carrier_idx, handle );
           FTM_MSG_6( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: Set Gain Est threshold condition:"
                                "crash_en=%d, counter=%d, low_txagc=%d, high_txagc=%d, meas_type=%d, run_idx_type=%d",
                                 param_ptr->crash_en, param_ptr->crash_counter, param_ptr->low_txagc_thresh, 
                                 param_ptr->high_txagc_thresh, param_ptr->meas_type, param_ptr->run_type);
           FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: Set Gain Est threshold condition:"
                                "gain_est_low_t= %d, gain_est_high_t=%d", 
                                 param_ptr->fbrx_debug_val_1, param_ptr->fbrx_debug_val_2 );

           response_ptr->status = TRUE;

           break;

          case FTM_FBRX_SET_GAIN_PA_STATE_THRESH_CONDITION:
           /* Set fbrx gain state threshold override values*/
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.crash_en = param_ptr->crash_en;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.counter_val = param_ptr->crash_counter;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.low_txagc_thresh = param_ptr->low_txagc_thresh;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.high_txagc_thresh = param_ptr->high_txagc_thresh;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.meas_type = param_ptr->meas_type;
           fbrx_data_ptr->fbrx_debug_ctrl.fbrx_debug_common_data.run_type = param_ptr->run_type;

           fbrx_data_ptr->fbrx_debug_ctrl.gain_pa_state_data.crash_en = param_ptr->crash_en;
           fbrx_data_ptr->fbrx_debug_ctrl.gain_pa_state_data.gain_pa_state_t = param_ptr->fbrx_debug_val_1;
           fbrx_data_ptr->fbrx_debug_ctrl.gain_pa_state_data.gain_pa_meas_ctrl_t = param_ptr->fbrx_debug_val_2;
           fbrx_data_ptr->fbrx_debug_ctrl.gain_pa_state_data.gain_pa_state_val = param_ptr->fbrx_debug_val_3;

           FTM_MSG_3( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: Set Gain state threshold condition:"
                                "rfm_tech= %d, carrier_idx=%d, tx_handle=%d", 
                                 param_ptr->rfm_tech, param_ptr->carrier_idx, handle );
           FTM_MSG_6( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: Set Gain state threshold condition:"
                                "crash_en=%d, counter=%d, low_txagc=%d, high_txagc=%d, meas_type=%d, run_idx_type=%d",
                                 param_ptr->crash_en, param_ptr->crash_counter, param_ptr->low_txagc_thresh, 
                                 param_ptr->high_txagc_thresh, param_ptr->meas_type, param_ptr->run_type);
           FTM_MSG_3( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: Set state threshold condition:"
                                "gain_pa_state_type= %d, gain_pa_ctrl_type=%d, gain_pa_state_val=%d", 
                                 param_ptr->fbrx_debug_val_1, param_ptr->fbrx_debug_val_2, param_ptr->fbrx_debug_val_3);

           response_ptr->status = TRUE;
           break;
           
           default:
              response_ptr->status = FALSE;
           break;
        }

        response_ptr->handle_id = handle;
        FTM_MSG_2( FTM_HIGH, "ftm_common_fbrx_set_debug_control_condition: result %d, handle %d", 
                              response_ptr->status, handle);
      }
      else
      {
        FTM_MSG_1( FTM_ERROR, "ftm_common_fbrx_set_debug_control_condition: Invalid Tx Handle= %d", handle);
      }

      #endif /* FEATURE_RF_FBRX */
    }
  }

  return rsp;
}


#endif

/*----------------------------------------------------------------------------*/
/*! @} */


/*! @} */


