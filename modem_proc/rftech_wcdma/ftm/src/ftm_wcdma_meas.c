/*!
  @file ftm_wcdma_meas.c

  @brief
  This file contains all declarations and definitions necessary to use
  the wcdma FTM Measurement Main Control.

  @details

*/

/*===========================================================================

  Copyright (c) 2013-2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Inc and all rights therein are expressly reserved.
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

$Header: //components/rel/rftech_wcdma.mpss/3.11/ftm/src/ftm_wcdma_meas.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/27/15   rmb     Remove featurisation for Tabasco compilation.
07/27/15   rmb     Add support for X2W and W2X IRAT based on Rx/Tx split architecture.
06/17/15   rmb     Fix the W2W tune by using Multi carrier index from correct state machine.
06/15/15   vs      Init params of RxAGC cmd for all RFM devices
04/24/15   rmb     Porting from mainline to dev branch.
03/31/15   nv      First version of CRAT FTM GUI
03/16/15   nv      Removed SRC buf idx and TGT buf idx as they are not used in FED 
                   based architecture and also updated variable name for CAR1 enable.
12/12/14   rmb     Fix to use the common meas API to build scripts for W2W.
09/19/14   rmb     DBDC support for W2X FTM IRAT cases.
08/11/14   rmb     Bypass RxAGC on flag for FTM X2W Rxagc read after tune to W.
06/06/14   ac      nbr support
05/14/14   rmb     Fix KW Error.
05/13/14   rmb     Enable W2X and L2W FTM IRAT.
04/14/14   rmb     Fix Compilation Error on Bolt 2.0 
04/14/14   rmb     Add changes for L2W FTM IRAT.
04/04/14   rmb     Assign valid RxLM index during Meas exit for Abort.
03/25/14   rmb     Enable G2W FTM IRAT.
03/25/14   rmb     FTM IRAT 2nd phase checkin.
02/27/14   aa      DimePM linker error fixex
02/27/14   rmb     First phase FTM IRAT checkin for G2W. 
02/13/14   dw      [1]Change AGC command function to support multiple device
                   [2]Code cleanup
01/03/14   rmb     Bolt IRAT changes for WCDMA.
06/05/13   aa      Turn of TX when exiting W
05/09/13   aa      updated W2W FTM IRAT for new FTM framework
04/25/13   pl      Populate exit_params fully and correctly
02/27/13   sar     Featurized header file to fix non-lte build.
02/19/13   pl      Remove unused fields
02/06/13   rmb     Fix Compilation Errors.
01/31/13   aa      added a simple state machine for W enter/exit procedure
01/11/13   aa      Initial Check-in
===========================================================================*/

#include "rfmeas_types.h"
#include "rfmeas_mc.h"
#include "ftm_meas.h"
#include "ftm_msg.h"
#include "ftm_wcdma_ber.h"
#include "rf_test_task_v.h"

#include "rfwcdma_mc.h"
#include "ftm_wcdma_ctl.h"
#include "ftm_wcdma_meas.h"
#include "rfmeas_wcdma.h"
#include "rfwcdma_mc_meas.h"
#include "rfwcdma_mdsp.h"
#include "rfmeas_ftm_mdsp.h"
#ifdef FEATURE_LTE
#include "intf_sys.h"
#endif
#include "rfwcdma_msg.h"
#include "ftm.h"
#include "rfcom.h"
#include "rfwcdma_core_rxctl.h"
#include "fws.h"
#include "ftm_common_control.h"
#include "rflm_api_wcdma.h"
#include "rflm_time.h"
#include "rfwcdma_mdsp_sync.h"
#include "rfwcdma_core.h"
#include "rfwcdma_core_util.h"
#include "rfwcdma_core_txctl.h"
#include "rfwcdma_data.h"
#include "rf_task_common_services.h"

/*! WL1 Start/Stop cmd timeout of 200 ms */
#define WL1_START_STOP_WAIT_TIMEOUT 200000 

/* Inverse of mid band frequencies (2^52 / freq in Hz) */
#define INV_MID_FREQ_WCDMA_2100_BAND    2104486
#define INV_MID_FREQ_WCDMA_1900_BAND    2297755
#define INV_MID_FREQ_WCDMA_BC3_BAND     2443624
#define INV_MID_FREQ_WCDMA_BC4_BAND     2111392
#define INV_MID_FREQ_WCDMA_800_BAND     5106122
#define INV_MID_FREQ_WCDMA_BC8_BAND     4775821
#define INV_MID_FREQ_WCDMA_BC9_BAND     2418689

#define RF_WCDMA_X2W_AGC_POLLING_INTERVAL 5 // Every 5 micro sec
#define RF_WCDMA_AGC_SETTLING_TIME 667 // 10 bpgs

/* This RxLM handle is updated during L2W measurements and passed to L1 for L2W Tune away */
extern uint32 l2w_ftm_rxlm_handle;

extern ftm_lm_buf_type ftm_wcdma_rxlm_buffer[];

extern ftm_lm_buf_type ftm_wcdma_txlm_buffer;

/*============================================================================
             LOCAL Function Prototypes                                                      
=============================================================================*/
boolean ftm_wcdma_meas_get_src_rxlm_buffer(rfm_meas_generic_params_type *test_param_ptr);
boolean ftm_wcdma_meas_get_src_txlm_buffer(rfm_meas_generic_params_type *test_param_ptr);
boolean ftm_wcdma_meas_enter_meas(rfm_meas_enter_param_type *meas_enter_param);
boolean ftm_wcdma_meas_build_script(rfm_meas_setup_param_type *meas_setup_param);
boolean ftm_wcdma_meas_exit_meas(rfm_meas_exit_param_type *meas_exit_param);
boolean ftm_wcdma_meas_exit_wcdma(rfm_meas_generic_params_type *test_param_ptr, boolean use_src_param);
boolean ftm_wcdma_meas_enter_wcdma(rfm_meas_generic_params_type *test_param_ptr, boolean use_src_param);
rfm_meas_tech_setup_params_type ftm_wcdma_meas_get_wcdma_param(rfm_meas_generic_params_type *test_param_ptr, boolean is_src);
boolean ftm_wcdma_meas_stop_rxagc( rfm_meas_generic_params_type *test_param_ptr, boolean is_src );
boolean ftm_wcdma_meas_tune_and_start_rxagc( rfm_meas_generic_params_type *test_param_ptr, boolean is_src );
boolean ftm_wcdma_meas_enable_tx( rfm_meas_generic_params_type *test_param_ptr );
boolean ftm_wcdma_meas_disable_tx( rfm_meas_generic_params_type *test_param_ptr );
boolean ftm_wcdma_meas_init_wcdma( void );
boolean ftm_wcdma_meas_deinit_wcdma( void );
uint32 ftm_wcdma_meas_get_inverse_dlo_freq(uint16 chan);
boolean ftm_wcdma_meas_wtow_tune_wcdma( rfm_meas_generic_params_type *test_param_ptr, boolean is_wcdma_neighbour );

/*============================================================================
             LOCAL/EXTERNAL VARIABLES                                                      
=============================================================================*/
static ftm_meas_func_tbl_type ftm_wcdma_meas_apis = \
{
   ftm_wcdma_meas_get_src_rxlm_buffer,
   ftm_wcdma_meas_get_src_txlm_buffer,	
   NULL,
   NULL,
   NULL,
   ftm_wcdma_meas_enter_meas,
   ftm_wcdma_meas_build_script,
   ftm_wcdma_meas_exit_wcdma,
   ftm_wcdma_meas_enter_wcdma,
   ftm_wcdma_meas_exit_meas,
   ftm_wcdma_meas_get_wcdma_param
};

void ftm_wcdma_meas_register(void)
{
   ftm_meas_register(RFCOM_WCDMA_MODE, &ftm_wcdma_meas_apis);
}
void ftm_wcdma_meas_deregister(void)
{
   ftm_meas_deregister(RFCOM_WCDMA_MODE);
}

