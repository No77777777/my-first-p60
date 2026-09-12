/*!
  @file rflte_handle_spur_db.h

  @brief
  This file contains all declarations and definitions necessary for Handling Spurs in
  the RXLM.

  @details

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

$Header: //components/rel/rftech_lte.mpss/3.11/rf/util/inc/rflte_handle_spur_db.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/16/15   stw     Proper Name used for Spur DB
11/13/15   stw     Spur Filter by BW and Reordering by RSSI threshold added
10/28/15   stw     Initial Check-in
===========================================================================*/

#ifndef RFLTE_MC_SPUR_DB_H
#define RFLTE_MC_SPUR_DB_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "rfcom.h"
#include "rfdevice_lte_type_defs.h"
#include "rfm_device_types.h"
#include "rflm_lte.h"
#include "rflte_mc_spur.h"
#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/


/*----------------------------------------------------------------------------*/
void rflte_handle_spur_db_create(rflm_lte_spur_db_type* spur_db);

/*----------------------------------------------------------------------------*/
boolean rflte_handle_spur_db_destroy(rflm_lte_spur_db_type* spur_db);

/*----------------------------------------------------------------------------*/
void rflte_handle_spur_db_append_spur(rflte_mc_spur_data_type const* spur_info,rflm_lte_spur_db_type* spur_db);

/*----------------------------------------------------------------------------*/
void rflte_handle_spur_db_device_vote_down(rfm_device_enum_type device, rflm_lte_spur_db_type* spur_db);

/*----------------------------------------------------------------------------*/
void rflte_handle_spur_db_append_replace(rflte_mc_spur_data_type const* spur_list,uint32 spur_list_size,rflm_lte_spur_db_type* spur_db);

/*----------------------------------------------------------------------------*/
void rflte_handle_spur_db_clean(rflm_lte_spur_db_type* spur_db);

/*----------------------------------------------------------------------------*/
void rflte_handle_spur_db_filter_reorder(rflm_lte_spur_db_type* spur_db,uint64 lo_freq_hz, uint64 bw_hz);

/*----------------------------------------------------------------------------*/
boolean rflte_handle_spur_db_is_changed(rflm_lte_spur_db_type* spur_db, rflm_lte_notch_db_type* notch_db);

#ifdef __cplusplus
}
#endif

#endif /* RFLTE_MC_SPUR_DB_H */


