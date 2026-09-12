#ifndef _MOD_CMN_API_H_
#define _MOD_CMN_API_H_

/*==============================================================================
  @file module_cmn_api.h
  @brief this file contains common module definitions

  Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/

/*==============================================================================
                       EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $header:

  when       who        what, where, why
  --------   --- ------------------------------------------------------
  09/06/17   sumeetj    Created file.
==============================================================================*/

/*------------------------------------------------------------------------------
 *  Header Includes
 *----------------------------------------------------------------------------*/
#include "mmdefs.h"
#include "apm_graph_properties.h"
#include "media_fmt_api.h"

/**
    ID of the Enable parameter used by any audio processing module.
    This generic/common parameter is used to configure or determine the
    state of any audio processing module.
 */
#define PARAM_ID_MODULE_ENABLE                                   0x08001026

/** @h2xmlp_parameter   {"PARAM_ID_MODULE_ENABLE", PARAM_ID_MODULE_ENABLE}
    @h2xmlp_description {Parameter for enabling/disabling the modules}  
    @h2xmlp_toolPolicy  {Calibration} */

/* Structure for Enable parameter for any Audio processing modules. */
typedef struct param_id_module_enable_t param_id_module_enable_t;

#include "gk_begin_pack.h"
struct param_id_module_enable_t
{
   uint32_t enable;
   /**< @h2xmle_description  {Specifies whether the module is to be enabled or disabled.}
        @h2xmle_rangeList    {"Disable"=0;
                              "Enable"=1}
        @h2xmle_default      {0}  
        @h2xmle_policy       {Basic} */
}
#include "gk_end_pack.h"
;

#endif /* _MOD_CMN_API_H_ */
