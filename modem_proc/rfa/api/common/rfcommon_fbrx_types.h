#ifndef RFCOMMON_FBRX_TYPES_H
#define RFCOMMON_FBRX_TYPES_H
/*!
  @file
  rfcommon_fbrx_types.h

  @brief
  This module contains prototypes and definitions used by common FBRx.
*/

/*==============================================================================

  Copyright (c) 2013 Qualcomm Technologies Incorporated. All Rights Reserved

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

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfa.mpss/4.11/api/common/rfcommon_fbrx_types.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
08/28/15   zhh     Added the Bit definition for FBRX enable NV item
08/31/15   aa      Update gain and rxscale threshold limits for FBRx autocal
08/07/15   aa      Added support for FBRx autocal
01/14/15   aa      Added new FBRx error codes
07/14/15   aa      Added support for LTE FBRx C1 NV container
11/18/14   aro     Renamed SW based ILPC variable to better suit the feature
08/15/14   aro/gh  Add support for FBRX ILPC
08/08/14   aa      Moved gps_state enum type to RFLM 
07/31/14   aa      Added api to set the gps state and update it to the fbrx dm
06/17/14   aa      Updated fbrx control enum type 
03/21/14   aa      Update the fbrx error type 
02/27/14   aa      Use rfcom_band_type_u instead
02/03/14   aa      Move enum tx_cfg_type to rflm_fbrx_api.h
02/03/14   aa      Initial version

==============================================================================*/

#include "rfcom.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RFCOMMON_FBRX_SIGNAL_OVERFLOW_THRESH       1780  /* mV10 */
#define RFCOMMON_FBRX_SIGNAL_UNDERFLOW_THRESH      10    /* mv10 */
#define RFCOMMON_FBRX_UBIAS_OVERFLOW_THRESHOLD     5
#define RFCOMMON_FBRX_UBIAS_UNDERFLOW_THRESHOLD    1
#define RFCOMMON_FBRX_RXSCALE_OVERFLOW_THRESHOLD   16384
#define RFCOMMON_FBRX_RXSCALE_UNDERFLOW_THRESHOLD  1000
#define RFCOMMON_FBRX_GAIN_OVERFLOW_THRESHOLD      20000
#define RFCOMMON_FBRX_GAIN_UNDERFLOW_THRESHOLD     1000

/*----------------------------------------------------------------------------*/
/*! Enum used to hold FBRx error codes */
typedef enum
{
  /* FBRx cal execution error */
  RFCOMMON_FBRX_ERROR = 0,
  /* FBRx cal execution success */
  RFCOMMON_FBRX_SUCCESS = 1,
  /* rms sig level > 1780 */
  RFCOMMON_FBRX_RMS_SIG_OVERFLOW = 2, 
  /* rms sig level < 10 */
  RFCOMMON_FBRX_RMS_SIG_UNDERFLOW = 3, 
  /* ubias > 5 */
  RFCOMMON_FBRX_UBIAS_OVERFLOW = 4,    
  /*  ubias < 1 */
  RFCOMMON_FBRX_UBIAS_UNDERFLOW = 5,  
  /* rx_scale > 16384 */
  RFCOMMON_FBRX_RXSCALE_OVERFLOW = 6, 
  /* rx_scale < 8192 */                                       
  RFCOMMON_FBRX_RXSCALE_UNDERFLOW = 7,
  /* fbrx_gain > 12000 */                                       
  RFCOMMON_FBRX_GAIN_OVERFLOW = 8, 
  /* fbrx_gain < 7500 */                                       
  RFCOMMON_FBRX_GAIN_UNDERFLOW = 9, 
  /* 1-tolerance < |RX_RX_CONJ_CORR / RX_RX_CONJ_CORR_SETPT| < 1+tolerance */
  /* warns about signal being non-stationary, i.e. Ryy* varies; tolerance = 0.05 (default) */
   RFCOMMON_FBRX_NON_STATIONARY_SIGNAL = 10, 
} rfcommon_fbrx_error_type;

typedef enum
{
  RFCOMMON_FBRX_1_CARRIER = 1,
  RFCOMMON_FBRX_2_CARRIER = 2,
  RFCOMMON_FBRX_3_CARRIER = 3,
} rfcommon_fbrx_num_carriers_type;

typedef enum
{
  RFCOMMON_FBRX_DISABLE_NO_UPDATES = 0,         /* Disable FBRx, Do not apply any FBRX updates */
  RFCOMMON_FBRX_ENABLE_NO_UPDATES = 1,          /* Enable FBRx, Do not apply any FBRX updates */
  RFCOMMON_FBRX_ENABLE_LIN_UPDATES = 3,         /* Enable FBRx and apply FBRX updates to TxLin table */ 
  RFCOMMON_FBRX_ENABLE_MTPL = 7,                /* Enable FBRx and apply FBRX updates using MTPL method */ 
  RFCOMMON_FBRX_ENABLE_IMM_DTR_ALWAYS = 9,      /* Enable FBRx and apply IMM DTR Updates every TXAGC */ 
  RFCOMMON_FBRX_ENABLE_IMM_DTR_OPT = 11,        /* Enable FBRx and apply IMM DTR Updates with TxLin Table updates */ 
  RFCOMMON_FBRX_ENABLE_SW_BASED_ILPC = 13,      /* Enable SW based of ILPC correction */ 
  RFCOMMON_FBRX_ENABLE_2ND_SW_BASED_ILPC = 15,  /* Enable SW based of ILPC correction with LUT update indication*/
  RFCOMMON_FBRX_INVALID                         /* Invalid : Disable FBRx, Disable updates */
}rfcommon_fbrx_control_enum_type;

typedef enum
{ 
  RFCOMMON_FBRX_NV_CONTAINER_C0 = 0,
  RFCOMMON_FBRX_NV_CONTAINER_C1 = 1,
  RFCOMMON_FBRX_NV_CONTAINER_MAX = 2,
}rfcommon_fbrx_nv_container_type;

typedef enum
{
  RFCOMMON_FBRX_WCDMA_ENABLE         = (0x1 << 0),  //RFLM_WCDMA_FBRX_ENABLE
  RFCOMMON_FBRX_WCDMA_LIN_CORRECTION = (0x1 << 1),  //RFLM_WCDMA_FBRX_LIN_CORRECTION
  RFCOMMON_FBRX_WCDMA_SW_BASED       = (0x1 << 2),  //RFLM_WCDMA_FBRX_SW_BASED
  RFCOMMON_FBRX_WCDMA_IMM_UPDATE     = (0x1 << 3)  //RFLM_WCDMA_FBRX_IMM_UPDATE
}rfcommon_fbrx_wcdma_bit_mask_type;

#ifdef __cplusplus
}
#endif

#endif /* RFCOMMON_FBRX_TYPES_H */

