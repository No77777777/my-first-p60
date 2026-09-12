#ifndef SLIP_IFACE_EXT_H
#define SLIP_IFACE_EXT_H
/*===========================================================================

              S L I P _ I F A C E _ E X T . H

DESCRIPTION
   This is SLIP iface handler external header file. It contains the
   external function declarations.


Copyright (c) 2009 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

  $Header: //components/rel/data.mpss/3.5..2.1/interface/api/slip_iface_ext.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"

/*===========================================================================
  List of commands external modules post to DCC task for SLIP processing
  - SLIP_IFACE_CALLP_UP_CMD - CallP UP command
===========================================================================*/
#define SLIP_IFACE_CALLP_UP_CMD 1

/*===========================================================================

FUNCTION SLIP_IFACE_POST_CMD

DESCRIPTION
  This function posts corresponding SLIP command to DCC task for processing.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  System asserts if command posting fails.

===========================================================================*/
void slip_iface_post_cmd
( 
  uint32 cmd
);

#endif /* SLIP_IFACE_EXT_H */
