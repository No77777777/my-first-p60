#ifndef RFDEVICE_GRFC_COUPLER_TYPEDEF_H
#define RFDEVICE_GRFC_COUPLER_TYPEDEF_H

/*=============================================================================

Copyright (c) 2014,2015  by Qualcomm Technologies, Inc.  All Rights Reserved.

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

  $Header: //components/rel/rfdevice_coupler.mpss/1.22/api/rfdevice_grfc_coupler_typedef.h#1 $
  $DateTime: 2021/03/09 03:37:09 $
  $Author: pwbldsvc $
 
when       who   what, where, why
--------   ---   ---------------------------------------------------------------
01/22/15   yb     Added bitmasks and bishifts for RFC data
12/26/14   ndb    Initial version
 
=============================================================================*/
  
/*=============================================================================

                            INCLUDE FILES FOR MODULE

=============================================================================*/
#include "comdef.h"
#include "rfcom.h"
#include "rf_hal_bus_types.h"
#include "rf_buffer_intf.h"
#include "rfdevice_comm_intf.h"
#include "rfdevice_coupler.h"

/*==========================================================================*/
/* DEFINES and ENUMS  */
/*==========================================================================*/

#define RFDEVICE_GRFC_COUPLER_MAX_NUM_GRFCS 5

/*==========================================================================*/
/* DATA STRUCTURES  */
/*==========================================================================*/


/*Maps the logic against the direction*/
typedef struct
{
  uint16 valid_flag:2;
  uint16 open_logic:2;
  uint16 fwd_logic:2;
  uint16 rev_logic:2;
  uint16 reserved:8;
}rfdevice_grfc_coupler_rfc_logic_direction_map_info_type;

/*HOLD the GRFC info and its corresponding logic based on the direction*/
typedef struct
{
  uint16 grfc_num;
  rfdevice_grfc_coupler_rfc_logic_direction_map_info_type logic_dir_data;
}rfdevice_grfc_coupler_rfc_grfc_info_type;

/*Number of GRFC's & their corresponding logic based on the direction that need to configured */
typedef struct
{
  uint8 num_grfc;
  rfdevice_grfc_coupler_rfc_grfc_info_type rfc_grfc_data[RFDEVICE_GRFC_COUPLER_MAX_NUM_GRFCS];
}rfdevice_grfc_coupler_rfc_grfc_tbl_type;

typedef struct
{
  rfdevice_grfc_coupler_rfc_grfc_tbl_type gsm[RFCOM_NUM_GSM_BANDS];
  rfdevice_grfc_coupler_rfc_grfc_tbl_type tdscdma[RFCOM_NUM_TDSCDMA_BANDS];
  rfdevice_grfc_coupler_rfc_grfc_tbl_type cdma[RFM_CDMA_MAX_BAND];
  rfdevice_grfc_coupler_rfc_grfc_tbl_type wcdma[RFCOM_NUM_WCDMA_BANDS];
  rfdevice_grfc_coupler_rfc_grfc_tbl_type lte[RFCOM_NUM_LTE_BANDS];
} rfdevice_grfc_coupler_rfc_grfc_tbl_band_map_type;


/*==========================================================================*/
/* Function Pointers  */
/*==========================================================================*/

#endif /*RFDEVICE_GRFC_COUPLER_TYPEDEF_H */
