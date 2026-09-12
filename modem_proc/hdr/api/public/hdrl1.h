#ifndef HDRL1_H
#define HDRL1_H

/*===========================================================================

        E X T E R N A L   H D R   L A Y E R 1   W R A P P E R

DESCRIPTION
  This contains the external declarations for the HDRMC Task.

Copyright (c) 2019 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/hdr.mpss/6.0/api/public/hdrl1.h#2 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
12/04/19   vaa     Changes to support clk plan V2.0 FR changes
02/14/12   smd     Removed power strobe APIs and data type. 
08/28/09   wsh     CMI: Removing conditional defines from API headers
07/21/09   wsh     Created module for public definitions in HDR L1

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "qw.h"

/* <EJECT> */ 
/*===========================================================================

             DEFINITIONS  FROM  THE  HDR  AIR  INTERFACE  SPEC

===========================================================================*/


/* <EJECT> */ 
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
typedef qword hdr_timestamp_t;

/* DO MCPM scenario type */

typedef enum
{
   HDR_IRAT_D2L_MEAS = 1
     /* Clk boost for D2L measurement scenario */
  ,HDR_RXD_ENABLE
     /* Clk boost for diversity scenario */
  ,HDR_QTA
     /* Clk boost for QTA scenario */
  ,HDR_NQTA
     /* Clk boost for nQTA scenario */
  ,HDR_MAX_SCENARIO = HDR_NQTA
     /* Max scenario number */
  ,HDR_INVALID_SCENARIO
  ,HDR_DEFAULT_SCENARIO = HDR_INVALID_SCENARIO
     /* Unused scenario number */
} hdr_scenario_type;


/* <EJECT> */ 
/*===========================================================================

                        FUNCTION DECLARATIONS

===========================================================================*/

/* <EJECT> */ 
/*===========================================================================

FUNCTION HDRL1_TIME_INVALIDATE

DESCRIPTION
  This function asserts that HDR CDMA time is no longer reliable.  1x CDMA
  time, or Time-of-Day should be used instead.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Time is marked as invalid.
===========================================================================*/
void hdrl1_time_invalidate( void );

/* <EJECT> */ 
/*===========================================================================

FUNCTION HDRL1_TIME_GET

DESCRIPTION
  This function gets the current HDR time.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if HDR Time is valid, and return the timestamp in 'ts_val'
  FALSE if HDR Time is invalid.

SIDE EFFECTS
  None
===========================================================================*/
boolean hdrl1_time_get
( 
  hdr_timestamp_t ts 
);

#endif /* HDRL1_H */


