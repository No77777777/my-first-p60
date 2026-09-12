/*=============================================================================

                             LOCTECH Circle Util API Header file

GENERAL DESCRIPTION
  This file contains the structure definitions and function prototypes 
  relevant to loctech circle support.
  
EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  
Copyright (c) 2019 Qualcomm Technologies, Inc. 
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

=============================================================================*/

/*=============================================================================

                           EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who      what, where, why
  ----------  ---  -----------------------------------------------------------
  4/04/19    dhiman    Initial creation of the file

=============================================================================*/

#ifndef LOCTECH_CIRCLE_UTIL_API_H
#define LOCTECH_CIRCLE_UTIL_API_H

#include "comdef.h"
#include "gps_common.h"
#include "aries_os_api.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*Coordinate Type*/
typedef struct
{
  double   d_Latitude;  /* Lat.: in degrees -90.0 to 90.0, positive for northern */
  double   d_Longitude; /* Long.: in degrees -180.0 to 180.0, positive for eastern */
}loctech_coordinate_s_type;

/*Circle Type*/
typedef struct
{
  uint32  u_radius; /*radius of a circle*/
  loctech_coordinate_s_type *pz_Coordinates; /*Pointer to the location of center of the circle*/
}loctech_circle_s_type;

/* position */
typedef struct
{
  loctech_coordinate_s_type   z_Coordinate;
  double    d_HorUncCirc;   /* horizontal location unc.: meters * this will be zero 
  in case client doesn't provide the input */
}loctech_position_lite_s_type;

typedef enum
{
	BREACH_RESULT_UNSPEC = 0,
	BREACH_RESULT_OUTSIDE,
	BREACH_RESULT_INSIDE,
	BREACH_RESULT_NEED_POS
}loctech_breach_result;
/*=============================================================================

FUNCTION 
  loctech_circle_breach

DESCRIPTION
  This function detects whether the position is inside\outside\unknown
  a circle.

DEPENDENCIES
  None.

PARAMETERS
  MANDATORY: pz_Circle - pointer to Circle. 
  MANDATORY: pz_Position - pointer to Position

RETURN VALUE
	loctech_breach_result:
  	BREACH_RESULT_OUTSIDE if position is outside the geofence.
	BREACH_RESULT_INSIDE if it's inside,
	and BREACH_RESULT_NEED_POS if the algorithm can't determine the relationship (for
	example, if the position inaccuracy intersects the geofence circle boundary).

SIDE EFFECTS
  None.

=============================================================================*/
loctech_breach_result loctech_circle_breach(const loctech_circle_s_type* const pz_Circle,
                                                          const loctech_position_lite_s_type* const pz_Position, loctech_breach_result prev);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*LOCTECH_CIRCLE_UTIL_API_H*/
