#ifndef DS_DSD_EFS_PARSER_H
#define DS_DSD_EFS_PARSER_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
         
                       D A T A   S E R V I C E S 
                        DSD EFS PARSER HEADER FILE

GENERAL DESCRIPTION

  All the declarations and definitions necessary to support parsing  
  of DSD related EFS file.

Copyright (c) 2003-2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*==================================================================
                        EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsd/src/ds_dsd_efs_parser.h#2 $

when       who         what, where, why
--------   -----       ----------------------------------------------------------
11/07/11   var         Checkin for first version of dsd_efs_parser.h

===========================================================================*/

/*===========================================================================
                              INCLUDE FILES
===========================================================================*/
#include "comdef.h"

/*===========================================================================
                              MACROS
===========================================================================*/


/*===========================================================================
                       EXTERNAL FUNCTION DEFINITIONS
===========================================================================*/

/*===========================================================================
FUNCTION      DS_DSD_EFS_PARSER_INIT

DESCRIPTION   This function initializes the EFS parser. This is during 
              power up. 
 
DEPENDENCIES  None.

RETURN VALUE  

SIDE EFFECTS  1. Reads EFS file is present.
===========================================================================*/
void ds_dsd_efs_parser_init(void);

#endif  /* DS_DSD_EFS_PARSER_H */