boolean ftm_wcdma_meas_get_src_rxlm_buffer(rfm_meas_generic_params_type *test_param_ptr)
{
  if(test_param_ptr == NULL)
  {
    FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_get_src_rxlm_buffer: Input params is NULL");
    return FALSE;
  }

  if(test_param_ptr->src_tech != RFCOM_WCDMA_MODE)
  {
    FTM_MSG_1( FTM_ERROR, "ftm_wcdma_meas_get_src_rxlm_buffer: Source tech %d is not WCDMA", test_param_ptr->src_tech);
    return FALSE;
  }

  /* RxLM handle would have allcoated by WCDMA during standalone tune. Re-use it*/
  
  /* Carrier path 0 - Primary */
  if (test_param_ptr->src_ca_path0_device >= RFM_WCDMA_MAX_DEVICE) 
  {
    FTM_MSG_2( FTM_ERROR, "ftm_wcdma_meas_get_src_rxlm_buffer: Source CA path 0 device %d exceed Max %d", 
               test_param_ptr->src_ca_path0_device, RFM_WCDMA_MAX_DEVICE);
    return FALSE;
  }


  if(ftm_wcdma_rxlm_buffer[test_param_ptr->src_ca_path0_device].is_buf_allocated)
  {
    test_param_ptr->src_ca_path0_prx_rxlm_buf_idx = ftm_wcdma_rxlm_buffer[test_param_ptr->src_ca_path0_device].buf_idx;
  }

  else
  {
    FTM_MSG_1( FTM_ERROR, "ftm_wcdma_meas_get_src_rxlm_buffer: no RxLM buffer was allocated for source CA path 0 device %d", 
               test_param_ptr->src_ca_path0_device);
    return FALSE;
  }

  /* Carrier path 0 - Diversity */
  if(test_param_ptr->src_ca_path0_div_en == TRUE)
  {
    if (test_param_ptr->src_ca_path0_div_device_id1 >= RFM_WCDMA_MAX_DEVICE) 
    {
      FTM_MSG_2( FTM_ERROR, "ftm_wcdma_meas_get_src_rxlm_buffer: Source CA path 0 div device %d exceed Max %d", 
                 test_param_ptr->src_ca_path0_div_device_id1, RFM_WCDMA_MAX_DEVICE);
      return FALSE;
    }


    if(ftm_wcdma_rxlm_buffer[test_param_ptr->src_ca_path0_div_device_id1].is_buf_allocated)
    {
      test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx = ftm_wcdma_rxlm_buffer[test_param_ptr->src_ca_path0_div_device_id1].buf_idx;
    }

    else
    {
      FTM_MSG_1( FTM_ERROR, "ftm_wcdma_meas_get_src_rxlm_buffer: no RxLM buffer was allocated for source CA path 0 div device %d", 
                 test_param_ptr->src_ca_path0_div_device_id1);
      return FALSE;
    }
  } /* if(test_param_ptr->src_div_en == TRUE)*/

  /* Carrier path 1 - Primary */
  if (test_param_ptr->src_ca_path1_flag_enable == TRUE)
  {
    if (test_param_ptr->src_ca_path1_device >= RFM_WCDMA_MAX_DEVICE) 
    {
      FTM_MSG_2( FTM_ERROR, "ftm_wcdma_meas_get_src_rxlm_buffer: Source CA path 1 device %d exceed Max %d", 
                 test_param_ptr->src_ca_path1_device, RFM_WCDMA_MAX_DEVICE);
      return FALSE;
    }


    if(ftm_wcdma_rxlm_buffer[test_param_ptr->src_ca_path1_device].is_buf_allocated)
    {
      test_param_ptr->src_ca_path1_prx_rxlm_buf_idx = ftm_wcdma_rxlm_buffer[test_param_ptr->src_ca_path1_device].buf_idx;
    }

    else
    {
      FTM_MSG_1( FTM_ERROR, "ftm_wcdma_meas_get_src_rxlm_buffer: no RxLM buffer was allocated for source CA path 1 device %d", 
                 test_param_ptr->src_ca_path1_device);
      return FALSE;
    }
    
    /* Carrier path 1 - Diversity */
    if( test_param_ptr->src_ca_path1_div_en == TRUE)
    {
      if (test_param_ptr->src_ca_path1_div_device_id1 >= RFM_WCDMA_MAX_DEVICE) 
      {
        FTM_MSG_2( FTM_ERROR, "ftm_wcdma_meas_get_src_rxlm_buffer: Source Ca path 1 div device %d exceed Max %d", 
                   test_param_ptr->src_ca_path1_div_device_id1, RFM_WCDMA_MAX_DEVICE);
        return FALSE;
      }


      if(ftm_wcdma_rxlm_buffer[test_param_ptr->src_ca_path1_div_device_id1].is_buf_allocated)
      {
        test_param_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx = ftm_wcdma_rxlm_buffer[test_param_ptr->src_ca_path1_div_device_id1].buf_idx;
      }

      else
      {
        FTM_MSG_1( FTM_ERROR, "ftm_wcdma_meas_get_src_rxlm_buffer: no RxLM buffer was allocated for source Ca path 1 div device %d", 
                   test_param_ptr->src_ca_path1_div_device_id1);
        return FALSE;
      }

    } /* if( test_param_ptr->src_div_device_carrier_1 == TRUE)*/

  } /* if (test_param_ptr->src_flag_enable_carrier_1 == TRUE) */

  /* WCDMA is not supported on 3rd carrier path */

  return TRUE;
}

boolean ftm_wcdma_meas_get_src_txlm_buffer(rfm_meas_generic_params_type *test_param_ptr)
{
  if(test_param_ptr == NULL)
  {
    FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_get_src_txlm_buffer: Input params is NULL");
    return FALSE;
  }

  if(test_param_ptr->src_tech != RFCOM_WCDMA_MODE)
  {
    FTM_MSG_1( FTM_ERROR, "ftm_wcdma_meas_get_src_txlm_buffer: Source tech %d is not WCDMA", test_param_ptr->src_tech);
    return FALSE;
  }

  /* TxLM handle would have allcoated by WCDMA during standalone tune. Re-use it
     There is only one Tx for WCDMA. It should be either on CA path 0 or 1*/

  /* Carrier path 0 - Tx */
  if(test_param_ptr->src_ca_path0_tx_en == TRUE)
  {
    if (test_param_ptr->src_ca_path0_tx_device >= RFM_WCDMA_MAX_DEVICE) 
    {
      FTM_MSG_2( FTM_ERROR, "ftm_wcdma_meas_get_src_txlm_buffer: Source CA path 0 Tx device %d exceed Max %d", 
                 test_param_ptr->src_ca_path0_tx_device, RFM_WCDMA_MAX_DEVICE);
      return FALSE;
    }


    if(ftm_wcdma_txlm_buffer.is_buf_allocated)
    {
      test_param_ptr->src_ca_path0_txlm_buf_idx = ftm_wcdma_txlm_buffer.buf_idx;
    }

    else
    {
      FTM_MSG_1( FTM_ERROR, "ftm_wcdma_meas_get_src_txlm_buffer: no TxLM buffer was allocated for source CA path 0 Tx device %d", 
                 test_param_ptr->src_ca_path0_tx_device);
      return FALSE;
    }
  } /* if(test_param_ptr->src_ca_path0_tx_en == TRUE)*/

  /* Carrier path 1 - Tx */
  else if ( (test_param_ptr->src_ca_path1_flag_enable == TRUE) && (test_param_ptr->src_ca_path1_tx_en == TRUE) )
  {
    if (test_param_ptr->src_ca_path1_tx_device >= RFM_WCDMA_MAX_DEVICE) 
    {
      FTM_MSG_2( FTM_ERROR, "ftm_wcdma_meas_get_src_txlm_buffer: Source CA path 1 Tx device %d exceed Max %d", 
                 test_param_ptr->src_ca_path1_tx_device, RFM_WCDMA_MAX_DEVICE);
      return FALSE;
    }

    if(ftm_wcdma_txlm_buffer.is_buf_allocated)
    {
      test_param_ptr->src_ca_path1_txlm_buf_idx = ftm_wcdma_txlm_buffer.buf_idx;
    }

    else
    {
      FTM_MSG_1( FTM_ERROR, "ftm_wcdma_meas_get_src_txlm_buffer: no TxLM buffer was allocated for source CA path 1 Tx device %d", 
                 test_param_ptr->src_ca_path1_tx_device);
      return FALSE;
    }
  } /*   else if ( (test_param_ptr->src_ca_path1_flag_enable == TRUE) && (test_param_ptr->src_ca_path1_tx_en == TRUE) ) */

  else
  {
    FTM_MSG( FTM_MED, "ftm_wcdma_meas_get_src_txlm_buffer: Tx is not enabled");
  }

  return TRUE;
}

