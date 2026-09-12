#ifndef MCFG_MULIT_MBN_PROC_H
#define MCFG_MULIT_MBN_PROC_H
/*==============================================================================

                M O D E M   C O N F I G   M U L T I   M B N

GENERAL DESCRIPTION
  Module for embedded multi MBN processing.

Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_proc/inc/mcfg_multi_mbn.h#1 $
$DateTime: 2021/04/08 00:04:19 $
$Author: pwbldsvc $
$Change: 30003610 $

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
09/11/15 cys   Modify module
09/04/15 ap    Initial creation
==============================================================================*/

#include "mcfg_common.h"

/*==============================================================================

                PUBLIC DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/
mcfg_error_e_type mcfg_multi_mbn_proc(boolean);

mcfg_error_e_type mcfg_multi_mbn_add_config
(
  mcfg_config_type_e_type  type,
  mcfg_config_s_type      *config,
  mcfg_storage_mode_e_type storage,
  boolean replace_config_based_on_version
);

boolean mcfg_multi_mbn_generate_id
(
  uint8 *p_img,
  uint32 data_seg_size,
  mcfg_config_id_s_type *config_id
);

#endif /* MCFG_MULIT_MBN_PROC_H */
