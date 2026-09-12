#ifndef HDRRMACMATH_H
#define HDRRMACMATH_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             H D R   R T C M A C   P R O T O C O L
                                    
                    M A T H   R O U T I N E S 

GENERAL DESCRIPTION
  This module contains math functions used by the RTCMAC3 algorithm

EXTERNALIZED FUNCTIONS

  hdrrmacmath_lin_2_db( uint32 linear_q22x10 )
    Convert from linear to dB

  hdrrmacmath_db_2_lin( int32 dB_q11x20 )
    Convert from dB to linear 

  hdrrmacmath_dbm_2_mw( int32 x_q21x10 )
    Convert from dbm to mW

  hdrrmacmath_mw_2_dbm(int32 x_q10x22)
    Convert from mW to dbm

  hdrrmacmath_interpolate_2d
    Bilinear interpolation routine

  hdrrmacmath_interpolate_1d
    Linear interpolation routine

  hdrrmacmath_min_u32
    Minimum of two unsigned 32 bit numbers
    
INITIALIZATION AND SEQUENCING REQUIREMENTS
  hdrrmac3c_powerup_init must be called at startup.  

  Copyright (c) 2001-2004 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrrmacmath.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/06/08   rmg     Corrected hdrrmacmath_mw_2_dbm().
11/29/07   rmg     Added hdrrmacmath_mw_2_dbm().
10/26/07   rmg     Added hdrrmacmath_dbm_2_mw().
06/20/07   cc      Renamed to hdrrmacmath.
01/27/06   sr      Exported exp_2() and log_2() functions
08/25/05   sr      Added two interface functions to do dB-to-linear conversion
                   using LUTs.
04/29/05   sr      Created file

===========================================================================*/
#include "hdr_variation.h"
#include "customer.h"

#ifdef FEATURE_HDR_REVA_L1

#define HDRRMACMATH_MAX_AXIS_PTS 16
  /* Maximum possible points on an x or y axis */
#define HDRRMACMATH_MAX_T2P_PTS 16
#define HDRRMACMATH_MAX_FRAB_PTS 8

typedef struct
{
  uint8  num_points;
    /* Number of points; could be less than MAX_AXIS_PTS */
  int32  values[HDRRMACMATH_MAX_T2P_PTS];
    /* The values that define this axis */
} hdrrmacmath_t2p_axis_struct_type;
  /* General one-dimensional "grid" type used in linear (1D) interpolation */

typedef struct
{
  uint8 num_points;
    /* Number of points; could be less than MAX_AXIS_PTS */
  int32 values[HDRRMACMATH_MAX_FRAB_PTS];
    /* The values that define this axis */
} hdrrmacmath_frab_axis_struct_type;
  /* General one-dimensional "grid" type used in linear (1D) interpolation */


/*=============================================================================
FUNCTION hdrrmacmath_LIN_2_DB                                         EXTERNAL

DESCRIPTION
  This function converts alinear value in Q22.10 format to dB in Q11.20 format.
  Both quantities are 32 bits wide. The number after the "." represents the
  fractional part or the resolution of the fixed-point representation.

DEPENDENCIES
  None.

PARAMETERS
  The linear value input in Q22.10

RETURN VALUE
  The dB representation in Q11.20

SIDE EFFECTS
  None.
===========================================================================*/
int32 hdrrmacmath_lin_2_db( uint32 linear_q22x10 );


/*=============================================================================
FUNCTION hdrrmacmath_DB_2_LIN                                         EXTERNAL

DESCRIPTION
  This function converts a dB value in Q11.20 format to a linear value in
  Q22.10 format.

DEPENDENCIES
  NOTE: Input saturation point [-30.0996dB, 66dB] in q11.20

PARAMETERS
  The input dB value represented in Q11.20

RETURN VALUE
  The linear value result in Q22.10

SIDE EFFECTS
  None.
===========================================================================*/
uint32 hdrrmacmath_db_2_lin( int32 dB_q11x20 );


/*=============================================================================
FUNCTION hdrrmacmath_DB_2_LIN_Q22x10                                  EXTERNAL

DESCRIPTION
  This function converts a dB value in 0.25 dB steps to a linear value in
  Q22.10 format using a LUT

DEPENDENCIES

PARAMETERS
  The input dB value in [0..63.75] dB

RETURN VALUE
  The linear value result in Q22.10

SIDE EFFECTS
  None.
===========================================================================*/
uint32 hdrrmacmath_db_2_lin_q22x10( int32 dB_q11x20 );


/*=============================================================================
FUNCTION hdrrmacmath_DB_2_LIN_Q6x10                                   EXTERNAL

DESCRIPTION
  This function converts a dB value in 1 dB steps to a linear value in
  Q6.10 format using a LUT

DEPENDENCIES

PARAMETERS
  The input dB value in [-16..15] dB

RETURN VALUE
  The linear value result in Q6.10

SIDE EFFECTS
  None.
===========================================================================*/
uint16 hdrrmacmath_db_2_lin_q6x10( int16 dB );


