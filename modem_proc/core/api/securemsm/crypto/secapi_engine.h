#ifndef SECAPI_ENGINE
#define SECAPI_ENGINE

/** 
  @brief General Cryptographic implementation/engine types
  Defines the type of engine/implementation to use for the cryptographic
  operation.
 * 
 */

/* ====================================================================
 * Copyright (c) 2017 - 2020 Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary – Qualcomm Technologies, Inc.
 * ====================================================================
 */

#include <stdint.h>

/**
  Crypto implementation/engine types
*/
typedef enum {
  SECAPI_CE_SW,             /**< SW Crypto Engine */
  SECAPI_CE_BAM,            /**< HW Crypto Engine using BAM Access */

  SECAPI_ENGINE_MAX_COUNT   /**< Maximum number of engine types */
}SECAPI_ENGINE_TYPE;

#endif
