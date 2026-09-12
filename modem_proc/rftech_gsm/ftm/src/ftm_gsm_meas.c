/*!
  @file ftm_gsm_meas.c

  @brief
  This file contains all declarations and definitions necessary to use
  the gsm FTM Measurement Main Control.

  @details

*/

/*===========================================================================

  Copyright (c) 2013 - 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/ftm/src/ftm_gsm_meas.c#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/14/15   cs      Save the source tech device for G2X measurement tuneback build
06/24/15   cs/sw   Need to make the rf_info used for meas_enter a local static as this
                   is now used in building x2g params in target G tech dataspace for exit
05/15/15   tsr     Remove GL1 deprecated api gl1_hw_rf_map_device2_as_id() and replace with new FTM api
05/08/15   sw      Use new IRAT DL path enums
04/08/15   zhw     Put rfcom.h outside feature flag so that flag definition can be imported.
04/08/15   sw      Merge GSM meas changes to mainline
04/07/15   nv      Merging the changes from dev branch on to Mailine
04/02/15   sw      FTM changes to work with new L1 callflow
03/31/15   nv      First version of CRAT FTM GUI
03/24/15   sw      Use new irat interface definitions
03/16/15   nv      Removed SRC buf idx and TGT buf idx as they are not used in FED
                   based architecture and also updated variable name for CAR1 enable.
02/13/15   sw      Fix for FTM IRAT G2X tune-back crash
11/06/14   vs      Fix linker errors on W/T compile out flavors
10/23/14   sc      Enable event type control from RFSW
10/21/14   sw      Update irat source tech information with valid data (from QRCT)
06/11/14   hk      Fixing OFT compiler errors
03/21/14   cj      Changes for W2G on Bolt
03/12/14   cj      Removing FW dependency
02/10/14   cj      g2x timing updated
02/03/14   cj      num_source_item added for DSDS
08/05/13   cj      Implementation added to receive GSM RSSI in meas mode
04/19/13   cj      Added header change support for CA changes
04/11/13   tws     Remove FEATURE_DUAL_ACTIVE featurization
03/12/13   cj      Removing app mode config API for OFT
03/12/13   cj      Removed call to GL1 functions for OFT till the availability
                     of overload APIs
03/10/13   cj      Added rxlm buffer execution and debug messages for G2X
03/08/13   cj      Added featurization till GL1 provides overload APIs
03/07/13   cj      Updated ftm_gsm_meas_fw_execute_rxlm
03/07/13   cj      Fixed Jenkins errors
03/07/13   cj      Initial Check-in
===========================================================================*/

#include "rfa_variation.h"
#include "target.h"
#include "comdef.h"
#include "customer.h"

#include "sys_type.h"

#include "msg.h"
#include "task.h"
#include "rfcom.h"

#include "rfmeas_types.h"
#include "rfmeas_mc.h"
#include "rfmeas_gsm.h"
#include "rfmeas_ftm_mdsp.h"

#include "ftm.h"
#include "ftm_msg.h"
#include "ftm_meas.h"
#include "ftm_common_control.h"
#include "ftmicap.h"
#include "ftm_common.h"
#include "ftm_rf_cmd.h"

#include "ftm_gsm_dispatch.h"
#include "ftm_gsm_ctl.h"
#include "ftm_gsm_device.h"
#include "ftm_gsm_meas.h"
#include "ftm_gsm_rfprot.h"

#include "rfgsm_mc.h"

#include "rfm.h"
#include "rfm_gsm.h"

#include "rflm_api_gsm.h"
#include "rflm_time.h"

#ifdef FEATURE_MODEM_RCINIT
#include "rcinit.h"
#endif /* FEATURE_MODEM_RCINIT */

#include "fws.h"

#include "rxlm_intf.h"

#include "geran_test_mode_api.h"

#include "gl1_defs.h"

#ifdef FEATURE_GSM_GPRS
#include "gtmrs.h"
#include "mdsp_intf.h"
#endif

#include "gfw_sw_intf.h"

#undef DEBUG_MDSP_SNOOPING_FIX

/* Ideally should use l1_os.h definitions here */
#define L1_GSM_WAIT_HW_SIG           0x00000004
#define L1_GSM_TIMEOUT_HW_SIG        0x00000008  /* mDSP init, TRM req */

#define FTM_MAX_CM_RSSI_MEASUREMENTS 1

#define FTM_MEAS_GSM_OFFSET          1250

/* How many frameticks to wait for FW to finish */
#define FTM_MEAS_GSM_WAIT_CNT        3

extern ftm_gsm_meas_results g_meas_results;