/*=============================================================================
FUNCTION hdrrmacmath_dbm_2_mw                                         EXTERNAL

DESCRIPTION
  This function converts a dBm value in Q21.10 format in the range of 
  [-60 dBm, 23 dBm] to milliwatts in Q10.22 format.

DEPENDENCIES
  NOTE: Input saturation point [-66 dBm, 30 dBm] in Q21.10

PARAMETERS
  The input dBm value represented in Q21.10

RETURN VALUE
  The converted value in milliwatts in Q10.22 format.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 hdrrmacmath_dbm_2_mw( int32 x_q21x10 );


/*=============================================================================
FUNCTION hdrrmacmath_mw_2_dbm                                         EXTERNAL

DESCRIPTION
  This function converts a mW value in Q10.22 format in the range of 
  [2.5119e-07 mW, 1000 mW] to dbm value in Q21.10 format.

DEPENDENCIES
  NOTE: Input saturation point [2.5119e-07 mW, 1000 mW] in Q10.22

PARAMETERS
  The input mW value represented in Q10.22

RETURN VALUE
  The converted value in dBm in Q21.10 format.

SIDE EFFECTS
  None.
===========================================================================*/
int32 hdrrmacmath_mw_2_dbm(uint32 x_q10x22);


/*=============================================================================
FUNCTION hdrrmacmath_INTERPOLATE_2D                                   EXTERNAL

DESCRIPTION
  This function performs bi-linear interpolation on the T2P (rows), FRAB 
  (columns) grid. See section 15-12 of IS-856-A

                       .
                       .
                       .
                       .  
                       |    |   |   |   |   |   |   |   |
                       |  --z---z---z---z---z---z---z---z--
                       |    |   |   |   |   |   |   |   |
                       |  --z---z---z---z---z---z---z---z--
                       T    |   |   |   |   |   |   |   |
                       2  --z---z---z---z---z---z---z---z--
                       P    |   |   |   |   |   |   |   |
                       |  --z---z---z---z---z---z---z---z--
                       |    |   |   |   |   |   |   |   |
                       |
                            ----------- FRAB ------------. . . .

DEPENDENCIES
  None.

PARAMETERS
  The T2P (x') and FRAB (y'), the x and y axes inputs.

RETURN VALUE
  z' = f(x', y'), the interpolated value

SIDE EFFECTS
  None.
===========================================================================*/
int32 hdrrmacmath_interpolate_2d
( 
  int32                               t2p,               /* x' */
  int32                               frab,              /* y' */
  hdrrmacmath_t2p_axis_struct_type  *t2p_axis_ptr,      /* x axis */
  hdrrmacmath_frab_axis_struct_type *frab_axis_ptr,     /* y axis */
  int32                               t2p_frab_grid[][HDRRMACMATH_MAX_FRAB_PTS]
                                        /* z = f(x,y), the defined values at 
                                         * all (x,y) belonging to  
                                         * {x-axis X y-axis}
                                         */
);


/*=============================================================================
FUNCTION hdrrmacmath_INTERPOLATE_1D                                   EXTERNAL

DESCRIPTION
  This function performs linear interpolation. See section 15-11 of IS-856-A

DEPENDENCIES
  None.

PARAMETERS
  x_prime is the input value (x').
  x is apointer to the points that define the (x) axis
  f_x is a pointer to the values of the function at these points, i.e., f(x)
  num_points is the number of points that are defined on the axis, x

RETURN VALUE
   y' = f(x'), the interpolated value

SIDE EFFECTS
  None.
===========================================================================*/
int32 hdrrmacmath_interpolate_1d
( 
  int32                               x_prime,    /* Input, x'                */
  int32                              *x,          /* points defining the axis */
  int32                              *f_x,        /* f(x): value @ these pts  */
  uint8                               num_points  /* # points on axis         */
);


/*=============================================================================
FUNCTION hdrrmacmath_min_u32                                          EXTERNAL

DESCRIPTION
  This function finds the min of 2 unsigned 32 bit numbers

DEPENDENCIES
  None.

PARAMETERS
  The two unsigned 32 bit numbers, a & b

RETURN VALUE
  min(a, b)

SIDE EFFECTS
  None.
===========================================================================*/
uint32 hdrrmacmath_min_u32
(
  uint32 a,
  uint32 b
);


/*=============================================================================
FUNCTION hdrrmacmath_msb_pos                                          INTERNAL

DESCRIPTION
  This function finds the most significant bit position (0 to 31) in a 32 bit
  unsigned number

DEPENDENCIES
  None.

PARAMETERS
  Unisigned 32 bit input, x

RETURN VALUE
  Most significant bit position in x 

SIDE EFFECTS
  None.
===========================================================================*/
uint32 hdrrmacmath_msb_pos( uint32 x );


/*=============================================================================
FUNCTION hdrrmacmath_EXP_2                                            INTERNAL

DESCRIPTION
  This function calculates 2^x in fixed-point.

DEPENDENCIES
  None.

PARAMETERS
  The exponent to which 2 is to be raised

RETURN VALUE
  64-bit fixed-point result = 2^x

SIDE EFFECTS
  None.
===========================================================================*/
uint64 hdrrmacmath_exp_2(int32 x_q11x20);


/*=============================================================================
FUNCTION hdrrmacmath_LOG_2                                            EXTERNAL

DESCRIPTION
  This function calculates log2 (log base 2) in fixed_point.

DEPENDENCIES
  None.

PARAMETERS
  The input in Q32.0 whose log2 value is desired

RETURN VALUE
  32-bit fixed-point result of log2( input )

SIDE EFFECTS
  None.
===========================================================================*/
uint32 hdrrmacmath_log_2(uint32 x_q32x0);

#endif /* FEATURE_HDR_REVA_L1 */

#endif /* HDRRMACMATH_H */
