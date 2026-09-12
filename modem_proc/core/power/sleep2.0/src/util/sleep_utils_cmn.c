/*==============================================================================
  FILE:         sleep_utils_cmn.c
  
  OVERVIEW:     Provides sleep global general interest common functions
 
  DEPENDENCIES: None

                Copyright (c) 2016-2017 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/sleep2.0/src/util/sleep_utils_cmn.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include <stdlib.h>
#include "sleep_utils.h"

/*==============================================================================
                        INTERNAL FUNCTION DEFINITIONS
 =============================================================================*/
/*
 * safe_unsigned_addition
 */
uint64 safe_unsigned_addition(uint64 a, uint64 b)
{
  if((UINT64_MAX - a) < b)
  {
    /* clip to maximum */
    return UINT64_MAX;
  }

  return (a + b);
}

