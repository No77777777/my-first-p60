/*============================================================================
  FILE:         cxo_lpr.c

  OVERVIEW:     This file provides the LPR definition for the CXO clock 
                low-power modes.

  DEPENDENCIES: None

                Copyright (c) 2012-2013 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Technologies Confidential and Proprietary
================================================================================
$Header: //components/rel/core.mpss/10.0/power/sleep2.0/src/lpr/cxo_lpr.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "vmpm.h"

/*==============================================================================
                               GLOBAL FUNCTIONS
 =============================================================================*/
/**
 * @brief CXOShutdownLPR_exit
 *
 * Exit function which gets called while exiting from Sleep on master side for
 * a Sleep cycle when master voted for cxo shutdown.
 */
void CXOShutdownLPR_exit( void )
{
  /* Triggering any potential mpm interrupts that may have occured during xo 
   * shutdown. */
  vmpm_TriggerInterrupts();
  return;
}

