
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        INTERRAT SURROUND CELL

GENERAL DESCRIPTION
   This module contains routines specific to performing TDS interRAT
   measurements while in GSM.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_irat_filter.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
23/02/16   sk       CR973190 Excessive F3 logging reduction
30/03/15   cja       CR800650 Tidy-up SC IRAT state machine
                     Initial version - files moved from l1_sc_irat.
===========================================================================*/


#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "l1_sc_irat.h"

#include "l1_sc.h"


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/*------------------------------------------------------------------------------------------------
            constants, macros' definition for l1_sc_g2x_lin_2_db
------------------------------------------------------------------------------------------------*/
/* MSB determination lookup table (byte hash look up table) */
LOCAL const uint16 l1_sc_g2x_msb_byte_lut[ 256 ] =
{
    0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,

    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,

    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,

    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

static const int32 l1_sc_g2x_log2_LUT_q0x16_M  = 5;
const uint32 l1_sc_g2x_log2_LUT_q0x20[] = /* log2 table in Q.20 format */
{
    121,    46667,   91820, 135666, 178277, 219720,  260058, 299349,
    337645, 374995, 411446, 447039, 481814, 515808,  549054, 581586,
    613432, 644622, 675183, 705138, 734511, 763325,  791600, 819357,
    846613, 873387, 899696, 925555, 950979, 975983, 1000580,1024784,
    1048607
};

/** the following is for dBm "+" operation */
#define TX_THRESHOLD_1 6  ///< 6 dB
#define TX_THRESHOLD_2 13 ///< 6-13 dB
#define TX_THRESHOLD_3 30 ///< 30 dB
#define TX_STEP_1      32 ///< 1/32 steps per dB
#define TX_STEP_2      16 ///< 1/16 steps per dB
#define TX_STEP_3      16  ///< 1/8 (was 1/4) steps per dB
#define TX_SHIFT_1     4  ///< index Q9 to Q5
#define TX_SHIFT_2     1  ///< index Q5 to Q4
#define TX_SHIFT_3     1  ///< index Q5 to Q4 (was Q2)

#define TX_SHIFT_2_Q9  5  ///< index Q9 to Q4
#define TX_SHIFT_3_Q9  5  ///< index Q9 to Q4

#define FIRST_BIAS  7.3
#define SECOND_BIAS 1.9
#define BIAS_OFFSET 0.5

#define ABS(a) (((a) > 0)?(a):-(a))

const static uint8 dBm_comb_LUT[576] =
{
          192, ///< 0.000000 dBm in U8Q6
          191, ///< -0.031250 dBm in U8Q6
          190, ///< -0.062500 dBm in U8Q6
          189, ///< -0.093750 dBm in U8Q6
          188, ///< -0.125000 dBm in U8Q6
          187, ///< -0.156250 dBm in U8Q6
          186, ///< -0.187500 dBm in U8Q6
          185, ///< -0.218750 dBm in U8Q6
          184, ///< -0.250000 dBm in U8Q6
          183, ///< -0.281250 dBm in U8Q6
          182, ///< -0.312500 dBm in U8Q6
          181, ///< -0.343750 dBm in U8Q6
          180, ///< -0.375000 dBm in U8Q6
          179, ///< -0.406250 dBm in U8Q6
          179, ///< -0.437500 dBm in U8Q6
          178, ///< -0.468750 dBm in U8Q6
          177, ///< -0.500000 dBm in U8Q6
          176, ///< -0.531250 dBm in U8Q6
          175, ///< -0.562500 dBm in U8Q6
          174, ///< -0.593750 dBm in U8Q6
          173, ///< -0.625000 dBm in U8Q6
          172, ///< -0.656250 dBm in U8Q6
          171, ///< -0.687500 dBm in U8Q6
          170, ///< -0.718750 dBm in U8Q6
          169, ///< -0.750000 dBm in U8Q6
          168, ///< -0.781250 dBm in U8Q6
          167, ///< -0.812500 dBm in U8Q6
          166, ///< -0.843750 dBm in U8Q6
          166, ///< -0.875000 dBm in U8Q6
          165, ///< -0.906250 dBm in U8Q6
          164, ///< -0.937500 dBm in U8Q6
          163, ///< -0.968750 dBm in U8Q6
          162, ///< -1.000000 dBm in U8Q6
          161, ///< -1.031250 dBm in U8Q6
          160, ///< -1.062500 dBm in U8Q6
          159, ///< -1.093750 dBm in U8Q6
          158, ///< -1.125000 dBm in U8Q6
          158, ///< -1.156250 dBm in U8Q6
          157, ///< -1.187500 dBm in U8Q6
          156, ///< -1.218750 dBm in U8Q6
          155, ///< -1.250000 dBm in U8Q6
          154, ///< -1.281250 dBm in U8Q6
          153, ///< -1.312500 dBm in U8Q6
          152, ///< -1.343750 dBm in U8Q6
          152, ///< -1.375000 dBm in U8Q6
          151, ///< -1.406250 dBm in U8Q6
          150, ///< -1.437500 dBm in U8Q6
          149, ///< -1.468750 dBm in U8Q6
          148, ///< -1.500000 dBm in U8Q6
          147, ///< -1.531250 dBm in U8Q6
          147, ///< -1.562500 dBm in U8Q6
          146, ///< -1.593750 dBm in U8Q6
          145, ///< -1.625000 dBm in U8Q6
          144, ///< -1.656250 dBm in U8Q6
          143, ///< -1.687500 dBm in U8Q6
          143, ///< -1.718750 dBm in U8Q6
          142, ///< -1.750000 dBm in U8Q6
          141, ///< -1.781250 dBm in U8Q6
          140, ///< -1.812500 dBm in U8Q6
          139, ///< -1.843750 dBm in U8Q6
          139, ///< -1.875000 dBm in U8Q6
          138, ///< -1.906250 dBm in U8Q6
          137, ///< -1.937500 dBm in U8Q6
          136, ///< -1.968750 dBm in U8Q6
          135, ///< -2.000000 dBm in U8Q6
          135, ///< -2.031250 dBm in U8Q6
          134, ///< -2.062500 dBm in U8Q6
          133, ///< -2.093750 dBm in U8Q6
          132, ///< -2.125000 dBm in U8Q6
          132, ///< -2.156250 dBm in U8Q6
          131, ///< -2.187500 dBm in U8Q6
          130, ///< -2.218750 dBm in U8Q6
          129, ///< -2.250000 dBm in U8Q6
          129, ///< -2.281250 dBm in U8Q6
          128, ///< -2.312500 dBm in U8Q6
          127, ///< -2.343750 dBm in U8Q6
          126, ///< -2.375000 dBm in U8Q6
          126, ///< -2.406250 dBm in U8Q6
          125, ///< -2.437500 dBm in U8Q6
          124, ///< -2.468750 dBm in U8Q6
          124, ///< -2.500000 dBm in U8Q6
          123, ///< -2.531250 dBm in U8Q6
          122, ///< -2.562500 dBm in U8Q6
          121, ///< -2.593750 dBm in U8Q6
          121, ///< -2.625000 dBm in U8Q6
          120, ///< -2.656250 dBm in U8Q6
          119, ///< -2.687500 dBm in U8Q6
          119, ///< -2.718750 dBm in U8Q6
          118, ///< -2.750000 dBm in U8Q6
          117, ///< -2.781250 dBm in U8Q6
          116, ///< -2.812500 dBm in U8Q6
          116, ///< -2.843750 dBm in U8Q6
          115, ///< -2.875000 dBm in U8Q6
          114, ///< -2.906250 dBm in U8Q6
          114, ///< -2.937500 dBm in U8Q6
          113, ///< -2.968750 dBm in U8Q6
          112, ///< -3.000000 dBm in U8Q6
          112, ///< -3.031250 dBm in U8Q6
          111, ///< -3.062500 dBm in U8Q6
          110, ///< -3.093750 dBm in U8Q6
          110, ///< -3.125000 dBm in U8Q6
          109, ///< -3.156250 dBm in U8Q6
          108, ///< -3.187500 dBm in U8Q6
          108, ///< -3.218750 dBm in U8Q6
          107, ///< -3.250000 dBm in U8Q6
          107, ///< -3.281250 dBm in U8Q6
          106, ///< -3.312500 dBm in U8Q6
          105, ///< -3.343750 dBm in U8Q6
          105, ///< -3.375000 dBm in U8Q6
          104, ///< -3.406250 dBm in U8Q6
          103, ///< -3.437500 dBm in U8Q6
          103, ///< -3.468750 dBm in U8Q6
          102, ///< -3.500000 dBm in U8Q6
          102, ///< -3.531250 dBm in U8Q6
          101, ///< -3.562500 dBm in U8Q6
          100, ///< -3.593750 dBm in U8Q6
          100, ///< -3.625000 dBm in U8Q6
          99, ///< -3.656250 dBm in U8Q6
          98, ///< -3.687500 dBm in U8Q6
          98, ///< -3.718750 dBm in U8Q6
          97, ///< -3.750000 dBm in U8Q6
          97, ///< -3.781250 dBm in U8Q6
          96, ///< -3.812500 dBm in U8Q6
          96, ///< -3.843750 dBm in U8Q6
          95, ///< -3.875000 dBm in U8Q6
          94, ///< -3.906250 dBm in U8Q6
          94, ///< -3.937500 dBm in U8Q6
          93, ///< -3.968750 dBm in U8Q6
          93, ///< -4.000000 dBm in U8Q6
          92, ///< -4.031250 dBm in U8Q6
          92, ///< -4.062500 dBm in U8Q6
          91, ///< -4.093750 dBm in U8Q6
          90, ///< -4.125000 dBm in U8Q6
          90, ///< -4.156250 dBm in U8Q6
          89, ///< -4.187500 dBm in U8Q6
          89, ///< -4.218750 dBm in U8Q6
          88, ///< -4.250000 dBm in U8Q6
          88, ///< -4.281250 dBm in U8Q6
          87, ///< -4.312500 dBm in U8Q6
          87, ///< -4.343750 dBm in U8Q6
          86, ///< -4.375000 dBm in U8Q6
          85, ///< -4.406250 dBm in U8Q6
          85, ///< -4.437500 dBm in U8Q6
          84, ///< -4.468750 dBm in U8Q6
          84, ///< -4.500000 dBm in U8Q6
          83, ///< -4.531250 dBm in U8Q6
          83, ///< -4.562500 dBm in U8Q6
          82, ///< -4.593750 dBm in U8Q6
          82, ///< -4.625000 dBm in U8Q6
          81, ///< -4.656250 dBm in U8Q6
          81, ///< -4.687500 dBm in U8Q6
          80, ///< -4.718750 dBm in U8Q6
          80, ///< -4.750000 dBm in U8Q6
          79, ///< -4.781250 dBm in U8Q6
          79, ///< -4.812500 dBm in U8Q6
          78, ///< -4.843750 dBm in U8Q6
          78, ///< -4.875000 dBm in U8Q6
          77, ///< -4.906250 dBm in U8Q6
          77, ///< -4.937500 dBm in U8Q6
          76, ///< -4.968750 dBm in U8Q6
          76, ///< -5.000000 dBm in U8Q6
          75, ///< -5.031250 dBm in U8Q6
          75, ///< -5.062500 dBm in U8Q6
          74, ///< -5.093750 dBm in U8Q6
          74, ///< -5.125000 dBm in U8Q6
          74, ///< -5.156250 dBm in U8Q6
          73, ///< -5.187500 dBm in U8Q6
          73, ///< -5.218750 dBm in U8Q6
          72, ///< -5.250000 dBm in U8Q6
          72, ///< -5.281250 dBm in U8Q6
          71, ///< -5.312500 dBm in U8Q6
          71, ///< -5.343750 dBm in U8Q6
          70, ///< -5.375000 dBm in U8Q6
          70, ///< -5.406250 dBm in U8Q6
          69, ///< -5.437500 dBm in U8Q6
          69, ///< -5.468750 dBm in U8Q6
          69, ///< -5.500000 dBm in U8Q6
          68, ///< -5.531250 dBm in U8Q6
          68, ///< -5.562500 dBm in U8Q6
          67, ///< -5.593750 dBm in U8Q6
          67, ///< -5.625000 dBm in U8Q6
          66, ///< -5.656250 dBm in U8Q6
          66, ///< -5.687500 dBm in U8Q6
          65, ///< -5.718750 dBm in U8Q6
          65, ///< -5.750000 dBm in U8Q6
          65, ///< -5.781250 dBm in U8Q6
          64, ///< -5.812500 dBm in U8Q6
          64, ///< -5.843750 dBm in U8Q6
          63, ///< -5.875000 dBm in U8Q6
          63, ///< -5.906250 dBm in U8Q6
          63, ///< -5.937500 dBm in U8Q6
          62, ///< -5.968750 dBm in U8Q6
          249, ///< -6.000000 dBm in U8Q8
          245, ///< -6.062500 dBm in U8Q8
          242, ///< -6.125000 dBm in U8Q8
          239, ///< -6.187500 dBm in U8Q8
          236, ///< -6.250000 dBm in U8Q8
          233, ///< -6.312500 dBm in U8Q8
          230, ///< -6.375000 dBm in U8Q8
          227, ///< -6.437500 dBm in U8Q8
          224, ///< -6.500000 dBm in U8Q8
          221, ///< -6.562500 dBm in U8Q8
          218, ///< -6.625000 dBm in U8Q8
          215, ///< -6.687500 dBm in U8Q8
          213, ///< -6.750000 dBm in U8Q8
          210, ///< -6.812500 dBm in U8Q8
          207, ///< -6.875000 dBm in U8Q8
          204, ///< -6.937500 dBm in U8Q8
          202, ///< -7.000000 dBm in U8Q8
          199, ///< -7.062500 dBm in U8Q8
          197, ///< -7.125000 dBm in U8Q8
          194, ///< -7.187500 dBm in U8Q8
          191, ///< -7.250000 dBm in U8Q8
          189, ///< -7.312500 dBm in U8Q8
          186, ///< -7.375000 dBm in U8Q8
          184, ///< -7.437500 dBm in U8Q8
          181, ///< -7.500000 dBm in U8Q8
          179, ///< -7.562500 dBm in U8Q8
          177, ///< -7.625000 dBm in U8Q8
          174, ///< -7.687500 dBm in U8Q8
          172, ///< -7.750000 dBm in U8Q8
          170, ///< -7.812500 dBm in U8Q8
          167, ///< -7.875000 dBm in U8Q8
          165, ///< -7.937500 dBm in U8Q8
          163, ///< -8.000000 dBm in U8Q8
          161, ///< -8.062500 dBm in U8Q8
          159, ///< -8.125000 dBm in U8Q8
          157, ///< -8.187500 dBm in U8Q8
          155, ///< -8.250000 dBm in U8Q8
          152, ///< -8.312500 dBm in U8Q8
          150, ///< -8.375000 dBm in U8Q8
          148, ///< -8.437500 dBm in U8Q8
          146, ///< -8.500000 dBm in U8Q8
          144, ///< -8.562500 dBm in U8Q8
          142, ///< -8.625000 dBm in U8Q8
          141, ///< -8.687500 dBm in U8Q8
          139, ///< -8.750000 dBm in U8Q8
          137, ///< -8.812500 dBm in U8Q8
          135, ///< -8.875000 dBm in U8Q8
          133, ///< -8.937500 dBm in U8Q8
          131, ///< -9.000000 dBm in U8Q8
          130, ///< -9.062500 dBm in U8Q8
          128, ///< -9.125000 dBm in U8Q8
          126, ///< -9.187500 dBm in U8Q8
          124, ///< -9.250000 dBm in U8Q8
          123, ///< -9.312500 dBm in U8Q8
          121, ///< -9.375000 dBm in U8Q8
          119, ///< -9.437500 dBm in U8Q8
          118, ///< -9.500000 dBm in U8Q8
          116, ///< -9.562500 dBm in U8Q8
          115, ///< -9.625000 dBm in U8Q8
          113, ///< -9.687500 dBm in U8Q8
          111, ///< -9.750000 dBm in U8Q8
          110, ///< -9.812500 dBm in U8Q8
          108, ///< -9.875000 dBm in U8Q8
          107, ///< -9.937500 dBm in U8Q8
          105, ///< -10.000000 dBm in U8Q8
          104, ///< -10.062500 dBm in U8Q8
          103, ///< -10.125000 dBm in U8Q8
          101, ///< -10.187500 dBm in U8Q8
          100, ///< -10.250000 dBm in U8Q8
          98, ///< -10.312500 dBm in U8Q8
          97, ///< -10.375000 dBm in U8Q8
          96, ///< -10.437500 dBm in U8Q8
          94, ///< -10.500000 dBm in U8Q8
          93, ///< -10.562500 dBm in U8Q8
          92, ///< -10.625000 dBm in U8Q8
          91, ///< -10.687500 dBm in U8Q8
          89, ///< -10.750000 dBm in U8Q8
          88, ///< -10.812500 dBm in U8Q8
          87, ///< -10.875000 dBm in U8Q8
          86, ///< -10.937500 dBm in U8Q8
          84, ///< -11.000000 dBm in U8Q8
          83, ///< -11.062500 dBm in U8Q8
          82, ///< -11.125000 dBm in U8Q8
          81, ///< -11.187500 dBm in U8Q8
          80, ///< -11.250000 dBm in U8Q8
          79, ///< -11.312500 dBm in U8Q8
          78, ///< -11.375000 dBm in U8Q8
          77, ///< -11.437500 dBm in U8Q8
          76, ///< -11.500000 dBm in U8Q8
          74, ///< -11.562500 dBm in U8Q8
          73, ///< -11.625000 dBm in U8Q8
          72, ///< -11.687500 dBm in U8Q8
          71, ///< -11.750000 dBm in U8Q8
          70, ///< -11.812500 dBm in U8Q8
          69, ///< -11.875000 dBm in U8Q8
          68, ///< -11.937500 dBm in U8Q8
          68, ///< -12.000000 dBm in U8Q8
          67, ///< -12.062500 dBm in U8Q8
          66, ///< -12.125000 dBm in U8Q8
          65, ///< -12.187500 dBm in U8Q8
          64, ///< -12.250000 dBm in U8Q8
          63, ///< -12.312500 dBm in U8Q8
          62, ///< -12.375000 dBm in U8Q8
          61, ///< -12.437500 dBm in U8Q8
          60, ///< -12.500000 dBm in U8Q8
          59, ///< -12.562500 dBm in U8Q8
          59, ///< -12.625000 dBm in U8Q8
          58, ///< -12.687500 dBm in U8Q8
          57, ///< -12.750000 dBm in U8Q8
          56, ///< -12.812500 dBm in U8Q8
          55, ///< -12.875000 dBm in U8Q8
          55, ///< -12.937500 dBm in U8Q8
          217, ///< -13.000000 dBm in U8Q10
          214, ///< -13.062500 dBm in U8Q10
          211, ///< -13.125000 dBm in U8Q10
          208, ///< -13.187500 dBm in U8Q10
          205, ///< -13.250000 dBm in U8Q10
          202, ///< -13.312500 dBm in U8Q10
          199, ///< -13.375000 dBm in U8Q10
          197, ///< -13.437500 dBm in U8Q10
          194, ///< -13.500000 dBm in U8Q10
          191, ///< -13.562500 dBm in U8Q10
          188, ///< -13.625000 dBm in U8Q10
          186, ///< -13.687500 dBm in U8Q10
          183, ///< -13.750000 dBm in U8Q10
          181, ///< -13.812500 dBm in U8Q10
          178, ///< -13.875000 dBm in U8Q10
          176, ///< -13.937500 dBm in U8Q10
          173, ///< -14.000000 dBm in U8Q10
          171, ///< -14.062500 dBm in U8Q10
          168, ///< -14.125000 dBm in U8Q10
          166, ///< -14.187500 dBm in U8Q10
          164, ///< -14.250000 dBm in U8Q10
          161, ///< -14.312500 dBm in U8Q10
          159, ///< -14.375000 dBm in U8Q10
          157, ///< -14.437500 dBm in U8Q10
          155, ///< -14.500000 dBm in U8Q10
          152, ///< -14.562500 dBm in U8Q10
          150, ///< -14.625000 dBm in U8Q10
          148, ///< -14.687500 dBm in U8Q10
          146, ///< -14.750000 dBm in U8Q10
          144, ///< -14.812500 dBm in U8Q10
          142, ///< -14.875000 dBm in U8Q10
          140, ///< -14.937500 dBm in U8Q10
          138, ///< -15.000000 dBm in U8Q10
          136, ///< -15.062500 dBm in U8Q10
          134, ///< -15.125000 dBm in U8Q10
          132, ///< -15.187500 dBm in U8Q10
          130, ///< -15.250000 dBm in U8Q10
          128, ///< -15.312500 dBm in U8Q10
          127, ///< -15.375000 dBm in U8Q10
          125, ///< -15.437500 dBm in U8Q10
          123, ///< -15.500000 dBm in U8Q10
          121, ///< -15.562500 dBm in U8Q10
          120, ///< -15.625000 dBm in U8Q10
          118, ///< -15.687500 dBm in U8Q10
          116, ///< -15.750000 dBm in U8Q10
          115, ///< -15.812500 dBm in U8Q10
          113, ///< -15.875000 dBm in U8Q10
          111, ///< -15.937500 dBm in U8Q10
          110, ///< -16.000000 dBm in U8Q10
          108, ///< -16.062500 dBm in U8Q10
          107, ///< -16.125000 dBm in U8Q10
          105, ///< -16.187500 dBm in U8Q10
          104, ///< -16.250000 dBm in U8Q10
          102, ///< -16.312500 dBm in U8Q10
          101, ///< -16.375000 dBm in U8Q10
          99, ///< -16.437500 dBm in U8Q10
          98, ///< -16.500000 dBm in U8Q10
          97, ///< -16.562500 dBm in U8Q10
          95, ///< -16.625000 dBm in U8Q10
          94, ///< -16.687500 dBm in U8Q10
          93, ///< -16.750000 dBm in U8Q10
          91, ///< -16.812500 dBm in U8Q10
          90, ///< -16.875000 dBm in U8Q10
          89, ///< -16.937500 dBm in U8Q10
          87, ///< -17.000000 dBm in U8Q10
          86, ///< -17.062500 dBm in U8Q10
          85, ///< -17.125000 dBm in U8Q10
          84, ///< -17.187500 dBm in U8Q10
          82, ///< -17.250000 dBm in U8Q10
          81, ///< -17.312500 dBm in U8Q10
          80, ///< -17.375000 dBm in U8Q10
          79, ///< -17.437500 dBm in U8Q10
          78, ///< -17.500000 dBm in U8Q10
          77, ///< -17.562500 dBm in U8Q10
          76, ///< -17.625000 dBm in U8Q10
          75, ///< -17.687500 dBm in U8Q10
          74, ///< -17.750000 dBm in U8Q10
          72, ///< -17.812500 dBm in U8Q10
          71, ///< -17.875000 dBm in U8Q10
          70, ///< -17.937500 dBm in U8Q10
          69, ///< -18.000000 dBm in U8Q10
          68, ///< -18.062500 dBm in U8Q10
          67, ///< -18.125000 dBm in U8Q10
          66, ///< -18.187500 dBm in U8Q10
          66, ///< -18.250000 dBm in U8Q10
          65, ///< -18.312500 dBm in U8Q10
          64, ///< -18.375000 dBm in U8Q10
          63, ///< -18.437500 dBm in U8Q10
          62, ///< -18.500000 dBm in U8Q10
          61, ///< -18.562500 dBm in U8Q10
          60, ///< -18.625000 dBm in U8Q10
          59, ///< -18.687500 dBm in U8Q10
          58, ///< -18.750000 dBm in U8Q10
          58, ///< -18.812500 dBm in U8Q10
          57, ///< -18.875000 dBm in U8Q10
          56, ///< -18.937500 dBm in U8Q10
          55, ///< -19.000000 dBm in U8Q10
          54, ///< -19.062500 dBm in U8Q10
          54, ///< -19.125000 dBm in U8Q10
          53, ///< -19.187500 dBm in U8Q10
          52, ///< -19.250000 dBm in U8Q10
          51, ///< -19.312500 dBm in U8Q10
          51, ///< -19.375000 dBm in U8Q10
          50, ///< -19.437500 dBm in U8Q10
          49, ///< -19.500000 dBm in U8Q10
          48, ///< -19.562500 dBm in U8Q10
          48, ///< -19.625000 dBm in U8Q10
          47, ///< -19.687500 dBm in U8Q10
          46, ///< -19.750000 dBm in U8Q10
          46, ///< -19.812500 dBm in U8Q10
          45, ///< -19.875000 dBm in U8Q10
          44, ///< -19.937500 dBm in U8Q10
          44, ///< -20.000000 dBm in U8Q10
          43, ///< -20.062500 dBm in U8Q10
          43, ///< -20.125000 dBm in U8Q10
          42, ///< -20.187500 dBm in U8Q10
          41, ///< -20.250000 dBm in U8Q10
          41, ///< -20.312500 dBm in U8Q10
          40, ///< -20.375000 dBm in U8Q10
          40, ///< -20.437500 dBm in U8Q10
          39, ///< -20.500000 dBm in U8Q10
          38, ///< -20.562500 dBm in U8Q10
          38, ///< -20.625000 dBm in U8Q10
          37, ///< -20.687500 dBm in U8Q10
          37, ///< -20.750000 dBm in U8Q10
          36, ///< -20.812500 dBm in U8Q10
          36, ///< -20.875000 dBm in U8Q10
          35, ///< -20.937500 dBm in U8Q10
          35, ///< -21.000000 dBm in U8Q10
          34, ///< -21.062500 dBm in U8Q10
          34, ///< -21.125000 dBm in U8Q10
          33, ///< -21.187500 dBm in U8Q10
          33, ///< -21.250000 dBm in U8Q10
          32, ///< -21.312500 dBm in U8Q10
          32, ///< -21.375000 dBm in U8Q10
          31, ///< -21.437500 dBm in U8Q10
          31, ///< -21.500000 dBm in U8Q10
          30, ///< -21.562500 dBm in U8Q10
          30, ///< -21.625000 dBm in U8Q10
          30, ///< -21.687500 dBm in U8Q10
          29, ///< -21.750000 dBm in U8Q10
          29, ///< -21.812500 dBm in U8Q10
          28, ///< -21.875000 dBm in U8Q10
          28, ///< -21.937500 dBm in U8Q10
          27, ///< -22.000000 dBm in U8Q10
          27, ///< -22.062500 dBm in U8Q10
          27, ///< -22.125000 dBm in U8Q10
          26, ///< -22.187500 dBm in U8Q10
          26, ///< -22.250000 dBm in U8Q10
          26, ///< -22.312500 dBm in U8Q10
          25, ///< -22.375000 dBm in U8Q10
          25, ///< -22.437500 dBm in U8Q10
          24, ///< -22.500000 dBm in U8Q10
          24, ///< -22.562500 dBm in U8Q10
          24, ///< -22.625000 dBm in U8Q10
          23, ///< -22.687500 dBm in U8Q10
          23, ///< -22.750000 dBm in U8Q10
          23, ///< -22.812500 dBm in U8Q10
          22, ///< -22.875000 dBm in U8Q10
          22, ///< -22.937500 dBm in U8Q10
          22, ///< -23.000000 dBm in U8Q10
          21, ///< -23.062500 dBm in U8Q10
          21, ///< -23.125000 dBm in U8Q10
          21, ///< -23.187500 dBm in U8Q10
          20, ///< -23.250000 dBm in U8Q10
          20, ///< -23.312500 dBm in U8Q10
          20, ///< -23.375000 dBm in U8Q10
          20, ///< -23.437500 dBm in U8Q10
          19, ///< -23.500000 dBm in U8Q10
          19, ///< -23.562500 dBm in U8Q10
          19, ///< -23.625000 dBm in U8Q10
          18, ///< -23.687500 dBm in U8Q10
          18, ///< -23.750000 dBm in U8Q10
          18, ///< -23.812500 dBm in U8Q10
          18, ///< -23.875000 dBm in U8Q10
          17, ///< -23.937500 dBm in U8Q10
          17, ///< -24.000000 dBm in U8Q10
          17, ///< -24.062500 dBm in U8Q10
          17, ///< -24.125000 dBm in U8Q10
          16, ///< -24.187500 dBm in U8Q10
          16, ///< -24.250000 dBm in U8Q10
          16, ///< -24.312500 dBm in U8Q10
          16, ///< -24.375000 dBm in U8Q10
          15, ///< -24.437500 dBm in U8Q10
          15, ///< -24.500000 dBm in U8Q10
          15, ///< -24.562500 dBm in U8Q10
          15, ///< -24.625000 dBm in U8Q10
          15, ///< -24.687500 dBm in U8Q10
          14, ///< -24.750000 dBm in U8Q10
          14, ///< -24.812500 dBm in U8Q10
          14, ///< -24.875000 dBm in U8Q10
          14, ///< -24.937500 dBm in U8Q10
          14, ///< -25.000000 dBm in U8Q10
          13, ///< -25.062500 dBm in U8Q10
          13, ///< -25.125000 dBm in U8Q10
          13, ///< -25.187500 dBm in U8Q10
          13, ///< -25.250000 dBm in U8Q10
          13, ///< -25.312500 dBm in U8Q10
          12, ///< -25.375000 dBm in U8Q10
          12, ///< -25.437500 dBm in U8Q10
          12, ///< -25.500000 dBm in U8Q10
          12, ///< -25.562500 dBm in U8Q10
          12, ///< -25.625000 dBm in U8Q10
          11, ///< -25.687500 dBm in U8Q10
          11, ///< -25.750000 dBm in U8Q10
          11, ///< -25.812500 dBm in U8Q10
          11, ///< -25.875000 dBm in U8Q10
          11, ///< -25.937500 dBm in U8Q10
          11, ///< -26.000000 dBm in U8Q10
          10, ///< -26.062500 dBm in U8Q10
          10, ///< -26.125000 dBm in U8Q10
          10, ///< -26.187500 dBm in U8Q10
          10, ///< -26.250000 dBm in U8Q10
          10, ///< -26.312500 dBm in U8Q10
          10, ///< -26.375000 dBm in U8Q10
          10, ///< -26.437500 dBm in U8Q10
          9, ///< -26.500000 dBm in U8Q10
          9, ///< -26.562500 dBm in U8Q10
          9, ///< -26.625000 dBm in U8Q10
          9, ///< -26.687500 dBm in U8Q10
          9, ///< -26.750000 dBm in U8Q10
          9, ///< -26.812500 dBm in U8Q10
          9, ///< -26.875000 dBm in U8Q10
          8, ///< -26.937500 dBm in U8Q10
          8, ///< -27.000000 dBm in U8Q10
          8, ///< -27.062500 dBm in U8Q10
          8, ///< -27.125000 dBm in U8Q10
          8, ///< -27.187500 dBm in U8Q10
          8, ///< -27.250000 dBm in U8Q10
          8, ///< -27.312500 dBm in U8Q10
          8, ///< -27.375000 dBm in U8Q10
          8, ///< -27.437500 dBm in U8Q10
          7, ///< -27.500000 dBm in U8Q10
          7, ///< -27.562500 dBm in U8Q10
          7, ///< -27.625000 dBm in U8Q10
          7, ///< -27.687500 dBm in U8Q10
          7, ///< -27.750000 dBm in U8Q10
          7, ///< -27.812500 dBm in U8Q10
          7, ///< -27.875000 dBm in U8Q10
          7, ///< -27.937500 dBm in U8Q10
          7, ///< -28.000000 dBm in U8Q10
          6, ///< -28.062500 dBm in U8Q10
          6, ///< -28.125000 dBm in U8Q10
          6, ///< -28.187500 dBm in U8Q10
          6, ///< -28.250000 dBm in U8Q10
          6, ///< -28.312500 dBm in U8Q10
          6, ///< -28.375000 dBm in U8Q10
          6, ///< -28.437500 dBm in U8Q10
          6, ///< -28.500000 dBm in U8Q10
          6, ///< -28.562500 dBm in U8Q10
          6, ///< -28.625000 dBm in U8Q10
          6, ///< -28.687500 dBm in U8Q10
          5, ///< -28.750000 dBm in U8Q10
          5, ///< -28.812500 dBm in U8Q10
          5, ///< -28.875000 dBm in U8Q10
          5, ///< -28.937500 dBm in U8Q10
          5, ///< -29.000000 dBm in U8Q10
          5, ///< -29.062500 dBm in U8Q10
          5, ///< -29.125000 dBm in U8Q10
          5, ///< -29.187500 dBm in U8Q10
          5, ///< -29.250000 dBm in U8Q10
          5, ///< -29.312500 dBm in U8Q10
          5, ///< -29.375000 dBm in U8Q10
          5, ///< -29.437500 dBm in U8Q10
          4, ///< -29.500000 dBm in U8Q10
          4, ///< -29.562500 dBm in U8Q10
          4, ///< -29.625000 dBm in U8Q10
          4, ///< -29.687500 dBm in U8Q10
          4, ///< -29.750000 dBm in U8Q10
          4, ///< -29.812500 dBm in U8Q10
          4, ///< -29.875000 dBm in U8Q10
          4 ///< -29.937500 dBm in U8Q10
};

/************************** function prototypes ***************************************/

/*===========================================================================
FUNCTION    l1_sc_g2x_dbm_add_Q9

DESCRIPTION calculate power add in unit of dBm, return value equals to
            10*log10[ 10^(M/10) + 10^(N/10) ]

            M, N data format is Q9,
            the format of return value is also Q9

DEPENDENCIES none

RETURN VALUE Q9

SIDE EFFECTS None
===========================================================================*/
int32 l1_sc_g2x_dbm_add_Q9
(
  int32 m, //Q9
  int32 n, //Q9
  gas_id_t gas_id
)
{
  ///====================================== derive the power difference =============================
  int32 diffPwrInt;

  uint32   index;
  uint32   delta;    ///< positive or 0 gain in power
  diffPwrInt = ABS(m-n); //Q9

  if(diffPwrInt <(TX_THRESHOLD_1 *512))
  {
     index = diffPwrInt >> TX_SHIFT_1;                              // 1/32 step in the 6 dB range
     delta = dBm_comb_LUT[index]<<3;                              // LUT is U8Q6, <<3 to Q9
  }
  else if(diffPwrInt <(TX_THRESHOLD_2 *512))                        // 6 to 13 dB range
  {
     index = (TX_STEP_1 * TX_THRESHOLD_1) +
             ((diffPwrInt - TX_THRESHOLD_1*512) >>(TX_SHIFT_2_Q9));  // 1/16 step in the 4 dB rabge
     delta = dBm_comb_LUT[index]<<1;                             // LUT is U8Q8, <<1 to Q9
  }
  else if(diffPwrInt <(TX_THRESHOLD_3 *512))                        // 13 to 30 dB
  {
     index = (TX_STEP_1 * TX_THRESHOLD_1) +
             (TX_STEP_2 * (TX_THRESHOLD_2 - TX_THRESHOLD_1)) +
             ((diffPwrInt - TX_THRESHOLD_2*512) >>(TX_SHIFT_3_Q9));  // 1/4 step in the 4 dB rabge
     delta = dBm_comb_LUT[index]>>1;                             //LUT is U8Q10, >>1 to Q9
  }
  else                                                           // (diffPwrInt > 30*512)  ///< Q9, 30 dB
  {
     delta = 0;                                                  // out of dynamic range and the gain is negligible once out of it
  }

  return(delta + ((m > n)? m: n));                              //Q9
}


/*=============================================================================
FUNCTION l1_sc_g2x_exp_2

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
uint64 l1_sc_g2x_exp_2(int32 x_q24x8, gas_id_t gas_id)
{
  int16 i;
  uint32 f;
  uint64 y;

  i = (x_q24x8 >> 8);
  f = x_q24x8 & 0xFF;

  y = 65484 + (( ( 45836 + (( (14548 + ((5144 * (uint64) f) >> 8)) * (uint64) f) >> 8) ) * (uint64) f + (1 << 7) ) >> 8); // y here is at most 2^17-1, 32 bit is enough
  y = y << 32;//Q.48

  if (i >= 0)
  {
    y = y << i;
      /* shift by i, here precision of small values is limited by Q16.48 format,
         i.e. 2^-16, here we need more bits for y! */
  }
  else
  {
    y = ( y + ((uint64)1 << (0-i-1)) ) >> (0-i);
  }

  return y;
}

