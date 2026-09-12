/*!
  @file
  lte_stm.h

  @brief
  This file contains definitions for the constants required
  by different *.stm files within LTE Access Stratum(AS)

  C structures, enums, typedefs are not allowed in this file

*/

/*=============================================================================

  Copyright (c) 2014 QUALCOMM Technologies Incorporated. All Rights Reserved

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

=============================================================================*/

/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order. 

$Header: //components/rel/lte.mpss/6.6.1/api/lte_stm.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
09/17/14   ap      initial version
=============================================================================*/

#ifndef LTE_STM_H
#define LTE_STM_H

/*=============================================================================

                   INCLUDE FILES

=============================================================================*/

/* DO NOT INCLUDE FILES THAT HAVE C STRUCTURES, ENUMS AND TYPEDEFS */

/*=============================================================================

                   EXTERNAL CONSTANT/MACRO DEFINITIONS

=============================================================================*/

/* DO NOT DEFINE C STRUCTURES, ENUMS AND TYPEDEFS IN THIS FILE */

#define FEATURE_DUAL_LTE

/*! Maximum number of configured SCells */
#define LTE_CPHY_MAX_C_SCELL 4

/*! @brief MAX number of carriers support SIB reading  */
#define LTE_ML1_DL_SIB_NUM_CARRIERS 2

#define LTE_RRC_SCMGR_SM_MAX_NUM_INSTANCE LTE_CPHY_MAX_C_SCELL

#ifdef FEATURE_DUAL_LTE
/*Define LTE RRC max stack number to 2 for L+L */
#define LTE_RRC_MAX_STACKS        2 
#else
#define LTE_RRC_MAX_STACKS        1 
#endif

/* Max number of L RRC STMs supported */
#define LTE_RRC_MAX_STMS 2

#endif /* LTE_STM_H */