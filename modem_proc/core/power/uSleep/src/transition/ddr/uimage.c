/*==============================================================================
  FILE:         uimage.c

  OVERVIEW:     This file provides uimage (software based) transition functions

  DEPENDENCIES: Functions are located in normal mode memory section
  
                Copyright (c) 2016-2019 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/uSleep/src/transition/ddr/uimage.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"
#include "uSleep_transi.h"
#include "CoreVerify.h"
#include "npa.h"
#include "rpmclient.h"
#include "uimage_trans.h"

/*==============================================================================
                            DEFINITIONS & TYPES
 =============================================================================*/
/* String definitions for the resources required to turn DDR on/off */
#define USLEEP_NPA_RESOURCE_SNOC  "/clk/snoc"
#define USLEEP_NPA_RESOURCE_BIMC  "/clk/bimc"

/*==============================================================================
                             EXTERNAL VARIABLES
 =============================================================================*/
extern uint32 g_uImageBIMCFreq;
extern uint32 g_uImageSNOCFreq;

/*==============================================================================
                             INTERNAL VARIABLES
 =============================================================================*/
/* NPA query handles for BIMC and SNOC frequencies so DDR can be turned on/off */
static npa_query_handle g_uSleepBIMCQueryHandle;
static npa_query_handle g_uSleepSNOCQueryHandle;

/*==============================================================================
                         EXTERNAL FUNCTION DEFINITIONS
 =============================================================================*/
/*
 * uSleep_transitionInitialize
 */
void uSleep_transitionInitialize(void)
{
  /* Create frequency query handles for DDR on/off ability */
  CORE_VERIFY_PTR(g_uSleepBIMCQueryHandle = 
                  npa_create_query_handle(USLEEP_NPA_RESOURCE_BIMC));

  CORE_VERIFY_PTR(g_uSleepSNOCQueryHandle = 
                  npa_create_query_handle(USLEEP_NPA_RESOURCE_SNOC));

  /* Setup required transition data */
  uImageTrans_initialize();
  return;
}

/*==============================================================================
                              GLOBAL EXIT FUNCTIONS
 =============================================================================*/
/*
 * uSleep_completeExit
 * 
 * Performed after kernel stage 1 exit call
 */
void uSleep_completeExit(uSleep_transition_mode mode)
{
  /* Reenable RPM interrupt that was disabled from the set sync on island entry */
  rpm_mask_interrupt(false);

  return;
}

/*==============================================================================
                              GLOBAL ENTER FUNCTIONS
 =============================================================================*/
/*
 * uSleep_prepareForEntry
 */
void uSleep_prepareForEntry(void)
{
  npa_query_type qres;

  /* Disable the RPM interrupt so the sync calls do not cause us to
   * exit uImage with the ACK interrupt */
  rpm_mask_interrupt(true);

  /* Get current snoc & bimc frequency votes. Note that we still have access
   * to NPA while we are normal operational mode */
  CORE_VERIFY(NPA_QUERY_SUCCESS == npa_query(g_uSleepBIMCQueryHandle,
                                             NPA_QUERY_CURRENT_STATE,
                                             &qres));
  g_uImageBIMCFreq = qres.data.value;
  CORE_VERIFY(g_uImageBIMCFreq > 0);

  CORE_VERIFY(NPA_QUERY_SUCCESS == npa_query(g_uSleepSNOCQueryHandle,
                                             NPA_QUERY_CURRENT_STATE,
                                             &qres));
  g_uImageSNOCFreq = qres.data.value;

  uImageTrans_prepareForEntry();
  return;
}