/*=============================================================================
FUNCTION l1_sc_g2x_db_2_lin

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
uint64 l1_sc_g2x_db_2_lin( int16 dB_q8x8, gas_id_t gas_id)
{
  /* TODO Clean up magic nos. */

  uint64 y;

  y = l1_sc_g2x_exp_2( (int32) ( ( (int64) dB_q8x8 * 2853514505LL
             + ((int64)1 << 32) ) >> 33 ) ,  gas_id);
  /* y is uint64 in Q16.48 format */


  return y;

}

/*=============================================================================
FUNCTION l1_sc_g2x_msb_pos

DESCRIPTION
  This function finds the most significant bit position (0 to 15) in a 16 bit
  unsigned number

DEPENDENCIES
  None.

PARAMETERS
  Unisigned 16 bit input, x

RETURN VALUE
  Most significant bit position in x

SIDE EFFECTS
  None.
===========================================================================*/
uint16 l1_sc_g2x_msb_pos( uint16 x, gas_id_t gas_id)
{
   uint16 b;


   if( x < 0x100 )
   {
        // search 8-bit nibbles
      b = l1_sc_g2x_msb_byte_lut[x];
   }
   else
   {
      b = 8 + l1_sc_g2x_msb_byte_lut[ x >> 8 ];
   }
   return  b;
}


