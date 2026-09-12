#ifndef __GPR_MPD_H__
#define __GPR_MPD_H__

/*========================================================================*/
/**
@file gpr_mpd_.h
@brief This file contains the user and kernel mpd api's for init called from gpr*/
/*========================================================================
Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================== */
/*========================================================================
Edit History

when       who        what, where, why
--------   ---       --------------------------------------------------
04/19/18   np        Created file.
========================================================================== */

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include "gpr_comdef.h"
#include "ipc_dl_api.h"

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/******************************************************************************
 * Defines                                                                    *
 *****************************************************************************/

GPR_INTERNAL uint32_t ipc_dl_mpd_user_init(uint32_t                 src_domain_id,
                                           uint32_t                 dest_domain_id,
                                           const gpr_to_ipc_vtbl_t *p_gpr_to_ipc_vtbl,
                                           ipc_to_gpr_vtbl_t **     pp_ipc_to_gpr_vtbl);

GPR_INTERNAL uint32_t ipc_dl_mpd_user_deinit(uint32_t src_domain_id, uint32_t dest_domain_id);

GPR_INTERNAL uint32_t ipc_dl_mpd_kernel_init(uint32_t                 src_domain_id,
		                                     uint32_t                 dest_domain_id,
                                             const gpr_to_ipc_vtbl_t *p_gpr_to_ipc_vtbl,
                                             ipc_to_gpr_vtbl_t **     pp_ipc_to_gpr_vtbl);

GPR_INTERNAL uint32_t ipc_dl_mpd_kernel_deinit(uint32_t src_domain_id, uint32_t dest_domain_id);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* __GPR_MPD_H__ */
