#ifndef QURT_NON_SYSTEM_H
#define QURT_NON_SYSTEM_H 

/*======================================================================

                        qurt_system.h 

GENERAL DESCRIPTION
  Contains misc system calls

 EXTERNALIZED FUNCTIONS
  none

 INITIALIZATION AND SEQUENCING REQUIREMENTS
  none

 Copyright (c) 2009-2011
 by Qualcomm Technologies, Inc. All Rights Reserved.
 ======================================================================*/

#include "qurt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
                        FUNCTIONS
=============================================================================*/

  /*
   * Perform a jump from a trusted user mode
   *
   * @param addr    Address where control will resume
   *
   */ 
void qurt_system_jump (qurt_addr_t addr);

#endif /* QURT_SYSTEM_H */