/*=============================================================================
FUNCTION l1_sc_g2x_log_2

DESCRIPTION
  This function calculates log2 (log base 2) in fixed_point.

DEPENDENCIES
  None.

PARAMETERS
  The input in Q16.8 whose log2 value is desired

RETURN VALUE
  32-bit fixed-point result of log2( input )

SIDE EFFECTS
  None.
===========================================================================*/
uint32 l1_sc_g2x_log_2(uint16 x_q16x8, gas_id_t gas_id)
{
   uint32 x_q32x8 = (uint32) x_q16x8;
   uint32 y;
   uint32 table_index0, last_c_bits;
   int32 c, minus_c;

   int32 a = (int32) l1_sc_g2x_msb_pos(x_q16x8, gas_id);


   if (a <= l1_sc_g2x_log2_LUT_q0x16_M) // no need for interpolation
   {
     minus_c = l1_sc_g2x_log2_LUT_q0x16_M - a; // left shift if a < M
     table_index0 = (x_q32x8 << minus_c) & 0x1F; // get rid of MSB
     y = l1_sc_g2x_log2_LUT_q0x20[table_index0] + ( a << 20 ); // (0x100000 * a);
   }
   else
   {
     c = a - l1_sc_g2x_log2_LUT_q0x16_M;
     table_index0 = (x_q32x8 >> c) & 0x1F; // table index is the next c bits of x after MSB
     last_c_bits = x_q32x8 & ((1 << c) - 1); // last c bits of x
     if (last_c_bits == 0) // no need for interpolation
       y = l1_sc_g2x_log2_LUT_q0x20[table_index0] + ( a << 20 ); // (0x100000 * a);
     else // % do linear table interpolation between 2 entries in table
       y = l1_sc_g2x_log2_LUT_q0x20[table_index0] +
           (uint32)( ( (uint64) last_c_bits *
          (l1_sc_g2x_log2_LUT_q0x20[table_index0+1] - l1_sc_g2x_log2_LUT_q0x20[table_index0]) +
          ((uint64)1 << (c-1)) ) >> c ) + (a << 20 );
   }

   return y;
}



