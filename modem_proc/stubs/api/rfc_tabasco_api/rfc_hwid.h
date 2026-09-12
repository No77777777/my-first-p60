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

Copyright (c) 1999 - 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved .
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //commercial/MPSS.TA.3.0.c1/Main/modem_proc/stubs/api/rfc_tabasco_api/rfc_hwid.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
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
  RF_HW_UNDEFINED                         = (uint8)0,
  RF_TARGET_NONE                          = RF_HW_UNDEFINED,
  RF_HW_WTR2965_CHN_ULCA_3P_APT           = (uint8)3,
  RF_HW_WTR4905_NON_CA                    = (uint8)31,
  RF_HW_WTR2965_DLCA_ULCA_4340            = (uint8)155,
  RF_HW_WTR2965_NON_CA_4373_V2            = (uint8)164,
  RF_HW_WTR2965_ULCA_QFE                  = (uint8)167,
  RF_HW_WTR2965_QRD_CHN_CA_8920           = (uint8)206,
  RF_HW_WTR2965_QRD_CHN_CA_8920_1         = (uint8)207,
  RF_HW_WTR2965_QRD_CHN_CA_4373_3         = (uint8)208,
  RF_HW_WTR3925_QRD_CHN_CA_8920           = (uint8)218,
  RF_HW_WTR3925_NA_DLCA_3550              = (uint8)229,
  RF_HW_WTR2965_V2_NA_CA_4320             = (uint8)233,
  RF_HW_WTR2965_V2_JP_CA_4320             = (uint8)235,
  RF_HW_WTR2965_V2_CHILE_CA_4320          = (uint8)236,
  RF_HW_WTR2965_NON_CA_SAWLESS_4320       = (uint8)237,
  RF_HW_WTR2965_V2_TXRXS_CHILE_4320       = (uint8)238,
  RF_HW_WTR2965_V2_CHILE_CA_4320_3G       = (uint8)246,
  RF_HW_WTR2965_QRD_CHN_CA_4373           = (uint8)88,
  RF_HW_WTR2965_QRD_CHN_CA_4373_2         = (uint8)89,
  RF_HW_WTR2965_V2_NA_CA_4320_B66         = (uint8)243,
  RF_HW_WTR2965_DLCA_ULCA_4340_V2         = (uint8)156, 
  RF_HW_WTR2965_QRD_CHN_CA_8940           = (uint8)202,
  RF_HW_WTR3925_CHILE_ULCA_DLCA           = (uint8)247,
  RF_HW_WTR2965_NON_CA_SAW_4320           = (uint8)248,
  RF_HW_WTR3925_CHILE_UL_DLCA_3550        = (uint8)249,
  RF_HW_WTR2965_DLCA_ULCA_4360_88XX       = (uint8)157,  


  /* Add any new HW ID before this line */
  RF_HW_MAX,
  RF_HW_DEFAULT = RF_HW_MAX, /* Default card for Off-Target Builds */
  RF_HW_EFS_CARD                          = (uint8)191, /* EFS Card */

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
