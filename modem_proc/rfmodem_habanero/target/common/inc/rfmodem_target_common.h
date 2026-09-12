#ifndef RFMODEM_TARGET_COMMON_H
#define RFMODEM_TARGET_COMMON_H

/*!
   @file
   rfmodem_target_dac_ctl.h

   @brief
   This file implements the Target Specific DAC specifications. These are platform
   dependent.

*/

/*===========================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:29:45 $ $Author: pwbldsvc $
$Header: //components/rel/rfmodem_habanero.mpss/1.11/target/common/inc/rfmodem_target_common.h#1 $

when       who     what, where, why
------------------------------------------------------------------------------- 
04/17/15   dej     Initial version.
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#ifdef _cplusplus
extern "C" {
#endif

typedef enum {
   RFMODEM_TARGET_MSM8952 = 0,
   RFMODEM_TARGET_MSM8976 = 1,
   RFMODEM_TARGET_MSM8953 = 2,
   RFMODEM_TARGET_MSM8940 = 3,
   RFMODEM_TARGET_CHIPSET_ID_MAX,
   RFMODEM_TARGET_CHIPSET_ID_INVALID = RFMODEM_TARGET_CHIPSET_ID_MAX
}rfmodem_chipset_id_enum_type;




#ifdef _cplusplus
}
#endif

#endif /* RFMODEM_TARGET_COMMON_H */