/*=============================================================================
FUNCTION l1_sc_g2x_lin_2_db

DESCRIPTION
  This function converts alinear value in Q16.8 format to dB in Q16.8 format.
  Both quantities are 16 bits wide. The number after the "." represents the
  fractional part or the resolution of the fixed-point representation.

DEPENDENCIES
  None.

PARAMETERS
  The linear value input in Q16.8

RETURN VALUE
  The dB representation in Q16.8

SIDE EFFECTS
  None.
===========================================================================*/
int16 l1_sc_g2x_lin_2_db( uint16 linear_q16x8, gas_id_t gas_id)
{
  /* TODO Clean up magic nos. */
   int32 y;
   int16 z;
   const int32 log10_sf = 25252226;
                           /* 25252226 = round(10*log10(2^8)*2^20),
                            * Q.20 format
                            */

   /* y = 10*log10(x) = 10*log2(x)/log2(10) = 3.01029996*log2(x) */
   y = l1_sc_g2x_log_2(linear_q16x8, gas_id); // log2 result is Q.20 format

   /* 3232284966 = round(10/log2(10)*2^30) */
   y = (int32) ( ( (int64) y * 3232284966LL + (1 << 29) ) >> 30);
   y = (y - log10_sf); /* adjust for scaling factor */
   z = (int16) (y >> 12); // change to Q.8 format
   return  z;
}

