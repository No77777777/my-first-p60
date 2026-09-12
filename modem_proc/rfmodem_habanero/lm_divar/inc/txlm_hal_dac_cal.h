#ifndef TXLM_HAL_LOCAL_H
#define TXLM_HAL_LOCAL_H

/*!
   @file
   txlm_hal_dac_cal.h

   @brief
   This file implements the TXLM MDM9K HAL for DAC calibration functionality. These are platform
   dependent.

*/

/*===========================================================================

Copyright (c) 2010 - 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/04/06 02:22:25 $ $Author: mplcsds1 $
$Header: //components/rel/rfmodem_habanero.mpss/1.11/lm_divar/inc/txlm_hal_dac_cal.h#1 $

when       who     what, where, why
-------------------------------------------------------------------------------
12/18/13   ljl      Initial version.
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#ifdef _cplusplus
extern "C" {
#endif

#include "comdef.h"
#include "err.h"

#include "txlm_intf.h"

// need rfcommon_msm_dac_cal_data_type for DAC cal related APIs
#include "rfcommon_msm_cal.h"


lm_status_type txlm_hal_setup_dac_cal(rfcommon_msm_dac_cal_data_type dac_cal_type,txlm_chain_type chain);

lm_status_type txlm_hal_trigger_dac_cal(rfcommon_msm_dac_cal_data_type dac_cal_type,txlm_chain_type chain);

lm_status_type txlm_hal_status_all_done_dac_cal(rfcommon_msm_dac_cal_data_type dac_cal_type,txlm_chain_type chain,uint32 * data);

lm_status_type txlm_hal_regarray_rd_dac_cal(rfcommon_msm_dac_cal_data_type dac_cal_type,txlm_chain_type chain);

lm_status_type txlm_hal_status_regarray_rd_done_dac_cal(rfcommon_msm_dac_cal_data_type dac_cal_type,txlm_chain_type chain,uint32 * data);

lm_status_type txlm_hal_extract_dac_cal_results(rfcommon_msm_dac_cal_read_data_type dac_cal_read_type,txlm_chain_type chain,uint32 mask,uint8 size,uint32 * data);

lm_status_type txlm_hal_exit_dac_cal(txlm_chain_type chain,rfcommon_msm_dac_cal_data_type dac_cal_type);

/*==============================================================
  Reference API for DAC cal result loading during mission mode DAC bringup
===============================================================*/
void rflm_dtr_tx_dac_load_regarray_msb_opt
(txlm_chain_type chain, uint32 mask, uint8 size, uint32* data);
void rflm_dtr_tx_txdac_load_regarray_dc_opt
(txlm_chain_type chain, uint32 mask, uint32* data);
void rflm_dtr_tx_txdac_power_up_opt
(txlm_chain_type chain, uint32* txdac_msb_data, uint32* txdac_dc_data);
void rflm_dtr_tx_etdac_power_up_opt
(txlm_chain_type chain, uint32* etdac_msb_data, uint32* etdac_rpoly_data);


#ifdef _cplusplus
}
#endif

#endif /* TXLM_HAL_H */