boolean ftm_wcdma_meas_enter_meas(rfm_meas_enter_param_type *meas_enter_param)
{
  rfm_meas_result_type status = RFM_MEAS_COMMON_FAILURE;
  boolean result = FALSE;
  boolean call_init_wcdma_routine = FALSE;

  if (meas_enter_param == NULL) 
  {
    FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_enter_meas: Meas enter params is NULL");
    return FALSE;
  }

  if (meas_enter_param->enter_params == NULL) 
  {
    FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_enter_meas: Common Enter params is NULL");
    return FALSE;
  }

  /* For G2W, enable WFW in SSIM mode. So, set FTM RF state = RFM_WCDMA_FTM_RF */
  if ( (meas_enter_param->enter_params->header.target_tech == RFCOM_WCDMA_MODE) &&
       (meas_enter_param->enter_params->header.source_tech == RFCOM_GSM_MODE)
     )    
  {
    rfm_wcdma_set_ftm_state(RFM_WCDMA_FTM_RF);
    call_init_wcdma_routine = TRUE;
    FTM_MSG( FTM_HIGH, "ftm_wcdma_meas_enter_meas: Set FTM state to FTM RF for G2W IRAT") ;
  }

  /* For L2W, enable WFW in SSIM mode. But set FTM RF state = RFM_WCDMA_FTM_RXONLY */
  else if ( (meas_enter_param->enter_params->header.target_tech == RFCOM_WCDMA_MODE) &&
            (meas_enter_param->enter_params->header.source_tech == RFCOM_LTE_MODE)
          )    
  {
    rfm_wcdma_set_ftm_state(RFM_WCDMA_FTM_RXONLY);
    call_init_wcdma_routine = TRUE;
    FTM_MSG( FTM_HIGH, "ftm_wcdma_meas_enter_meas: Set FTM state to FTM Rx only for L2W IRAT") ;
  }

  /* Vote for Maximum CPU Speed only if this is NULL2W and set FTM RF state = RFM_WCDMA_FTM_RXONLY */
  else if ( (meas_enter_param->enter_params->header.target_tech == RFCOM_WCDMA_MODE) &&
       (meas_enter_param->enter_params->script_type == RFM_MEAS_PROGRAM_NULL2X) 
     )
  {
    rfm_wcdma_set_ftm_state(RFM_WCDMA_FTM_RXONLY);
    ftm_vote_cpu_speed( RF_Q6_MAX_CPU_SPEED_KHZ );
    call_init_wcdma_routine = TRUE;
    FTM_MSG( FTM_HIGH, "ftm_wcdma_meas_enter_meas: For NULL2X, voting CPU for max speed and set FTM RF state") ;
  }

  if (call_init_wcdma_routine == TRUE) 
  {
    if( ftm_wcdma_meas_init_wcdma() == FALSE)
    {
      FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_enter_meas: Init WCDMA failed");
    }

    FTM_MSG( FTM_HIGH, "ftm_wcdma_meas_enter_meas: Called Init WCDMA") ;
  }

    #if 0
    /* If W2W then just return with no error */
    if( (meas_enter_param->header.source_tech == RFCOM_WCDMA_MODE) &&
        (meas_enter_param->header.target_tech == RFCOM_WCDMA_MODE) )
    {
       if (meas_enter_param->target_param != NULL) 
       {
          wtow_meas_param = (rfm_meas_wcdma_enter_param_type*)meas_enter_param->target_param;
          /* Update RxLM buf index into state variable for neighbour */
          ftm_wcdma_neighbour_rxlm_buffer = wtow_meas_param->rxlm_buf_index;

          /* Update RxLM buffer for Neighbour before building scripts */
          rfm_update_rxlm_buffer( wtow_meas_param->device, 
                                  wtow_meas_param->channel,
                                  wtow_meas_param->carrier_type,
                                  (uint32)(wtow_meas_param->rxlm_buf_index) );
          return TRUE;
       }

       else
       {
         MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "ftm_wcdma_meas_enter_meas: Target enter Params is NULL");
         return FALSE;
       }
    }

  if(meas_enter_param->enter_params->header.source_tech == RFCOM_GSM_MODE)
  {
    /* Send G2W mdsp sync init command when we start measurement*/
    result = rfwcdma_mdsp_sync_send_cmd( RFWCDMA_MDSP_G2W_INIT_CMD_ID, NULL);

    if(result == FALSE)
    {
      FTM_MSG(FTM_ERROR, "G2W MDSP SYNC INIT command failed");
      return result;
    }

    /* Wait for Sync init command to complete */
    DALSYS_BusyWait(10000);

    FTM_MSG(FTM_HIGH, "Called G2W MDSP SYNC INIT command");     
  }

  else if(meas_enter_param->enter_params->header.source_tech == RFCOM_LTE_MODE)
  {
    /* Send L2W mdsp sync init command when we start measurement*/
    result = rfwcdma_mdsp_sync_send_cmd( RFWCDMA_MDSP_L2W_INIT_CMD_ID, NULL);

    if(result == FALSE)
    {
      FTM_MSG(FTM_ERROR, "L2W MDSP SYNC INIT command failed");
      return result;
    }

    /* Wait for Sync init command to complete */
    DALSYS_BusyWait(10000);

    FTM_MSG(FTM_HIGH, "Called L2W MDSP SYNC INIT command"); 
  }
  #endif
  
  status = rfmeas_mc_enter(meas_enter_param, NULL);
  if(status != RFM_MEAS_COMMON_SUCCESS)
  {
    FTM_MSG(FTM_ERROR,"ftm_wcdma_meas_enter_meas: rfmeas_mc_enter() returned failure");
    return FALSE;
  }
  return TRUE;
}

boolean ftm_wcdma_meas_build_script(rfm_meas_setup_param_type *meas_setup_param)
{
  rfm_meas_result_type status = RFM_MEAS_COMMON_FAILURE;
  boolean result = TRUE;

  if(meas_setup_param == NULL)
  {
    FTM_MSG(FTM_ERROR,"ftm_wcdma_meas_build_script: Meas script params is NULL");
    return FALSE;
  }

  if (meas_setup_param->setup_params == NULL) 
  {
    FTM_MSG(FTM_ERROR,"ftm_wcdma_meas_build_script: Meas Script setup params is NULL");
    return FALSE;
  }

  status = rfmeas_build_scripts(meas_setup_param, NULL);
  if(status != RFM_MEAS_COMMON_SUCCESS)
  {
    result = FALSE;
  }

  return result;
}


boolean ftm_wcdma_meas_exit_meas(rfm_meas_exit_param_type *meas_exit_param)
{
  rfm_meas_result_type status = RFM_MEAS_COMMON_FAILURE;
  boolean result = FALSE;
  timetick_type start_time = 0;
  rex_tcb_type* current_tcb_ptr = rex_self();

  /* Check for NULL pointers */
  if (meas_exit_param == NULL)
  {
    FTM_MSG(FTM_ERROR,"ftm_wcdma_meas_exit_meas: Meas exit params is NULL");
    return result;
  }

  if (meas_exit_param->exit_params == NULL) 
  {
    FTM_MSG(FTM_ERROR,"ftm_wcdma_meas_exit_meas: Common Exit params is NULL");
    return result;
  }

  status = rfmeas_mc_exit(meas_exit_param, NULL);
  if(status != RFM_MEAS_COMMON_SUCCESS)
  {
    FTM_MSG(FTM_ERROR,"ftm_wcdma_meas_exit_meas: rfmeas_mc_exit() returned failure");
    return FALSE;
  }

  /* Call de-init for G2W, L2W IRAT and NULL2W cases*/
  if ( (meas_exit_param->exit_params->header.target_tech == RFCOM_WCDMA_MODE) &&
       ( (meas_exit_param->exit_params->header.source_tech == RFCOM_GSM_MODE) ||
         (meas_exit_param->exit_params->header.source_tech == RFCOM_LTE_MODE) ||
         (meas_exit_param->exit_params->script_type == RFM_MEAS_PROGRAM_NULL2X)
        )
     )    
  {
    if(ftm_wcdma_meas_deinit_wcdma() == FALSE)
    {
      FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_exit_meas: De-init WCDMA failed");
    }

    FTM_MSG_3( FTM_HIGH, "ftm_wcdma_meas_exit_meas: De-Init WCDMA is called for src tech %d, tgt tech %d, script type %d",
               meas_exit_param->exit_params->header.source_tech, meas_exit_param->exit_params->header.target_tech,
               meas_exit_param->exit_params->script_type) ;
  }

  #if 0
  /* Send L2W deinit command after all measurements are done*/
  if( (meas_exit_param->exit_params->header.source_tech == RFCOM_LTE_MODE) && (meas_exit_param->exit_params->header.target_tech == RFCOM_WCDMA_MODE) )
  {
    /* Send L2W deinit command at last after WFW Stop command and also after Abort command if any */
    result = rfwcdma_mdsp_sync_send_cmd( RFWCDMA_MDSP_L2W_DEINIT_CMD_ID, NULL);

    if(result == FALSE)
    {
      FTM_MSG(FTM_ERROR, "L2W DEINIT command failed");
      return result;
    }

    /* Wait for Deinit command to complete */
    DALSYS_BusyWait(10000);

    FTM_MSG(FTM_HIGH, "Called L2W DEINIT command");     
  }
  #endif

  return TRUE;
}


boolean ftm_wcdma_meas_exit_wcdma(rfm_meas_generic_params_type *test_param_ptr, boolean use_src_param)
{
  boolean result = TRUE;

  if(test_param_ptr == NULL)
  {
    FTM_MSG(FTM_ERROR, "ftm_wcdma_meas_exit_wcdma: NULL param pointer");
    return FALSE;
  }

  /* If source WCDMA has Tx enabled, disable Tx before RxAGC stop on source */
  if ( (use_src_param == TRUE) && (test_param_ptr->src_ca_path0_tx_en == TRUE) ) 
  {
    FTM_MSG(FTM_HIGH, "ftm_wcdma_meas_exit_wcdma: Disable Tx");
    result = ftm_wcdma_meas_disable_tx(test_param_ptr);
  }

  /* Stop RxAGC on WCDMA */
  result &= ftm_wcdma_meas_stop_rxagc(test_param_ptr, use_src_param);

  if(result != TRUE)
  {
    FTM_MSG_3(FTM_ERROR, "ftm_wcdma_meas_exit_wcdma: Stop WCDMA failed for use_src_param %d, src_tech %d, tgt_tech %d",
              use_src_param, test_param_ptr->src_tech, test_param_ptr->tgt_tech);
    return result;
  }

  return result;
}