/*=============================================================================
FUNCTION l1_sc_g2x_filter_treatment

DESCRIPTION


DEPENDENCIES
  None.

PARAMETERS


RETURN VALUE


SIDE EFFECTS
  None.
===========================================================================*/
void l1_sc_g2x_filter_treatment(t_gsm_l1_lte_wcdma_cell_energy_details**  cell_energy,
                                     int16 power, uint16 freq_idx, uint16 meas_idx, rr_l1_irat_rat_e rat_type, gas_id_t gas_id)
{
    int32 filter0, filter1;
    int16 alpha_second = 128;
    int16 alpha_afterward = 86;
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    if(rat_type == RR_L1_IRAT_RAT_LTE)
    {
      MSG_GERAN_HIGH_4_G(" new_rsrp= %d existing RSRP_avg= %d, rsrp_last= %d, rsrp_last2= %d ",
                                  power, 
                                  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg, 
                                  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last, 
                                  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last2
                        );

      if((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last2 == 0) &&
          (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last == 0))
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg = power - (int16)(FIRST_BIAS + BIAS_OFFSET);
      }
      else if((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last2 == 0) &&
          (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last < 0))
      {
        filter0 = ((int32)(power << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_second, gas_id))<<1;
        filter1 = ((int32)(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_second, gas_id))<<1;

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg  = (int16)((l1_sc_g2x_dbm_add_Q9(filter0, filter1, gas_id)>>1)>>8) - (int16)(SECOND_BIAS + BIAS_OFFSET);

        MSG_GERAN_HIGH_3_G("RSRP_avg = %d, filter0 = %d, filter1 = %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg, filter0, filter1);
      }
      else if((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last2 < 0) &&
          (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last < 0))
      {
        filter0 = ((int32)(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_afterward, gas_id))<<1;
        filter1 = ((int32)(power << 8) + (int32)l1_sc_g2x_lin_2_db((256 - alpha_afterward), gas_id))<<1;

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg = (int16)(l1_sc_g2x_dbm_add_Q9(filter0, filter1, gas_id)>>1)>>8;

        MSG_GERAN_HIGH_3_G("RSRP_avg = %d, filter0 = %d, filter1 = %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg, filter0, filter1);
      }
      else
      {
        MSG_GERAN_ERROR_2_G("invalid history rsrp value = %d, value2 = %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last2);
      }

      if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg > 0)
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_avg  = -1024;
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last = 0;
      }

      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last2 =
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last;

      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].lte_cell.rsrp_last = power;

      return;
    }

    if(rat_type == RR_L1_IRAT_RAT_WCDMA)
    {
       /* Excessive F3 reduction */
        MSG_GERAN_HIGH_4_G("RSSI = %d, RSSI_avg = %d, rssi_last = %d, rssi_last2 = %d",power, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last2, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last);


      if((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last2 == 0) &&
          (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last == 0))
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg = power - (int16)(FIRST_BIAS + BIAS_OFFSET);
      }
      else if((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last2 == 0) &&
          (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last < 0))
      {
        filter0 = ((int32)(power << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_second, gas_id))<<1;
        filter1 = ((int32)(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_second, gas_id))<<1;

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg = (int16)((l1_sc_g2x_dbm_add_Q9(filter0, filter1, gas_id)>>1)>>8) - (int16)(SECOND_BIAS + BIAS_OFFSET);

        MSG_GERAN_HIGH_3_G("RSSI_avg = %d, filter0 = %d, filter1 = %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg, filter0, filter1);
      }
      else if((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last2 < 0) &&
          (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last < 0))
      {
        filter0 = ((int32)(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_afterward, gas_id))<<1;
        filter1 = ((int32)(power << 8) + (int32)l1_sc_g2x_lin_2_db((256 - alpha_afterward), gas_id))<<1;

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg = (int16)(l1_sc_g2x_dbm_add_Q9(filter0, filter1, gas_id)>>1)>>8;

        MSG_GERAN_HIGH_3_G("RSSI_avg = %d, filter0 = %d, filter1 = %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg, filter0, filter1);
      }
      else
      {
        MSG_GERAN_ERROR_2_G("invalid history rssi value = %d, value2 = %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last2);
      }

      if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg > 0)
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg = 0;
      }

      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last2 =
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last;

      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_last = power;

      return;

    }
    if(rat_type == RR_L1_IRAT_RAT_UTRAN_TDD)
    {
#ifdef FEATURE_GSM_TDS
    if(cell_energy!=NULL)
    {
        if(((*cell_energy)->wcdma_cell.RSCP_last == MIN_TDD_CELL_RSCP_VALUE) &&
              ((*cell_energy)->wcdma_cell.RSCP_last2 == MIN_TDD_CELL_RSCP_VALUE))
        {
          (*cell_energy)->wcdma_cell.RSCP_avg = power - (int16)(FIRST_BIAS + BIAS_OFFSET);
        }
        else if(((*cell_energy)->wcdma_cell.RSCP_last2 == MIN_TDD_CELL_RSCP_VALUE) &&
              ((*cell_energy)->wcdma_cell.RSCP_last > MIN_TDD_CELL_RSCP_VALUE))
        {
            filter0 = ((int32)(power << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_second, gas_id))<<1;

            filter1 = ((int32)((*cell_energy)->wcdma_cell.RSCP_last << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_second, gas_id))<<1;

            (*cell_energy)->wcdma_cell.RSCP_avg = (int16)((l1_sc_g2x_dbm_add_Q9(filter0, filter1, gas_id)>>1)>>8) - (int16)(SECOND_BIAS + BIAS_OFFSET);

            MSG_GERAN_HIGH_3_G("RSCP_avg = %d, filter0 = %d, filter1 = %d",(*cell_energy)->wcdma_cell.RSCP_avg, filter0, filter0);
        }
        else if(((*cell_energy)->wcdma_cell.RSCP_last2 > MIN_TDD_CELL_RSCP_VALUE) &&
              ((*cell_energy)->wcdma_cell.RSCP_last > MIN_TDD_CELL_RSCP_VALUE))
        {
            filter0 = ((int32)((*cell_energy)->wcdma_cell.RSCP_avg << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_afterward, gas_id))<<1;

            filter1 = ((int32)(power << 8) + (int32)l1_sc_g2x_lin_2_db((256 - alpha_afterward), gas_id))<<1;

            (*cell_energy)->wcdma_cell.RSCP_avg = (int16)(l1_sc_g2x_dbm_add_Q9(filter0, filter1, gas_id)>>1)>>8;

            MSG_GERAN_HIGH_3_G("RSCP_avg = %d, filter0 = %d, filter1 = %d",(*cell_energy)->wcdma_cell.RSCP_avg, filter0, filter1);
        }
        else
        {
            MSG_GERAN_ERROR_2_G("invalid history rscp value = %d, value2 = %d",(*cell_energy)->wcdma_cell.RSCP_last, (*cell_energy)->wcdma_cell.RSCP_last2);
        }

        if((*cell_energy)->wcdma_cell.RSCP_avg < MIN_TDD_CELL_RSCP_VALUE)
        {
            (*cell_energy)->wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;
        }

        (*cell_energy)->wcdma_cell.RSCP_last2 =
              (*cell_energy)->wcdma_cell.RSCP_last;

        (*cell_energy)->wcdma_cell.RSCP_last = power;

    }
    else
    {
        if((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last == MIN_TDD_CELL_RSCP_VALUE) &&
              (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2 == MIN_TDD_CELL_RSCP_VALUE))
        {
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = power - (int16)(FIRST_BIAS + BIAS_OFFSET);
        }
        else if((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2 == MIN_TDD_CELL_RSCP_VALUE) &&
              (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last > MIN_TDD_CELL_RSCP_VALUE))
        {
            filter0 = ((int32)(power << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_second, gas_id))<<1;

            filter1 = ((int32)(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_second, gas_id))<<1;

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = (int16)((l1_sc_g2x_dbm_add_Q9(filter0, filter1, gas_id)>>1)>>8) - (int16)(SECOND_BIAS + BIAS_OFFSET);

            MSG_GERAN_HIGH_3_G("RSCP_avg = %d, filter0 = %d, filter1 = %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg, filter0, filter1);
        }
        else if((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2 > MIN_TDD_CELL_RSCP_VALUE) &&
              (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last > MIN_TDD_CELL_RSCP_VALUE))
        {
            filter0 = ((int32)(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg << 8) + (int32)l1_sc_g2x_lin_2_db(alpha_afterward, gas_id))<<1;

            filter1 = ((int32)(power << 8) + (int32)l1_sc_g2x_lin_2_db((256 - alpha_afterward), gas_id))<<1;

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = (int16)(l1_sc_g2x_dbm_add_Q9(filter0, filter1, gas_id)>>1)>>8;

            MSG_GERAN_HIGH_3_G("RSCP_avg = %d, filter0 = %d, filter1 = %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg, filter0, filter1);
        }
        else
        {
            MSG_GERAN_ERROR_2_G("invalid history rscp value = %d, value2 = %d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2);
        }

        if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg < MIN_TDD_CELL_RSCP_VALUE)
        {
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = MIN_TDD_CELL_RSCP_VALUE;
        }

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last2 =
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last;

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = power;
    }
#endif
      return;
    }
}



/* EOF */



