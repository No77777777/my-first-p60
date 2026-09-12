#ifndef __VCSRPMRAILIMPL_H__
#define __VCSRPMRAILIMPL_H__
/*
===========================================================================
*/
/**
  @file VCSRPMRailImpl.h

  Interface definitions for the RPM rail impl type.
*/
/*
  ====================================================================

  Copyright (c) 2017 Qualcomm Technologies Incorporated.  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ====================================================================

  $Header: //components/rel/core.mpss/10.0/systemdrivers/vcs/src/RailImpls/VCSRPMRailImpl.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "VCSBSP.h"


/*=========================================================================
      Extern data
==========================================================================*/

extern VCSRailControlType VCSRPMRailImplControl;

/*=========================================================================
      Type Definitions
==========================================================================*/

/*
 * VCSRPMRailImplType
 *
 * 
 */
typedef struct VCSRPMRailImplType
{
  const char* szLocalName;
  const char* szRemoteName;
  const char* szUnits;
} VCSRPMRailImplType;


#endif  /* __VCSRPMAILIMPL_H__ */ 