boolean ftm_wcdma_meas_enter_wcdma(rfm_meas_generic_params_type *test_param_ptr, boolean use_src_param)
{
  boolean result = FALSE; 

  if(test_param_ptr == NULL)
  {
    FTM_MSG(FTM_ERROR, "ftm_wcdma_meas_enter_wcdma: NULL param pointer");
    return FALSE;
  }

  /* Tune away to W or Tune back to W and start RxAGC */
  result = ftm_wcdma_meas_tune_and_start_rxagc(test_param_ptr, use_src_param);

  /* If source WCDMA has Tx enabled, enable Tx after RxAGC start on source */
  if ( (use_src_param == TRUE) && (test_param_ptr->src_ca_path0_tx_en == TRUE) ) 
  {
    FTM_MSG(FTM_HIGH, "ftm_wcdma_meas_enter_wcdma: Enable Tx");
    result = ftm_wcdma_meas_enable_tx(test_param_ptr);
  }

  if(result != TRUE)
  {
    FTM_MSG_3(FTM_ERROR, "ftm_wcdma_meas_enter_wcdma:Start WCDMA failed for use_src_param %d, src_tech %d, tgt_tech %d",
              use_src_param, test_param_ptr->src_tech, test_param_ptr->tgt_tech);
    return result;
  }

  return result;
}

/*===========================================================================

  FUNCTION:  ftm_wcdma_meas_validate_device

===========================================================================*/
/*!
  @brief
  This function validates the RFM device if it is a valid device
 
  @param rfm_device
  RFM device to be validated
 
  @return
  True - valid device
  False -  Invalid device
*/
/*=========================================================================*/

boolean ftm_wcdma_meas_validate_device( rfm_device_enum_type rfm_device )
{
  if((rfm_device < RFM_DEVICE_0) || (rfm_device >= RFM_WCDMA_MAX_DEVICE))
  {
    RF_MSG_1(RF_ERROR, "ftm_wcdma_meas_validate_device: Invalid device %d", rfm_device);
    return FALSE;
  }

  return TRUE; 
} /* ftm_wcdma_meas_validate_device */

/*===========================================================================

  FUNCTION:  ftm_wcdma_meas_enable_tx

===========================================================================*/
/*!
  @brief
  This function enables Front end Tx devices and starts TxAGC
  Called in only CM gaps
 
  @param test_param_ptr
  Parameters like device, RFLM handle etc for the chains to be enabled
 
  @return
  Status for function failure or success
*/
/*=========================================================================*/
boolean ftm_wcdma_meas_enable_tx( rfm_meas_generic_params_type *test_param_ptr )
{
  rfm_wcdma_tx_enable_params tx_e_params;
  rfm_device_enum_type tx_device = RFM_INVALID_DEVICE;
  memset(&tx_e_params, 0, sizeof(rfm_wcdma_tx_enable_params));

  if (test_param_ptr == NULL) 
  {
    FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_enable_tx: Test param ptr is NULL");
    return FALSE;
  }

  /* Tx is always on CA path 0*/
  if (test_param_ptr->src_ca_path0_tx_en == FALSE) 
  {
    FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_enable_tx: Source tech CA path 0 does not have Tx enabled");
    return TRUE;
  }

  tx_device = test_param_ptr->src_ca_path0_tx_device;

  if (ftm_wcdma_meas_validate_device(tx_device) == FALSE)
  {
    FTM_MSG_1( FTM_ERROR, "ftm_wcdma_meas_enable_tx: Invalid Tx device %d", tx_device);
    return FALSE;
  }

  tx_e_params.device = tx_device;

  /* Get the UL channel from State machine for Single carrier */
  tx_e_params.tx_channel[0] = rfwcdma_data_get_curr_chan(tx_device, RFCOM_SINGLE_CARRIER);

  FTM_MSG_1( FTM_HIGH, "ftm_wcdma_meas_enable_tx: UL channel %d", tx_e_params.tx_channel[0]);
  rfm_wcdma_tx_enable(&tx_e_params, NULL, NULL);

  /* ldo_off_flag */
  rfwcdma_core_tx_ftm_disable_ldo_off(rfwcdma_data_get_rflm_buf_idx(tx_device), TRUE);

  rfm_turn_on_pa();

  /* Turn on the PA & other tx front end devices */
  ftm_wcdma_set_pa(tx_device, TRUE);
  ftm_clk_rex_wait(1);

  ftm_send_txlm_cfg_cmd(ftm_wcdma_txlm_buffer.buf_idx, TRUE);

  /* We really want to see a WCDMA pilot so we will go ahead and turn
  ** on right here. At this time there is no real reason to do
  ** it elsewhere.
  */
  ftm_wcdma_set_uplink_waveform();

  /* set Tx power to the last known value, only after the PA has actually been turned ON */
  ftm_wcdma_set_pdm(FTM_PDM_TX_AGC_ADJ, ftm_wcdma_get_cur_tx_pdm_value());

  return TRUE;
} /* ftm_wcdma_meas_enable_tx */

/*===========================================================================

  FUNCTION:  ftm_wcdma_meas_disable_tx

===========================================================================*/
/*!
  @brief
  This function disables Front end Tx devices and stops TxAGC
  Called in only CM gaps
 
  @param test_param_ptr
  Parameters like device, RFLM handle etc for the chains to be enabled
 
  @return
  Status for function failure or success
*/
/*=========================================================================*/
boolean ftm_wcdma_meas_disable_tx( rfm_meas_generic_params_type *test_param_ptr )
{
  rfm_wcdma_tx_disable_params tx_d_params;
  rfm_device_enum_type tx_device = RFM_INVALID_DEVICE;
  memset(&tx_d_params, 0, sizeof(rfm_wcdma_tx_disable_params));

  if (test_param_ptr == NULL) 
  {
    FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_disble_tx: Test param ptr is NULL");
    return FALSE;
  }

  /* Tx is always on CA path 0*/
  if (test_param_ptr->src_ca_path0_tx_en == FALSE) 
  {
    FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_disable_tx: Source tech CA path 0 does not have Tx enabled");
    return TRUE;
  }

  tx_device = test_param_ptr->src_ca_path0_tx_device;

  if (ftm_wcdma_meas_validate_device(tx_device) == FALSE)
  {
    FTM_MSG_1( FTM_ERROR, "ftm_wcdma_meas_disable_tx: Invalid Tx device %d", tx_device);
    return FALSE;
  }

  tx_d_params.device = tx_device;

  ftm_disable_eul_waveform();

  /* Turn off the PA & other tx front end components */
  rfm_wcdma_tx_disable(&tx_d_params, NULL, NULL);

  /* Make sure PA is turned off after WTR off */
  ftm_wcdma_set_pa(tx_device, FALSE);

  ftm_uplink_dpcch_config(FALSE);

  ftm_send_txlm_cfg_cmd(ftm_wcdma_txlm_buffer.buf_idx, FALSE);

  return TRUE;

} /* ftm_wcdma_meas_disable_tx */

/*===========================================================================

  FUNCTION:  ftm_wcdma_meas_stop_rxagc

===========================================================================*/
/*!
  @brief
  This function calls Disable RxAGC for the required chains
 
  @param test_param_ptr
  Parameters like device, RFLM handle etc for the chains to be enabled
 
  @param is_src
  Indicating whether to disable RxAGC on source tech or the target tech
 
  @pre
  WFW should have been enabled in desired mode before this function call
 
  @return
  Status for function failure or success
*/
/*=========================================================================*/

