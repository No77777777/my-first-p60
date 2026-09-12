#ifndef WCDMADIAGIF_H
#define WCDMADIAGIF_H
/*===========================================================================
                 WCDMA DIAG Interface

GENERAL DESCRIPTION
  This file contains the code for exporting DIAG apis outside of modem.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2009 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/wcdma.mpss/8.4.0/api/public/wcdmadiagif.h#3 $
$DateTime: 2020/01/24 03:56:36 $
$Author: pwbldsvc $

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/01/09   scm     Remove nested comment.
10/07/09   rm      Initial Release

===========================================================================*/

/*===========================================================================
FUNCTION        WL1_GET_FRAME_NUMBER_FOR_DIAG

DESCRIPTION     This function returns the frame number for diag

PARAMETER       NONE

RETURN VALUE    uint32 value.
===========================================================================*/
extern uint32 wl1_get_frame_number_for_diag(void);


#endif /* WCDMADIAGIF_H */
