#ifndef __GRM_TYPE_H
#define __GRM_TYPE_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            GRM HEADER

GENERAL DESCRIPTION
  Provides common definitions for use by GERAN entities supporting Multi SIM

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gcommon/inc/grm_type.h#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when        who     what, where, why
--------    ---     --------------------------------------------------------
02/03/18   sp        CR2199093 Add LTE Tx CLients
06/06/16   sp       CR1024701: Featurize GSM3 Clients
29/10/17   km       CR2133917 GL1 support for TA.3.0 L+L
09/05/15   cah/pa   CRXXXXXX: Changes MSIM/SSIM split RX/TX support
31/03/15    pa/sw   CR815621:GRM restructuring for TH 2.0.  
===========================================================================*/
#include "geran_variation.h"
#ifndef CUSTOMER_H
  #include "customer.h"
#endif
#ifndef COMDEF_H
  #include "comdef.h"
#endif

#include "trm.h"

typedef enum
{
  GRM_1X                  = TRM_1X,
  GRM_1X_SECONDARY        = TRM_1X_SECONDARY,
  GRM_HDR                 = TRM_HDR,
  GRM_HDR_SECONDARY       = TRM_HDR_SECONDARY,
  GRM_GPS                 = TRM_GPS,
  GRM_UMTS                = TRM_UMTS,
  GRM_UMTS_SECONDARY      = TRM_UMTS_SECONDARY,
  GRM_GSM1                = TRM_GSM1,
  GRM_GSM2                = TRM_GSM2,
  GRM_WCDMA               = TRM_WCDMA,
  GRM_TDSCDMA             = TRM_TDSCDMA,
  GRM_TDSCDMA_SECONDARY   = TRM_TDSCDMA_SECONDARY,
  GRM_UMTS_CA             = TRM_UMTS_CA,
  GRM_UMTS_CA_SECONDARY   = TRM_UMTS_CA_SECONDARY,
  GRM_LTE                 = TRM_LTE,
  GRM_LTE_SECONDARY       = TRM_LTE_SECONDARY,
  GRM_LTE_CA              = TRM_LTE_CA,
  GRM_LTE_CA_SECONDARY    = TRM_LTE_CA_SECONDARY,
  GRM_LTE_SUB2            = TRM_LTE_SUB2, 
  GRM_LTE_SUB2_SECONDARY  = TRM_LTE_SUB2_SECONDARY,
  GRM_LTE_TX              = TRM_LTE_TX,
  GRM_LTE_SUB2_TX          =TRM_LTE_SUB2_TX, 
  GRM_CM                  = TRM_CM,
  GRM_RF                  = TRM_RF,
  GRM_IRAT                = TRM_IRAT,
  GRM_WLAN                = TRM_WLAN,
  GRM_GPRS1               = TRM_GPRS1,
  GRM_GPRS2               = TRM_GPRS2,
  GRM_LTE_CA1             = TRM_LTE_CA1,
  GRM_LTE_CA1_SECONDARY   = TRM_LTE_CA1_SECONDARY,
  GRM_GSM1_SECONDARY      = TRM_GSM1_SECONDARY,
  GRM_GSM2_SECONDARY      = TRM_GSM2_SECONDARY,
#ifdef FEATURE_TRIPLE_SIM
  GRM_GSM3                = TRM_GSM3,
  GRM_GSM3_SECONDARY      = TRM_GSM3_SECONDARY,
#endif  
  GRM_LTE_HO_SECONDARY1   = TRM_LTE_HO_SECONDARY1,
  GRM_LTE_HO_SECONDARY2   = TRM_LTE_HO_SECONDARY2,
  GRM_LTE_EMBMS1          = TRM_LTE_EMBMS1,
  GRM_LTE_EMBMS2          = TRM_LTE_EMBMS2,
  GRM_IRAT2               = TRM_IRAT2,
  GRM_UMTS2               = TRM_UMTS2,
  GRM_UMTS_SECONDARY2     = TRM_UMTS_SECONDARY2,
  GRM_IRAT_TX             = TRM_IRAT_TX,
  GRM_IRAT2_TX            = TRM_IRAT2_TX,
  GRM_GSM1_TX             = TRM_GSM1_TX,
  GRM_GSM2_TX             = TRM_GSM2_TX,
  GRM_GPRS1_TX            = TRM_GPRS1_TX,
  GRM_GPRS2_TX            = TRM_GPRS2_TX,
  GRM_LAST_CLIENT         = TRM_LAST_CLIENT,
  GRM_MAX_CLIENTS         = TRM_MAX_CLIENTS,
  GRM_NO_CLIENT           = TRM_NO_CLIENT
}grm_client_enum_t;

#ifdef FEATURE_DUAL_WCDMA
/* When this feature is enabled, there are 2 clients being used by the WCDMA access stratum */
/* If the GAS_ID (or AS_ID) of the GERAN stack is 1: the opposite WCDMA stack uses TRM_UMTS */
/* If the GAS_ID (or AS_ID) of the GERAN stack is 0: the opposite WCDMA stack has TRM_UMTS2 */
#define WCDMA_CLIENT_FOR_THIS_SUB(sub_id)  (((int)(sub_id) == 0) ? GRM_UMTS : GRM_UMTS2)
#define WCDMA_CLIENT_FOR_OTHER_SUB(sub_id) (((int)(sub_id) != 0) ? GRM_UMTS : GRM_UMTS2)
#else
/* in the legacy case there is only one WCDMA client */
#define WCDMA_CLIENT_FOR_THIS_SUB(sub_id)  (GRM_UMTS)
#define WCDMA_CLIENT_FOR_OTHER_SUB(sub_id) (GRM_UMTS)
#endif /* FEATURE_DUAL_WCDMA */

#define LTE_CLIENT_FOR_THIS_SUB(sub_id)  (((int)(sub_id) == 0) ? GRM_LTE : GRM_LTE_SUB2)

#endif /* __GRM_TYPE_H */

/* EOF */

