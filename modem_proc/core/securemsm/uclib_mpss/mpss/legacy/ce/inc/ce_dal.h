#ifndef __CE_DAL_H__
#define __CE_DAL_H__

/**
Copyright (c) 2020 by Qualcomm Technologies, Incorporated.
All Rights Reserved.
QUALCOMM Proprietary

@file ce_dal.h
@brief Crypto Engine DAL API header file

@section S1 Introduction

Public interface declaration for the Crypto Engine DAL APIs. 
This file describes the CE DAL external interface which is 
common to all MSMs. 

@addtogroup ce_group Crypto Engine APIs
@{


*/

/**********************************************************************
 * ce_dal.h
 *
 * Public interface declaration for the Crypto Engine Driver.
 *
 * This file describes the CE DAL external interface, common to all
 * MSMs.
 *
 * Copyright (c) 2009 - 2020 Qualcomm Technologies Incorporated. All Rights Reserved.
 * Qualcomm Confidential and Proprietary
 *
 **********************************************************************/

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

/**
 * Max number of bytes Hash engine can process as a block: 64 K)
 */
#define HAL_CRYPTO_MAX_AUTH_BLOCK_SIZE          0x8000

#include "comdef.h"
#include "core_variation.h"

/**
 * Error code definitions for CE API functions
 */
typedef enum
{
    CE_ERROR_NONE,
    CE_ERROR_BUSY,
    CE_ERROR_FAILED,
    CE_ERROR_INVALID_PARAM,
    CE_ERROR_DMOV,
    CE_ERROR_UNSUPPORTED,
    CE_ERROR_BAD_STATE
} CE_Result_Type;


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/* FEATURE_SEC_HW_CRYPTO get support function */
CE_Result_Type CE_Get_Support(void);

/**
 * @brief This function reverses the bytes for each four byte 
 *        set in the input data
 *
 * @param out_bytes_ptr  [out] Pointer to output data
 * @param in_bytes_ptr   [in]  Pointer to input data
 * @param byte_count     [in]  Length of data in bytes. 
 *
 * @return CE_Result_Type
 *
 * @see 
 *
 */
CE_Result_Type reverse_bytes(uint8 *out_bytes_ptr, uint8 *in_bytes_ptr, 
                             uint32 byte_count);

/** @} End of ce_group */
#endif /* __CE_DAL_H__ */

