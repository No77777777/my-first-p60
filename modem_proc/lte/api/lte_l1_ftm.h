/*!
  @file
  lte_l1_ftm.h

  Interface for FTM to use L1 to tune

*/

/*===========================================================================

  Copyright (c) 2008 - 2011 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/16/14   tmai    Added support for L2W RF testing

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "lte_as.h"
#include "rfm_device_types.h"
#include "rfm_meas.h"
#include "wl1x2wsrchapi.h"
#ifndef _LTE_ML1_COMMON_FTM_H_
#define _LTE_ML1_COMMON_FTM_H_
/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/
/*! Maximum detected cells per freq is 32*/
#define LTE_ML1_MD_WCDMA_MAX_PSC_PER_FREQ  32

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/
/* Bitmask value for assigning each client a bit */

/*----------------------------------------------------------------------------*/
/*! @brief: Information pertaining to each WCDMA UARFCN. Gives the number
of detected cells in the frequency */
typedef struct
{
  /*! Frequency */
  uint16                              freq;

  /*! Number of peaks found after the NASTT search Max would be 32*/
  uint16                              num_psc;

  /*! Results associated with each PSC  */
  wl1_l2w_cell_info_s                 psc_info[L2W_MAX_PSC_PER_FREQ];

  /* W frequency used in rf enter and build script */
  uint16 tuned_w_freq;  

  /* Unique transaction id for this sequence */
  uint32                           l2w_trm_transaction_id;

  /*! Whether TRM has been requested for this measurement */
  boolean trm_requested;

  /* WL1 struct for the freq_info */
  wl1_l2w_freq_info_s                 l2w_freq_info;

  /* W RF device for L2W measurment */
  rfm_device_enum_type wcdma_device;

#ifdef FEATURE_RXLM
  lm_handle_type wcdma_lm_handle;
#endif

  /*! Common RF parameters */
  rfm_meas_common_param_type *rf_common_param;

  /* l2w_handle for RXLM buffer index */
  lm_handle_type l2w_handle;
}lte_ml1_md_wcdma_freq_info_s;

/*===========================================================================

                         LOCAL VARIABLES

===========================================================================*/

/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================

                             INTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================

  FUNCTION:  lte_l1_ftm_tune
===========================================================================*/
/*!
    @brief 
    LTE L1 will tune, using the FW via scripts.

    The L1 FTM module must be initialized via lte_l1_ftm_init() prior to 
    calling this function.  FW will capture samples during this process
    which can be dumped and extracted for sanity purposes.

    All other RATs must release the FW resources required for IRAT
    prior to calling this function.  The RF and clocks will be in LTE
    mode following the execution of this function.

    The caller of this function must have the scripts prepared in 
    the RF tune buffer.

    Multiple lte_l1_ftm_tune() calls can happen between init and deinit.
    
*/
/*=========================================================================*/
void lte_l1_ftm_tune
(
  /*! LTE Downlink center frequency to tune to */
  lte_earfcn_t     earfcn,
  /*! RF tune script index */
  uint8            rf_tune_script_index,
  /*! Rxlm buffer for antenna 0, ignored for non-rxlm targets */
  uint8            rxlm_buf_ant0,
  /*! Rxlm buffer for antenna 1, ignored for non-rxlm targets */
  uint8            rxlm_buf_ant1
);

/*===========================================================================

  FUNCTION:  lte_l1_ftm_init
===========================================================================*/
/*!
    @brief 
    Initializes the L1 FTM module.

    Init must be called prior to tuning via lte_l1_ftm_tune().  
    lte_l1_ftm_deinit() must be called prior to calling lte_l1_ftm_init()
    again.  
    
    On genesis init will configure some LTE-only clocks, these should not
    be modified prior to lte_l1_ftm_deinit() being called
*/
/*=========================================================================*/
void lte_l1_ftm_init( void );

/*===========================================================================

  FUNCTION:  lte_l1_ftm_deinit
===========================================================================*/
/*!
    @brief 
    Deinitializes the L1 FTM module.

    The LTE app will be disabled after this function is called and 
    all LTE resource will be freed.
*/
/*=========================================================================*/
void lte_l1_deinit_req( void );

/*===========================================================================

  FUNCTION:   lte_l1_ftm_x2w_start_wcdma

===========================================================================*/
/*!
    @brief
    This function call Full window search method for performing
    searches in connected mode
 
    @return None


*/
/*=========================================================================*/
void lte_l1_ftm_l2w_start_and_tune_wcdma
(
  lte_mem_instance_type instance,
  /*! Cells for which measurements need to be made. */
  lte_ml1_md_wcdma_freq_info_s  *freq_cell_list_ptr
);

/*===========================================================================

  FUNCTION: lte_l1_ftm_l2w_stop_wcdma

===========================================================================*/
/*!
    @brief
    To stop WCDMA FW
 
    @return
    None

*/
/*=========================================================================*/
void lte_l1_ftm_l2w_stop_wcdma
(
  lte_mem_instance_type instance
);
/*===========================================================================

  FUNCTION: lte_l1_ftm_l2w_config_init

===========================================================================*/
/*!
    @brief
    Init L2W MDSP interface
 
    @details
    This is the first API that needd to be called to Initialize
    L2W MDSP interface before anything is called in L2W
 
    @param
    None
 
    @return
    None

*/
/*=========================================================================*/
void lte_l1_ftm_l2w_config_init
(
  void
);
/*===========================================================================

  FUNCTION: lte_l1_ftm_l2w_config_deinit

===========================================================================*/
/*!
    @brief
    Deinit L2W Config and cleanup
 
    @details
    This is the last API to be called after WFW Stop. This will deinitialise
    and cleanup everything
 
    @param
    None
 
    @return
    None

*/
/*=========================================================================*/
void lte_l1_ftm_l2w_config_deinit
(
  void
);

#endif

