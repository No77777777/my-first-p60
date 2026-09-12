/*==============================================================================
  FILE:         uimage_trans.c

  OVERVIEW:     This file provides uImage (software based) transition hardware routines

  DEPENDENCIES: None
  
                Copyright (c) 2017-2019 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/uSleep/src/transition/ddr/uimage_trans.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"
#include "CoreVerify.h"
#include "uimage_trans.h"
#include "icbarb.h"
#include "npa.h"

/*==============================================================================
                            INTERNAL REFERENCES
 =============================================================================*/

static const char* sleep_active_deps[] = 
{
  "/icb/arbiter",
};

/*==============================================================================
                       INTERNAL FUNCTION DEFINITIONS
 =============================================================================*/
static void uSleep_transNodeAvailable(void           *context,
                                      unsigned int   event_type,
                                      void           *data,
                                      unsigned int   data_size)
{
  return; 
}

/*==============================================================================
                          EXTERNAL FUNCTION DEFINITIONS
 =============================================================================*/
/*
 * uImageTrans_initialize
 */
void uImageTrans_initialize(void)
{
  /* Create query handles for DDR on/off ability */
  npa_resources_available_cb(NPA_ARRAY(sleep_active_deps),
                             uSleep_transNodeAvailable,
                             NULL);
  
  return;
}

/*
 * uImageTrans_prepareForEntry
 */
void uImageTrans_prepareForEntry(void)
{
  return;
}

