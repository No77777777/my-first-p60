/*!
  @file
  rf_tdscdma_core_txctl.c

  @brief
  Provides TX control. Example PA ctl is handled here.

  @details


*/

/*===========================================================================

Copyright (c) 2011 - 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:09:33 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_tdscdma.mpss/3.11/rf/core/src/rf_tdscdma_core_txctl.c#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/22/13   jyu     Merged from NikeL
04/24/12   jfc     Remove unused headers
05/27/11   jhe     Initial version

============================================================================*/

#include "rf_tdscdma.h"
#include "rfnv_tdscdma.h"
#include "rf_tdscdma_mdsp.h"
#include "rf_tdscdma_core_txctl.h"
#include "rf_tdscdma_core_txlin.h"
#include "rf_tdscdma_core_util.h"
#include "rf_tdscdma_mdsp.h"
#include "ftm.h"
#include "rflm_tds_mc.h"
#include "rfcommon_math.h"


#if 0
// it is included in TxLM static, to delete
/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function enables/disables the PA_ON enable bit for TDSCDMA Tx.

*/
void rf_tdscdma_core_txctl_set_tx_modulator(boolean enable)
{
  /* This should not be here */
  hw_pa_on_enable(enable);
}
#endif



//TODO - for below 2 funct, will remove and let the caller call mdsp api directly

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function freeze/unfreezes the TX accumulator.

  @details
*/
int16 rf_tdscdma_core_txctl_set_tx_freezeaccum(uint8 val)
{
  //rf_tdscdma_mdsp_set_txfreezeaccum(val);

  return 0;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function enables UL closed loop power control by removing the TX AGC
  gain value override.

  @details
*/
void rf_tdscdma_core_txctl_enable_pwr_ctrl(void)
{
  //rf_tdscdma_mdsp_enable_pwr_ctrl();
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function calculates the PA compensation value and populates it to rflm 

  @details

  @param band
  TDSCDMA band 
 
  @param tx_chan
  Tx chan 

*/
void rf_tdscdma_core_calc_pa_comp_vs_freq 
(
  rfcom_tdscdma_band_type band,
  uint16 tx_chan
)
{
  int8 pa_idx = 0;
  int8 freq_idx = 0;
  uint32 tx_freq = 0;
  tx_band_cal_type *tx_band_cal_data = rf_tdscdma_core_txlin_get_band_data(band);
  int16 pa_comp_vs_freq_up[RFCOM_TXLIN_NUM_PA_STATES] = {0};
  int16 pa_comp_vs_freq_dn[RFCOM_TXLIN_NUM_PA_STATES] = {0};
  int16 pa_comp_vs_freq_array[RFCOM_TXLIN_NUM_CHANNELS] = {0};
  
  /* NULL pointer checks */
  if ( tx_band_cal_data == NULL )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rf_tdscdma_core_calc_pa_comp_vs_freq: null pointer!", 0);
    return;
  }

  tx_freq = rf_tdscdma_core_util_get_freq_from_chan(tx_chan);

  /* Process pa_comp_vs_freq NV for R1,R2,R3,R4,R5,R6,R7 */
  for(pa_idx=1; pa_idx<RFCOM_TXLIN_NUM_PA_STATES; pa_idx++)
  {
    /* Extract and interpolate for pa_comp_vs_freq_up */
    for(freq_idx=0; freq_idx<RFCOM_TXLIN_NUM_CHANNELS; freq_idx++)
    {
      pa_comp_vs_freq_array[freq_idx] = rfnv_tdscdma_tx_tbl_ptr->pa_static.pa_comp_up_vs_freq[freq_idx][pa_idx];
    }
    pa_comp_vs_freq_up[pa_idx] = rf_lininterp_u32_s16( &(tx_band_cal_data->tx_cal_freq[0]),
                                                        pa_comp_vs_freq_array,
                                                        tx_band_cal_data->tx_cal_chan_size,
                                                        tx_freq );
                                                        
    /* Add PA comp freq comp to base PA comp value */
    pa_comp_vs_freq_up[pa_idx] += rfnv_tdscdma_tx_tbl_ptr->pa_static.pa_comp_up[pa_idx];

    /* Extract and interpolate for pa_comp_vs_freq_down */
    for(freq_idx=0; freq_idx<RFCOM_TXLIN_NUM_CHANNELS; freq_idx++)
    {
      pa_comp_vs_freq_array[freq_idx] = rfnv_tdscdma_tx_tbl_ptr->pa_static.pa_comp_dn_vs_freq[freq_idx][pa_idx];
    }
    pa_comp_vs_freq_dn[pa_idx] = rf_lininterp_u32_s16( &(tx_band_cal_data->tx_cal_freq[0]),
                                                        pa_comp_vs_freq_array,
                                                        tx_band_cal_data->tx_cal_chan_size,
                                                        tx_freq );
                                                        
    /* Add PA comp freq comp to base PA comp value */
    pa_comp_vs_freq_dn[pa_idx] += rfnv_tdscdma_tx_tbl_ptr->pa_static.pa_comp_dn[pa_idx];
  }                         

  /* update value to rflm */
  rflm_tds_set_pa_comp(pa_comp_vs_freq_up, pa_comp_vs_freq_dn);
}

