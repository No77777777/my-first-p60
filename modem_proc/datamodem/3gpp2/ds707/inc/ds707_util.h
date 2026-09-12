#ifndef DS707_UTIL_H
#define DS707_UTIL_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 
                              D S 7 0 7  U T I L

GENERAL DESCRIPTION
  This file contains utility APIs and definitions for all modules within 707
 
 Copyright (c) 2018 by Qualcomm Technologies Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ds707.h_v   1.6   15 Nov 2002 17:38:26   akhare  $
  $Header: //components/rel/data.mpss/3.5..2.1/3gpp2/ds707/inc/ds707_util.h#1 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    -------------------------------------------------------
04/01/12    rs     Initial version
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA_IS707

/*===========================================================================
                        TYPEDEFS AND VARIABLES
===========================================================================*/

typedef enum
{
  DS707_LOCAL_ERR_VAL_NONE = -1,
  DS707_LOCAL_ERR_VAL_0 = 0,
  DS707_LOCAL_ERR_VAL_1,
  DS707_LOCAL_ERR_VAL_2,
  DS707_LOCAL_ERR_VAL_3,
  DS707_LOCAL_ERR_VAL_4,
  DS707_LOCAL_ERR_VAL_5,
  DS707_LOCAL_ERR_VAL_6,
  DS707_LOCAL_ERR_VAL_7,
  DS707_LOCAL_ERR_VAL_8,
  DS707_LOCAL_ERR_VAL_9,
  DS707_LOCAL_ERR_VAL_10,
  DS707_LOCAL_ERR_VAL_11,
  DS707_LOCAL_ERR_VAL_12,
  DS707_LOCAL_ERR_VAL_13,
  DS707_LOCAL_ERR_VAL_14,
  DS707_LOCAL_ERR_VAL_15,
  DS707_LOCAL_ERR_VAL_16,
  DS707_LOCAL_ERR_VAL_17,
  DS707_LOCAL_ERR_VAL_18,
  DS707_LOCAL_ERR_VAL_19,
  DS707_LOCAL_ERR_VAL_20,
  DS707_LOCAL_ERR_VAL_21,
  DS707_LOCAL_ERR_VAL_22,
  DS707_LOCAL_ERR_VAL_23,
  DS707_LOCAL_ERR_VAL_24,
  DS707_LOCAL_ERR_VAL_25,
  DS707_LOCAL_ERR_VAL_26,
  DS707_LOCAL_ERR_VAL_27,
  DS707_LOCAL_ERR_VAL_28,
  DS707_LOCAL_ERR_VAL_29,
  DS707_LOCAL_ERR_VAL_30,
  DS707_LOCAL_ERR_VAL_31,
  DS707_LOCAL_ERR_VAL_32,
  DS707_LOCAL_ERR_VAL_33,
  DS707_LOCAL_ERR_VAL_34,
  DS707_LOCAL_ERR_VAL_35,
  DS707_LOCAL_ERR_VAL_36,
  DS707_LOCAL_ERR_VAL_37,
  DS707_LOCAL_ERR_VAL_38,
  DS707_LOCAL_ERR_VAL_39,
  DS707_LOCAL_ERR_VAL_40,
  DS707_LOCAL_ERR_VAL_41,
  DS707_LOCAL_ERR_VAL_42,
  DS707_LOCAL_ERR_VAL_43,
  DS707_LOCAL_ERR_VAL_44,
  DS707_LOCAL_ERR_VAL_45,
  DS707_LOCAL_ERR_VAL_46,
  DS707_LOCAL_ERR_VAL_47,
  DS707_LOCAL_ERR_VAL_48,
  DS707_LOCAL_ERR_VAL_49,
  DS707_LOCAL_ERR_VAL_50
} ds707_local_err_val_e_type;

#endif /* FEATURE_DATA_IS707 */
#endif /* DS707_UTIL_H */
