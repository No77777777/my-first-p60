#ifndef RF_TDSCDMA_CORE_TXCTL_H
#define RF_TDSCDMA_CORE_TXCTL_H

/*!
   @file
   rf_tdscdma_core_txctl.h

   @brief
   Provides TX control. Example PA ctl is handled here.

   @details


*/

/*===========================================================================

Copyright (c) 2011, 2013, 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:09:33 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_tdscdma.mpss/3.11/rf/core/inc/rf_tdscdma_core_txctl.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/14   jyu     Added support for therm read 
02/22/13   jyu     Merged from NikeL 
05/27/11   jhe     Initial version

============================================================================*/
#include "comdef.h"
#include "rfcom.h"


/*----------------------------------------------------------------------------*/
void rf_tdscdma_core_txctl_enable_pwr_ctrl(void);

/*----------------------------------------------------------------------------*/
int16 rf_tdscdma_core_txctl_set_tx_freezeaccum(uint8 val);

/*----------------------------------------------------------------------------*/
void rf_tdscdma_core_temp_comp_timer(int4 t_unused);

/*----------------------------------------------------------------------------*/
void rf_tdscdma_core_calc_pa_comp_vs_freq 
(
  rfcom_tdscdma_band_type band,
  uint16 tx_chan
);

#endif /* RF_TDSCDMA_CORE_TXCTL_H */
