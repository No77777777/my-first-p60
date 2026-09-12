#ifndef RFDEVICE_TRX_COMMON_INTF_CLASS
#define RFDEVICE_TRX_COMMON_INTF_CLASS
/*!
  @file rfdevice_trx_common_intf_class.h 

  @brief
  This file defines the RF device transceiver common interface class and its APIs

*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/rfdevice_interface.mpss/2.11/api/rfdevice_trx_common_intf_class.h#1 $

when       who     what, where, why
-------------------------------------------------------------------------------- 
08/26/15   shb     Renamed RFDEVICE_TRX_MAX_VCO_PER_ILNA_PORT to 
                   RFDEVICE_MAX_TRX_DEVICE_PER_SIG_PATH 
08/17/15   shb     Added get_vco_info() and get_tech_path_data() APIs
08/07/15   shb     Initial revision.


===========================================================================*/

#include "rfdevice_class.h"
#include "rfdevice_cmn_type_defs.h"

#ifdef __cplusplus

class rfdevice_trx_common_intf_class:public rfdevice_class
{
  public:
  rfdevice_trx_common_intf_class();

  virtual ~rfdevice_trx_common_intf_class();

  virtual const rfdevice_trx_vco_info_type* get_vco_info(uint8& vco_info_tbl_size) = 0;

  virtual boolean get_tech_path_data(rfdevice_trx_tech_type tech,
                                     rfdevice_rxtx_enum_type rxtx,
                                     uint32 sig_path,
                                     const int32* rfc_data,
                                     uint8 rfc_data_size,
                                     rfdevice_trx_path_data_type (&trx_path_data)[RFDEVICE_MAX_TRX_DEVICE_PER_SIG_PATH],
                                     uint8& trx_path_data_size) = 0; 

};

#else 

struct rfdevice_trx_common_intf_struct;
typedef struct rfdevice_trx_common_intf_struct rfdevice_trx_common_intf_class;

#endif  /* #ifdef __cplusplus */

#endif /* #ifndef RFDEVICE_TRX_COMMON_INTF_CLASS */