boolean ftm_wcdma_meas_stop_rxagc( rfm_meas_generic_params_type *test_param_ptr, boolean is_src )
{
  rfwcdma_core_rxctl_enable_rxagc_params rxagc_disable_params = {0};

  if (test_param_ptr == NULL) 
  {
    FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_stop_rxagc: Test param ptr is NULL");
    return FALSE;
  }

  memset(&(rxagc_disable_params.cmd_action), RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));

  FTM_MSG(FTM_HIGH, "inside ftm_wcdma_meas_stop_rxagc");

  /* Disable RxAGC on Target device list*/
  if (is_src == FALSE) 
  {
    FTM_MSG_2( FTM_HIGH, "ftm_wcdma_meas_stop_rxagc: Neighbour with PRx handle %d DRx handle %d",
               test_param_ptr->tgt_prx_rxlm_buf_idx, test_param_ptr->tgt_drx_rxlm_buf_idx);

    rxagc_disable_params.multi_carrier_idx = 1; // Target is always one carrier

    /* Target device on PRx has to a valid device */
    if (ftm_wcdma_meas_validate_device(test_param_ptr->tgt_device) == TRUE)
    {
      /* Set the RxAGC disable params */
      rxagc_disable_params.cmd_action[test_param_ptr->tgt_device] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
      rxagc_disable_params.rxlm_buf_idx[test_param_ptr->tgt_device] = test_param_ptr->tgt_prx_rxlm_buf_idx;
      rxagc_disable_params.ant_idx[test_param_ptr->tgt_device] = 0; // Since for Primary device, ant idx = 0
    }

    if ( (test_param_ptr->tgt_div_en == TRUE) &&
         (ftm_wcdma_meas_validate_device(test_param_ptr->tgt_div_device) == TRUE)
       )
    {
      /* Set the RxAGC disable params */
      rxagc_disable_params.cmd_action[test_param_ptr->tgt_div_device] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
      rxagc_disable_params.rxlm_buf_idx[test_param_ptr->tgt_div_device] = test_param_ptr->tgt_drx_rxlm_buf_idx;
      rxagc_disable_params.ant_idx[test_param_ptr->tgt_div_device] = 1; // Since for Diversity device, ant idx = 1  
    }
  } /*   if (is_src == FALSE) */

  /* Disable RxAGC on Source device list*/
  else
  {
    FTM_MSG_4( FTM_HIGH, "ftm_wcdma_meas_stop_rxagc: Home CA path 0 with PRx handle %d CA path 0 DRx handle %d CA path 1 PRx handle %d CA path 1 DRx handle %d",
               test_param_ptr->src_ca_path0_prx_rxlm_buf_idx, test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx, 
               test_param_ptr->src_ca_path1_prx_rxlm_buf_idx, test_param_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx);

    /* Source PRx device can be a valid or invalid device depending on the scenario like
    IRAT, QXTA */
    if (ftm_wcdma_meas_validate_device(test_param_ptr->src_ca_path0_device) == TRUE)
    {
      /* Set the RxAGC disable params */
      rxagc_disable_params.cmd_action[test_param_ptr->src_ca_path0_device] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
      rxagc_disable_params.rxlm_buf_idx[test_param_ptr->src_ca_path0_device] = test_param_ptr->src_ca_path0_prx_rxlm_buf_idx;
      rxagc_disable_params.ant_idx[test_param_ptr->src_ca_path0_device] = 0; // Since for Primary device, ant idx = 0
      rxagc_disable_params.multi_carrier_idx = rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path0_device);
    }

    if ( (test_param_ptr->src_ca_path0_div_en == TRUE) &&
         (ftm_wcdma_meas_validate_device(test_param_ptr->src_ca_path0_div_device_id1) == TRUE)
       )
    {
      /* Set the RxAGC disable params */
      rxagc_disable_params.cmd_action[test_param_ptr->src_ca_path0_div_device_id1] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
      rxagc_disable_params.rxlm_buf_idx[test_param_ptr->src_ca_path0_div_device_id1] = test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx;
      rxagc_disable_params.ant_idx[test_param_ptr->src_ca_path0_div_device_id1] = 1; // Since for Diversity device, ant idx = 1
      rxagc_disable_params.multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path0_div_device_id1);

    }

    if (test_param_ptr->src_ca_path1_flag_enable == TRUE)
    {
      if (ftm_wcdma_meas_validate_device(test_param_ptr->src_ca_path1_device) == TRUE)
      {
        /* Set the RxAGC disable params */
        rxagc_disable_params.cmd_action[test_param_ptr->src_ca_path1_device] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
        rxagc_disable_params.rxlm_buf_idx[test_param_ptr->src_ca_path1_device] = test_param_ptr->src_ca_path1_prx_rxlm_buf_idx;
        rxagc_disable_params.ant_idx[test_param_ptr->src_ca_path1_device] = 0; // Since for Primary device, ant idx = 0
        rxagc_disable_params.multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path1_device);
      }

      if ( (test_param_ptr->src_ca_path1_div_en == TRUE) &&
           (ftm_wcdma_meas_validate_device(test_param_ptr->src_ca_path1_div_device_id1) == TRUE)
         )
      {
        /* Set the RxAGC disable params */
        rxagc_disable_params.cmd_action[test_param_ptr->src_ca_path1_div_device_id1] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
        rxagc_disable_params.rxlm_buf_idx[test_param_ptr->src_ca_path1_div_device_id1] = test_param_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx;
        rxagc_disable_params.ant_idx[test_param_ptr->src_ca_path1_div_device_id1] = 1; // Since for Diversity device, ant idx = 1
        rxagc_disable_params.multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path1_div_device_id1);
      } 
    }
  }/* else (is_src == FALSE) */

  /* Set the remaining RxAGC enable params */
  rxagc_disable_params.rf_warmup_en = FALSE;
  rxagc_disable_params.nbr_en_flag = FALSE;
  rxagc_disable_params.nbr_rxlm_buf_idx = 0;

  /* Stop RxAGC on Source or Target */
  rfwcdma_core_rxctl_enable_rxagc(&rxagc_disable_params);

  return TRUE;
}

