#ifndef HDRSRCHTRI_H
#define HDRSRCHTRI_H

/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                       H D R   T R I A G E   R O U T I N E S

                          HDR Triage Header File

GENERAL DESCRIPTION

   This file contains the databases, typedefs, enums and defines used by
   the HDR Triage algorithm.


EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000, 2001, 2002,
                2003 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$PVCSPath: L:/src/asw/COMMON/vcs/hdrsrchtri.h_v   1.6   11 Mar 2003 15:34:44   aneufeld  $
$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchtri.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------
03/11/2003   ajn     Updated Copyright notice
11/22/2000   ajn     Code review changes
10/23/2000   ajn     Added VCS Header line
07/31/2000   aaj     Removed typedefs from .h to .c
07/20/2000   aaj     Comment cleanup
07/05/2000   aaj     Created this file

==========================================================================*/


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "comdef.h"

#include "hdrsrchdrv.h"


/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

/*==========================================================================

                    FUNCTION PROTOTYPES USED IN THE MODULE

This section contains function prototypes for the functions used in this
module.

==========================================================================*/

/*=========================================================================

FUNCTION     : HDRSRCHTRI_TRIAGE

DESCRIPTION  : HDR Finger assignment algorithm

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Affects Finger/Sector/Path Tables

=========================================================================*/
extern void hdrsrchtri_triage ( hdrsrchdrv_srch_list_struct_type *srch_list,
                                int8 srch_cnt );

#endif /* HDRSRCHTRI_H */
