#ifndef _UCLIB_ENGINE
#define _UCLIB_ENGINE

/* ====================================================================
 * Copyright (c) 2017 - 2020 Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary – Qualcomm Technologies, Inc.
 * ====================================================================
 */

/*===========================================================================

Unified Crypto Library - Engines

GENERAL DESCRIPTION


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include <stdint.h>

/*===========================================================================
                 DEFINITIONS AND TYPE DECLARATIONS
 ===========================================================================*/
typedef enum {
  UCLIB_ENGINE_INV = 0,
  UCLIB_CE_SW,     /** SW Crypto Engine */
  UCLIB_CE_ARMV8,  /** Crypto Engine using ARMv8 CE extention */
  UCLIB_CE_HWIO,   /** HW Crypto Engine using HWIO Access */
  UCLIB_CE_BAM,    /** HW Crypto Engine using BAM Access */
  UCLIB_CE_PKA,    /** Public Key Acceleration Engine v1 */
  UCLIB_CE_HRNG,   /** HW PRNG Engine */
  UCLIB_CE_TME_SEQ, /** Crypto Engine using the TME Sequencer */
  UCLIB_CE_TMECOM,  /** Remote Engine */
  UCLIB_CE_ARMV8_2, /** Crypto Engine using ARMv8.2 Crypto extensions */

  UCLIB_ENGINE_MAX_COUNT
}UCLIB_ENGINE_TYPE;

#endif /* _UCLIB_ENGINE */