/*===========================================================================

  FUNCTION:  ftm_wcdma_meas_tune_and_start_rxagc

===========================================================================*/
/*!
  @brief
  This function calls FED apis for Preload/Trigger to tuneaway or tuneback
  and also enable RxAGC on all the required chains
 
  @param test_param_ptr
  Parameters like device, RFLM handle etc for the chains to be enabled
 
  @param is_src
  Indicating whether to enable source tech or the target tech
  In otherwords, whether to tune away or tune back
 
  @pre
  WFW should have been enabled in desired mode before this function call
 
  @return
  Status for function failure or success
*/
/*=========================================================================*/
boolean ftm_wcdma_meas_tune_and_start_rxagc( rfm_meas_generic_params_type *test_param_ptr, boolean is_src )
{
  rflm_wcdma_rf_send_script_in_t  in_preload = {0,FALSE,RF_WCDMA_EVENT_RX_MEAS_PRELOAD};
  rflm_wcdma_rf_send_script_in_t  in_trigger = {0,FALSE,RF_WCDMA_EVENT_RX_MEAS_TRIGGER};
  rflm_wcdma_rf_send_script_out_t out_preload, out_trigger;
  rflm_err_t ret_err = RFLM_ERR_NONE;
  rfwcdma_core_rxctl_enable_rxagc_params rxagc_enable_params = {0};

  if (test_param_ptr == NULL) 
  {
    FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_tune_and_start_rxagc: Test param ptr is NULL");
    return FALSE;
  }

  memset(&(rxagc_enable_params.cmd_action), RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));


  FTM_MSG(FTM_HIGH, "inside ftm_wcdma_meas_tune_and_start_rxagc");

  in_preload.effect_time = rflm_time_read_ustmr_count();

  /* Add about 1 ms (19200) between Preload and trigger write time */
  in_trigger.effect_time = rflm_time_read_ustmr_count() + 19200;

  /* Call Preload, Trigger and enable RxAGC*/

  /* Tune away to neighbour */
  if (is_src == FALSE) 
  {
    FTM_MSG_2( FTM_HIGH, "ftm_wcdma_meas_tune_and_start_rxagc: Neighbour with PRx handle %d DRx handle %d",
               test_param_ptr->tgt_prx_rxlm_buf_idx, test_param_ptr->tgt_drx_rxlm_buf_idx);

    /* Push DM buffer to CCS for Preload event */
    ret_err |= rflm_wcdma_rf_send_script( test_param_ptr->tgt_prx_rxlm_buf_idx,
                                          &in_preload,
                                          &out_preload );

    /* Push DM buffer to CCS for Trigger/Non Trx event */
    ret_err |= rflm_wcdma_rf_send_script( test_param_ptr->tgt_prx_rxlm_buf_idx,
                                          &in_trigger,
                                          &out_trigger );


    /* Wait for Trigger to get completed */
    DALSYS_BusyWait(2000);

    /* Enable RxAGC on Target device list*/

    rxagc_enable_params.multi_carrier_idx = 1; // Target is always one carrier

    /* Target device on PRx has to a valid device */
    if (ftm_wcdma_meas_validate_device(test_param_ptr->tgt_device) == TRUE)
    {
      /* Set the RxAGC enable params */
      rxagc_enable_params.cmd_action[test_param_ptr->tgt_device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
      rxagc_enable_params.rxlm_buf_idx[test_param_ptr->tgt_device] = test_param_ptr->tgt_prx_rxlm_buf_idx;
      rxagc_enable_params.ant_idx[test_param_ptr->tgt_device] = 0; // Since for Primary device, ant idx = 0
    }

    if ( (test_param_ptr->tgt_div_en == TRUE) &&
         (ftm_wcdma_meas_validate_device(test_param_ptr->tgt_div_device) == TRUE)
       )
    {
      /* Set the RxAGC enable params */
      rxagc_enable_params.cmd_action[test_param_ptr->tgt_div_device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
      rxagc_enable_params.rxlm_buf_idx[test_param_ptr->tgt_div_device] = test_param_ptr->tgt_drx_rxlm_buf_idx;
      rxagc_enable_params.ant_idx[test_param_ptr->tgt_div_device] = 1; // Since for Diversity device, ant idx = 1
    }
  } /*   if (is_src == FALSE) */

  /* Tune back to home */
  else
  {
    FTM_MSG_4( FTM_HIGH, "ftm_wcdma_meas_tune_and_start_rxagc: Home CA path 0 with PRx handle %d CA path 0 DRx handle %d CA path 1 PRx handle %d CA path 1 DRx handle %d",
               test_param_ptr->src_ca_path0_prx_rxlm_buf_idx, test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx, 
               test_param_ptr->src_ca_path1_prx_rxlm_buf_idx, test_param_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx);

    /* Push DM buffer to CCS for Preload event */
    /* No Preload for Home */
    ret_err |= rflm_wcdma_rf_send_script( test_param_ptr->src_ca_path0_prx_rxlm_buf_idx,
                                          &in_preload,
                                          &out_preload );
    
    /* Push DM buffer to CCS for Trigger/Non Trx event */
    ret_err |= rflm_wcdma_rf_send_script( test_param_ptr->src_ca_path0_prx_rxlm_buf_idx,
                                          &in_trigger,
                                          &out_trigger );

    /* Wait for Trigger to get completed */
    DALSYS_BusyWait(2000);
    
    /* Enable RxAGC on Source device list*/

    /* Source PRx device can be a valid or invalid device depending on the scenario like
    IRAT, QXTA */
    if (ftm_wcdma_meas_validate_device(test_param_ptr->src_ca_path0_device) == TRUE)
    {
      /* Set the RxAGC enable params */
      rxagc_enable_params.cmd_action[test_param_ptr->src_ca_path0_device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
      rxagc_enable_params.rxlm_buf_idx[test_param_ptr->src_ca_path0_device] = test_param_ptr->src_ca_path0_prx_rxlm_buf_idx;
      rxagc_enable_params.ant_idx[test_param_ptr->src_ca_path0_device] = 0; // Since for Primary device, ant idx = 0
      rxagc_enable_params.multi_carrier_idx = rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path0_device);
    }

    if ( (test_param_ptr->src_ca_path0_div_en == TRUE)  &&
         (ftm_wcdma_meas_validate_device(test_param_ptr->src_ca_path0_div_device_id1) == TRUE)
       )
    {
      /* Set the RxAGC enable params */
      rxagc_enable_params.cmd_action[test_param_ptr->src_ca_path0_div_device_id1] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
      rxagc_enable_params.rxlm_buf_idx[test_param_ptr->src_ca_path0_div_device_id1] = test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx;
      rxagc_enable_params.ant_idx[test_param_ptr->src_ca_path0_div_device_id1] = 1; // Since for Diversity device, ant idx = 1
      rxagc_enable_params.multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path0_div_device_id1);
    }

    if (test_param_ptr->src_ca_path1_flag_enable == TRUE)
    {
      if (ftm_wcdma_meas_validate_device(test_param_ptr->src_ca_path1_device) == TRUE) 
      {
        /* Set the RxAGC enable params */
        rxagc_enable_params.cmd_action[test_param_ptr->src_ca_path1_device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
        rxagc_enable_params.rxlm_buf_idx[test_param_ptr->src_ca_path1_device] = test_param_ptr->src_ca_path1_prx_rxlm_buf_idx;
        rxagc_enable_params.ant_idx[test_param_ptr->src_ca_path1_device] = 0; // Since for Primary device, ant idx = 0
        rxagc_enable_params.multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path1_device);
      } 
       
      if ( (test_param_ptr->src_ca_path1_div_en == TRUE) &&
           (ftm_wcdma_meas_validate_device(test_param_ptr->src_ca_path1_div_device_id1) == TRUE)
         )
      {
        /* Set the RxAGC enable params */
        rxagc_enable_params.cmd_action[test_param_ptr->src_ca_path1_div_device_id1] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
        rxagc_enable_params.rxlm_buf_idx[test_param_ptr->src_ca_path1_div_device_id1] = test_param_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx;
        rxagc_enable_params.ant_idx[test_param_ptr->src_ca_path1_div_device_id1] = 1; // Since for Diversity device, ant idx = 1
        rxagc_enable_params.multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path1_div_device_id1);
      } 
    } /* if (test_param_ptr->src_ca_path1_flag_enable == TRUE)*/

  } /*  else (is_src == FALSE) */

  /* Set the remaining RxAGC enable params */
  rxagc_enable_params.rf_warmup_en = FALSE;
  rxagc_enable_params.nbr_en_flag = FALSE;
  rxagc_enable_params.nbr_rxlm_buf_idx = 0;

  /* Start RxAGC on Source or Target*/
  rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);


    /* Wait for Trigger to get completed */
    //DALSYS_BusyWait(2000);

    //ERR_FATAL( "********* Forced 2ms after Enable RxAGC command *********", 0, 0, 0 );

    if(ret_err != RFLM_ERR_NONE)
    {
      FTM_MSG_1( FTM_ERROR, "ftm_wcdma_meas_start: Neighbour RxLM %d", 
                 test_param_ptr->tgt_prx_rxlm_buf_idx);
      return FALSE;
    }

  return TRUE;
}
/*===========================================================================

  FUNCTION:  ftm_wcdma_meas_init_wcdma

===========================================================================*/
/*!
  @brief
  This function initialises WCDMA stack which includes
  MDSP async and sync shared memory initialisation
  and enables or starts WFW in the desired mode
 
  @details
  For G2W, enable WFW in SSIM mode. So, set FTM RF state = RFM_WCDMA_FTM_RF
  In this state, theoretically WCDMA can perform both Rx and Tx but since this
  is G2W measurements, there will be no Tx.
 
  For L2W, enable WFW in SSIM mode. But set FTM RF state = RFM_WCDMA_FTM_RXONLY
  so that WCDMA can have both Rx and Tx.
 
  For W2W, W2G and W2L scenarios, WFW is already enaled in SSIM mode with FTM RF state = RFM_WCDMA_FTM_RF.
  In this scenario, do not need to do anything.
 
  @param None
 
  @return
  Status for function failure or success
*/
/*=========================================================================*/
boolean ftm_wcdma_meas_init_wcdma( void )
{
  boolean status = FALSE; 
  l1_ext_cmd_type  *l1_cmd_ptr;
  timetick_type start_time = 0;
  rf_time_tick_type profiler_var;
  rf_time_type profiler_var_val;

  /* Send cmd to WL1 to start WCDMA mode */
  l1_cmd_ptr = l1_get_cmd_buf();
  ASSERT(l1_cmd_ptr != NULL);

  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_START_WCDMA_MODE_REQ;

  l1_put_cmd(l1_cmd_ptr);
  profiler_var = rf_time_get_tick() ; 

  /* wait for WL1 to complete */
  start_time = timetick_get();
  do
  { 
    rex_wait(FTM_WCDMA_BER_CMD_Q_SIG);
  }while((!ftm_wcdma_ber_is_rrc_cmd_start_wcdma_mode()) &&
         (timetick_get_elapsed(start_time, T_USEC) < WL1_START_STOP_WAIT_TIMEOUT)
        );

  /* check if timeout occured */
  if(timetick_get_elapsed(start_time, T_USEC) >= WL1_START_STOP_WAIT_TIMEOUT)
  {
    ERR_FATAL("ftm_wcdma_meas_start: Timeout occured while waiting for WL1!",
              0, 0, 0);
  }
 
  profiler_var_val = rf_time_get_elapsed ( profiler_var , RF_USEC ) ; 
  FTM_MSG_1( FTM_MED, "ftm_wcdma_meas_start: time spent in waiting for L1: %d",profiler_var_val) ;

  /* Clear RRC cmd Q after sending WL1 start cmd */
  ftm_wcdma_ber_clear_rrc_cmd_q();

  fws_app_enable(FW_APP_RFCMD);

  ftm_wcdma_ber_set_state(FTM_WCDMA_BER_STATE_NULL);

  return TRUE;
}
/*===========================================================================

  FUNCTION:  ftm_wcdma_meas_init_wcdma

===========================================================================*/
/*!
  @brief
  This function deinitialises WCDMA stack which includes
  MDSP async and sync shared memory de-initialisation
  and disabled WFW
 
  @param None
 
  @return
  Status for function failure or success
*/
/*=========================================================================*/
boolean ftm_wcdma_meas_deinit_wcdma( void )
{
  timetick_type start_time = 0;
  rex_tcb_type* current_tcb_ptr = rex_self();

  /* Once RxAGC is stopped, Stop WCDMA */
  fws_app_disable(FW_APP_RFCMD);

  ftm_wcdma_ber_send_CPHY_STOP_WCDMA_MODE_REQ();

  /* Wait for WL1 to complete if currently in rf or ftm task */
  if( (current_tcb_ptr == TASK_HANDLE(ftm)) || (current_tcb_ptr == TASK_HANDLE(rf)) )
  {
    start_time = timetick_get();
    do
    {
      /* If in FTM task then wait for FTM_WCDMA_BER_CMD_Q_SIG */
      if(current_tcb_ptr == TASK_HANDLE(ftm))
      {
        rex_wait(FTM_WCDMA_BER_CMD_Q_SIG);
      }
      /* If in RF task then wait for FTM_WCDMA_ENTER_EXIT_COMPLETE_SIG */
      else if (current_tcb_ptr == TASK_HANDLE(rf))
      {
        rex_wait(FTM_WCDMA_ENTER_EXIT_COMPLETE_SIG);
        (void) rex_clr_sigs(TASK_HANDLE(rf), FTM_WCDMA_ENTER_EXIT_COMPLETE_SIG);
      }
    }while((!ftm_wcdma_ber_is_rrc_cmd_stop_wcdma_mode()) &&
           (timetick_get_elapsed(start_time, T_USEC) < WL1_START_STOP_WAIT_TIMEOUT)
          );
  
    
    /* check if timeout occured */
    if(timetick_get_elapsed(start_time, T_USEC) >= WL1_START_STOP_WAIT_TIMEOUT)
    {
      ERR_FATAL("ftm_wcdma_meas_stop: Timeout occured while waiting for WL1!",
                0, 0, 0);
    }
  
    /* Clear RRC cmd Q for any pending cmds */
    ftm_wcdma_ber_clear_rrc_cmd_q();

  }

  ftm_wcdma_ber_set_state(FTM_WCDMA_BER_STATE_NULL);

  /* Reset FTM RF state */
  rfm_wcdma_set_ftm_state(RFM_WCDMA_FTM_NONE);

  return TRUE;
}

