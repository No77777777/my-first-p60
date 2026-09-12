#ifndef HDR1XMC_H
#define HDR1XMC_H
/*===========================================================================

                  HDR TO 1X EXTERNAL INTERFACE


DESCRIPTION
  This contains the declarations for the HDR to 1X external interface.

Copyright (c) 2003 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/hdr.mpss/6.0/cp/api/hdr1xmc.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/08/03   mpa     Created module

===========================================================================*/

/*===========================================================================

                    INCLUDE  FILES  FOR  MODULE

===========================================================================*/
#include "comdef.h"
#include "hdrcp_api.h"
/* <EJECT> */
/*===========================================================================

                           DEFINITIONS

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                        DATA  DECLARATIONS

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      FUNCTION  DECLARATIONS

===========================================================================*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDR1XMC_GET_LOCATION

DESCRIPTION
  This function externalizes the HDR Location Information.

DEPENDENCIES
  None

PARAMETERS
  location_ptr - A pointer to the location value of the current HDR system

RETURN VALUE
  TRUE - The location is valid and returned in location_ptr
  FALSE - The location is unknown or in a format unknown to IS2000-A

SIDE EFFECTS
  None
===========================================================================*/

extern boolean hdr1xmc_get_location ( hdr1xmc_location_type * location_ptr );


#endif /* HDR1XMC_H */
