#ifndef RFLM_CCS_VERSION_H
#define RFLM_CCS_VERSION_H

/*!
  @file
  rflm_ccs_version.h

  @brief
  RFC image version.
 
  @detail
  description of functions in this file
 
*/

/*===========================================================================

  Copyright (c) 2013-2015 Qualcomm Technologies Incorporated. All Rights Reserved

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
/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rflm.mpss/2.11/api/ccs/rflm_ccs_version.h#1 $

when         who     what, where, why
----------   ---     ----------------------------------------------------------- 
12/11/15     sbo     Increase IRAT PDMEM size by 200 bytes.
11/05/15     sbo     [TA]Enable FULL Mode Memory Scrutiny.
09/03/15     ra      Support PDMEM extension for Device Hopping 
06/23/15     sbo     [TA]Support RFC_SW_LOG via MTC DBG register
05/11/15     sbo     [TA]Move init code to Q6
06/04/15     ra      Support for SCRUTINY LIGHT on TH.2.x
05/15/15     sbo     Support for SCRUTINY LIGHT on TA.2.0
10/31/14     rca     Support for CCS M3 DMA Remap and New Mode 
07/16/13     ra      Initial port to Bolt RF SW
==============================================================================*/

/*! @brief Version information.
*/
#if !(defined(FEATURE_JOLOKIA_MODEM) || defined(FEATURE_TABASCO_MODEM))
#define RFLM_CCS_VERSION_MAJOR                  2
#else
#define RFLM_CCS_VERSION_MAJOR                  1
#endif
#define RFLM_CCS_VERSION_MINOR                  1
#define RFLM_CCS_VERSION_RELEASE                1
#define RFLM_CCS_VERSION_PATCH                  0

/*! @brief Version ID.
*/
#define RFLM_CCS_VERSION \
  ((RFLM_CCS_VERSION_MAJOR << 24) | \
   (RFLM_CCS_VERSION_MINOR << 16) | \
   (RFLM_CCS_VERSION_RELEASE << 8) | \
   (RFLM_CCS_VERSION_PATCH))


#endif /* RFLM_CCS_VERSION_H */
