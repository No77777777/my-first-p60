/*!
  @file rflte_mc_spur.h

  @brief
  This file contains all declarations and definitions necessary for Handling Spurs in
  the LTE Main Control.

  @details

*/

/*===========================================================================

  Copyright (c) 2015-2020 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/rftech_lte.mpss/3.11/api/rflte_mc_spur.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/24/20   ssl     [CR2708896]Creating supr_db per sub_id
10/28/15   stw     Initial Check-in
===========================================================================*/

#ifndef RFLTE_MC_SPUR_H
#define RFLTE_MC_SPUR_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "rfcom.h"
#include "rfdevice_lte_type_defs.h"
#include "rfm_device_types.h"
#include "rflm_lte.h"
#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*Macro to store spurs in Spur DB. Maximum number of spurs can be more than number of Notch HW*/
#define RFLTE_MC_MAX_NUM_SPURS    8


/*-----------------------------------------------------------------------*/
/*!
  @brief
  This enum denotes the rflm_lte_spur_mitigated_type. Used for storing Spurs in Spur DB
  
*/
typedef struct
{
   /*! Spur mitigated type */
  rflm_lte_spur_mitigated_type  spur_type;

  /*! Spur freq*/
  uint32 notch_application_freq;

  /*! Notch depth*/
  uint32 notch_depth;

  /*! RSSI Threshold in dB10*/
  int32 spur_thresh;

  /*! Number of valid associated devices*/
  uint32 num_associated_devices;

  /*!Associated Devices that cause this spur*/
  rfm_device_enum_type associated_devices[RFLM_ASSOCIATED_DEVICES_PER_SPUR];
}rflte_mc_spur_data_type;

/*-----------------------------------------------------------------------*/
/*!
  @brief
  This enum denotes the rflte_mc_spur_per_device_data_type. Used for storing Spurs per device in Spur DB
  
*/
typedef struct
{
   /*! Spur Data for device*/
   rflte_mc_spur_data_type spur_data[RFLTE_MC_MAX_NUM_SPURS];
   /*! Number of valid spurs*/
   uint32 num_valid_spurs;
   /*! Spur Data per device*/
   rfm_device_enum_type device;
}rflte_mc_spur_per_device_data_type;

typedef struct _rflte_mc_spur_database_type rflte_mc_spur_database_type;

/*----------------------------------------------------------------------------*/

rflte_mc_spur_database_type* rflte_mc_spur_db_create(uint8 sub_id);

/*----------------------------------------------------------------------------*/
boolean rflte_mc_spur_db_destroy(rflte_mc_spur_database_type* spur_db);

/*----------------------------------------------------------------------------*/
void rflte_mc_spur_db_add(rflte_mc_spur_database_type* spur_db,rfm_device_enum_type device,rflte_mc_spur_data_type* spur_data);

/*----------------------------------------------------------------------------*/
rflte_mc_spur_per_device_data_type const* rflte_mc_spur_db_query(rflte_mc_spur_database_type* spur_db,rfm_device_enum_type device);

#ifdef __cplusplus
}
#endif

#endif /* RFLTE_MC_SPUR_H */


