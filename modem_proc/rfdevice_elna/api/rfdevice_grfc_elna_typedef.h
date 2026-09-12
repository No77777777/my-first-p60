#ifndef RFDEVICE_GRFC_ELNA_TYPEDEF_H
#define RFDEVICE_GRFC_ELNA_TYPEDEF_H

/*=============================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S.
  Government. Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

  $Header: //components/rel/rfdevice_elna.mpss/1.22/api/rfdevice_grfc_elna_typedef.h#1 $
  $DateTime: 2021/03/09 03:41:28 $
  $Author: pwbldsvc $

when         who   what, where, why
--------   ---   ---------------------------------------------------------------
03/24/15   vv    Add GRFC eLNA sleep support
02/19/15   vv    Initial version

=============================================================================*/

/*=============================================================================

                            INCLUDE FILES FOR MODULE

=============================================================================*/
#include "comdef.h"
#include "rfcom.h"
#include "rf_hal_bus_types.h"
#include "rf_buffer_intf.h"
#include "rfdevice_type_defs.h"

/*==========================================================================*/
/* DEFINES and ENUMS  */
/*==========================================================================*/

#define RFDEVICE_GRFC_ELNA_MAX_NUM_GRFCS 5

/*==========================================================================*/
/* DATA STRUCTURES  */
/*==========================================================================*/

/*To capture the eLNA gain settings per GRFC*/
typedef struct
{
  boolean valid_flag;
  uint16 grfc_num;
  uint8  gain_states[RFDEVICE_MAX_RX_GAIN_STATES];
  uint8  sleep;
}rfdevice_elna_grfc_rfc_info_type;

/*To capture the GRFCs settings per tech*/
typedef struct
{
  uint8 num_grfc;
  rfdevice_elna_grfc_rfc_info_type rfc_grfc_data[RFDEVICE_GRFC_ELNA_MAX_NUM_GRFCS];
}rfdevice_elna_grfc_rfc_tbl_type;

/*To capture the eLNA GRFC info for all techs from RFC*/
typedef struct
{
  rfdevice_elna_grfc_rfc_tbl_type gsm[RFM_MAX_WAN_DEVICES][RFCOM_NUM_GSM_BANDS];
  rfdevice_elna_grfc_rfc_tbl_type tdscdma[RFM_MAX_WAN_DEVICES][RFCOM_NUM_TDSCDMA_BANDS];
  rfdevice_elna_grfc_rfc_tbl_type cdma[RFM_MAX_WAN_DEVICES][RFM_CDMA_MAX_BAND];
  rfdevice_elna_grfc_rfc_tbl_type wcdma[RFM_MAX_WAN_DEVICES][RFCOM_NUM_WCDMA_BANDS];
  rfdevice_elna_grfc_rfc_tbl_type lte[RFM_MAX_WAN_DEVICES][RFCOM_NUM_LTE_BANDS];
} rfdevice_elna_grfc_rfc_tbl_band_map_type;

#endif /*RFDEVICE_GRFC_ELNA_TYPEDEF_H */

