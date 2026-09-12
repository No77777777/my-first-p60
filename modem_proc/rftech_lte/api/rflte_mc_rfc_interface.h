/*!
  @file rflte_mc_rfc_interface.h

  @brief
  This file contains all declarations and definitions necessary to use
  between LTE Main Control and RFC.

  @details

*/

/*===========================================================================

  Copyright (c) 2009 - 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/rftech_lte.mpss/3.11/api/rflte_mc_rfc_interface.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/01/15   am      carrier index added
10/19/15   stw     Initial Check-in
===========================================================================*/

#ifndef RFLTE_MC_RFC_INTERFACE_H
#define RFLTE_MC_RFC_INTERFACE_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "rfcom.h"
#include "rfc_common.h"
#include "rfdevice_lte_type_defs.h"
#include "rfm_device_types.h"
#include "rflte_msg.h"


#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/* Max number of split bands */
#define RFLTE_MC_MAX_NUM_SPLITS       3

typedef struct
{
  /*!> Carrier to tune to*/
  uint32 carrier_idx;
  /*!> Band to tune to*/
  rfcom_lte_band_type     band;
  /*!> Channel to tune to*/
  rfcom_lte_earfcn_type   channel;
  /*!> BW to use for tunning*/
  rfcom_lte_bw_type       bw;
  /*!> device to tune*/
  rfcom_device_enum_type  device;  
  /*!> alternate path : Used in Online Mode*/
  uint8              alternate_path;
  /*!> path override : Used in Cal Mode*/
  uint8 path_override_index;
  /*!> device pointer associated with data*/
  void* device_ptr;
}rflte_mc_band_decision_config_params_s_type;

typedef struct
{
  /*!> Full Band*/
  rfcom_lte_band_type     full_band;
  /*!> List Index Associated with full_band*/
  uint32 associated_list_indices[RFA_RF_LTE_MAX_CELL_SUPPORTED];
  /*!> List Index Associated with full_band*/
  uint32 size_list_indices;
}rflte_mc_band_index_list_s_type;

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* RFLTE_MC_RFC_INTERFACE_H */


