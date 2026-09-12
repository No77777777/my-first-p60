#ifndef SLIM_NV_EFS_H
#define SLIM_NV_EFS_H

#ifdef __cplusplus
extern "C" {
#endif

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           SLIM EFS NV Items Module Internal header file

GENERAL DESCRIPTION
  This file contains data-structure/function prototypes for EFS access in
  the SLIM module
     
EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  

  Copyright (c) 2018 Qualcomm Technologies, Inc. All Rights Reserved. 


Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*=============================================================================

                                INCLUDE FILES

=============================================================================*/
#include "comdef.h"
#include "gps_nv_efs.h"
#include "gps_types.h"


/*===========================================================================
=============================================================================

     Macros
   
=============================================================================
===========================================================================*/



/*===========================================================================
=============================================================================

   SLIM data structures and declarations
   
=============================================================================
===========================================================================*/

/* EFS NV for SLIM Test Mode Config */
typedef PACKED struct PACKED_POST
{
  uint8  u_TestMode;                     // 0=Test Mode Off(default), 1=Test Mode On(Use values below)
  uint8  u_InitProviderSetting;          // 0=SSC(default), 1=NATIVE, 2+ = reserved, see slim_ProviderSettingEnumType
  uint32 q_ProviderDisableMask;          // Bitmask for slim_ProviderEnumType, 0=SW Default, 1=Disable
  uint32 q_ProviderTimeSyncDisableMask;  // Bitmask for time-sync for slim_ProviderEnumType, 0=SW Default, 1=Disable
  byte   reserved[6];           // Don't display in NV browser
} slim_EfsNvTestConfigType;

/*==============================================================================

       Functions

==============================================================================*/

/*===========================================================================

FUNCTION slim_NvEfsInit

DESCRIPTION
  This function is used to initialize internal SAML-NV data efs structure.
  
RETURN VALUE
  None
  
DEPENDENCIES
 
===========================================================================*/
void slim_NvEfsInit(void);

/*===========================================================================

FUNCTION slim_NvEfsItemRead

DESCRIPTION
  This function is used to read NV item. If the read fails, it sets it to
  default value .
  
RETURN VALUE
  TRUE : If read operation is successful
  FALSE: Otherwise
  
DEPENDENCIES
 
===========================================================================*/
boolean slim_NvEfsItemRead(
   cgps_nv_efs_reg_item_type e_Item,
   uint8 b_DataSize,
   void *p_Data
   );

/*===========================================================================

FUNCTION slim_NvEfsItemWrite

DESCRIPTION
  This function is used to write a SAML-NV item. 

  
RETURN VALUE
  TRUE : If write operation is successful
  FALSE: Otherwise

  
DEPENDENCIES
 
===========================================================================*/
boolean slim_NvEfsItemWrite(
   cgps_nv_efs_reg_item_type e_Item,
   uint8 b_DataSize,
   const void *p_Data
   );

/*===========================================================================

FUNCTION slim_NvEfsGetTestConfig

DESCRIPTION
  This function reads NV item about SLIM Test config.
 
PARAMETER 
  pz_TestCfg : Slim Test Config
 
RETURN VALUE
  None
  
===========================================================================*/
void slim_NvEfsGetTestConfig(slim_EfsNvTestConfigType* pz_TestCfg);

#ifdef __cplusplus
}
#endif

#endif /* SLIM_NV_EFS_H */