/*===========================================================================

  FUNCTION:  ftm_wcdma_meas_suspend_resume

===========================================================================*/
boolean ftm_wcdma_meas_suspend_resume(rfm_meas_generic_params_type *test_param_ptr)
{
  boolean result = TRUE;
  rfwcdma_mdsp_sync_w2x_cmd_param_type w2x_cmd;

  FTM_MSG(FTM_MED, "inside ftm_wcdma_meas_suspend_resume");

  /* Set up paramaters to suspend and resume to neighbour in W2X Idle scenario*/
  w2x_cmd.rxlm_home_buf_idx = test_param_ptr->src_ca_path0_prx_rxlm_buf_idx; // Home RxLM index for Primary
  w2x_cmd.rxlm_home_rxd_buf_idx = test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx; // Home RxLM index for Diversity
  w2x_cmd.pre_cb = NULL;
  w2x_cmd.post_cb = NULL;

  if(test_param_ptr->tgt_tech == RFCOM_GSM_MODE)
  {
    w2x_cmd.mode = MDSP_SYNC_W2X_IDLE_CMD_W2G; // W2G Scenario
  }

  else if(test_param_ptr->tgt_tech == RFCOM_LTE_MODE)
  {
    w2x_cmd.mode = MDSP_SYNC_W2X_IDLE_CMD_W2L; // W2L Scenario
  }

  else
  {
    FTM_MSG_1(FTM_ERROR, "ftm_wcdma_meas_suspend_resume: Invalid tech %d for W2X Scenario", test_param_ptr->tgt_tech);
    return FALSE;
  }

  /* Send W2X Suspend/Resume command to WFW for home cell W to suspend/resume accordingly*/
  result = rfwcdma_mdsp_sync_send_cmd( RFWCDMA_MDSP_W2X_SUSPEND_RESUME_CMD_ID, &w2x_cmd);

  DALSYS_BusyWait(10000); 
  
  FTM_MSG(FTM_HIGH, "Called W2X Suspend Resume Command");

  if(result == FALSE)
  {
    FTM_MSG(FTM_ERROR, "W2X Suspend Resume Command failed");
  }

  return result;
}

