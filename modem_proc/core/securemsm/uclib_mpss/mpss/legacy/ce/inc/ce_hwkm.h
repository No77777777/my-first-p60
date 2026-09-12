#ifndef _ce_hwkm
#define _ce_hwkm

/*===========================================================================
GENERAL DESCRIPTION
Crypto Engine HWKM wrapper Module API

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2020 by Qualcomm Technologies, Incorporated.
All Rights Reserved.
QUALCOMM Proprietary
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "comdef.h"
#include "rex.h"
#include "hwkm.h"
#include "hwkm_err.h"
#include "hwkm_hal.h"
#include "uclib.h"
/*===========================================================================
                 DEFINITIONS AND TYPE DECLARATIONS
===========================================================================*/

/**
 * @brief This function unwrap the key in PHK slot 
 *
 * @param wrap_key        [in ] Pointer to wrapped key
 * @param wrap_key_sz     [in] wrapped key size
 *                  
 * @return 
 *
 * @see
 *
 */

int hwkm_unwrap_import_phk (const uint8* wrap_key, size_t wrap_key_sz);

/**
 * @brief This function get the PHK l3 drived wrapped key from HWKM master
 *
 * @param algorithm          [in]  ALgorithm for which L3 is derived
 * @param wrap_key           [out] wraped key
 * @param wrap_key_sz        [in]  wrape_key buffer size
 * @param osz                [out] outpout length of wrapped key
 *
 * @return CeMLErrorType
 *
 * @see
 *
 */


int hwkm_wrap_export_phk(hwkm_alg_t algorithm, uint8* wrap_key, size_t wrap_key_sz, size_t *osz);

#endif