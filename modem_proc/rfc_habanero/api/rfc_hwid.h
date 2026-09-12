#ifndef RFC_HWID_H
#define RFC_HWID_H

/*===========================================================================


      R F  D r i v e r  C o m m o n  H a r d w a r e  I d e n t i f i e r

                            H e a d e r  F i l e

DESCRIPTION
  This file contains the Hardware IDs for RF Cards used across multiple
  layers of the RF driver.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 1999 - 2025 by Qualcomm Technologies, Incorporated.  All Rights Reserved .
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfc_habanero.mpss/1.11/api/rfc_hwid.h#5 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/28/25   ssk     add RF_HW_WTR2965_GLOBAL_2W_SM6115
08/09/24   ssk     add RF_HW_WTR2965_NON_CA_2W_SKU2 
03/16/23   ssk     add RF_HW_WTR2965_NON_CA_QPA8673_V3 and RF_HW_WTR3925_ROW_NON_CA_QPA6560_V2
04/17/20   akm     add RF_HW_WTR3925_QRD2250_CA_QPA8673
03/14/16   mmd     Removing support for WTR2955 cards
07/29/15   fhuo    Add WTR2955_V2_NA_CA_4320 and WTR2955_V2_JP_CA_4320 cards
07/13/15   fhuo    Change WTR2955_ULCA_QFE to External
07/13/15   fhuo    Add WTR2955_NON_CA_4373 and WTR2955_NON_CA_4373_LB cards
06/09/15   fhuo    Add support WTR2955_NA_CA_4320 card
06/03/15   fhuo    Add support WTR2955_ULCA_QFE card
05/13/15   sml     Fix compilation errors on TA 2.0
04/09/15   fhuo    Added support for RFC_WTR2955_CHINA_CMCC_5M_DLCA
03/19/15   rp      Added Dummy HW IDs to resolve compile time dependency on ULCA changes.
04/23/14   sd      Initial revision for DimePM 2.0 RFC HW IDs.

============================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/* -------------------------------------------------------
** The RF Card Id used in the target
** Note: The Id needs to be sequential
** ------------------------------------------------------- */
typedef enum {
  RF_HW_UNDEFINED                         = (uint32)0,
  RF_TARGET_NONE                          = RF_HW_UNDEFINED,
  RF_HW_WTR3925_NA_DLCA_QPA6560           = (uint32)563,
  RF_HW_WTR3925_SDR425_ROW_DL_6560        = (uint32)565,
  RF_HW_WTR2965_NON_CA_QPA8673            = (uint32)564,
  RF_HW_WTR3925_QRD_CA_QPA8673            = (uint32)584,
  RF_HW_WGR7640                           = (uint32)174,
  RF_HW_WTR3925_QRD2250_CA_QPA8673        = (uint32)646,
  RF_HW_WTR3925_QRD6225_CA_QPA6560        = (uint32)904,
  RF_HW_WTR2965_NON_CA_QPA8673_V3         = (uint32)1088,
  RF_HW_WTR3925_ROW_NON_CA_QPA6560_V2     = (uint32)1110,
  RF_HW_WTR2965_NON_CA_2W_SKU2            = (uint32)1178,
  RF_HW_WTR2965_GLOBAL_2W_SM6115          = (uint32)1301,
  


  /* Add any new HW ID before this line */
  RF_HW_MAX,
  RF_HW_DEFAULT = RF_HW_MAX, /* Default card for Off-Target Builds */
  RF_HW_EFS_CARD                          = (uint32)191, /* EFS Card */

  /* The following definitions are not needed and will need to be removed  */
  RF_HW_WTR1605_SGLTE                     = RF_HW_MAX,
  RF_HW_WTR1605_SGLTE_DSDA                = RF_HW_MAX,
  RF_HW_WTR3925_TP130                     = RF_HW_MAX,
  RF_HW_WTR3925_TP130_2                   = RF_HW_MAX,
  RF_HW_WTR3925_TP130_2_CA                = RF_HW_MAX,  
  RF_HW_WTR3925_TP130_CA                  = RF_HW_MAX,
  RF_HW_WTR3925_TP160_0                   = RF_HW_MAX,
  RF_HW_WTR3925_FDD_ULCA_4K               = RF_HW_MAX,
  RF_HW_WTR3925_TDD_ULCA_DSDA_QFE4X       = RF_HW_MAX,
  RF_HW_WTR3925_TDD_ULCA_DSDA             = RF_HW_MAX, 
  RF_HW_WTR3925_TDD_ULCA_V2               = RF_HW_MAX,
} rf_hw_type;

#ifdef __cplusplus
}
#endif

#endif  /* RFC_HWID_H */
