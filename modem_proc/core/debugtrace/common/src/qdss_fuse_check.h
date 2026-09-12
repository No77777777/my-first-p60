/*=========================================================================
                       QDSS e-fuse check

GENERAL DESCRIPTION
   The interface to check efuses associated with the QDSS for accessing its
   trace functionality is implemented. Fuse checking is done using the STM
   AUTHSTATUS register.

EXTERNALIZED FUNCTIONS
   qdss_fuse_trace_access

INITIALIZATION AND SEQUENCING REQUIREMENTS
   Requires System FPB clock to access the efuse registers.

==========================================================================*/
/*=============================================================================
  Copyright (c) 2012-2014 Qualcomm Technologies, Inc.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/
/*==========================================================================
 $Header: //components/rel/core.mpss/10.0/debugtrace/common/src/qdss_fuse_check.h#1 $
==========================================================================*/

#define QDSS_TRACE_ACCESS_DENIED   0  // Fuse(s) blown, do not permit trace access
#define QDSS_TRACE_ACCESS_ALLOWED  1  // Fuse(s) intact, trace access is permitted

/*---------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
Description:
   Checks the efuse associated with QDSS and returns whether
   trace access is possible.

Return:  QDSS_TRACE_ACCESS_DENIED
         QDSS_TRACE_ACCESS_ALLOWED
*/
int qdss_fuse_trace_access(void);

