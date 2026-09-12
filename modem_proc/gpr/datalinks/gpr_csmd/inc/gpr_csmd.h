#ifndef _GPR_CSMD_H_
#define _GPR_CSMD_H_
/*=============================================================================
Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.

===============================================================================
                                 Edit History

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //source/qcom/qct/multimedia2/gpr/gpr2/mainline/main/latest/datalink/gpr_csmd/inc/gpr_csmd.h
  $Author: mplcsds1 $

when         who   what, where, why
----------   ---   ------------------------------------------------------------
4/11/18    np      Created
=============================================================================*/

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include "gpr_comdef.h"
#include "ipc_dl_api.h"
#include "ar_osal_error.h"
#include "ar_types.h"
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/******************************************************************************
 * Defines                                                                    *
 *****************************************************************************/
ar_result_t ipc_dl_csmd_init(uint32_t                 src_domain_id,
                             uint32_t                 dest_domain_id,
                                       const gpr_to_ipc_vtbl_t *p_gpr_to_ipc_vtbl,
                             ipc_to_gpr_vtbl_t **     pp_ipc_to_gpr_vtbl);

ar_result_t ipc_dl_csmd_deinit(uint32_t src_domain_id, uint32_t dest_domain_id);

#ifdef __cplusplus
}
#endif //__cplusplus
#endif /* _GPR_CSMD_H_ */
