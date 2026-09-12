#ifndef RFGSM_MC_TX_H
#define RFGSM_MC_TX_H

/*!
   @file
   rfgsm_mc_tx.h

   @brief


   @details
   
*/

/*===========================================================================
Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:10:17 $
$Header: //components/rel/rftech_gsm.mpss/3.11/api/rfgsm_mc_tx.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
05/28/15   sc      Enable thermistor and vbatt reads
03/12/15   sc      Add Tx Wakeup/Sleep MC interface
03/12/15   sc      Initial version.

============================================================================*/
#include "rfcom.h"
#include "rfgsm_mc.h"
#include "rfm_device_types.h"
#include "rfgsm_msg.h"
#include "rfgsm_core_types.h"



/*----------------------------------------------------------------------------*/
extern rfgsm_mc_status_type rfgsm_mc_tx_wakeup( uint8 sub_id, 
                                                rfcom_device_enum_type tx_device,
                                                uint32 txlm_buf_index,
                                                void * gfw_buff_ptr,
                                                void * therm_read_buff_ptr,
                                                rfgsm_timing_info_type * timing_info );

/*----------------------------------------------------------------------------*/
extern rfgsm_mc_status_type rfgsm_mc_tx_sleep( uint8 sub_id,
                                               rfcom_device_enum_type tx_device );


/*----------------------------------------------------------------------------*/
extern rfgsm_mc_status_type rfgsm_mc_process_therm_read_rsp( rfgsm_core_data_type *core_data, 
                                                             uint8 tag, 
                                                             boolean therm_read_ok );

/*----------------------------------------------------------------------------*/
extern rfgsm_mc_status_type rfgsm_mc_stop_therm_read( rfm_device_enum_type tx_device );

#endif /* RFGSM_MC_TX_H */
