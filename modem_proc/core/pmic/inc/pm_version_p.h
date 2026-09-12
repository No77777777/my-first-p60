#ifndef __PM_VERSION_P_H__
#define __PM_VERSION_P_H__

/** @file pm_version.h 
 *  
 *  
 *   This header file contains enums and API definitions for PMIC version 
 *   detection.
*/
/*
 *  Copyright (c) 2010-2015 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
/* ======================================================================= */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/pmic/inc/pm_version_p.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/14/13   kt      Adding pm_get_pbs_info API to get the pmic device pbs info.  
01/28/13   kt      Adding pm_get_pmic_info API to get the pmic device info.  
01/28/13   kt      Removing pm_set_hardware_version and pm_get_hardware_version APIs.  
01/10/13   kt      Changing the file as per Henry's Phase1 changes.  
03/10/12   wra     Removed old PMIC versions and added Badger PMIC versions. 
                    Reduced hardware versions in enumeration
10/18/11   jtn/mpt Add PM8821 and PM8038
04/04/11   hw      Added pm_get_hardware_version and pm_set_hardware_version API
02/08/11   hw      Merging changes from the PMIC Distributed Driver Arch branch
07/05/10   wra     Added pm_model_type entry for the PM8921 and PM8018
07/05/10   wra     Added file header and pm_model_type entry for the PM8901 
                   and ISL9519
=============================================================================*/
/*===========================================================================

                        HEADER FILES

===========================================================================*/
#include "com_dtypes.h"
#include "pm_err_flags.h"
#include "pm_version.h"
/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
/** @addtogroup pm_version
@{ */

/** 
  Stores the PMIC's model type value, the all layer
  revision number, and the metal revision number. See
  #pm_model_type for more information as to which PMIC
  model type value represents which PMIC. For example, 
  for PM8x26 v2.1, the PMIC model type value is 4 
  (since the PM8x26 model type value is 4, as per pm_model_type), 
  the all layer revision number is 2, and the metal revision number is 1.

 */
typedef struct
{
  pm_model_type  ePmicModel;             /**< PMIC device model type. */
  uint32         nPmicAllLayerRevision;  /**< PMIC device all layer revision number. */
  uint32         nPmicMetalRevision;     /**< PMIC device metal revision number. */
} pm_device_info_type;

/**
  Number of LOT IDs. Each lot ID represents an ASCII value.
 */
#define PM_PBS_INFO_NUM_LOT_IDS   12

/** 
  @struct pm_pbs_info_mfgid_type 
  @brief Stores the PMIC Manufacture ID Info.
         Major Version, Minor Version and Shrink/Non-Shrink info.
 */
typedef struct
{
  uint32  major : 4;                        /* FT Major revision */
  uint32  minor : 3;                        /* FT Minor revision */
  uint32  shrink : 1;                       /* FT Shrink/Non-Shrink */
  uint32  reserved : 24;                    /* FT Shrink/Non-Shrink */
} pm_pbs_info_mfgid_type;

/** 
  Stores the PMIC PBS-related information, such as PBS Lot
         ID, ROM version, and RAM version. The PBS ROM/RAM revision
         ID and variant (or branch) ID are stored in the last 16
         bits(upper and lower 8 bits) of nPBSROMVersion and
         nPBSRAMVersion.
 */
typedef struct
{
  uint8          nLotId[PM_PBS_INFO_NUM_LOT_IDS];  /**< PMIC device lot number. */
  uint32         nPBSROMVersion;                   /**< PBS ROM Version number. */
  uint32         nPBSRAMVersion;                   /**< PBS RAM Version number. */
  uint32         nFabId;                           /**< PBS Fab Id */
  uint32         nWaferId;                         /**< PBS Wafer Id */
  uint32         nXcoord;                          /**< PBS X Coord */
  uint32         nYcoord;                          /**< PBS Y Coord */
  uint32         nTestPgmRev;                      /**< PBS Test PGM Rev */
  pm_pbs_info_mfgid_type  eMfgId;                  /**< PBS MGF ID */
} pm_pbs_info_type;

/*===========================================================================

                        API PROTOTYPE

===========================================================================*/

/** 
 *  Returns the PMIC's all layer revision number. For example,
 *  this function returns 1 for PMIC v1.x, 2 for PMIC v2.x, etc.
 *  
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 *
 * @return
 *  - Valid PMIC all layer revision number if successful.
 *  - 0 for error conditions (e.g., invalid device index parameter).
 */
uint8 pm_get_pmic_revision(uint8 pmic_chip);

/** 
 *  Returns information about a PMIC device for a specific device index
 *  in the pmic_device_info argument. This function returns the PMIC's
 *  model type value, the all layer revision number, and the metal revision
 *  number in the pmic_device_info structure. See the #pm_device_info_type
 *  structure for more information. For example, for PM8x26 v2.1, 
 *  the PMIC model type value is 4 (since the PM8x26 model type value is 4,
 *  as per pm_model_type), the all-layer revision number is 2, and the metal 
 *  revision number is 1.
 *  
 * @datatypes
 *  #pm_device_info_type
 * 
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[out] pmic_device_info Variable returned to the caller with the
 *                              PMIC device information. See
 *                              #pm_device_info_type.
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_get_pmic_info(uint8 pmic_chip, pm_device_info_type* pmic_device_info);

/** 
 *  Returns pmic option pin information. 
 * @datatypes
 *  #pm_device_info_type
 * 
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[out] pmic_opt_pin_data Option pin data returned to the caller 
 *
 * @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_get_pmic_option_pin_info(uint8 pmic_index, uint8 *pmic_opt_pin_data);


/** 
 *  Returns information about the specified PMIC device, such as 
 *  PBS lot ID, ROM version, RAM version, fab ID, wafer ID,
 *  X coordinate, and Y coordinate, for a specified device index in
 *  the pbs_info argument.
 *  
 * @datatypes
 *  #pm_pbs_info_type
 * 
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1.
 * @param[out] pbs_info Variable returned to the caller with the PMIC
 *                      device PBS information. See #pm_pbs_info_type.
 *
 *  @return 
 *  SUCCESS or Error -- See #pm_err_flag_type.
 */
pm_err_flag_type pm_get_pbs_info(uint8 pmic_chip, pm_pbs_info_type* pbs_info);


/** 
 *  Returns information if the target supports specified pmic 
 *  
 * @datatypes
 *  #pm_model_type.
 * 
 * @param[in] model 		Model of pmic which needs to be checked.
 * @param[out] *pmic_index 	Variable returned to the caller with the PMIC
 *                      	index of requested model if present.
 *  @return 
 *  SUCCESS if PMIC is supported 
 *  Error if PMIC is not supported
 */

pm_err_flag_type pm_is_pmic_supported(pm_model_type model, uint8 *pmic_index);


/** @} */ /* end_addtogroup pm_version */

#endif // __PM_VERSION_P_H__
