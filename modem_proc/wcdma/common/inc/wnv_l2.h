/*============================================================================
                            W N V _ L 2 . H
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

$Header: //components/rel/wcdma.mpss/8.4.0/common/inc/wnv_l2.h#2 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     -------------------------------------------------------- 
05/22/15    as      Initial checkin 
===========================================================================*/

#ifndef _WNV_L2_H_
#define _WNV_L2_H_

#include "comdef.h"
#include "wnv.h"
#include "msgcfg.h"


/*************************************************************************** 
                   Exported structure definitions
****************************************************************************/

/* L2 Feature flag 
   Bit 0 control the FEATURE_WCDMA_EVT6D_ENH enable disable
   Bit 1 controls the ETFCI based (0)/L1 power based (1)
   Bit  2 to Bit 31 - reserved
*/
#define WNV_ID_L2_FEATURE_FLAG_DEFAULT              3

/* Placeholder for L2 NV types to make memory allocations easier */
typedef union
{
  uint32 wl2_feature_flg_nv;
} wnv_l2_default;

/*************************************************************************** 
                          Exported functions NV default functions
****************************************************************************/
/*=========================================================
FUNCTION		wnv_default_l2

DESCRIPTION 	Updates default value for L2 related NV.
===========================================================*/
wnv_api_status wnv_default_l2(wsub_id_e_type wsub_id,wnv_nv_id_enum nv_id, void* out_buffer, uint32 output_len, boolean write_to_file);

#endif /* _WNV_RRC_H_*/
