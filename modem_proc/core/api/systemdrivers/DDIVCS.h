#ifndef DDIVCS_H
#define DDIVCS_H
/*
===========================================================================
*/
/**
  @file DDIVCS.h
  @brief Public interface include file for accessing the VCS device driver.

  The DDIVCS.h file is the public API interface to the VCS 
  Device Access Library (DAL).
*/
/*===========================================================================
NOTE: The @brief description and any detailed descriptions above do not appear 
      in the PDF. 

      The VCS_API_mainpage.dox file contains all file/group descriptions that 
      are in the output PDF generated using Doxygen and LaTeX. To edit or 
      update any of the file/group text in the PDF, edit the 
      VCS_API_mainpage.dox file or contact Tech Pubs.

===========================================================================*/
/*
  ===========================================================================

  Copyright (c) 2014 QUALCOMM Technologies Incorporated.  
  All Rights Reserved.
  QUALCOMM Proprietary and Confidential.

  ===========================================================================

  $Header: //components/rel/core.mpss/10.0/api/systemdrivers/DDIVCS.h#1 $
  $DateTime: 2019/04/24 00:03:26 $
  $Author: pwbldsvc $

  when       who     what, where, why
  --------   ---     ---------------------------------------------------------- 

  ===========================================================================
*/


/*=========================================================================
      Include Files
==========================================================================*/

#include "DALSys.h"
#include "VCSDefs.h"


/*=========================================================================
      Type Definitions
==========================================================================*/


/**
  Defines the interface version.
 */
#define DALVCS_INTERFACE_VERSION DALINTERFACE_VERSION(1,0)


/*=========================================================================
      Interface Definitions
==========================================================================*/

/** @cond 
*/

typedef struct DalVCS DalVCS;
struct DalVCS
{
   struct DalDevice DalDevice;
};

typedef struct DalVCSHandle DalVCSHandle;
struct DalVCSHandle
{
   uint32 dwDalHandleId;
   const DalVCS * pVtbl;
   void * pClientCtxt;
};

#define DAL_VCSDeviceAttach(DevId,hDalDevice)\
        DAL_DeviceAttachEx(NULL,DevId,DALVCS_INTERFACE_VERSION,hDalDevice)

/** @endcond */


/*=========================================================================
      Interface Declarations
==========================================================================*/


/** @addtogroup rail_functions
@{ */


/** @} */ /* end_addtogroup ldo_functions */


#endif /* !DDIVCS_H */

