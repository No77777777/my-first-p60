#ifndef _PM_VERSION__H
#define _PM_VERSION__H

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

  $Header: //components/rel/core.mpss/10.0/api/pmic/pm/pm_version.h#4 $

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

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/
/** @addtogroup pm_version
@{ */

/** 
  Provides the PMIC model type values (peripheral
  subtype values) for the various PMIC models. 
 */
typedef enum
{
   PMIC_IS_UNKNOWN   = 0,  /**< PMIC model is unknown. */
   PMIC_IS_PM8941    = 1,  /**< PMIC model is PM8941. */
   PMIC_IS_PM8841    = 2,  /**< PMIC model is PM8841. */
   PMIC_IS_PM8019    = 3,  /**< PMIC model is PM8019. */
   PMIC_IS_PM8026    = 4,  /**< PMIC model is PM8026. */
   PMIC_IS_PM8110    = 5,  /**< PMIC model is PM8110. */
   PMIC_IS_PMA8084   = 6,  /**< PMIC model is PMA8084. */
   PMIC_IS_PMI8962   = 7,  /**< PMIC model is PMI8962. */
   PMIC_IS_PMD9635   = 8,  /**< PMIC model is PMD9635. */
   PMIC_IS_PMD9645   = 8,  /**< PMIC model is PMD9645. */
   PMIC_IS_PM8994    = 9,  /**< PMIC model is PM8994. */
   PMIC_IS_PMI8994   = 10, /**< PMIC model is PMI8994. */
   PMIC_IS_PM8916    = 11, /**< PMIC model is PM8916. */
   PMIC_IS_PM8004    = 12, /**< PMIC model is PM8004. */
   PMIC_IS_PM8909    = 13, /**< PMIC model is PM8909. */
   PMIC_IS_PMD9655   = 15, /**< PMIC model is PMD9655. */
   PMIC_IS_PM8950    = 16, /**< PMIC model is PM8950. */
   PMIC_IS_PMI8950   = 17, /**< PMIC model is PMI8950. */
   PMIC_IS_PMK8001   = 18, /**< PMIC model is PMK8001. */
   PMIC_IS_PMI8996   = 19, /**< PMIC model is PMI8996. */
   PMIC_IS_PM8998    = 20, /**< PMIC model is PM8998. */
   PMIC_IS_PMI8998   = 21, /**< PMIC model is PMI8998. */
   PMIC_IS_PM8005    = 24, /**< PMIC model is PM8005. */
   PMIC_IS_PM8937    = 25, /**< PMIC model is PM8937. */
   PMIC_IS_PM660L    = 26, /**< PMIC model is PM660L. */
   PMIC_IS_PM660     = 27, /**< PMIC model is PM660. */
   PMIC_IS_PM855     = 30,/**< PMIC model is PM855. */
   PMIC_IS_PM855L    = 31,/**< PMIC model is PM855L. */
   PMIC_IS_PM855B    = 32,/**< PMIC model is PM855B. */
   PMIC_IS_PMK8002   = 33, /**< PMIC model is PMK8002. */
   PMIC_IS_PMX24     = 34, /**< PMIC model is PMX24. */
   PMIC_IS_PM8009    = 36, /**< PMIC model is PM8009. */
   PMIC_IS_PMI632    = 37,/**< PMIC model is PMI632. */
   PMIC_IS_PM855P    = 38, /**< PMIC model is PM855P. */
   PMIC_IS_PM855A    = 39,/**< PMIC model is PM855A. */
   PMIC_IS_PM6150    = 40,/**< PMIC model is PM6150. */   
   PMIC_IS_PM6125    = 45,/**<PMIC MODEL is PM6125*/ 
   PMIC_IS_TIOMAN    = 46, /**<PMIC MODEL is TIOMAN*/ 
   PMIC_IS_PM2250    = 55 ,/**<PMIC Model is COCOS*/    
   PMIC_IS_PMG1110   = 67, /**<pmic MODEL IS SECO JR */
   PMIC_IS_INVALID   = 0x7FFFFFFF,
} pm_model_type;

/*===========================================================================

                        API PROTOTYPE

===========================================================================*/

/** 
 *  Returns the PMIC's model type value. For example, this function returns
 *  4 for PM8x26. See #pm_model_type for more information
 *  as to which PMIC model type value represents which PMIC.
 *  
 * @param[in] pmic_chip Primary PMIC -- 0; Secondary PMIC -- 1. 
 *
 * @return
 *  - Valid PMIC model/tier type if successful.
 *  - PMIC_IS_INVALID for error conditions (e.g.,
 *    invalid device index parameter).
 */
pm_model_type pm_get_pmic_model(uint8 pmic_chip);

/** @} */ /* end_addtogroup pm_version */

#endif // PM_VERSION__H
