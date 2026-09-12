/*============================================================================
                        W N V _ C O M M O N . H
DESCRIPTION

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2014-2014 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/common/inc/wnv_common.h#2 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     -------------------------------------------------------- 
07/15/14    ac      Initial checkin 
===========================================================================*/

#ifndef _WNV_COMMON_H
#define _WNV_COMMON_H

#include "wnv.h"

/* Enum for storing the NV release indicator locally */
typedef enum {
  L2_REL_INDICATOR_99 = 0,
  L2_REL_INDICATOR_5,
  L2_REL_INDICATOR_6
} l2_nv_rel_indicator_e_type;


/***************HSDPA CAT defaults************************************** */
                            
#define WNV_HSDPA_CAT_DEFAULT  24

#define WNV_ID_HSDPA_CAT_I_DEFAULT                        WNV_HSDPA_CAT_DEFAULT

#define WNV_ID_GPRS_ANITE_GCF_I_DEFAULT                   FALSE /* Not in GCF mode */

#define WNV_ID_RRC_VERSION_I_FOR_L2_DEFAULT L2_REL_INDICATOR_5

typedef union
{
  uint8   hsdpa_cat;
  boolean gprs_anite_gcf;
  l2_nv_rel_indicator_e_type wcdma_rrc_version;
  nv_pref_mode_type          pref_mode;
} wnv_common_legacy_default;

/*************************************************************************** 
                              NV default functions
****************************************************************************/
wnv_api_status wnv_default_common_legacy(wsub_id_e_type wsub_id, wnv_nv_id_enum nv_id, void* out_buffer, uint32 output_len, boolean write_to_file);

/* Legacy NV read function */
wnv_api_status wnv_int_legacy_read(wsub_id_e_type wsub_id, nv_items_enum_type nv_item, void* out_buffer);

#endif /* _WNV_COMMON_H */
