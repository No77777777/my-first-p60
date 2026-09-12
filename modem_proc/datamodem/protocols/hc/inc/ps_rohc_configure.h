#ifndef ROHC_CONFIGURE_H
#define ROHC_CONFIGURE_H
/*===========================================================================
                        P S _ R O H C _ C O N F I G U R E . H

DESCRIPTION
  This file contains the compile time configuration parameters divided into
  configurable and non-configurable compile time values.

Copyright (c) 2006-2009 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/protocols/hc/inc/ps_rohc_configure.h#2 $
  $Author: pwbldsvc $
  $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
03/02/09    pp     ZI optimizations. Removed ROHC heap configurable param.
07/06/05    ifk    Created module.
===========================================================================*/
/*===========================================================================

                             INCLUDE FILES 

===========================================================================*/
#include "comdef.h"
#include "customer.h"


/*===========================================================================

                             CONFIGURABLE PARAMETERS

===========================================================================*/
/*---------------------------------------------------------------------------
  Size of the memory heap used by ROHC
---------------------------------------------------------------------------*/
#define ROHC_MEMHEAP_SIZE  65535    /* Size of the memheap used by ROHC    */

/*---------------------------------------------------------------------------
  Enumeration of configurable parameters
---------------------------------------------------------------------------*/
typedef enum rohci_dynamic_config_e_type
{
  ROHCI_DYNAMIC_CONFIG_MEMHEAP_SIZE = 0,
  ROHCI_DYNAMIC_CONFIG_MAX
} rohci_dynamic_config_e_type;


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION ROHCI_GET_CONFIG

DESCRIPTION
  Function called by ROHC library to acquire a configurable compile time
  parameter.

DEPENDENCIES
  None

RETURN VALUE
   0  Success
  -1 Failure

SIDE EFFECTS
  None
===========================================================================*/
int rohci_get_config
(
  rohci_dynamic_config_e_type  config_param,
  void                        *config_val,
  uint32                       config_val_size 
);
#endif /* ROHC_CONFIGURE_H */