/* Parameter Check. At least one tech should be gsm */
#define FTM_GSM_MEAS_PAR_CHCK( pARAM_p )                               \
  if ( !pARAM_p )                                                      \
  {                                                                    \
    FTM_MSG( FTM_ERROR,                                                \
             FTM_GSM_MEAS_FUNC" NULL "#pARAM_p );                      \
                                                                       \
    return;                                                            \
  }                                                                    \
                                                                       \
  if ( ( pARAM_p->src_tech != RFCOM_GSM_MODE ) &&                      \
       ( pARAM_p->tgt_tech != RFCOM_GSM_MODE )                         \
     )                                                                 \
  {                                                                    \
    FTM_MSG_2( FTM_ERROR,                                              \
               FTM_GSM_MEAS_FUNC" not GSM src_tech:%d or tgt_tech:%d", \
               pARAM_p->src_tech, pARAM_p->tgt_tech );                 \
                                                                       \
    return;                                                            \
  }

#define FTM_GSM_MEAS_PAR_CHCK_RTN( pARAM_p, rETURN_fAIL_vAL )          \
  if ( !pARAM_p )                                                      \
  {                                                                    \
    FTM_MSG( FTM_ERROR,                                                \
             FTM_GSM_MEAS_FUNC" NULL "#pARAM_p );                      \
                                                                       \
    return ( rETURN_fAIL_vAL );                                        \
  }                                                                    \
                                                                       \
  if ( ( pARAM_p->src_tech != RFCOM_GSM_MODE ) &&                      \
       ( pARAM_p->tgt_tech != RFCOM_GSM_MODE )                         \
     )                                                                 \
  {                                                                    \
    FTM_MSG_2( FTM_ERROR,                                              \
               FTM_GSM_MEAS_FUNC" not GSM src_tech:%d or tgt_tech:%d", \
               pARAM_p->src_tech, pARAM_p->tgt_tech );                 \
                                                                       \
    return ( rETURN_fAIL_vAL );                                        \
  }

/*============================================================================
             LOCAL Function Prototypes
=============================================================================*/

boolean ftm_gsm_meas_get_src_rxlm_buffer(
          rfm_meas_generic_params_type *test_param_p );

boolean ftm_gsm_meas_enter_meas(
          rfm_meas_enter_param_type *meas_enter_param_p );

boolean ftm_gsm_meas_build_script(
          rfm_meas_setup_param_type *meas_setup_param_p );
boolean ftm_gsm_meas_fw_execute_rxlm(
          rfm_meas_generic_params_type *test_param_p,
          boolean                       execute_src_rxlm );

boolean ftm_gsm_meas_enter_gsm(
          rfm_meas_generic_params_type *test_param_p,
          boolean                       use_src_param );
boolean ftm_gsm_meas_exit_gsm(
          rfm_meas_generic_params_type *test_param_p,
          boolean                       use_src_param );

boolean ftm_gsm_meas_exit_meas(
          rfm_meas_exit_param_type *meas_exit_param_p );

void ftm_gsm_allocate_rxlm( void );
void ftm_gsm_deallocate_rxlm( void );

static
void ftm_gsm_meas_power_meas_cmd(
       rfm_meas_generic_params_type *test_param_p,
       sys_modem_as_id_e_type        as_id );

static
void ftm_meas_gsm_wait_for_fw(
       sys_modem_as_id_e_type as_id );

static
void ftm_gsm_meas_set_rf_device(
       rfm_device_enum_type tgt_device );
static
rfm_device_enum_type ftm_gsm_meas_get_rf_device( void );

static
sys_band_T ftm_meas_gsm_map_rfband_to_sys_band(
             uint16 band );

/*============================================================================
             LOCAL/EXTERNAL VARIABLES
=============================================================================*/

static rfm_meas_params_channel_type ftm_gsm_source_channel_param;
static rfm_meas_params_channel_type ftm_gsm_target_channel_param;

static ftm_meas_func_tbl_type ftm_gsm_meas_apis = \
{
  ftm_gsm_meas_get_src_rxlm_buffer,
  NULL,
  NULL,
  NULL,
  NULL,
  ftm_gsm_meas_enter_meas,
  ftm_gsm_meas_build_script,
  ftm_gsm_meas_exit_gsm,
  ftm_gsm_meas_enter_gsm,
  ftm_gsm_meas_exit_meas,
  NULL,
  NULL
};

static ftm_lm_buf_type            ftm_gsm_rxlm_buffer;

/* These must be global here as this data is passed into x2g data used for meas_exit */
static gl1_hw_cm_rf_info_t        ftm_gsm_cm_rf_info           = { 0 };
static rfm_meas_common_param_type ftm_gsm_irat_src_meas_params = { 0 };

static rfm_device_enum_type       ftm_gsm_meas_tgt_irat_device = RFM_INVALID_DEVICE;

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_param_init function will basically allow RFSW
    to initialize common variables.

  @param
    rfm_meas_generic_params_type: ptr to data structure for all
    input info to be setup initially.

  @retval
    Returns boolean to indicate success/fail.
*/
boolean ftm_gsm_param_init(
          rfm_meas_generic_params_type *test_param_p )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_param_init()"

  /* Parameter Check. At least one tech should be gsm */
  FTM_GSM_MEAS_PAR_CHCK_RTN( test_param_p, FALSE );

  ftm_gsm_source_channel_param.serving_band =
    (rf_card_band_type)test_param_p->src_ca_path0_band;
  ftm_gsm_source_channel_param.serving_chan  =
    test_param_p->src_ca_path0_chan_car0;

  ftm_gsm_source_channel_param.neighbor_band =
    (rf_card_band_type)test_param_p->tgt_band;
  ftm_gsm_source_channel_param.neighbor_chan =
    test_param_p->tgt_chan;

  ftm_gsm_rxlm_buffer.is_buf_allocated = FALSE;

  ftm_gsm_meas_set_rf_device( RFM_MAX_WAN_DEVICES );

  memset( &ftm_gsm_cm_rf_info, 0, sizeof( ftm_gsm_cm_rf_info ) );
  memset( &ftm_gsm_irat_src_meas_params, 0, sizeof( ftm_gsm_irat_src_meas_params ) );

  return ( TRUE );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_register function will enable iRAT measurement API to set
    any required callbacks.
*/
void ftm_gsm_meas_register( void )
{
  ftm_meas_register( RFCOM_GSM_MODE, &ftm_gsm_meas_apis );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_deregister function will enable iRAT measurement API to clear
    any registered callbacks.
*/
void ftm_gsm_meas_deregister( void )
{
  ftm_meas_deregister( RFCOM_GSM_MODE );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_allocate_rxlm function will allocate RXLM buffer.

  @pre
    ftm_gsm_param_init() must have already been called.
    We should be in a certain technology.
*/
void ftm_gsm_allocate_rxlm( void )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_allocate_rxlm()"

  if ( !ftm_gsm_rxlm_buffer.is_buf_allocated )
  {
    /* Chain param is now irrelevant */
    lm_status_type status = rxlm_allocate_buffer(
                              0, LM_GSM,
                              &ftm_gsm_rxlm_buffer.buf_idx );

    if ( status != LM_SUCCESS )
    {
      FTM_MSG_2( FTM_ERROR,
                 FTM_GSM_MEAS_FUNC"->rxlm_allocate_buffer failed"
                 " status:%d buf_idx:%d",
                 status, ftm_gsm_rxlm_buffer.buf_idx );
    }
    else
    {
      FTM_MSG_1( FTM_HIGH,
                 FTM_GSM_MEAS_FUNC"->rxlm_allocate_buffer"
                 " buf_idx:%d",
                 ftm_gsm_rxlm_buffer.buf_idx );

      ftm_gsm_rxlm_buffer.is_buf_allocated = TRUE;
    }
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_get_src_rxlm_buffer function will basically allow RFSW
    to return source tech rxlm buffer if valid.

  @param
    rfm_meas_generic_params_type: ptr to data structure for all
    input info to identify source tech.

  @retval
    Returns boolean to indicate success/fail.
*/
boolean ftm_gsm_meas_get_src_rxlm_buffer(
          rfm_meas_generic_params_type *test_param_p )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_meas_get_src_rxlm_buffer()"

  if ( !test_param_p  )
  {
    FTM_MSG( FTM_ERROR,
             FTM_GSM_MEAS_FUNC" NULL test_param_p" );

    return ( FALSE );
  }

  if ( test_param_p->src_tech != RFCOM_GSM_MODE )
  {
    FTM_MSG_1( FTM_ERROR,
               FTM_GSM_MEAS_FUNC" non-GSM src_tech:%d",
               test_param_p->src_tech );

    return ( FALSE );
  }

  ftm_gsm_allocate_rxlm();

  if ( ftm_gsm_rxlm_buffer.is_buf_allocated )
  {
    test_param_p->src_ca_path0_prx_rxlm_buf_idx =
      ftm_gsm_rxlm_buffer.buf_idx;
  }

  return ( TRUE );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_deallocate_rxlm function will allow the allocated source rxlm
    buffer to be released.
*/
void ftm_gsm_deallocate_rxlm( void )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_deallocate_rxlm()"

  if ( ftm_gsm_rxlm_buffer.is_buf_allocated )
  {
    lm_status_type status =
                     rxlm_deallocate_buffer( ftm_gsm_rxlm_buffer.buf_idx );

    if ( status != LM_SUCCESS )
    {
      FTM_MSG_2( FTM_ERROR,
                 FTM_GSM_MEAS_FUNC"->rxlm_deallocate_buffer failed"
                 " status:%d buf_idx:%d",
                 status, ftm_gsm_rxlm_buffer.buf_idx );
    }
    else
    {
      FTM_MSG_1( FTM_HIGH,
                 FTM_GSM_MEAS_FUNC"->rxlm_deallocate_buffer"
                 " buf_idx:%d",
                 ftm_gsm_rxlm_buffer.buf_idx );

      ftm_gsm_rxlm_buffer.is_buf_allocated = FALSE;
    }
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_enter_meas function will basically allow RFSW to configure
    GSM as the source tech when performing iRAT measurements.

  @param
    rfm_meas_enter_param_type: ptr to data structure for all
    input info to identify source tech.

  @pre
    ftm_gsm_param_init() must have already been called.
    We should be in a certain technology.

  @retval
    Returns a boolean for success/fail.
*/
boolean ftm_gsm_meas_enter_meas(
          rfm_meas_enter_param_type *meas_enter_param_p )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_meas_enter_meas()"

  boolean result = TRUE;

  /* Check for NULL pointers */
  if ( !meas_enter_param_p )
  {
    FTM_MSG( FTM_ERROR,
             FTM_GSM_MEAS_FUNC" NULL meas_enter_param_p" );

    result = FALSE;
  }
  else if ( !meas_enter_param_p->enter_params )
  {
    FTM_MSG( FTM_ERROR,
             FTM_GSM_MEAS_FUNC" NULL enter_params" );

    result = FALSE;
  }
  else if ( meas_enter_param_p->enter_params->header.source_tech != RFCOM_GSM_MODE )
  {
    /*
     * Save away in local store for configuring L1 for X2G
     * tuneback to X != G
     */
    ftm_gsm_irat_src_meas_params = *meas_enter_param_p->enter_params;
  }

  if ( result &&
       ( RFCOM_GSM_MODE == meas_enter_param_p->enter_params->header.source_tech )
     )
  {
    rfm_meas_result_type status = rfmeas_mc_enter( meas_enter_param_p, NULL );

    if ( status != RFM_MEAS_COMMON_SUCCESS )
    {
      FTM_MSG_1( FTM_ERROR,
                 FTM_GSM_MEAS_FUNC"->rfmeas_mc_enter returned failure:%d",
                 status );

      result = FALSE;
    }
    else
    {
      FTM_MSG( FTM_HIGH,
               FTM_GSM_MEAS_FUNC"->rfmeas_mc_enter SUCCESS" );
    }
  }

  return ( result );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_build_script function will basically allow RFSW to configure
    GSM RF build scripts as the source tech when performing iRAT measurements.

  @param
    rfm_meas_setup_param_type: ptr to data structure for all
    input info to identify source tech.

  @pre
    ftm_gsm_param_init() must have already been called.
    We should be in a certain technology.

  @retval
    Returns a boolean for success/fail.
*/
boolean ftm_gsm_meas_build_script(
          rfm_meas_setup_param_type *meas_setup_param_p )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_meas_build_script()"

  boolean result = TRUE;

  /* Check for NULL pointers */
  if ( !meas_setup_param_p )
  {
    FTM_MSG( FTM_ERROR,
             FTM_GSM_MEAS_FUNC" NULL meas_setup_param_p" );

    result = FALSE;
  }
  else if ( !meas_setup_param_p->setup_params )
  {
    FTM_MSG( FTM_ERROR,
             FTM_GSM_MEAS_FUNC" NULL setup_params" );

    result = FALSE;
  }

  /* G2X build tuneback scripts */
  if ( result &&
       ( RFCOM_GSM_MODE == meas_setup_param_p->setup_params->header.source_tech )
     )
  {
    rfm_meas_result_type status = rfmeas_build_scripts( meas_setup_param_p, NULL );

    if ( status != RFM_MEAS_COMMON_SUCCESS )
    {
      FTM_MSG_1( FTM_ERROR,
                 FTM_GSM_MEAS_FUNC"->rfmeas_build_scripts returned failure:%d",
                 status );

      result = FALSE;
    }
    else
    {
      FTM_MSG( FTM_HIGH,
               FTM_GSM_MEAS_FUNC"->rfmeas_build_scripts SUCCESS" );
    }
  }

  return ( result );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_fw_execute_rxlm function will basically allow RFSW to pass to
    L1 he clean-up scripts.

  @param
    rfm_meas_generic_params_type: ptr to data structure for all
    input info to identify source tech.
  @param
    boolean: Use src rxlm parameters, not used currently.

  @pre
    ftm_gsm_param_init() must have already been called.
    We should be in a certain technology.

  @retval
    Returns a boolean denoting whether the function did work
    correctly or not.
*/
boolean ftm_gsm_meas_fw_execute_rxlm(
          rfm_meas_generic_params_type *test_param_p,
          boolean                       execute_src_rxlm )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_meas_fw_execute_rxlm()"

  boolean                       return_status = TRUE;

#ifndef TEST_FRAMEWORK
  rflm_handle_rx_t              rxlm_buf_idx;
  rflm_gsm_irat_ccs_input_type  script_vars_in;
  rflm_gsm_irat_ccs_output_type script_vars_out;
  rflm_err_t                    cleanup_status;

  if ( !test_param_p )
  {
    FTM_MSG( FTM_ERROR,
             FTM_GSM_MEAS_FUNC" NULL test_param_p" );

    return ( !return_status );
  }

  rxlm_buf_idx = test_param_p->src_ca_path0_prx_rxlm_buf_idx;

  FTM_MSG_2( FTM_HIGH,
             FTM_GSM_MEAS_FUNC"->rflm_gsm_prog_irat_trigger_ccs_script"
             " with src_chan:%d rxlm_buf_idx:%d",
             test_param_p->src_ca_path0_chan_car0, rxlm_buf_idx );

  /* Trigger FW to execute Script */
  script_vars_in.trigger_time = ( rflm_time_read_ustmr_count() + 2000 );

  cleanup_status = rflm_gsm_prog_irat_trigger_ccs_script(
                     rxlm_buf_idx, &script_vars_in, &script_vars_out );

  FTM_MSG_2( FTM_HIGH,
             "Exiting "FTM_GSM_MEAS_FUNC" rxlm_buf_idx:%d"
             " cleanup_status:%d",
             rxlm_buf_idx, cleanup_status );

  return_status = ( RFLM_ERR_NONE == cleanup_status );
#endif /* !TEST_FRAMEWORK */

  return ( return_status );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_exit_meas function will basically allow RFSW to pass to
    L1 the exit measurement command params.

  @param
    rfm_meas_exit_param_type: data structure for all info to be returned

  @pre
    ftm_gsm_param_init() must have already been called along with meas_enter_meas.
    We should be in a certain technology.

  @retval
    Returns a boolean denoting whether the function did work
    correctly or not.
*/
boolean ftm_gsm_meas_exit_meas(
          rfm_meas_exit_param_type *meas_exit_param_p )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_meas_exit_meas()"

  boolean result = TRUE;

  /* Check for NULL pointers */
  if ( !meas_exit_param_p )
  {
    FTM_MSG( FTM_ERROR,
             FTM_GSM_MEAS_FUNC" NULL meas_exit_param_p" );

    result = FALSE;
  }
  else if ( !meas_exit_param_p->exit_params )
  {
    FTM_MSG( FTM_ERROR,
             FTM_GSM_MEAS_FUNC" NULL exit_params" );

    result = FALSE;
  }

  if ( result &&
       ( RFCOM_GSM_MODE == meas_exit_param_p->exit_params->header.source_tech )
     )
  {
    rfm_meas_result_type status = rfmeas_mc_exit( meas_exit_param_p, NULL );

    if ( status != RFM_MEAS_COMMON_SUCCESS )
    {
      FTM_MSG_1( FTM_ERROR,
                 FTM_GSM_MEAS_FUNC"->rfmeas_mc_exit returned failure:%d",
                 status );

      result = FALSE;
    }
    else
    {
      FTM_MSG( FTM_HIGH,
               FTM_GSM_MEAS_FUNC"->rfmeas_mc_exit SUCCESS" );
    }
  }

  return ( result );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_exit_gsm function will basically allow RFSW to pass to
    L1 the exit measurement command params.

  @param
    rfm_meas_generic_params_type: data structure for all info to be returned
  @param
    boolean: Use src rxlm parameters, not used currently.

  @pre
    ftm_gsm_param_init() must have already been called along with meas_enter_gsm.
    We should be in a certain technology.

  @retval
    Returns a boolean denoting whether the function did work
    correctly or not.
*/
boolean ftm_gsm_meas_exit_gsm(
          rfm_meas_generic_params_type *test_param_p,
          boolean                       use_src_param )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_meas_exit_gsm()"

  boolean                 result = TRUE;

#ifndef TEST_FRAMEWORK
  ftm_gsm_chain_vars_s   *chain_vars_p;
  sys_modem_as_id_e_type  as_id;
  rfm_device_enum_type    remapped_rx_device, rxd_device, tx_device;

  uint16                  offset  = FTM_MEAS_GSM_OFFSET;

  rfm_device_enum_type    rfm_dev = ftm_gsm_meas_get_rf_device();

  /* Parameter Check. At least one tech should be gsm */
  FTM_GSM_MEAS_PAR_CHCK_RTN( test_param_p, !result );

  /* Sanity check that tuneback device is set correctly */
  if ( rfm_dev >= RFM_MAX_WAN_DEVICES )
  {
    FTM_MSG_1( FTM_ERROR,
               FTM_GSM_MEAS_FUNC"->ftm_gsm_meas_get_rf_device Invalid dispatch WAN device:%d",
               rfm_dev );

    return ( !result );
  }

  /* Re map the device */
  remapped_rx_device = ftm_gsm_get_remapped_rx_device_info( rfm_dev );
  tx_device          = ftm_gsm_get_associated_tx_device( remapped_rx_device );
  rxd_device         = ftm_gsm_get_associated_rx_device( remapped_rx_device );

  ftm_gsm_create_and_init_vars( remapped_rx_device, remapped_rx_device );

  /* Allocate DRx device FTM vars */
  ftm_gsm_create_and_init_vars( rxd_device, remapped_rx_device );

  /* Allocate TX device FTM vars */
  if ( tx_device >= RFM_MAX_WAN_DEVICES )
  {
    ftm_gsm_create_and_init_vars( tx_device, remapped_rx_device );
  }

  /*Update the GSM chain variables by retrieving RX/TX/DRX devices from RFC*/
  chain_vars_p =
    ftm_gsm_device_get_and_update_chain_vars( remapped_rx_device );

  if ( !chain_vars_p )
  {
    FTM_MSG_2( FTM_ERROR,
               FTM_GSM_MEAS_FUNC" chain_vars_p NULL for dispatch device:%d"
               " and remapped_rx_device:%d",
               rfm_dev, remapped_rx_device );

    return ( !result );
  }

  /* Obtain geran as id from ftm sub id */
  as_id = ftm_gsm_ctl_map_sub_id_to_as_id( chain_vars_p->sub_id );

  FTM_MSG_3( FTM_HIGH,
             FTM_GSM_MEAS_FUNC" start FN:%d for device:%d and remapped_rx_device:%d",
             geran_test_mode_api_get_FN( as_id ), rfm_dev, remapped_rx_device );

  if ( RFCOM_GSM_MODE == test_param_p->src_tech )
  {
    ftm_rx_burst_type ftm_rx_burst;

    /* Step : 1. Stop AGC */
    ftm_rx_burst.ftm_rx_funct =
      (ftm_enum_rx_function_type)FTM_RX_FUNCTION_RF_RX_HW_OFF;

    ftm_gsm_rx_burst_command( rfm_dev, &ftm_rx_burst, FTM_DIV_FACTOR );
  }

  ftm_meas_gsm_wait_for_fw( as_id );

  FTM_MSG_1( FTM_HIGH,
             FTM_GSM_MEAS_FUNC"->geran_test_mode_api_cm_cleanup FN:%d",
             geran_test_mode_api_get_FN( as_id ) );

  geran_test_mode_api_cm_cleanup( offset, as_id );

  ftm_meas_gsm_wait_for_fw( as_id );

  FTM_MSG_1( FTM_HIGH,
             FTM_GSM_MEAS_FUNC"->geran_test_mode_api_cm_gsm_terminate FN:%d",
             geran_test_mode_api_get_FN( as_id ) );

  geran_test_mode_api_cm_gsm_terminate( as_id );

  FTM_MSG_1( FTM_HIGH,
             FTM_GSM_MEAS_FUNC" end FN:%d",
             geran_test_mode_api_get_FN( as_id ) );
#endif /* !TEST_FRAMEWORK */

  return ( result );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_enter_gsm function will basically allow RFSW to pass to
    L1 the exit measurement command params.

  @param
    rfm_meas_generic_params_type: data structure for all info to be returned
  @param
    boolean: Use src rxlm parameters, not used currently.

  @pre
    ftm_gsm_param_init() must have already been called.
    We should be in a certain technology.

  @retval
    Returns a boolean denoting whether the function did work
    correctly or not.
*/
boolean ftm_gsm_meas_enter_gsm(
          rfm_meas_generic_params_type *test_param_p,
          boolean                       use_src_param )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_meas_enter_gsm()"

  boolean                 result = TRUE;

#ifndef TEST_FRAMEWORK
  sys_modem_as_id_e_type  as_id;
  rfm_device_enum_type    rfm_dev, remapped_rx_device, rxd_device, tx_device;
  rex_tcb_type           *task_p;
  ftm_gsm_chain_vars_s   *chain_vars_p;

  uint16                  offset = FTM_MEAS_GSM_OFFSET;

  /* Parameter Check. At least one tech should be gsm */
  FTM_GSM_MEAS_PAR_CHCK_RTN( test_param_p, !result );

  if ( test_param_p->src_tech != RFCOM_GSM_MODE )
  {
    /* X2G must be on Multi-mode SUB for non W+W */
#ifndef FEATURE_DUAL_WCDMA
    if ( test_param_p->tgt_device != RFM_DEVICE_0 )
    {
      FTM_MSG_1( FTM_ERROR,
                 FTM_GSM_MEAS_FUNC" Invalid X2G Multimode G device:%d",
                 test_param_p->tgt_device );
    }

    ftm_gsm_meas_set_rf_device( RFM_DEVICE_0 );
#else
    ftm_gsm_meas_set_rf_device( test_param_p->tgt_device );
#endif /* !FEATURE_DUAL_WCDMA */
  }
  else
  {
    /* G2X so store away source tech device for tuneback to G */
    ftm_gsm_meas_set_rf_device( test_param_p->src_ca_path0_device );
  }

  rfm_dev = ftm_gsm_meas_get_rf_device();

  /* Sanity check that tuneback device is set correctly */
  if ( rfm_dev >= RFM_MAX_WAN_DEVICES )
  {
    FTM_MSG_1( FTM_ERROR,
               FTM_GSM_MEAS_FUNC"->ftm_gsm_meas_get_rf_device Invalid dispatch WAN device:%d",
               rfm_dev );

    return ( !result );
  }

  /* Re map the device */
  remapped_rx_device = ftm_gsm_get_remapped_rx_device_info( rfm_dev );
  tx_device          = ftm_gsm_get_associated_tx_device( remapped_rx_device );
  rxd_device         = ftm_gsm_get_associated_rx_device( remapped_rx_device );

  ftm_gsm_create_and_init_vars( remapped_rx_device, remapped_rx_device );

  /* Allocate DRx device FTM vars */
  ftm_gsm_create_and_init_vars( rxd_device, remapped_rx_device );

  /* Allocate TX device FTM vars */
  if ( tx_device < RFM_MAX_WAN_DEVICES )
  {
    ftm_gsm_create_and_init_vars( tx_device, remapped_rx_device );
  }

  /* Update the GSM chain variables by retrieving RX/TX/DRX devices from RFC */
  chain_vars_p =
    ftm_gsm_device_get_and_update_chain_vars( remapped_rx_device );

  if ( !chain_vars_p )
  {
    FTM_MSG_2( FTM_ERROR,
               FTM_GSM_MEAS_FUNC" chain_vars_p NULL for dispatch device:%d"
               " and remapped rx device:%d",
               rfm_dev, remapped_rx_device );

    return ( !result );
  }

  /* Obtain geran as id from ftm sub id */
  as_id = ftm_gsm_ctl_map_sub_id_to_as_id( chain_vars_p->sub_id );

  FTM_MSG_3( FTM_HIGH,
             FTM_GSM_MEAS_FUNC" start FN:%d for device:%d and remapped_rx_device:%d",
             geran_test_mode_api_get_FN( as_id ), rfm_dev, remapped_rx_device );

  task_p = TASK_HANDLE( ftm );

  FTM_MSG_1( FTM_HIGH,
             FTM_GSM_MEAS_FUNC"->geran_test_mode_api_cm_gsm_init FN:%d",
             geran_test_mode_api_get_FN( as_id ) );

  geran_test_mode_api_cm_gsm_init( NULL, task_p, NULL, L1_GSM_WAIT_HW_SIG,
                                   L1_GSM_TIMEOUT_HW_SIG, as_id );

  if ( test_param_p->src_tech != RFCOM_GSM_MODE )
  {
    /* X2G */
    ftm_gsm_cm_rf_info.rxlm_gsm_buf_idx = test_param_p->tgt_prx_rxlm_buf_idx;
    ftm_gsm_cm_rf_info.target_band      = test_param_p->tgt_band;
    ftm_gsm_cm_rf_info.target_rf_device = rfm_dev;

    /* Populate common tech X parts now uses the one from ftm_meas.c */
    ftm_gsm_cm_rf_info.common_rf_structure_ptr = &ftm_gsm_irat_src_meas_params;

    FTM_MSG_2( FTM_HIGH,
               FTM_GSM_MEAS_FUNC"->geran_test_mode_api_cm_startup"
               " FN:%d offset:%d",
               geran_test_mode_api_get_FN( as_id ), offset );

    geran_test_mode_api_cm_startup(
      offset, (const gl1_hw_cm_rf_info_t *)&ftm_gsm_cm_rf_info, as_id );

    /* X2G so config single power measure */
    ftm_gsm_meas_power_meas_cmd( test_param_p, as_id );
  }
  else
  {
    FTM_MSG_3( FTM_HIGH,
               FTM_GSM_MEAS_FUNC"->geran_test_mode_api_cm_cleanup FN:%d"
               " offset:%d use_src_param:%d",
               geran_test_mode_api_get_FN( as_id ), offset, use_src_param );

    /* G2X and we are tuning back */
    /* Execute IRAT cleanup script*/
    geran_test_mode_api_cm_cleanup( offset, as_id );

    /* Send RXLM to FW */
    result = ftm_gsm_meas_fw_execute_rxlm( test_param_p, use_src_param );

    if ( !result )
    {
      FTM_MSG_3( FTM_ERROR,
                 FTM_GSM_MEAS_FUNC"->ftm_gsm_meas_fw_execute_rxlm"
                 " use_src_param:%d src_tech:%d tgt_tech:%d",
                 use_src_param, test_param_p->src_tech, test_param_p->tgt_tech );

      return ( result );
    }

    ftm_meas_gsm_wait_for_fw( as_id );

    /* G2X so config single power measure before rxlm deallocated */
    ftm_gsm_meas_power_meas_cmd( test_param_p, as_id );

    ftm_gsm_deallocate_rxlm();
  }

  FTM_MSG_1( FTM_HIGH,
             FTM_GSM_MEAS_FUNC" end FN:%d",
             geran_test_mode_api_get_FN( as_id ) );
#endif /* !TEST_FRAMEWORK */

  return ( result );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_power_meas_cmd function will basically allow RFSW to pass to
    L1 how to do a single power measurement in iRAT.

  @param
    rfm_meas_generic_params_type: ptr to data structure for all
    input info to identify source tech
  @param
    sys_modem_as_id_e_type: active subs as_id.

  @pre
    We should be in a certain technology.
*/
#define FTM_MEAS_NUM_PWR_MEAS_SAMPLES 1

static
void ftm_gsm_meas_power_meas_cmd(
       rfm_meas_generic_params_type *test_param_p,
       sys_modem_as_id_e_type        as_id )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_meas_power_meas_cmd()"

#ifndef TEST_FRAMEWORK
  ARFCN_T                    arfcns[MAX_CM_RSSI_MEASUREMENTS_REQ];
  gl1_hw_rx_tags_type        tags[MAX_CM_RSSI_MEASUREMENTS_REQ];
  gl1_hw_burst_position_type position[MAX_CM_RSSI_MEASUREMENTS_REQ];
  uint16                     offset[MAX_CM_RSSI_MEASUREMENTS_REQ];
  dBx16_T                    rx_signal_strength[MAX_CM_RSSI_MEASUREMENTS_REQ];
  gl1_hw_measurement_T       samples[FTM_MEAS_NUM_PWR_MEAS_SAMPLES];
  uint16                     i, num_of_req_samples, channel, band;
  boolean                    result;

  /* Parameter Check. At least one tech should be gsm */
  FTM_GSM_MEAS_PAR_CHCK( test_param_p );

  FTM_MSG_2( FTM_HIGH,
             FTM_GSM_MEAS_FUNC" start FN:%d as_id:%d",
             geran_test_mode_api_get_FN( as_id ), as_id );

  if ( RFCOM_GSM_MODE == test_param_p->src_tech )
  {
    /* G2X */
    channel = (uint16)test_param_p->src_ca_path0_chan_car0;
    band    = test_param_p->src_ca_path0_band;
  }
  else
  {
    /* X2G */
    channel = (uint16)test_param_p->tgt_chan;
    band    = test_param_p->tgt_band;
  }

  for ( i = 0; i < FTM_MAX_CM_RSSI_MEASUREMENTS; i++ )
  {
    arfcns[i].num  = channel;
    arfcns[i].band = ftm_meas_gsm_map_rfband_to_sys_band( band );

    FTM_MSG_2( FTM_LOW,
               FTM_GSM_MEAS_FUNC"Sys band:%d channel:%d",
               arfcns[i].band, arfcns[i].num );

    position[i] = GL1_HW_ONE_BURST_IN_GAP;

    memset( &tags[i], 0, sizeof( gl1_hw_rx_tags_type ) );

    rx_signal_strength[i] = 0;
  }

  num_of_req_samples = FTM_MEAS_NUM_PWR_MEAS_SAMPLES;

  for ( i = 0; i < num_of_req_samples; i++ )
  {
    offset[i] = ( FTM_MEAS_GSM_OFFSET + FRAME_TICK_SETUP_MARGIN_QS );
  }

  FTM_MSG_2( FTM_HIGH,
             FTM_GSM_MEAS_FUNC"->geran_test_mode_api_cm_power_measure FN:%d offset:%d",
             geran_test_mode_api_get_FN( as_id ), offset[0] );

  geran_test_mode_api_cm_power_measure( arfcns, rx_signal_strength, num_of_req_samples,
                                        offset, position, tags, as_id );

  ftm_meas_gsm_wait_for_fw( as_id );

  FTM_MSG_1( FTM_HIGH,
             FTM_GSM_MEAS_FUNC"->geran_test_mode_api_cm_get_power_measure_results FN:%d",
             geran_test_mode_api_get_FN( as_id ) );

  result = geran_test_mode_api_cm_get_power_measure_results(
             samples, num_of_req_samples, tags, as_id );

  for ( i = 0; i < num_of_req_samples; i++ )
  {
    if ( !result)
    {
      FTM_MSG_3( FTM_ERROR,
                 FTM_GSM_MEAS_FUNC"->geran_test_mode_api_cm_get_power_measure_results"
                 " src_tech:%d tgt_tech:%d status:%d",
                 test_param_p->src_tech, test_param_p->tgt_tech, samples[i].status );

      samples[i].dBm_x16 = 0;
    }
    else
    {
      FTM_MSG_4( FTM_HIGH,
                 FTM_GSM_MEAS_FUNC"->geran_test_mode_api_cm_get_power_measure_results"
                 " samples[%d]:%ddBmx16 rx_gain_range:%d status:%d",
                 i, samples[i].dBm_x16, samples[i].rx_gain_range, samples[i].status );
    }
  }

  mdsp_ftm_wait_for_frame_tick( as_id );

  /* Single sample at present multiple mons possible */
  g_meas_results.x2g_irat_enabled = TRUE;
  g_meas_results.db_x16           = samples[0].dBm_x16;

  FTM_MSG_1( FTM_HIGH,
             FTM_GSM_MEAS_FUNC" end FN:%d",
             geran_test_mode_api_get_FN( as_id ) );
#endif /* !TEST_FRAMEWORK */
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_meas_gsm_map_rfband_to_sys_band function maps an RF band
    to a corresponding sys_band_T.

  @param
    uint16: RF test band.

  @retval
    Corresponding sys_band_T if valid.
*/
static
sys_band_T ftm_meas_gsm_map_rfband_to_sys_band(
             uint16 band )
{
  #define FTM_GSM_MEAS_FUNC "ftm_meas_gsm_map_rfband_to_sys_band()"

  sys_band_T sys_band = SYS_BAND_NONE;

  switch( band )
  {
    case RFCOM_BAND_GSM850:
      sys_band = SYS_BAND_CELL_850;
    break;

    case RFCOM_BAND_GSM900:
      sys_band = SYS_BAND_EGSM_900;
    break;

    case RFCOM_BAND_GSM1800:
      sys_band = SYS_BAND_DCS_1800;
    break;

    case RFCOM_BAND_GSM1900:
      sys_band = SYS_BAND_PCS_1900;
    break;

    default:
      FTM_MSG_1( FTM_ERROR,
                 FTM_GSM_MEAS_FUNC" Invalid band:%d received",
                 band );
    break;
  }

  return ( sys_band );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_meas_gsm_wait_for_fw function is delay for FW completion.

  @param
    sys_modem_as_id_e_type: active subs as_id.
*/

/* Delay for FW completion */
static
void ftm_meas_gsm_wait_for_fw(
       sys_modem_as_id_e_type as_id )
{
  #define FTM_GSM_MEAS_FUNC "ftm_meas_gsm_wait_for_fw()"

  uint16 wait_cnt = FTM_MEAS_GSM_WAIT_CNT;

  do
  {
    FTM_MSG_2( FTM_MED,
               FTM_GSM_MEAS_FUNC" FN:%d wait_cnt:%d",
               geran_test_mode_api_get_FN( as_id ), wait_cnt );

    mdsp_ftm_wait_for_frame_tick( as_id );
  } while ( wait_cnt-- );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_set_rf_device function will set active meas_enter RF device.

  @param
    rfm_device_enum_type: RF device on iRAT measurement entry for target.
*/
static
void ftm_gsm_meas_set_rf_device(
       rfm_device_enum_type tgt_device )
{
  ftm_gsm_meas_tgt_irat_device = tgt_device;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    ftm_gsm_meas_get_rf_device function allows for the requested iRAT device
    to be saved away from enter measurement.

  @pre
    ftm_gsm_meas_set_rf_device() must have already been called.
    We should be in a certain technology.

  @retval
    Returns an RF device set for target tech.
*/
static
rfm_device_enum_type ftm_gsm_meas_get_rf_device( void )
{
  #define FTM_GSM_MEAS_FUNC "ftm_gsm_meas_get_rf_device()"

  if ( ftm_gsm_meas_tgt_irat_device >= RFM_MAX_WAN_DEVICES )
  {
    FTM_MSG_1( FTM_ERROR,
               FTM_GSM_MEAS_FUNC" Invalid WAN Device:%d!!",
               ftm_gsm_meas_tgt_irat_device );
  }

  return ( ftm_gsm_meas_tgt_irat_device );
}