/*===========================================================================

  FUNCTION:  ftm_wcdma_meas_get_inverse_dlo_freq

===========================================================================*/
/*!
  @brief
  This function retrieves inverse of the DL frequency based on the operating channel.
 
  @param
  chan - DL channel

  @return
  Inverse of DL channel
*/
/*=========================================================================*/
uint32 ftm_wcdma_meas_get_inverse_dlo_freq(uint16 chan)
  {
  rfcom_wcdma_band_type wcdma_band = rfwcdma_core_convert_chan_to_band(chan);
  uint32 inv_wcdma_freq;

  switch(wcdma_band)
  { 
  case RFCOM_BAND_IMT:
    inv_wcdma_freq = INV_MID_FREQ_WCDMA_2100_BAND;
    break;

  case RFCOM_BAND_1900:
    inv_wcdma_freq = INV_MID_FREQ_WCDMA_1900_BAND;
    break;

  case RFCOM_BAND_BC3:
    inv_wcdma_freq = INV_MID_FREQ_WCDMA_BC3_BAND;
    break;

  case RFCOM_BAND_BC4:
    inv_wcdma_freq = INV_MID_FREQ_WCDMA_BC4_BAND;
    break;

  case RFCOM_BAND_800:
    inv_wcdma_freq = INV_MID_FREQ_WCDMA_800_BAND;
    break;

  case RFCOM_BAND_BC8:
    inv_wcdma_freq = INV_MID_FREQ_WCDMA_BC8_BAND;
    break;

  case RFCOM_BAND_BC9:
    inv_wcdma_freq = INV_MID_FREQ_WCDMA_BC9_BAND;
    break;

  default:
    inv_wcdma_freq = INV_MID_FREQ_WCDMA_2100_BAND;
    break;
  }

  return inv_wcdma_freq;
}
/*----------------------------------------------------------------------------*/
/*!
   @brief
   This function will test interfreq and interband for WCDMA to WCDMA
   by writing preload and trigger scripts for either home or neighbour
   depeding on RFLM handle and multi carrier index

    @param test_param_ptr
    All paramters that has info about source and target info in W2W
 
    @param neighborxlm_buf_idx
    Home/Neighbour WCDMA RxLM buffer Index
 
    @return
    API return status

*/
boolean ftm_wcdma_meas_wtow_tune_wcdma( rfm_meas_generic_params_type *test_param_ptr, boolean is_wcdma_neighbour )
{
  rflm_wcdma_rf_send_script_in_t  in_preload = {0,FALSE,RF_WCDMA_EVENT_RX_MEAS_PRELOAD};
  rflm_wcdma_rf_send_script_in_t  in_trigger = {0,FALSE,RF_WCDMA_EVENT_RX_MEAS_TRIGGER};
  rflm_wcdma_rf_send_script_out_t out_preload, out_trigger;
  rflm_err_t ret_err = RFLM_ERR_NONE;
  rfwcdma_mdsp_agc_action_type cmd_action_source[RFM_WCDMA_MAX_DEVICE];
  rfwcdma_mdsp_agc_action_type cmd_action_target[RFM_WCDMA_MAX_DEVICE];
  uint32 rxlm_buf_idx[RFM_WCDMA_MAX_DEVICE] = {0};
  uint32 multi_carrier_idx = 0;
  boolean tune_api_status = TRUE;

  memset(&cmd_action_source, RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));
  memset(&cmd_action_target, RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));


  FTM_MSG( FTM_HIGH, "ftm_wcdma_meas_wtow_tune_wcdma: Start");

  if (test_param_ptr == NULL) 
  {
     MSG(MSG_SSID_FTM, MSG_LEGACY_ERROR,"ftm_wcdma_meas_wtow_tune_wcdma: Test Params is NULL");
     return FALSE;
  }

  in_preload.effect_time = rflm_time_read_ustmr_count();

  /* Add about 1 ms (19200) between Preload and trigger write time */
  in_trigger.effect_time = rflm_time_read_ustmr_count() + 19200;

  if(is_wcdma_neighbour)
  {
    FTM_MSG_2( FTM_HIGH, "ftm_wcdma_meas_wtow_tune_wcdma: Neighbour with handle %d Div handle %d",
             test_param_ptr->tgt_prx_rxlm_buf_idx, test_param_ptr->tgt_drx_rxlm_buf_idx);

    /* Push DM buffer to CCS for Preload event */
    ret_err |= rflm_wcdma_rf_send_script( test_param_ptr->tgt_prx_rxlm_buf_idx,
                                          &in_preload,
                                          &out_preload );

    /* Push DM buffer to CCS for Trigger/Non Trx event */
    ret_err |= rflm_wcdma_rf_send_script( test_param_ptr->tgt_prx_rxlm_buf_idx,
                                          &in_trigger,
                                          &out_trigger );

    /* Disable RxAGC on Source device list*/
    cmd_action_source[test_param_ptr->src_ca_path0_device] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
    rxlm_buf_idx[test_param_ptr->src_ca_path0_device] =  test_param_ptr->src_ca_path0_prx_rxlm_buf_idx;
    multi_carrier_idx = rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path0_device);

    if (test_param_ptr->src_ca_path0_div_en == TRUE) 
    {
      cmd_action_source[test_param_ptr->src_ca_path0_div_device_id1] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
      rxlm_buf_idx[test_param_ptr->src_ca_path0_div_device_id1] = test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx;
      multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path0_div_device_id1);
    }

    if (test_param_ptr->src_ca_path1_flag_enable == TRUE)
    {
      cmd_action_source[test_param_ptr->src_ca_path1_device] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
      rxlm_buf_idx[test_param_ptr->src_ca_path1_device] = test_param_ptr->src_ca_path1_prx_rxlm_buf_idx;
      multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path1_device); 
      
      if (test_param_ptr->src_ca_path1_div_en == TRUE) 
      {
        cmd_action_source[test_param_ptr->src_ca_path1_div_device_id1] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
        rxlm_buf_idx[test_param_ptr->src_ca_path1_div_device_id1] = test_param_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx;
        multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path1_div_device_id1); 
      } 
    }

    /* Stop RxAGC on Source*/
    #if 0
    rfwcdma_core_rxctl_enable_rxagc(multi_carrier_idx,
                                    cmd_action_source,
                                    rxlm_buf_idx,
                                    FALSE,0,0);
                                    #endif

    /* Wait for Disable command to be completed */
    DALSYS_BusyWait(2000);

    /* Enable RxAGC on Target device list*/
    multi_carrier_idx = 1; // Target is always one carrier

    cmd_action_target[test_param_ptr->tgt_device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
    rxlm_buf_idx[test_param_ptr->tgt_device] =  test_param_ptr->tgt_prx_rxlm_buf_idx;

    if (test_param_ptr->tgt_div_en == TRUE) 
    {
      cmd_action_target[test_param_ptr->tgt_div_device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
      rxlm_buf_idx[test_param_ptr->tgt_div_device] = test_param_ptr->tgt_drx_rxlm_buf_idx;   
    }

    /* Start RxAGC on Target*/
    #if 0
    rfwcdma_core_rxctl_enable_rxagc(multi_carrier_idx,
                                    cmd_action_target,
                                    rxlm_buf_idx,
                                    FALSE,0,0);
                                    #endif
  } /*   if(is_wcdma_neighbour)*/

  else
  {
    FTM_MSG_4( FTM_HIGH, "ftm_wcdma_meas_wtow_tune_wcdma: CA path 0 Home with handle %d CA path 0 Div handle %d CA path 1 handle %d CA path 1 div handle %d",
               test_param_ptr->src_ca_path0_prx_rxlm_buf_idx, test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx, 
               test_param_ptr->src_ca_path1_prx_rxlm_buf_idx, test_param_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx);

    /* Push DM buffer to CCS for Preload event */
    /* No Preload for Home */
    ret_err |= rflm_wcdma_rf_send_script( test_param_ptr->src_ca_path0_prx_rxlm_buf_idx,
                                          &in_preload,
                                          &out_preload );
    
    /* Push DM buffer to CCS for Trigger/Non Trx event */
    ret_err |= rflm_wcdma_rf_send_script( test_param_ptr->src_ca_path0_prx_rxlm_buf_idx,
                                          &in_trigger,
                                          &out_trigger ); 


    /* Disable RxAGC on Target device list*/
    multi_carrier_idx = 1; // Target is always one carrier

    cmd_action_target[test_param_ptr->tgt_device] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
    rxlm_buf_idx[test_param_ptr->tgt_device] = test_param_ptr->tgt_prx_rxlm_buf_idx;

    if (test_param_ptr->tgt_div_en == TRUE) 
    {
      cmd_action_target[test_param_ptr->tgt_div_device] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
      rxlm_buf_idx[test_param_ptr->tgt_div_device] = test_param_ptr->tgt_drx_rxlm_buf_idx;   
    }

    /* Stop RxAGC on Target */
    #if 0
    rfwcdma_core_rxctl_enable_rxagc(multi_carrier_idx,
                                    cmd_action_target,
                                    rxlm_buf_idx,
                                    FALSE,0,0);
                                    #endif


    /* Wait for Disable command to be completed */
    DALSYS_BusyWait(2000);

    /* Enable RxAGC on Source device list*/
    cmd_action_source[test_param_ptr->src_ca_path0_device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
    rxlm_buf_idx[test_param_ptr->src_ca_path0_device] =  test_param_ptr->src_ca_path0_prx_rxlm_buf_idx;
    multi_carrier_idx = rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path0_device);

    if (test_param_ptr->src_ca_path0_div_en == TRUE) 
    {
      cmd_action_source[test_param_ptr->src_ca_path0_div_device_id1] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
      rxlm_buf_idx[test_param_ptr->src_ca_path0_div_device_id1] = test_param_ptr->src_ca_path0_drx_device_1_rxlm_buf_idx;
      multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path0_div_device_id1);
    }

    if (test_param_ptr->src_ca_path1_flag_enable == TRUE)
    {
      cmd_action_source[test_param_ptr->src_ca_path1_device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
      rxlm_buf_idx[test_param_ptr->src_ca_path1_device] = test_param_ptr->src_ca_path1_prx_rxlm_buf_idx;
      multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path1_device); 
      
      if (test_param_ptr->src_ca_path1_div_en == TRUE) 
      {
        cmd_action_source[test_param_ptr->src_ca_path1_div_device_id1] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
        rxlm_buf_idx[test_param_ptr->src_ca_path1_div_device_id1] = test_param_ptr->src_ca_path1_drx_device_1_rxlm_buf_idx;
        multi_carrier_idx |= rfwcdma_data_get_carrier_mask(test_param_ptr->src_ca_path1_div_device_id1); 
      } 
    }

    /* Start RxAGC on Source */
    #if 0
    rfwcdma_core_rxctl_enable_rxagc(multi_carrier_idx,
                                    cmd_action_source,
                                    rxlm_buf_idx,
                                    FALSE,0,0);
                                    #endif

  } /* else(is_wcdma_neighbour)*/

  if(ret_err != RFLM_ERR_NONE)
  {
    FTM_MSG_2( FTM_ERROR, "ftm_wcdma_meas_wtow_tune_wcdma: Failed for Home RxLM %d and Neighbour RxLM %d", 
               test_param_ptr->src_ca_path0_prx_rxlm_buf_idx, test_param_ptr->tgt_prx_rxlm_buf_idx);
    tune_api_status = FALSE;
  }

  return tune_api_status;
}
rfm_meas_tech_setup_params_type ftm_wcdma_meas_get_wcdma_param(rfm_meas_generic_params_type *test_param_ptr, boolean is_src)
{
  rfm_meas_tech_setup_params_type wcdma_tech_setup_param = {0};

  /* Parameter Check */
  if(test_param_ptr == NULL)
  {
    FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_get_wcdma_param: Input params is NULL");
    return wcdma_tech_setup_param;
  }

  if( (test_param_ptr->src_tech != RFCOM_WCDMA_MODE) && (test_param_ptr->tgt_tech != RFCOM_WCDMA_MODE) )
  {
    FTM_MSG_2( FTM_ERROR, "ftm_wcdma_meas_get_wcdma_param: Source tech %d or Target tech %d is not WCDMA",
               test_param_ptr->src_tech, test_param_ptr->tgt_tech );
    return wcdma_tech_setup_param;
  }

  /* Update WCDMA channel and carrier type information based on carrier path
   and number of channels on each carrier path */
  if(is_src == TRUE)
  {
    /* Primary CA path */
    if (test_param_ptr->src_ca_path0_car_type == 0) 
    {
      FTM_MSG( FTM_MED, "ftm_wcdma_meas_get_wcdma_param: CA path 0, W Source Carr type is Single carrier");
      wcdma_tech_setup_param.wcdma_params.carrier_types[RFM_MEAS_PCELL] = RFCOM_SINGLE_CARRIER;
      wcdma_tech_setup_param.wcdma_params.channels[RFM_MEAS_PCELL][RFCOM_SINGLE_CARRIER] = test_param_ptr->src_ca_path0_chan_car0;
    }

    else if (test_param_ptr->src_ca_path0_car_type == 1) 
    {
      wcdma_tech_setup_param.wcdma_params.carrier_types[RFM_MEAS_PCELL] = RFCOM_DUAL_CARRIER;
      wcdma_tech_setup_param.wcdma_params.channels[RFM_MEAS_PCELL][RFCOM_SINGLE_CARRIER] = test_param_ptr->src_ca_path0_chan_car0;
      wcdma_tech_setup_param.wcdma_params.channels[RFM_MEAS_PCELL][RFCOM_DUAL_CARRIER] = test_param_ptr->src_ca_path0_chan_car1;
      FTM_MSG( FTM_MED, "ftm_wcdma_meas_get_wcdma_param: CA path 0, W Source Carr type is Dual carrier");
    }

    else
    {
      wcdma_tech_setup_param.wcdma_params.carrier_types[RFM_MEAS_PCELL] = RFCOM_MAX_CARRIERS;
      FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_get_wcdma_param: CA path 0, Invalid Carrier type for W Source");
    }

    /* Secondary CA path for source only  */
    if (test_param_ptr->src_ca_path1_flag_enable == TRUE)
    {
      if (test_param_ptr->src_ca_path1_car_type == 0) 
      {
        FTM_MSG( FTM_MED, "ftm_wcdma_meas_get_wcdma_param: CA path 1, W Source Carr type is Single carrier");
        wcdma_tech_setup_param.wcdma_params.carrier_types[RFM_MEAS_SCELL_1] = RFCOM_SINGLE_CARRIER;
        wcdma_tech_setup_param.wcdma_params.channels[RFM_MEAS_SCELL_1][RFCOM_SINGLE_CARRIER] = test_param_ptr->src_ca_path1_chan_car0;
      }

      else if (test_param_ptr->src_ca_path1_car_type == 1) 
      {
        wcdma_tech_setup_param.wcdma_params.carrier_types[RFM_MEAS_SCELL_1] = RFCOM_DUAL_CARRIER;
        wcdma_tech_setup_param.wcdma_params.channels[RFM_MEAS_SCELL_1][RFCOM_SINGLE_CARRIER] = test_param_ptr->src_ca_path1_chan_car0;
        wcdma_tech_setup_param.wcdma_params.channels[RFM_MEAS_SCELL_1][RFCOM_DUAL_CARRIER] = test_param_ptr->src_ca_path1_chan_car1;
        FTM_MSG( FTM_MED, "ftm_wcdma_meas_get_wcdma_param: CA path 1, W Source Carr type is Dual carrier");
      }

      else
      {
        wcdma_tech_setup_param.wcdma_params.carrier_types[RFM_MEAS_SCELL_1] = RFCOM_MAX_CARRIERS;
        FTM_MSG( FTM_ERROR, "ftm_wcdma_meas_get_wcdma_param: CA path 1, Invalid Carrier type for W Source");
      }
    }/* if (test_param_ptr->src_ca_path1_flag_enable == TRUE)*/
 
  } /*   if(is_src == TRUE)*/

  /* Target is always single carrier, CA path 0 */
  else
  {
    wcdma_tech_setup_param.wcdma_params.carrier_types[RFM_MEAS_PCELL] = RFCOM_SINGLE_CARRIER; 
    wcdma_tech_setup_param.wcdma_params.channels[RFM_MEAS_PCELL][RFCOM_SINGLE_CARRIER] = test_param_ptr->tgt_chan;
  } /* else (is_src == TRUE)*/

  return (wcdma_tech_setup_param);
}/* ftm_wcdma_meas_get_wcdma_param*/


