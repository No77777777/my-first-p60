#ifndef PLACEHOLDER_MODULE_PRV_API_H
#define PLACEHOLDER_MODULE_PRV_API_H
/*========================================================================*/
/**
@file placeholder_module_prv.h

@brief  The placeholder enc and dec modules are defined in common_enc_dec_api.h
 * This file defines private APIs for those modules.
*/
/* =========================================================================
    Copyright (c) 2019 Qualcomm Technologies, Inc.
    All Rights Reserved.
    Confidential and Proprietary - Qualcomm Technologies, Inc.
   ========================================================================== */
/*========================================================================
Edit History
when       who        what, where, why
--------   ---       --------------------------------------------------
09/26/19   rb        Created file.
========================================================================== */
 
/*------------------------------------------------------------------------
 * Include files
 * -----------------------------------------------------------------------*/
#include "common_enc_dec_api.h"
/**
 * This parameter is supported by MODULE_ID_PLACEHOLDER_ENCODER, MODULE_ID_PLACEHOLDER_DECODER modules.
 *
 * After setting the real-module-id to the placeholder using PARAM_ID_REAL_MODULE_ID
 * the module is no longer a placeholder. Any set/get API directly goes to the CAPI.
 * This API can be used to reset the placeholder module.
 * The reset involves destroying the current real-module-id. After resetting, the
 * PARAM_ID_REAL_MODULE_ID can be sent again.
 *
 * No Payload
 */
#define PARAM_ID_RESET_PLACEHOLDER_MODULE  0x08001173


#endif /* PLACEHOLDER_MODULE_PRV_API_H */
