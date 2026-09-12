
/*
WARNING: This file is auto-generated.

Generated using: pa_autogen.pl
Generated from:  2.1.82 of RFDevice_PA.xlsm
*/

/*=============================================================================

          R F C     A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

Copyright (c) 2009 - 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //Commercial/MPSS.DI.2.0.1.c1/Main/modem_proc/rfdevice_pa/src/rfdevice_pa_sm5643_51_data_ag.cpp#2 $ 


=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/
#include "comdef.h"

#include "rfdevice_pa_sm5643_51_data_ag.h"


#define RFDEVICE_PA_sm5643_51_NUM_PORTS 187

#define RFDEVICE_PA_sm5643_51_PA_SET_BIAS_NUM_REGS 2
static uint8 rfdevice_pa_sm5643_51_pa_set_bias_regs[RFDEVICE_PA_sm5643_51_PA_SET_BIAS_NUM_REGS] =  {0x01,0x03 };
static int16 rfdevice_pa_sm5643_51_pa_set_bias_data[RFDEVICE_PA_sm5643_51_NUM_PORTS][4][RFDEVICE_PA_sm5643_51_PA_SET_BIAS_NUM_REGS] =
{
#if 0
  { /* PORT NUM: 0 LTE_B12_LB1*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },
#else
   { /* PORT NUM: 0 LTE_B12_LB1_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
#endif  
  { /* PORT NUM: 1 LTE_B12_LB2*/
    #if 1 // 118
    { 0x84, 0X98 },  /* PA Range: 0 */
    { 0x54, 0X78 },  /* PA Range: 1 */
    { 0x54, 0X78 },  /* PA Range: 2 */
    { 0x54, 0X78 },  /* PA Range: 3 */
	#else
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
    #endif
  },
  { /* PORT NUM: 2 LTE_B12_LB3*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 3 LTE_B12_LB4*/
    #if 1 // port 97
    { 0x2B, 0X8A },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
    #else // port 3
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
    #endif
  },
  { /* PORT NUM: 4 LTE_B12_LB5*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },
#if 0  
  { /* PORT NUM: 5 LTE_B17_LB1*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
#else
  { /* PORT NUM: 5 LTE_B17_LB1_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },      
#endif
  { /* PORT NUM: 6 LTE_B17_LB2*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 7 LTE_B17_LB3*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 8 LTE_B17_LB4*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 9 LTE_B17_LB5*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 10 LTE_B28_LB1*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 11 LTE_B28_LB2*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 12 LTE_B28_LB3*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 13 LTE_B28_LB4*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 14 LTE_B28_LB5*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 15 LTE_B13_LB1*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 16 LTE_B13_LB2*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 17 LTE_B13_LB3*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },
#if 0  
  { /* PORT NUM: 18 LTE_B13_LB4*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },
#else
  { /* PORT NUM: 18 LTE_B13_LB4_LB2IN*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },  
#endif
  { /* PORT NUM: 19 LTE_B13_LB5*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 20 LTE_B14_LB1*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 21 LTE_B14_LB2*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 22 LTE_B14_LB3*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 23 LTE_B14_LB4*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 24 LTE_B14_LB5*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 25 LTE_B20_LB1*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 26 LTE_B20_LB2*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 27 LTE_B20_LB3*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 28 LTE_B20_LB4*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 29 LTE_B20_LB5*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 30 LTE_B5_LB1*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 31 LTE_B5_LB2*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 32 LTE_B5_LB3*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 33 LTE_B5_LB4*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 34 LTE_B5_LB5*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 35 LTE_B26_LB1*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 36 LTE_B26_LB2*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 37 LTE_B26_LB3*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 38 LTE_B26_LB4*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 39 LTE_B26_LB5*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 40 LTE_B8_LB1*/
    { 0x0A, 0XAA },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 41 LTE_B8_LB2*/
    { 0x0A, 0XAA },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 42 LTE_B8_LB3*/
    { 0x0A, 0XAA },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 43 LTE_B8_LB4*/
    { 0x0A, 0XAA },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 44 LTE_B8_LB5*/
    { 0x0A, 0XAA },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 45 LTE_B4_MB1*/
    { 0x9F, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 46 LTE_B4_MB2*/
    { 0x9F, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 47 LTE_B4_MB3*/
    { 0x9F, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 48 LTE_B4_MB4*/
    { 0x9F, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 49 LTE_B4_MB5*/
    { 0x9F, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 50 LTE_B3_MB1*/
    { 0x9F, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 51 LTE_B3_MB2*/
    { 0x9F, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 52 LTE_B3_MB3*/
    { 0x9F, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 53 LTE_B3_MB4*/
    { 0x9F, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 54 LTE_B3_MB5*/
    { 0x9F, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 55 LTE_B2_MB1*/
    { 0x85, 0X89 },  /* PA Range: 0 */
    { 0x55, 0X68 },  /* PA Range: 1 */
    { 0x55, 0X68 },  /* PA Range: 2 */
    { 0x55, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 56 LTE_B2_MB2*/
    { 0x85, 0X89 },  /* PA Range: 0 */
    { 0x55, 0X68 },  /* PA Range: 1 */
    { 0x55, 0X68 },  /* PA Range: 2 */
    { 0x55, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 57 LTE_B2_MB3*/
    { 0x85, 0X89 },  /* PA Range: 0 */
    { 0x55, 0X68 },  /* PA Range: 1 */
    { 0x55, 0X68 },  /* PA Range: 2 */
    { 0x55, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 58 LTE_B2_MB4*/
    { 0x85, 0X89 },  /* PA Range: 0 */
    { 0x55, 0X68 },  /* PA Range: 1 */
    { 0x55, 0X68 },  /* PA Range: 2 */
    { 0x55, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 59 LTE_B2_MB5*/
    { 0x85, 0X89 },  /* PA Range: 0 */
    { 0x55, 0X68 },  /* PA Range: 1 */
    { 0x55, 0X68 },  /* PA Range: 2 */
    { 0x55, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 60 LTE_B1_MB1*/
    { 0x84, 0X98 },  /* PA Range: 0 */
    { 0x54, 0X78 },  /* PA Range: 1 */
    { 0x54, 0X78 },  /* PA Range: 2 */
    { 0x54, 0X78 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 61 LTE_B1_MB2*/
    { 0x84, 0X98 },  /* PA Range: 0 */
    { 0x54, 0X78 },  /* PA Range: 1 */
    { 0x54, 0X78 },  /* PA Range: 2 */
    { 0x54, 0X78 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 62 LTE_B1_MB3*/
    { 0x84, 0X98 },  /* PA Range: 0 */
    { 0x54, 0X78 },  /* PA Range: 1 */
    { 0x54, 0X78 },  /* PA Range: 2 */
    { 0x54, 0X78 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 63 LTE_B1_MB4*/
    { 0x84, 0X98 },  /* PA Range: 0 */
    { 0x54, 0X78 },  /* PA Range: 1 */
    { 0x54, 0X78 },  /* PA Range: 2 */
    { 0x54, 0X78 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 64 LTE_B1_MB5*/
    { 0x84, 0X98 },  /* PA Range: 0 */
    { 0x54, 0X78 },  /* PA Range: 1 */
    { 0x54, 0X78 },  /* PA Range: 2 */
    { 0x54, 0X78 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 65 LTE_B39_MB1*/
    { 0x88, 0X89 },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 66 LTE_B39_MB2*/
    { 0x88, 0X89 },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 67 LTE_B39_MB3*/
    { 0x88, 0X89 },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 68 LTE_B39_MB4*/
    { 0x88, 0X89 },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 69 LTE_B39_MB5*/
    { 0x88, 0X89 },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 70 LTE_B34_MB1*/
    { 0x87, 0X98 },  /* PA Range: 0 */
    { 0x67, 0X68 },  /* PA Range: 1 */
    { 0x67, 0X68 },  /* PA Range: 2 */
    { 0x67, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 71 LTE_B34_MB2*/
    { 0x87, 0X98 },  /* PA Range: 0 */
    { 0x67, 0X68 },  /* PA Range: 1 */
    { 0x67, 0X68 },  /* PA Range: 2 */
    { 0x67, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 72 LTE_B34_MB3*/
    { 0x87, 0X98 },  /* PA Range: 0 */
    { 0x67, 0X68 },  /* PA Range: 1 */
    { 0x67, 0X68 },  /* PA Range: 2 */
    { 0x67, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 73 LTE_B34_MB4*/
    { 0x87, 0X98 },  /* PA Range: 0 */
    { 0x67, 0X68 },  /* PA Range: 1 */
    { 0x67, 0X68 },  /* PA Range: 2 */
    { 0x67, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 74 LTE_B34_MB5*/
    { 0x87, 0X98 },  /* PA Range: 0 */
    { 0x67, 0X68 },  /* PA Range: 1 */
    { 0x67, 0X68 },  /* PA Range: 2 */
    { 0x67, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 75 LTE_B7_HB1*/
    { 0x92, 0XC5 },  /* PA Range: 0 */
    { 0x51, 0X79 },  /* PA Range: 1 */
    { 0x51, 0X79 },  /* PA Range: 2 */
    { 0x51, 0X79 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 76 LTE_B7_HB2*/
    { 0x92, 0XC5 },  /* PA Range: 0 */
    { 0x51, 0X79 },  /* PA Range: 1 */
    { 0x51, 0X79 },  /* PA Range: 2 */
    { 0x51, 0X79 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 77 LTE_B7_HB3*/
    { 0x92, 0XC5 },  /* PA Range: 0 */
    { 0x51, 0X79 },  /* PA Range: 1 */
    { 0x51, 0X79 },  /* PA Range: 2 */
    { 0x51, 0X79 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 78 LTE_B7_HB4*/
    { 0x92, 0XC5 },  /* PA Range: 0 */
    { 0x51, 0X79 },  /* PA Range: 1 */
    { 0x51, 0X79 },  /* PA Range: 2 */
    { 0x51, 0X79 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 79 LTE_B40_HB1*/
    { 0x83, 0XB8 },  /* PA Range: 0 */
    { 0x43, 0X76 },  /* PA Range: 1 */
    { 0x43, 0X76 },  /* PA Range: 2 */
    { 0x43, 0X76 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 80 LTE_B40_HB2*/
    { 0x83, 0XB8 },  /* PA Range: 0 */
    { 0x43, 0X76 },  /* PA Range: 1 */
    { 0x43, 0X76 },  /* PA Range: 2 */
    { 0x43, 0X76 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 81 LTE_B40_HB3*/
    { 0x83, 0XB8 },  /* PA Range: 0 */
    { 0x43, 0X76 },  /* PA Range: 1 */
    { 0x43, 0X76 },  /* PA Range: 2 */
    { 0x43, 0X76 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 82 LTE_B40_HB4*/
    { 0x83, 0XB8 },  /* PA Range: 0 */
    { 0x43, 0X76 },  /* PA Range: 1 */
    { 0x43, 0X76 },  /* PA Range: 2 */
    { 0x43, 0X76 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 83 LTE_B38_41_HB1*/
    { 0x91, 0XC8 },  /* PA Range: 0 */
    { 0x51, 0X79 },  /* PA Range: 1 */
    { 0x51, 0X79 },  /* PA Range: 2 */
    { 0x51, 0X79 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 84 LTE_B38_41_HB2*/
    { 0x91, 0XC8 },  /* PA Range: 0 */
    { 0x51, 0X79 },  /* PA Range: 1 */
    { 0x51, 0X79 },  /* PA Range: 2 */
    { 0x51, 0X79 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 85 LTE_B38_41_HB3*/
    { 0x91, 0XC8 },  /* PA Range: 0 */
    { 0x51, 0X79 },  /* PA Range: 1 */
    { 0x51, 0X79 },  /* PA Range: 2 */
    { 0x51, 0X79 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 86 LTE_B38_41_HB4*/
    { 0x91, 0XC8 },  /* PA Range: 0 */
    { 0x51, 0X79 },  /* PA Range: 1 */
    { 0x51, 0X79 },  /* PA Range: 2 */
    { 0x51, 0X79 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 87 WCDMA_B5_LB1*/
    { 0x2E, 0X86 },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 88 WCDMA_B5_LB2*/
    { 0x2E, 0X86 },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 89 WCDMA_B5_LB3*/
    { 0x2E, 0X86 },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 90 WCDMA_B5_LB4*/
    { 0x2E, 0X86 },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 91 WCDMA_B5_LB5*/
    { 0x2E, 0X86 },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 92 WCDMA_B8_LB1*/
    { 0x2E, 0X86 },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 93 WCDMA_B8_LB2*/
    { 0x2E, 0X86 },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 94 WCDMA_B8_LB3*/
    { 0x2E, 0X86 },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 95 WCDMA_B8_LB4*/
    { 0x2E, 0X86 },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 96 WCDMA_B8_LB5*/
    { 0x2E, 0X86 },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 97 CDMA_BC0_LB1*/
    #if 0 // port 97
    { 0x2B, 0X8A },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
    #else // port 3
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
	#endif
  },
  { /* PORT NUM: 98 CDMA_BC0_LB2*/
    { 0x2B, 0X8A },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 99 CDMA_BC0_LB3*/
    { 0x2B, 0X8A },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 100 CDMA_BC0_LB4*/
    { 0x2B, 0X8A },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 101 CDMA_BC0_LB5*/
    { 0x2B, 0X8A },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 102 WCDMA_B4_MB1*/
    { 0x96, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 103 WCDMA_B4_MB2*/
    { 0x96, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 104 WCDMA_B4_MB3*/
    { 0x96, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 105 WCDMA_B4_MB4*/
    { 0x96, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 106 WCDMA_B4_MB5*/
    { 0x96, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 107 WCDMA_B3_MB1*/
    { 0x96, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 108 WCDMA_B3_MB2*/
    { 0x96, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 109 WCDMA_B3_MB3*/
    { 0x96, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 110 WCDMA_B3_MB4*/
    { 0x96, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 111 WCDMA_B3_MB5*/
    { 0x96, 0X8A },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 112 WCDMA_B2_MB1*/
    { 0x85, 0X89 },  /* PA Range: 0 */
    { 0x55, 0X68 },  /* PA Range: 1 */
    { 0x55, 0X68 },  /* PA Range: 2 */
    { 0x55, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 113 WCDMA_B2_MB2*/
    { 0x85, 0X89 },  /* PA Range: 0 */
    { 0x55, 0X68 },  /* PA Range: 1 */
    { 0x55, 0X68 },  /* PA Range: 2 */
    { 0x55, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 114 WCDMA_B2_MB3*/
    { 0x85, 0X89 },  /* PA Range: 0 */
    { 0x55, 0X68 },  /* PA Range: 1 */
    { 0x55, 0X68 },  /* PA Range: 2 */
    { 0x55, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 115 WCDMA_B2_MB4*/
    { 0x85, 0X89 },  /* PA Range: 0 */
    { 0x55, 0X68 },  /* PA Range: 1 */
    { 0x55, 0X68 },  /* PA Range: 2 */
    { 0x55, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 116 WCDMA_B2_MB5*/
    { 0x85, 0X89 },  /* PA Range: 0 */
    { 0x55, 0X68 },  /* PA Range: 1 */
    { 0x55, 0X68 },  /* PA Range: 2 */
    { 0x55, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 117 WCDMA_B1_MB1*/
    { 0x84, 0X98 },  /* PA Range: 0 */
    { 0x54, 0X78 },  /* PA Range: 1 */
    { 0x54, 0X78 },  /* PA Range: 2 */
    { 0x54, 0X78 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 118 WCDMA_B1_MB2*/
    #if 0 // 118
    { 0x84, 0X98 },  /* PA Range: 0 */
    { 0x54, 0X78 },  /* PA Range: 1 */
    { 0x54, 0X78 },  /* PA Range: 2 */
    { 0x54, 0X78 },  /* PA Range: 3 */
    #else // 1
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
	#endif
  },
  { /* PORT NUM: 119 WCDMA_B1_MB3*/
    { 0x84, 0X98 },  /* PA Range: 0 */
    { 0x54, 0X78 },  /* PA Range: 1 */
    { 0x54, 0X78 },  /* PA Range: 2 */
    { 0x54, 0X78 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 120 WCDMA_B1_MB4*/
    { 0x84, 0X98 },  /* PA Range: 0 */
    { 0x54, 0X78 },  /* PA Range: 1 */
    { 0x54, 0X78 },  /* PA Range: 2 */
    { 0x54, 0X78 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 121 WCDMA_B1_MB5*/
    { 0x84, 0X98 },  /* PA Range: 0 */
    { 0x54, 0X78 },  /* PA Range: 1 */
    { 0x54, 0X78 },  /* PA Range: 2 */
    { 0x54, 0X78 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 122 TDSCDMA_B39_MB1*/
    { 0x88, 0X89 },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 123 TDSCDMA_B39_MB2*/
    { 0x88, 0X89 },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 124 TDSCDMA_B39_MB3*/
    { 0x88, 0X89 },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 125 TDSCDMA_B39_MB4*/
    { 0x88, 0X89 },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 126 TDSCDMA_B39_MB5*/
    { 0x88, 0X89 },  /* PA Range: 0 */
    { 0x58, 0X68 },  /* PA Range: 1 */
    { 0x58, 0X68 },  /* PA Range: 2 */
    { 0x58, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 127 TDSCDMA_B34_MB1*/
    { 0x87, 0X98 },  /* PA Range: 0 */
    { 0x67, 0X68 },  /* PA Range: 1 */
    { 0x67, 0X68 },  /* PA Range: 2 */
    { 0x67, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 128 TDSCDMA_B34_MB2*/
    { 0x87, 0X98 },  /* PA Range: 0 */
    { 0x67, 0X68 },  /* PA Range: 1 */
    { 0x67, 0X68 },  /* PA Range: 2 */
    { 0x67, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 129 TDSCDMA_B34_MB3*/
    { 0x87, 0X98 },  /* PA Range: 0 */
    { 0x67, 0X68 },  /* PA Range: 1 */
    { 0x67, 0X68 },  /* PA Range: 2 */
    { 0x67, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 130 TDSCDMA_B34_MB4*/
    { 0x87, 0X98 },  /* PA Range: 0 */
    { 0x67, 0X68 },  /* PA Range: 1 */
    { 0x67, 0X68 },  /* PA Range: 2 */
    { 0x67, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 131 TDSCDMA_B34_MB5*/
    { 0x87, 0X98 },  /* PA Range: 0 */
    { 0x67, 0X68 },  /* PA Range: 1 */
    { 0x67, 0X68 },  /* PA Range: 2 */
    { 0x67, 0X68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 132 LTE_B12_LB1_LB1IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 133 LTE_B12_LB2_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 134 LTE_B12_LB3_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 135 LTE_B12_LB4_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 136 LTE_B12_LB5_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 137 LTE_B17_LB1_LB1IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 138 LTE_B17_LB2_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 139 LTE_B17_LB3_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 140 LTE_B17_LB4_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 141 LTE_B17_LB5_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 142 LTE_B28_LB1_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 143 LTE_B28_LB2_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 144 LTE_B28_LB3_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 145 LTE_B28_LB4_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 146 LTE_B28_LB5_LB2IN*/
    { 0xFC, 0X9A },  /* PA Range: 0 */
    { 0xF9, 0X46 },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
 { /*  PORT NUM:147 LTE_B13_LB1_LB2IN*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 148 LTE_B13_LB2_LB2IN*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 149 LTE_B13_LB3_LB2IN*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 150 LTE_B13_LB4_LB1IN*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 151 LTE_B13_LB5_LB2IN*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 152 LTE_B14_LB1_LB2IN*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 153 LTE_B14_LB2_LB2IN*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 154 LTE_B14_LB3_LB2IN*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 155 LTE_B14_LB4_LB2IN*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 156 LTE_B14_LB5_LB2IN*/
    { 0x4A, 0XAA },  /* PA Range: 0 */
    { 0x49, 0X46 },  /* PA Range: 1 */
    { 0x49, 0X46 },  /* PA Range: 2 */
    { 0x49, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 157 LTE_B20_LB1_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 158 LTE_B20_LB2_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 159 LTE_B20_LB3_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 160 LTE_B20_LB4_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 161 LTE_B20_LB5_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 162 LTE_B5_LB1_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 163 LTE_B5_LB2_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 164 LTE_B5_LB3_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 165 LTE_B5_LB4_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 166 LTE_B5_LB5_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 167 LTE_B26_LB1_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 168 LTE_B26_LB2_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 169 LTE_B26_LB3_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 170 LTE_B26_LB4_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 171 LTE_B26_LB5_LB2IN*/
    { 0x2A, 0XAA },  /* PA Range: 0 */
    { 0x29, 0X36 },  /* PA Range: 1 */
    { 0x29, 0X36 },  /* PA Range: 2 */
    { 0x29, 0X36 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 172 LTE_B8_LB1_LB2IN*/
    { 0x0A, 0XAA },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 173 LTE_B8_LB2_LB2IN*/
    { 0x0A, 0XAA },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 174 LTE_B8_LB3_LB2IN*/
    { 0x0A, 0XAA },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 175 LTE_B8_LB4_LB2IN*/
    { 0x0A, 0XAA },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 176 LTE_B8_LB5_LB2IN*/
    { 0x0A, 0XAA },  /* PA Range: 0 */
    { 0x09, 0X36 },  /* PA Range: 1 */
    { 0x09, 0X36 },  /* PA Range: 2 */
    { 0x09, 0X36 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 177 LTE_B71_LB1_LB2IN*/
    { 0xFF, 0X9A },  /* PA Range: 0 */
    { 0xFF, 0X9A },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 178 LTE_B71_LB2_LB2IN*/
    { 0xFF, 0X9A },  /* PA Range: 0 */
    { 0xFF, 0X9A },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 179 LTE_B71_LB3_LB2IN*/
    { 0xFF, 0X9A },  /* PA Range: 0 */
    { 0xFF, 0X9A },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 180 LTE_B71_LB4_LB2IN*/
    { 0xFF, 0X9A },  /* PA Range: 0 */
    { 0xFF, 0X9A },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 181 LTE_B71_LB5_LB2IN*/
    { 0xFF, 0X9A },  /* PA Range: 0 */
    { 0xFF, 0X9A },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 182 LTE_B71_LB1_LB2IN*/
    { 0xFF, 0X9A },  /* PA Range: 0 */
    { 0xFF, 0X9A },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 183 LTE_B71_LB2_LB2IN*/
    { 0xFF, 0X9A },  /* PA Range: 0 */
    { 0xFF, 0X9A },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 184 LTE_B71_LB3_LB2IN*/
    { 0xFF, 0X9A },  /* PA Range: 0 */
    { 0xFF, 0X9A },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 185 LTE_B71_LB4_LB2IN*/
    { 0xFF, 0X9A },  /* PA Range: 0 */
    { 0xFF, 0X9A },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 186 LTE_B71_LB5_LB2IN*/
    { 0xFF, 0X9A },  /* PA Range: 0 */
    { 0xFF, 0X9A },  /* PA Range: 1 */
    { 0xF9, 0X46 },  /* PA Range: 2 */
    { 0xF9, 0X46 },  /* PA Range: 3 */
  },
};


#define RFDEVICE_PA_sm5643_51_PA_SET_RANGE_NUM_REGS 1
static uint8 rfdevice_pa_sm5643_51_pa_set_range_regs[RFDEVICE_PA_sm5643_51_PA_SET_RANGE_NUM_REGS] =  {0x00,};
static int16 rfdevice_pa_sm5643_51_pa_set_range_data[RFDEVICE_PA_sm5643_51_NUM_PORTS][4][RFDEVICE_PA_sm5643_51_PA_SET_RANGE_NUM_REGS] =
{
#if 0	
  { /* PORT NUM: 0 LTE_B12_LB1*/
    { 0x0C,  },  /* PA Range: 0 */
    { 0x0E,  },  /* PA Range: 1 */
    { 0x0E,  },  /* PA Range: 2 */
    { 0x0E,  },  /* PA Range: 3 */
  },
 #else
 { /* PORT NUM: 0 LTE_B12_LB1_LB2IN*/
    { 0x0D, },  /* PA Range: 0 */
    { 0x0F, },  /* PA Range: 1 */
    { 0x0F, },  /* PA Range: 2 */
    { 0x0F, },  /* PA Range: 3 */
  },
 #endif
  { /* PORT NUM: 1 LTE_B12_LB2*/
    #if 1 // port 118
    { 0x3C, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
    #else // port 1
    { 0x14,  },  /* PA Range: 0 */
    { 0x16,  },  /* PA Range: 1 */
    { 0x16,  },  /* PA Range: 2 */
    { 0x16,  },  /* PA Range: 3 */
    #endif
  },
  { /* PORT NUM: 2 LTE_B12_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 3 LTE_B12_LB4*/
    #if 1 // port 97
    { 0x0C, },	/* PA Range: 0 */
    { 0x0E, },	/* PA Range: 1 */
    { 0x0E, },	/* PA Range: 2 */
    { 0x0E, },	/* PA Range: 3 */
    #else // port 3
    { 0x24, },	/* PA Range: 0 */
    { 0x26, },	/* PA Range: 1 */
    { 0x26, },	/* PA Range: 2 */
    { 0x26, },	/* PA Range: 3 */
    #endif
  },
  { /* PORT NUM: 4 LTE_B12_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
#if 0  
  { /* PORT NUM: 5 LTE_B17_LB1*/
    { 0x0C,  },  /* PA Range: 0 */
    { 0x0E,  },  /* PA Range: 1 */
    { 0x0E,  },  /* PA Range: 2 */
    { 0x0E,  },  /* PA Range: 3 */
  },  
#else  
   { /* PORT NUM: 5 LTE_B17_LB1_LB2IN*/
    { 0x0D, },  /* PA Range: 0 */
    { 0x0F, },  /* PA Range: 1 */
    { 0x0F, },  /* PA Range: 2 */
    { 0x0F, },  /* PA Range: 3 */
  },
#endif
  { /* PORT NUM: 6 LTE_B17_LB2*/
    { 0x14,  },  /* PA Range: 0 */
    { 0x16,  },  /* PA Range: 1 */
    { 0x16,  },  /* PA Range: 2 */
    { 0x16,  },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 7 LTE_B17_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 8 LTE_B17_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 9 LTE_B17_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 10 LTE_B28_LB1*/
    { 0x0C,  },  /* PA Range: 0 */
    { 0x0E,  },  /* PA Range: 1 */
    { 0x0E,  },  /* PA Range: 2 */
    { 0x0E,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 11 LTE_B28_LB2*/
    { 0x14,  },  /* PA Range: 0 */
    { 0x16,  },  /* PA Range: 1 */
    { 0x16,  },  /* PA Range: 2 */
    { 0x16,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 12 LTE_B28_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 13 LTE_B28_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 14 LTE_B28_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 15 LTE_B13_LB1*/
    { 0x0C,  },  /* PA Range: 0 */
    { 0x0E,  },  /* PA Range: 1 */
    { 0x0E,  },  /* PA Range: 2 */
    { 0x0E,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 16 LTE_B13_LB2*/
    { 0x14,  },  /* PA Range: 0 */
    { 0x16,  },  /* PA Range: 1 */
    { 0x16,  },  /* PA Range: 2 */
    { 0x16,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 17 LTE_B13_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },
#if 0  
  { /* PORT NUM: 18 LTE_B13_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },
#else
  { /* PORT NUM: 18 LTE_B13_LB4_LB2IN*/
    { 0x25, },  /* PA Range: 0 */
    { 0x27, },  /* PA Range: 1 */
    { 0x27, },  /* PA Range: 2 */
    { 0x27, },  /* PA Range: 3 */
  },
#endif
  { /* PORT NUM: 19 LTE_B13_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 20 LTE_B14_LB1*/
    { 0x0C,  },  /* PA Range: 0 */
    { 0x0E,  },  /* PA Range: 1 */
    { 0x0E,  },  /* PA Range: 2 */
    { 0x0E,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 21 LTE_B14_LB2*/
    { 0x14,  },  /* PA Range: 0 */
    { 0x16,  },  /* PA Range: 1 */
    { 0x16,  },  /* PA Range: 2 */
    { 0x16,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 22 LTE_B14_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 23 LTE_B14_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 24 LTE_B14_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 25 LTE_B20_LB1*/
    { 0x0C,  },  /* PA Range: 0 */
    { 0x0E,  },  /* PA Range: 1 */
    { 0x0E,  },  /* PA Range: 2 */
    { 0x0E,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 26 LTE_B20_LB2*/
    { 0x14,  },  /* PA Range: 0 */
    { 0x16,  },  /* PA Range: 1 */
    { 0x16,  },  /* PA Range: 2 */
    { 0x16,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 27 LTE_B20_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 28 LTE_B20_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 29 LTE_B20_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 30 LTE_B5_LB1*/
    { 0x0C,  },  /* PA Range: 0 */
    { 0x0E,  },  /* PA Range: 1 */
    { 0x0E,  },  /* PA Range: 2 */
    { 0x0E,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 31 LTE_B5_LB2*/
    { 0x14,  },  /* PA Range: 0 */
    { 0x16,  },  /* PA Range: 1 */
    { 0x16,  },  /* PA Range: 2 */
    { 0x16,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 32 LTE_B5_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 33 LTE_B5_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 34 LTE_B5_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 35 LTE_B26_LB1*/
    { 0x0C,  },  /* PA Range: 0 */
    { 0x0E,  },  /* PA Range: 1 */
    { 0x0E,  },  /* PA Range: 2 */
    { 0x0E,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 36 LTE_B26_LB2*/
    { 0x14,  },  /* PA Range: 0 */
    { 0x16,  },  /* PA Range: 1 */
    { 0x16,  },  /* PA Range: 2 */
    { 0x16,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 37 LTE_B26_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 38 LTE_B26_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 39 LTE_B26_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 40 LTE_B8_LB1*/
    { 0x0C,  },  /* PA Range: 0 */
    { 0x0E,  },  /* PA Range: 1 */
    { 0x0E,  },  /* PA Range: 2 */
    { 0x0E,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 41 LTE_B8_LB2*/
    { 0x14,  },  /* PA Range: 0 */
    { 0x16,  },  /* PA Range: 1 */
    { 0x16,  },  /* PA Range: 2 */
    { 0x16,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 42 LTE_B8_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 43 LTE_B8_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 44 LTE_B8_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 45 LTE_B4_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 46 LTE_B4_MB2*/
    { 0x3C, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 47 LTE_B4_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 48 LTE_B4_MB4*/
    { 0x4C, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 49 LTE_B4_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 50 LTE_B3_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 51 LTE_B3_MB2*/
    { 0x3C, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 52 LTE_B3_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 53 LTE_B3_MB4*/
    { 0x4C, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 54 LTE_B3_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 55 LTE_B2_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },
  { /* PORT NUM:56 LTE_B2_MB2*/
    { 0x3C, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 57 LTE_B2_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 58 LTE_B2_MB4*/
    { 0x4C, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 59 LTE_B2_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 60 LTE_B1_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 61 LTE_B1_MB2*/
    { 0x3C, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 62 LTE_B1_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 63 LTE_B1_MB4*/
    { 0x4C, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 64 LTE_B1_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 65 LTE_B39_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 66 LTE_B39_MB2*/
    { 0x3C, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 67 LTE_B39_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 68 LTE_B39_MB4*/
    { 0x4C, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 69 LTE_B39_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 70 LTE_B34_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 71 LTE_B34_MB2*/
    { 0x3C, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 72 LTE_B34_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 73 LTE_B34_MB4*/
    { 0x4C, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 74 LTE_B34_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 75 LTE_B7_HB1*/
    { 0x5C, },  /* PA Range: 0 */
    { 0x5E, },  /* PA Range: 1 */
    { 0x5E, },  /* PA Range: 2 */
    { 0x5E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 76 LTE_B7_HB2*/
    { 0x64, },  /* PA Range: 0 */
    { 0x66, },  /* PA Range: 1 */
    { 0x66, },  /* PA Range: 2 */
    { 0x66, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 77 LTE_B7_HB3*/
    { 0x6C, },  /* PA Range: 0 */
    { 0x6E, },  /* PA Range: 1 */
    { 0x6E, },  /* PA Range: 2 */
    { 0x6E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 78 LTE_B7_HB4*/
    { 0x74, },  /* PA Range: 0 */
    { 0x76, },  /* PA Range: 1 */
    { 0x76, },  /* PA Range: 2 */
    { 0x76, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 79 LTE_B40_HB1*/
    { 0x5C, },  /* PA Range: 0 */
    { 0x5E, },  /* PA Range: 1 */
    { 0x5E, },  /* PA Range: 2 */
    { 0x5E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 80 LTE_B40_HB2*/
    { 0x64, },  /* PA Range: 0 */
    { 0x66, },  /* PA Range: 1 */
    { 0x66, },  /* PA Range: 2 */
    { 0x66, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 81 LTE_B40_HB3*/
    { 0x6C, },  /* PA Range: 0 */
    { 0x6E, },  /* PA Range: 1 */
    { 0x6E, },  /* PA Range: 2 */
    { 0x6E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 82 LTE_B40_HB4*/
    { 0x74, },  /* PA Range: 0 */
    { 0x76, },  /* PA Range: 1 */
    { 0x76, },  /* PA Range: 2 */
    { 0x76, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 83 LTE_B38_41_HB1*/
    { 0x5C, },  /* PA Range: 0 */
    { 0x5E, },  /* PA Range: 1 */
    { 0x5E, },  /* PA Range: 2 */
    { 0x5E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 84 LTE_B38_41_HB2*/
    { 0x64, },  /* PA Range: 0 */
    { 0x66, },  /* PA Range: 1 */
    { 0x66, },  /* PA Range: 2 */
    { 0x66, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 85 LTE_B38_41_HB3*/
    { 0x6C, },  /* PA Range: 0 */
    { 0x6E, },  /* PA Range: 1 */
    { 0x6E, },  /* PA Range: 2 */
    { 0x6E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 86 LTE_B38_41_HB4*/
    { 0x74, },  /* PA Range: 0 */
    { 0x76, },  /* PA Range: 1 */
    { 0x76, },  /* PA Range: 2 */
    { 0x76, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 87 WCDMA_B5_LB1*/
    { 0x0C, },  /* PA Range: 0 */
    { 0x0E, },  /* PA Range: 1 */
    { 0x0E, },  /* PA Range: 2 */
    { 0x0E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 88 WCDMA_B5_LB2*/
    { 0x14, },  /* PA Range: 0 */
    { 0x16, },  /* PA Range: 1 */
    { 0x16, },  /* PA Range: 2 */
    { 0x16, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 89 WCDMA_B5_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 90 WCDMA_B5_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 91 WCDMA_B5_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 92 WCDMA_B8_LB1*/
    { 0x0C, },  /* PA Range: 0 */
    { 0x0E, },  /* PA Range: 1 */
    { 0x0E, },  /* PA Range: 2 */
    { 0x0E, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 93 WCDMA_B8_LB2*/
    { 0x14, },  /* PA Range: 0 */
    { 0x16, },  /* PA Range: 1 */
    { 0x16, },  /* PA Range: 2 */
    { 0x16, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 94 WCDMA_B8_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 95 WCDMA_B8_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 96 WCDMA_B8_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 97 CDMA_BC0_LB1*/
    #if 0 // port 97
    { 0x0C, },  /* PA Range: 0 */
    { 0x0E, },  /* PA Range: 1 */
    { 0x0E, },  /* PA Range: 2 */
    { 0x0E, },  /* PA Range: 3 */
    #else // port 3
    { 0x24, },	/* PA Range: 0 */
    { 0x26, },	/* PA Range: 1 */
    { 0x26, },	/* PA Range: 2 */
    { 0x26, },	/* PA Range: 3 */
    #endif
  },  
  { /* PORT NUM: 98 CDMA_BC0_LB2*/
    { 0x14, },  /* PA Range: 0 */
    { 0x16, },  /* PA Range: 1 */
    { 0x16, },  /* PA Range: 2 */
    { 0x16, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 99 CDMA_BC0_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 100 CDMA_BC0_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 101 CDMA_BC0_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 102 WCDMA_B4_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 103 WCDMA_B4_MB2*/
    { 0x3c, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 104 WCDMA_B4_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 105 WCDMA_B4_MB4*/
    { 0x4c, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 106 WCDMA_B4_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 107 WCDMA_B3_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 108 WCDMA_B3_MB2*/
    { 0x3c, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 109 WCDMA_B3_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 110 WCDMA_B3_MB4*/
    { 0x4c, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 111 WCDMA_B3_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 112 WCDMA_B2_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 113 WCDMA_B2_MB2*/
    { 0x3C, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 114 WCDMA_B2_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 115 WCDMA_B2_MB4*/
    { 0x4C, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 116 WCDMA_B2_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 117 WCDMA_B1_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 118 WCDMA_B1_MB2*/
    #if 0 // port 118
    { 0x3C, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
    #else // port 1
    { 0x14,  },  /* PA Range: 0 */
    { 0x16,  },  /* PA Range: 1 */
    { 0x16,  },  /* PA Range: 2 */
    { 0x16,  },  /* PA Range: 3 */
    #endif
  },
  { /* PORT NUM: 119 WCDMA_B1_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 120 WCDMA_B1_MB4*/
    { 0x4C, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 121 WCDMA_B1_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 122 TDSCDMA_B39_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 123 TDSCDMA_B39_MB2*/
    { 0x3c, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 124 TDSCDMA_B39_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 125 TDSCDMA_B39_MB4*/
    { 0x4c, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 126 TDSCDMA_B39_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 127 TDSCDMA_B34_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x36, },  /* PA Range: 2 */
    { 0x36, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 128 TDSCDMA_B34_MB2*/
    { 0x3c, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3E, },  /* PA Range: 2 */
    { 0x3E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 129 TDSCDMA_B34_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x46, },  /* PA Range: 2 */
    { 0x46, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 130 TDSCDMA_B34_MB4*/
    { 0x4c, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4E, },  /* PA Range: 2 */
    { 0x4E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 131 TDSCDMA_B34_MB5*/
    { 0x54, },  /* PA Range: 0 */
    { 0x56, },  /* PA Range: 1 */
    { 0x56, },  /* PA Range: 2 */
    { 0x56, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 132 LTE_B12_LB1_LB1IN*/
    { 0x0C, },  /* PA Range: 0 */
    { 0x0E, },  /* PA Range: 1 */
    { 0x0E, },  /* PA Range: 2 */
    { 0x0E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 133 LTE_B12_LB2_LB2IN*/
    { 0x15, },  /* PA Range: 0 */
    { 0x17, },  /* PA Range: 1 */
    { 0x17, },  /* PA Range: 2 */
    { 0x17, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 134 LTE_B12_LB3_LB2IN*/
    { 0x1D, },  /* PA Range: 0 */
    { 0x1F, },  /* PA Range: 1 */
    { 0x1F, },  /* PA Range: 2 */
    { 0x1F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 135 LTE_B12_LB4_LB2IN*/
    { 0x25, },  /* PA Range: 0 */
    { 0x27, },  /* PA Range: 1 */
    { 0x27, },  /* PA Range: 2 */
    { 0x27, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 136 LTE_B12_LB5_LB2IN*/
    { 0x2D, },  /* PA Range: 0 */
    { 0x2F, },  /* PA Range: 1 */
    { 0x2F, },  /* PA Range: 2 */
    { 0x2F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 137 LTE_B17_LB1_LB1IN*/
    { 0x0C, },  /* PA Range: 0 */
    { 0x0E, },  /* PA Range: 1 */
    { 0x0E, },  /* PA Range: 2 */
    { 0x0E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 138 LTE_B17_LB2_LB2IN*/
    { 0x15, },  /* PA Range: 0 */
    { 0x17, },  /* PA Range: 1 */
    { 0x17, },  /* PA Range: 2 */
    { 0x17, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 139 LTE_B17_LB3_LB2IN*/
    { 0x1D, },  /* PA Range: 0 */
    { 0x1F, },  /* PA Range: 1 */
    { 0x1F, },  /* PA Range: 2 */
    { 0x1F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 140 LTE_B17_LB4_LB2IN*/
    { 0x25, },  /* PA Range: 0 */
    { 0x27, },  /* PA Range: 1 */
    { 0x27, },  /* PA Range: 2 */
    { 0x27, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 141 LTE_B17_LB5_LB2IN*/
    { 0x2D, },  /* PA Range: 0 */
    { 0x2F, },  /* PA Range: 1 */
    { 0x2F, },  /* PA Range: 2 */
    { 0x2F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 142 LTE_B28_LB1_LB2IN*/
    { 0x0D, },  /* PA Range: 0 */
    { 0x0F, },  /* PA Range: 1 */
    { 0x0F, },  /* PA Range: 2 */
    { 0x0F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 143 LTE_B28_LB2_LB2IN*/
    { 0x15, },  /* PA Range: 0 */
    { 0x17, },  /* PA Range: 1 */
    { 0x17, },  /* PA Range: 2 */
    { 0x17, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 144 LTE_B28_LB3_LB2IN*/
    { 0x1D, },  /* PA Range: 0 */
    { 0x1F, },  /* PA Range: 1 */
    { 0x1F, },  /* PA Range: 2 */
    { 0x1F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 145 LTE_B28_LB4_LB2IN*/
    { 0x25, },  /* PA Range: 0 */
    { 0x27, },  /* PA Range: 1 */
    { 0x27, },  /* PA Range: 2 */
    { 0x27, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 146 LTE_B28_LB5_LB2IN*/
    { 0x2D, },  /* PA Range: 0 */
    { 0x2F, },  /* PA Range: 1 */
    { 0x2F, },  /* PA Range: 2 */
    { 0x2F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 147 LTE_B13_LB1_LB2IN*/
    { 0x0D, },  /* PA Range: 0 */
    { 0x0F, },  /* PA Range: 1 */
    { 0x0F, },  /* PA Range: 2 */
    { 0x0F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 148 LTE_B13_LB2_LB2IN*/
    { 0x15, },  /* PA Range: 0 */
    { 0x17, },  /* PA Range: 1 */
    { 0x17, },  /* PA Range: 2 */
    { 0x17, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 149 LTE_B13_LB3_LB2IN*/
    { 0x1D, },  /* PA Range: 0 */
    { 0x1F, },  /* PA Range: 1 */
    { 0x1F, },  /* PA Range: 2 */
    { 0x1F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 150 LTE_B13_LB4_LB1IN*/
    { 0x24, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 151 LTE_B13_LB5_LB2IN*/
    { 0x2D, },  /* PA Range: 0 */
    { 0x2F, },  /* PA Range: 1 */
    { 0x2F, },  /* PA Range: 2 */
    { 0x2F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 152 LTE_B14_LB1_LB2IN*/
    { 0x0D, },  /* PA Range: 0 */
    { 0x0F, },  /* PA Range: 1 */
    { 0x0F, },  /* PA Range: 2 */
    { 0x0F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 153 LTE_B14_LB2_LB2IN*/
    { 0x16, },  /* PA Range: 0 */
    { 0x18, },  /* PA Range: 1 */
    { 0x18, },  /* PA Range: 2 */
    { 0x18, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 154 LTE_B14_LB3_LB2IN*/
    { 0x1E, },  /* PA Range: 0 */
    { 0x20, },  /* PA Range: 1 */
    { 0x20, },  /* PA Range: 2 */
    { 0x20, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 155 LTE_B14_LB4_LB2IN*/
    { 0x26, },  /* PA Range: 0 */
    { 0x28, },  /* PA Range: 1 */
    { 0x28, },  /* PA Range: 2 */
    { 0x28, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 156 LTE_B14_LB5_LB2IN*/
    { 0x2E, },  /* PA Range: 0 */
    { 0x30, },  /* PA Range: 1 */
    { 0x30, },  /* PA Range: 2 */
    { 0x30, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 157 LTE_B20_LB1_LB2IN*/
    { 0x0E, },  /* PA Range: 0 */
    { 0x0F, },  /* PA Range: 1 */
    { 0x0F, },  /* PA Range: 2 */
    { 0x0F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 158 LTE_B20_LB2_LB2IN*/
    { 0x15, },  /* PA Range: 0 */
    { 0x17, },  /* PA Range: 1 */
    { 0x17, },  /* PA Range: 2 */
    { 0x17, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 159 LTE_B20_LB3_LB2IN*/
    { 0x1D, },  /* PA Range: 0 */
    { 0x1F, },  /* PA Range: 1 */
    { 0x1F, },  /* PA Range: 2 */
    { 0x1F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 160 LTE_B20_LB4_LB2IN*/
    { 0x25, },  /* PA Range: 0 */
    { 0x27, },  /* PA Range: 1 */
    { 0x27, },  /* PA Range: 2 */
    { 0x27, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 161 LTE_B20_LB5_LB2IN*/
    { 0x2D, },  /* PA Range: 0 */
    { 0x2F, },  /* PA Range: 1 */
    { 0x2F, },  /* PA Range: 2 */
    { 0x2F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 162 LTE_B5_LB1_LB2IN*/
    { 0x0D, },  /* PA Range: 0 */
    { 0x0F, },  /* PA Range: 1 */
    { 0x0F, },  /* PA Range: 2 */
    { 0x0F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 163 LTE_B5_LB2_LB2IN/ */
    { 0x15, },  /* PA Range: 0 */
    { 0x17, },  /* PA Range: 1 */
    { 0x17, },  /* PA Range: 2 */
    { 0x17, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 164 LTE_B5_LB3_LB2IN*/
    { 0x1D, },  /* PA Range: 0 */
    { 0x1F, },  /* PA Range: 1 */
    { 0x1F, },  /* PA Range: 2 */
    { 0x1F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 165 LTE_B5_LB4_LB2IN*/
    { 0x25, },  /* PA Range: 0 */
    { 0x27, },  /* PA Range: 1 */
    { 0x27, },  /* PA Range: 2 */
    { 0x27, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 166  LTE_B5_LB5_LB2IN*/
    { 0x2D, },  /* PA Range: 0 */
    { 0x2F, },  /* PA Range: 1 */
    { 0x2F, },  /* PA Range: 2 */
    { 0x2F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 167 LTE_B26_LB1_LB2IN*/
    { 0x0D, },  /* PA Range: 0 */
    { 0x0F, },  /* PA Range: 1 */
    { 0x0F, },  /* PA Range: 2 */
    { 0x0F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 168 LTE_B26_LB2_LB2IN*/
    { 0x15,  },  /* PA Range: 0 */
    { 0x17,  },  /* PA Range: 1 */
    { 0x17,  },  /* PA Range: 2 */
    { 0x17,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 169 LTE_B26_LB3_LB2IN*/
    { 0x1D, },  /* PA Range: 0 */
    { 0x1F, },  /* PA Range: 1 */
    { 0x1F, },  /* PA Range: 2 */
    { 0x1F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 170 LTE_B26_LB4_LB2IN*/
    { 0x25, },  /* PA Range: 0 */
    { 0x27, },  /* PA Range: 1 */
    { 0x27, },  /* PA Range: 2 */
    { 0x27, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 171 LTE_B26_LB5_LB2IN*/
    { 0x2D, },  /* PA Range: 0 */
    { 0x2F, },  /* PA Range: 1 */
    { 0x2F, },  /* PA Range: 2 */
    { 0x2F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 172 LTE_B8_LB1_LB2IN*/
    { 0x0D, },  /* PA Range: 0 */
    { 0x0F, },  /* PA Range: 1 */
    { 0x0F, },  /* PA Range: 2 */
    { 0x0F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 173 LTE_B8_LB2_LB2IN*/
    { 0x15, },  /* PA Range: 0 */
    { 0x17, },  /* PA Range: 1 */
    { 0x17, },  /* PA Range: 2 */
    { 0x17, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 174 LTE_B8_LB3_LB2IN*/
    { 0x1D, },  /* PA Range: 0 */
    { 0x1F, },  /* PA Range: 1 */
    { 0x1F, },  /* PA Range: 2 */
    { 0x1F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 175 LTE_B8_LB4_LB2IN*/
    { 0x25, },  /* PA Range: 0 */
    { 0x27, },  /* PA Range: 1 */
    { 0x27, },  /* PA Range: 2 */
    { 0x27, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 176 LTE_B8_LB5_LB2IN*/
    { 0x2D, },  /* PA Range: 0 */
    { 0x2F, },  /* PA Range: 1 */
    { 0x2F, },  /* PA Range: 2 */
    { 0x2F, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 177 LTE_B71_LB1*/
    { 0x0E, },  /* PA Range: 0 */
    { 0x0E, },  /* PA Range: 1 */
    { 0x0E, },  /* PA Range: 2 */
    { 0x0E, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 178 LTE_B71_LB2*/
    { 0x16, },  /* PA Range: 0 */
    { 0x16, },  /* PA Range: 1 */
    { 0x16, },  /* PA Range: 2 */
    { 0x16, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 179 LTE_B71_LB3*/
    { 0x1E, },  /* PA Range: 0 */
    { 0x1E, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1E, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 180 LTE_B71_LB4*/
    { 0x26, },  /* PA Range: 0 */
    { 0x26, },  /* PA Range: 1 */
    { 0x26, },  /* PA Range: 2 */
    { 0x26, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 181 LTE_B71_LB5*/
    { 0x2E, },  /* PA Range: 0 */
    { 0x2E, },  /* PA Range: 1 */
    { 0x2E, },  /* PA Range: 2 */
    { 0x2E, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 182 LTE_B71_LB1_LB2IN*/
    { 0x0F, },  /* PA Range: 0 */
    { 0x0F, },  /* PA Range: 1 */
    { 0x0F, },  /* PA Range: 2 */
    { 0x0F, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 183 LTE_B71_LB2_LB2IN*/
    { 0x17, },  /* PA Range: 0 */
    { 0x17, },  /* PA Range: 1 */
    { 0x17, },  /* PA Range: 2 */
    { 0x17, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 184 LTE_B71_LB3_LB2IN*/
    { 0x1F, },  /* PA Range: 0 */
    { 0x1F, },  /* PA Range: 1 */
    { 0x1F, },  /* PA Range: 2 */
    { 0x1F, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 185 LTE_B71_LB4_LB2IN*/
    { 0x27, },  /* PA Range: 0 */
    { 0x27, },  /* PA Range: 1 */
    { 0x27, },  /* PA Range: 2 */
    { 0x27, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 186 LTE_B71_LB5_LB2IN*/
    { 0x2F, },  /* PA Range: 0 */
    { 0x2F, },  /* PA Range: 1 */
    { 0x2F, },  /* PA Range: 2 */
    { 0x2F, },  /* PA Range: 3 */
  },
};


#define RFDEVICE_PA_sm5643_51_PA_ON_NUM_REGS 1
static uint8 rfdevice_pa_sm5643_51_pa_on_regs[RFDEVICE_PA_sm5643_51_PA_ON_NUM_REGS] =  {RFFE_INVALID_REG_ADDR /*Warning: Not Specified*/, };
static int16 rfdevice_pa_sm5643_51_pa_on_data[RFDEVICE_PA_sm5643_51_NUM_PORTS][RFDEVICE_PA_sm5643_51_PA_ON_NUM_REGS] =
{
  { /* PORT NUM: 0 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 1 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 2 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 3 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 4 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 5 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 6 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 7 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 8 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 9 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 10 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 11 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 12 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 13 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 14 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 15 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 16 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 17 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 18 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 19 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 20 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 21 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 22 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 23 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 24 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 25 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 26 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 27 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 28 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 29 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 30 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 31 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 32 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 33 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 34 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 35 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 36 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 37 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 38 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 39 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 40 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 41 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 42 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 43 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 44 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 45 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 46 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 47 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 48 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 49 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 50 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 51 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 52 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 53 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 54 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 55 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 56 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 57 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 58 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 59 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 60 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 61 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 62 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 63 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 64 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 65 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 66 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 67 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 68 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 69 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 70 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 71 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 72 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 73 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 74 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 75 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 76 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 77 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 78 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 79 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 80 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 81 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 82 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 83 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 84 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 85 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 86 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 87 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 88 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 89 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 90 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 91 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 92 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 93 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 94 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 95 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 96 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 97 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 98 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 99 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 100 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 101 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 102 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 103 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 104 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 105 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 106 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 107 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 108 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 109 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 110 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 111 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 112 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 113 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 114 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 115 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 116 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 117 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 118 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 119 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 120 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 121 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 122 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 123 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 124 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 125 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 126 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 127 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 128 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 129 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 130 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 131 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 132 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 133 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 134 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 135 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 136 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 137 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 138 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 139 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 140 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 141 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 142 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 143 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 144 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 145 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 146 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 147 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 148 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 149 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 150 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 151 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 152 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 153 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 154 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 155 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 156 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 157 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 158 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 159 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 160 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 161 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 162 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 163 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 164 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 165 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 166 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 167 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 168 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 169 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 170 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 171 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 172 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 173 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 174 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 175 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 176 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 177 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 178 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 179 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 180 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 181 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 182 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 183 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 184 */
    RF_REG_INVALID, 
  },
  { /* PORT NUM: 186 */
    RF_REG_INVALID, 
  },

};


#define RFDEVICE_PA_sm5643_51_PA_OFF_NUM_REGS 1
static uint8 rfdevice_pa_sm5643_51_pa_off_regs[RFDEVICE_PA_sm5643_51_PA_OFF_NUM_REGS] =  {0x00, };
static int16 rfdevice_pa_sm5643_51_pa_off_data[RFDEVICE_PA_sm5643_51_NUM_PORTS][RFDEVICE_PA_sm5643_51_PA_OFF_NUM_REGS] =
{
  { /* PORT NUM: 0 */
    0x00, 
  },
  { /* PORT NUM: 1 */
    0x00, 
  },
  { /* PORT NUM: 2 */
    0x00, 
  },
  { /* PORT NUM: 3 */
    0x00, 
  },
  { /* PORT NUM: 4 */
    0x00, 
  },
  { /* PORT NUM: 5 */
    0x00, 
  },
  { /* PORT NUM: 6 */
    0x00, 
  },
  { /* PORT NUM: 7 */
    0x00, 
  },
  { /* PORT NUM: 8 */
    0x00, 
  },
  { /* PORT NUM: 9 */
    0x00, 
  },
  { /* PORT NUM: 10 */
    0x00, 
  },
  { /* PORT NUM: 11 */
    0x00, 
  },
  { /* PORT NUM: 12 */
    0x00, 
  },
  { /* PORT NUM: 13 */
    0x00, 
  },
  { /* PORT NUM: 14 */
    0x00, 
  },
  { /* PORT NUM: 15 */
    0x00, 
  },
  { /* PORT NUM: 16 */
    0x00, 
  },
  { /* PORT NUM: 17 */
    0x00, 
  },
  { /* PORT NUM: 18 */
    0x00, 
  },
  { /* PORT NUM: 19 */
    0x00, 
  },
  { /* PORT NUM: 20 */
    0x00, 
  },
  { /* PORT NUM: 21 */
    0x00, 
  },
  { /* PORT NUM: 22 */
    0x00, 
  },
  { /* PORT NUM: 23 */
    0x00, 
  },
  { /* PORT NUM: 24 */
    0x00, 
  },
  { /* PORT NUM: 25 */
    0x00, 
  },
  { /* PORT NUM: 26 */
    0x00, 
  },
  { /* PORT NUM: 27 */
    0x00, 
  },
  { /* PORT NUM: 28 */
    0x00, 
  },
  { /* PORT NUM: 29 */
    0x00, 
  },
  { /* PORT NUM: 30 */
    0x00, 
  },
  { /* PORT NUM: 31 */
    0x00, 
  },
  { /* PORT NUM: 32 */
    0x00, 
  },
  { /* PORT NUM: 33 */
    0x00, 
  },
  { /* PORT NUM: 34 */
    0x00, 
  },
  { /* PORT NUM: 35 */
    0x00, 
  },
  { /* PORT NUM: 36 */
    0x00, 
  },
  { /* PORT NUM: 37 */
    0x00, 
  },
  { /* PORT NUM: 38 */
    0x00, 
  },
  { /* PORT NUM: 39 */
    0x00, 
  },
  { /* PORT NUM: 40 */
    0x00, 
  },
  { /* PORT NUM: 41 */
    0x00, 
  },
  { /* PORT NUM: 42 */
    0x00, 
  },
  { /* PORT NUM: 43 */
    0x00, 
  },
  { /* PORT NUM: 44 */
    0x00, 
  },
  { /* PORT NUM: 45 */
    0x00, 
  },
  { /* PORT NUM: 46 */
    0x00, 
  },
  { /* PORT NUM: 47 */
    0x00, 
  },
  { /* PORT NUM: 48 */
    0x00, 
  },
  { /* PORT NUM: 49 */
    0x00, 
  },
  { /* PORT NUM: 50 */
    0x00, 
  },
  { /* PORT NUM: 51 */
    0x00, 
  },
  { /* PORT NUM: 52 */
    0x00, 
  },
  { /* PORT NUM: 53 */
    0x00, 
  },
  { /* PORT NUM: 54 */
    0x00, 
  },
  { /* PORT NUM: 55 */
    0x00, 
  },
  { /* PORT NUM: 56 */
    0x00, 
  },
  { /* PORT NUM: 57 */
    0x00, 
  },
  { /* PORT NUM: 58 */
    0x00, 
  },
  { /* PORT NUM: 59 */
    0x00, 
  },
  { /* PORT NUM: 60 */
    0x00, 
  },
  { /* PORT NUM: 61 */
    0x00, 
  },
  { /* PORT NUM: 62 */
    0x00, 
  },
  { /* PORT NUM: 63 */
    0x00, 
  },
  { /* PORT NUM: 64 */
    0x00, 
  },
  { /* PORT NUM: 65 */
    0x00, 
  },
  { /* PORT NUM: 66 */
    0x00, 
  },
  { /* PORT NUM: 67 */
    0x00, 
  },
  { /* PORT NUM: 68 */
    0x00, 
  },
  { /* PORT NUM: 69 */
    0x00, 
  },
  { /* PORT NUM: 70 */
    0x00, 
  },
  { /* PORT NUM: 71 */
    0x00, 
  },
  { /* PORT NUM: 72 */
    0x00, 
  },
  { /* PORT NUM: 73 */
    0x00, 
  },
  { /* PORT NUM: 74 */
    0x00, 
  },
  { /* PORT NUM: 75 */
    0x00, 
  },
  { /* PORT NUM: 76 */
    0x00, 
  },
  { /* PORT NUM: 77 */
    0x00, 
  },
  { /* PORT NUM: 78 */
    0x00, 
  },
  { /* PORT NUM: 79 */
    0x00, 
  },
  { /* PORT NUM: 80 */
    0x00, 
  },
  { /* PORT NUM: 81 */
    0x00, 
  },
  { /* PORT NUM: 82 */
    0x00, 
  },
  { /* PORT NUM: 83 */
    0x00, 
  },
  { /* PORT NUM: 84 */
    0x00, 
  },
  { /* PORT NUM: 85 */
    0x00, 
  },
  { /* PORT NUM: 86 */
    0x00, 
  },
  { /* PORT NUM: 87 */
    0x00, 
  },
  { /* PORT NUM: 88 */
    0x00, 
  },
  { /* PORT NUM: 89 */
    0x00, 
  },
  { /* PORT NUM: 90 */
    0x00, 
  },
  { /* PORT NUM: 91 */
    0x00, 
  },
  { /* PORT NUM: 92 */
    0x00, 
  },
  { /* PORT NUM: 93 */
    0x00, 
  },
  { /* PORT NUM: 94 */
    0x00, 
  },
  { /* PORT NUM: 95 */
    0x00, 
  },
  { /* PORT NUM: 96 */
    0x00, 
  },
  { /* PORT NUM: 97 */
    0x00, 
  },
  { /* PORT NUM: 98 */
    0x00, 
  },
  { /* PORT NUM: 99 */
    0x00, 
  },
  { /* PORT NUM: 100 */
    0x00, 
  },
  { /* PORT NUM: 101 */
    0x00, 
  },
  { /* PORT NUM: 102 */
    0x00, 
  },
  { /* PORT NUM: 103 */
    0x00, 
  },
  { /* PORT NUM: 104 */
    0x00, 
  },
  { /* PORT NUM: 105 */
    0x00, 
  },
  { /* PORT NUM: 106 */
    0x00, 
  },
  { /* PORT NUM: 107 */
    0x00, 
  },
  { /* PORT NUM: 108 */
    0x00, 
  },
  { /* PORT NUM: 109 */
    0x00, 
  },
  { /* PORT NUM: 110 */
    0x00, 
  },
  { /* PORT NUM: 111 */
    0x00, 
  },
  { /* PORT NUM: 112 */
    0x00, 
  },
  { /* PORT NUM: 113 */
    0x00, 
  },
  { /* PORT NUM: 114 */
    0x00, 
  },
  { /* PORT NUM: 115 */
    0x00, 
  },
  { /* PORT NUM: 116 */
    0x00, 
  },
  { /* PORT NUM: 117 */
    0x00, 
  },
  { /* PORT NUM: 118 */
    0x00, 
  },
  { /* PORT NUM: 119 */
    0x00, 
  },
  { /* PORT NUM: 120 */
    0x00, 
  },
  { /* PORT NUM: 121 */
    0x00, 
  },
  { /* PORT NUM: 122 */
    0x00, 
  },
  { /* PORT NUM: 123 */
    0x00, 
  },
  { /* PORT NUM: 124 */
    0x00, 
  },
  { /* PORT NUM: 125 */
    0x00, 
  },
  { /* PORT NUM: 126 */
    0x00, 
  },
  { /* PORT NUM: 127 */
    0x00, 
  },
  { /* PORT NUM: 128 */
    0x00, 
  },
  { /* PORT NUM: 129 */
    0x00, 
  },
  { /* PORT NUM: 130 */
    0x00, 
  },
  { /* PORT NUM: 131 */
    0x00, 
  },
  { /* PORT NUM: 132 */
    0x00, 
  },
  { /* PORT NUM: 133 */
    0x00, 
  },
  { /* PORT NUM: 134 */
    0x00, 
  },
  { /* PORT NUM: 135 */
    0x00, 
  },
  { /* PORT NUM: 136 */
    0x00, 
  },
  { /* PORT NUM: 137 */
    0x00, 
  },
  { /* PORT NUM: 138 */
    0x00, 
  },
  { /* PORT NUM: 139 */
    0x00, 
  },
  { /* PORT NUM: 140 */
    0x00, 
  },
  { /* PORT NUM: 141 */
    0x00, 
  },
  { /* PORT NUM: 142 */
    0x00, 
  },
  { /* PORT NUM: 143 */
    0x00, 
  },
  { /* PORT NUM: 144 */
    0x00, 
  },
  { /* PORT NUM: 145 */
    0x00, 
  },
  { /* PORT NUM: 146 */
    0x00, 
  },
  { /* PORT NUM: 147 */
    0x00, 
  },
  { /* PORT NUM: 148 */
    0x00, 
  },
  { /* PORT NUM: 149 */
    0x00, 
  },
  { /* PORT NUM: 150 */
    0x00, 
  },
  { /* PORT NUM: 151 */
    0x00, 
  },
  { /* PORT NUM: 152 */
    0x00, 
  },
  { /* PORT NUM: 153 */
    0x00, 
  },
  { /* PORT NUM: 154 */
    0x00, 
  },
  { /* PORT NUM: 155 */
    0x00, 
  },
  { /* PORT NUM: 156 */
    0x00, 
  },
  { /* PORT NUM: 157 */
    0x00, 
  },
  { /* PORT NUM: 158 */
    0x00, 
  },
  { /* PORT NUM: 159 */
    0x00, 
  },
  { /* PORT NUM: 160 */
    0x00, 
  },
  { /* PORT NUM: 161 */
    0x00, 
  },
  { /* PORT NUM: 162 */
    0x00, 
  },
  { /* PORT NUM: 163 */
    0x00, 
  },
  { /* PORT NUM: 164 */
    0x00, 
  },
  { /* PORT NUM: 165 */
    0x00, 
  },
  { /* PORT NUM: 166 */
    0x00, 
  },
  { /* PORT NUM: 167 */
    0x00, 
  },
  { /* PORT NUM: 168 */
    0x00, 
  },
  { /* PORT NUM: 169 */
    0x00, 
  },
  { /* PORT NUM: 170 */
    0x00, 
  },
  { /* PORT NUM: 171 */
    0x00, 
  },
  { /* PORT NUM: 172 */
    0x00, 
  },
  { /* PORT NUM: 173 */
    0x00, 
  },
  { /* PORT NUM: 174 */
    0x00, 
  },
  { /* PORT NUM: 175 */
    0x00, 
  },
  { /* PORT NUM: 176 */
    0x00, 
  },
  { /* PORT NUM: 177 */
    0x00, 
  },
  { /* PORT NUM: 178 */
    0x00, 
  },
  { /* PORT NUM: 179 */
    0x00, 
  },
  { /* PORT NUM: 180 */
    0x00, 
  },
  { /* PORT NUM: 181 */
    0x00, 
  },
  { /* PORT NUM: 182 */
    0x00, 
  },
  { /* PORT NUM: 183 */
    0x00, 
  },
  { /* PORT NUM: 184 */
    0x00, 
  },
  { /* PORT NUM: 185 */
    0x00, 
  },
  { /* PORT NUM: 186 */
    0x00, 
  },
};


#define RFDEVICE_PA_sm5643_51_PA_TRIGGER_NUM_REGS 1
static uint8 rfdevice_pa_sm5643_51_pa_trigger_regs[RFDEVICE_PA_sm5643_51_PA_TRIGGER_NUM_REGS] =  {0x1C, };
static int16 rfdevice_pa_sm5643_51_pa_trigger_data[RFDEVICE_PA_sm5643_51_NUM_PORTS][RFDEVICE_PA_sm5643_51_PA_TRIGGER_NUM_REGS] =
{
  { /* PORT NUM: 0 */
    0x07, 
  },
  { /* PORT NUM: 1 */
    0x07, 
  },
  { /* PORT NUM: 2 */
    0x07, 
  },
  { /* PORT NUM: 3 */
    0x07, 
  },
  { /* PORT NUM: 4 */
    0x07, 
  },
  { /* PORT NUM: 5 */
    0x07, 
  },
  { /* PORT NUM: 6 */
    0x07, 
  },
  { /* PORT NUM: 7 */
    0x07, 
  },
  { /* PORT NUM: 8 */
    0x07, 
  },
  { /* PORT NUM: 9 */
    0x07, 
  },
  { /* PORT NUM: 10 */
    0x07, 
  },
  { /* PORT NUM: 11 */
    0x07, 
  },
  { /* PORT NUM: 12 */
    0x07, 
  },
  { /* PORT NUM: 13 */
    0x07, 
  },
  { /* PORT NUM: 14 */
    0x07, 
  },
  { /* PORT NUM: 15 */
    0x07, 
  },
  { /* PORT NUM: 16 */
    0x07, 
  },
  { /* PORT NUM: 17 */
    0x07, 
  },
  { /* PORT NUM: 18 */
    0x07, 
  },
  { /* PORT NUM: 19 */
    0x07, 
  },
  { /* PORT NUM: 20 */
    0x07, 
  },
  { /* PORT NUM: 21 */
    0x07, 
  },
  { /* PORT NUM: 22 */
    0x07, 
  },
  { /* PORT NUM: 23 */
    0x07, 
  },
  { /* PORT NUM: 24 */
    0x07, 
  },
  { /* PORT NUM: 25 */
    0x07, 
  },
  { /* PORT NUM: 26 */
    0x07, 
  },
  { /* PORT NUM: 27 */
    0x07, 
  },
  { /* PORT NUM: 28 */
    0x07, 
  },
  { /* PORT NUM: 29 */
    0x07, 
  },
  { /* PORT NUM: 30 */
    0x07, 
  },
  { /* PORT NUM: 31 */
    0x07, 
  },
  { /* PORT NUM: 32 */
    0x07, 
  },
  { /* PORT NUM: 33 */
    0x07, 
  },
  { /* PORT NUM: 34 */
    0x07, 
  },
  { /* PORT NUM: 35 */
    0x07, 
  },
  { /* PORT NUM: 36 */
    0x07, 
  },
  { /* PORT NUM: 37 */
    0x07, 
  },
  { /* PORT NUM: 38 */
    0x07, 
  },
  { /* PORT NUM: 39 */
    0x07, 
  },
  { /* PORT NUM: 40 */
    0x07, 
  },
  { /* PORT NUM: 41 */
    0x07, 
  },
  { /* PORT NUM: 42 */
    0x07, 
  },
  { /* PORT NUM: 43 */
    0x07, 
  },
  { /* PORT NUM: 44 */
    0x07, 
  },
  { /* PORT NUM: 45 */
    0x07, 
  },
  { /* PORT NUM: 46 */
    0x07, 
  },
  { /* PORT NUM: 47 */
    0x07, 
  },
  { /* PORT NUM: 48 */
    0x07, 
  },
  { /* PORT NUM: 49 */
    0x07, 
  },
  { /* PORT NUM: 50 */
    0x07, 
  },
  { /* PORT NUM: 51 */
    0x07, 
  },
  { /* PORT NUM: 52 */
    0x07, 
  },
  { /* PORT NUM: 53 */
    0x07, 
  },
  { /* PORT NUM: 54 */
    0x07, 
  },
  { /* PORT NUM: 55 */
    0x07, 
  },
  { /* PORT NUM: 56 */
    0x07, 
  },
  { /* PORT NUM: 57 */
    0x07, 
  },
  { /* PORT NUM: 58 */
    0x07, 
  },
  { /* PORT NUM: 59 */
    0x07, 
  },
  { /* PORT NUM: 60 */
    0x07, 
  },
  { /* PORT NUM: 61 */
    0x07, 
  },
  { /* PORT NUM: 62 */
    0x07, 
  },
  { /* PORT NUM: 63 */
    0x07, 
  },
  { /* PORT NUM: 64 */
    0x07, 
  },
  { /* PORT NUM: 65 */
    0x07, 
  },
  { /* PORT NUM: 66 */
    0x07, 
  },
  { /* PORT NUM: 67 */
    0x07, 
  },
  { /* PORT NUM: 68 */
    0x07, 
  },
  { /* PORT NUM: 69 */
    0x07, 
  },
  { /* PORT NUM: 70 */
    0x07, 
  },
  { /* PORT NUM: 71 */
    0x07, 
  },
  { /* PORT NUM: 72 */
    0x07, 
  },
  { /* PORT NUM: 73 */
    0x07, 
  },
  { /* PORT NUM: 74 */
    0x07, 
  },
  { /* PORT NUM: 75 */
    0x07, 
  },
  { /* PORT NUM: 76 */
    0x07, 
  },
  { /* PORT NUM: 77 */
    0x07, 
  },
  { /* PORT NUM: 78 */
    0x07, 
  },
  { /* PORT NUM: 79 */
    0x07, 
  },
  { /* PORT NUM: 80 */
    0x07, 
  },
  { /* PORT NUM: 81 */
    0x07, 
  },
  { /* PORT NUM: 82 */
    0x07, 
  },
  { /* PORT NUM: 83 */
    0x07, 
  },
  { /* PORT NUM: 84 */
    0x07, 
  },
  { /* PORT NUM: 85 */
    0x07, 
  },
  { /* PORT NUM: 86 */
    0x07, 
  },
  { /* PORT NUM: 87 */
    0x07, 
  },
  { /* PORT NUM: 88 */
    0x07, 
  },
  { /* PORT NUM: 89 */
    0x07, 
  },
  { /* PORT NUM: 90 */
    0x07, 
  },
  { /* PORT NUM: 91 */
    0x07, 
  },
  { /* PORT NUM: 92 */
    0x07, 
  },
  { /* PORT NUM: 93 */
    0x07, 
  },
  { /* PORT NUM: 94 */
    0x07, 
  },
  { /* PORT NUM: 95 */
    0x07, 
  },
  { /* PORT NUM: 96 */
    0x07, 
  },
  { /* PORT NUM: 97 */
    0x07, 
  },
  { /* PORT NUM: 98 */
    0x07, 
  },
  { /* PORT NUM: 99 */
    0x07, 
  },
  { /* PORT NUM: 100 */
    0x07, 
  },
  { /* PORT NUM: 101 */
    0x07, 
  },
  { /* PORT NUM: 102 */
    0x07, 
  },
  { /* PORT NUM: 103 */
    0x07, 
  },
  { /* PORT NUM: 104 */
    0x07, 
  },
  { /* PORT NUM: 105 */
    0x07, 
  },
  { /* PORT NUM: 106 */
    0x07, 
  },
  { /* PORT NUM: 107 */
    0x07, 
  },
  { /* PORT NUM: 108 */
    0x07, 
  },
  { /* PORT NUM: 109 */
    0x07, 
  },
  { /* PORT NUM: 110 */
    0x07, 
  },
  { /* PORT NUM: 111 */
    0x07, 
  },
  { /* PORT NUM: 112 */
    0x07, 
  },
  { /* PORT NUM: 113 */
    0x07, 
  },
  { /* PORT NUM: 114 */
    0x07, 
  },
  { /* PORT NUM: 115 */
    0x07, 
  },
  { /* PORT NUM: 116 */
    0x07, 
  },
  { /* PORT NUM: 117 */
    0x07, 
  },
  { /* PORT NUM: 118 */
    0x07, 
  },
  { /* PORT NUM: 119 */
    0x07, 
  },
  { /* PORT NUM: 120 */
    0x07, 
  },
  { /* PORT NUM: 121 */
    0x07, 
  },
  { /* PORT NUM: 122 */
    0x07, 
  },
  { /* PORT NUM: 123 */
    0x07, 
  },
  { /* PORT NUM: 124 */
    0x07, 
  },
  { /* PORT NUM: 125 */
    0x07, 
  },
  { /* PORT NUM: 126 */
    0x07, 
  },
  { /* PORT NUM: 127 */
    0x07, 
  },
  { /* PORT NUM: 128 */
    0x07, 
  },
  { /* PORT NUM: 129 */
    0x07, 
  },
  { /* PORT NUM: 130 */
    0x07, 
  },
  { /* PORT NUM: 131 */
    0x07, 
  },
  { /* PORT NUM: 132 */
    0x07, 
  },
  { /* PORT NUM: 133 */
    0x07, 
  },
  { /* PORT NUM: 134 */
    0x07, 
  },
  { /* PORT NUM: 135 */
    0x07, 
  },
  { /* PORT NUM: 136 */
    0x07, 
  },
  { /* PORT NUM: 137 */
    0x07, 
  },
  { /* PORT NUM: 138 */
    0x07, 
  },
  { /* PORT NUM: 139 */
    0x07, 
  },
  { /* PORT NUM: 140 */
    0x07, 
  },
  { /* PORT NUM: 141 */
    0x07, 
  },
  { /* PORT NUM: 142 */
    0x07, 
  },
  { /* PORT NUM: 143 */
    0x07, 
  },
  { /* PORT NUM: 144 */
    0x07, 
  },
  { /* PORT NUM: 145 */
    0x07, 
  },
  { /* PORT NUM: 146 */
    0x07, 
  },
  { /* PORT NUM: 147 */
    0x07, 
  },
  { /* PORT NUM: 148 */
    0x07, 
  },
  { /* PORT NUM: 149 */
    0x07, 
  },
  { /* PORT NUM: 150 */
    0x07, 
  },
  { /* PORT NUM: 151 */
    0x07, 
  },
  { /* PORT NUM: 152 */
    0x07, 
  },
  { /* PORT NUM: 153 */
    0x07, 
  },
  { /* PORT NUM: 154 */
    0x07, 
  },
  { /* PORT NUM: 155 */
    0x07, 
  },
  { /* PORT NUM: 156 */
    0x07, 
  },
  { /* PORT NUM: 157 */
    0x07, 
  },
  { /* PORT NUM: 158 */
    0x07, 
  },
  { /* PORT NUM: 159 */
    0x07, 
  },
  { /* PORT NUM: 160 */
    0x07, 
  },
  { /* PORT NUM: 161 */
    0x07, 
  },
  { /* PORT NUM: 162 */
    0x07, 
  },
  { /* PORT NUM: 163 */
    0x07, 
  },
  { /* PORT NUM: 164 */
    0x07, 
  },
  { /* PORT NUM: 165 */
    0x07, 
  },
  { /* PORT NUM: 166 */
    0x07, 
  },
  { /* PORT NUM: 167 */
    0x07, 
  },
  { /* PORT NUM: 168 */
    0x07, 
  },
  { /* PORT NUM: 169 */
    0x07, 
  },
  { /* PORT NUM: 170 */
    0x07, 
  },
  { /* PORT NUM: 171 */
    0x07, 
  },
  { /* PORT NUM: 172 */
    0x07, 
  },
  { /* PORT NUM: 173 */
    0x07, 
  },
  { /* PORT NUM: 174 */
    0x07, 
  },
  { /* PORT NUM: 175 */
    0x07, 
  },
  { /* PORT NUM: 176 */
    0x07, 
  },
  { /* PORT NUM: 177 */
    0x07, 
  },
  { /* PORT NUM: 178 */
    0x07, 
  },
  { /* PORT NUM: 179 */
    0x07, 
  },
  { /* PORT NUM: 180 */
    0x07, 
  },
  { /* PORT NUM: 181 */
    0x07, 
  },
  { /* PORT NUM: 182 */
    0x07, 
  },
  { /* PORT NUM: 183 */
    0x07, 
  },
  { /* PORT NUM: 184 */
    0x07, 
  },
  { /* PORT NUM: 185 */
    0x07, 
  },
  { /* PORT NUM: 186 */
    0x07, 
  },
};

/* singleton instance ptr */
rfdevice_pa_data *rfdevice_pa_sm5643_51_data_ag::rfdevice_pa_sm5643_51_data_ptr = NULL;

rfdevice_pa_data * rfdevice_pa_sm5643_51_data_ag::get_instance()
{
  if (rfdevice_pa_sm5643_51_data_ptr == NULL)
  {
    rfdevice_pa_sm5643_51_data_ptr = (rfdevice_pa_data *)new rfdevice_pa_sm5643_51_data_ag();
  }
  return( (rfdevice_pa_data *)rfdevice_pa_sm5643_51_data_ptr);
}

//constructor
rfdevice_pa_sm5643_51_data_ag::rfdevice_pa_sm5643_51_data_ag()
  :rfdevice_pa_data()
{
}

//destructor
rfdevice_pa_sm5643_51_data_ag::~rfdevice_pa_sm5643_51_data_ag()
{
  rfdevice_pa_sm5643_51_data_ptr = NULL;
}

boolean rfdevice_pa_sm5643_51_data_ag::settings_data_get( rfdevice_pa_cfg_params_type *cfg, 
                                                          rfdevice_pa_reg_settings_type *settings)
{
  boolean ret_val = FALSE;

  if (NULL == settings || NULL == cfg)
  {
    return FALSE;
  }

  if (cfg->port >= RFDEVICE_PA_sm5643_51_NUM_PORTS )
  {
    settings->addr = NULL;
    settings->data = NULL;
    settings->num_regs = 0;
    return FALSE;
  }

  if ( (cfg->req == RFDEVICE_PA_SET_BIAS_DATA) && (cfg->pa_range < 4) )
  {
    settings->addr = &(rfdevice_pa_sm5643_51_pa_set_bias_regs[0]);
    settings->data = &(rfdevice_pa_sm5643_51_pa_set_bias_data[cfg->port][cfg->pa_range][0]);
    settings->num_regs = RFDEVICE_PA_sm5643_51_PA_SET_BIAS_NUM_REGS;
    settings->bias_reg_idx = 0;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_SET_RANGE_DATA) && (cfg->pa_range < 4) )
  {
    settings->addr = &(rfdevice_pa_sm5643_51_pa_set_range_regs[0]);
    settings->data = &(rfdevice_pa_sm5643_51_pa_set_range_data[cfg->port][cfg->pa_range][0]);
    settings->num_regs = RFDEVICE_PA_sm5643_51_PA_SET_RANGE_NUM_REGS;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_ON_DATA) )
  {
    settings->addr = &(rfdevice_pa_sm5643_51_pa_on_regs[0]);
    settings->data = &(rfdevice_pa_sm5643_51_pa_on_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_PA_sm5643_51_PA_ON_NUM_REGS;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_OFF_DATA) )
  {
    settings->addr = &(rfdevice_pa_sm5643_51_pa_off_regs[0]);
    settings->data = &(rfdevice_pa_sm5643_51_pa_off_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_PA_sm5643_51_PA_OFF_NUM_REGS;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_TRIGGER_DATA) )
  {
    settings->addr = &(rfdevice_pa_sm5643_51_pa_trigger_regs[0]);
    settings->data = &(rfdevice_pa_sm5643_51_pa_trigger_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_PA_sm5643_51_PA_TRIGGER_NUM_REGS;
    ret_val = TRUE;
  }

  return ret_val;
}

boolean rfdevice_pa_sm5643_51_data_ag::sequence_data_get( rfdevice_pa_cfg_params_type *cfg, 
                                                          rfdevice_pa_cmd_seq_type *cmd_seq)
{
  boolean ret_val = FALSE;

  if (NULL == cmd_seq || NULL == cfg)
  {
    return FALSE;
  }

  else if (RFDEVICE_PA_RESET_DATA == cfg->req)
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
    ret_val = TRUE;
  }
  else if (RFDEVICE_PA_THERM_ON_DATA == cfg->req)
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
    ret_val = TRUE;
  }
  else if (RFDEVICE_PA_THERM_READ_DATA == cfg->req)
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
    ret_val = TRUE;
  }
  else if (RFDEVICE_PA_THERM_OFF_DATA == cfg->req)
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
    ret_val = TRUE;
  }
  else
  {
    cmd_seq->cmds = NULL;
    cmd_seq->num_cmds = 0;
  }

  return ret_val;
}

boolean rfdevice_pa_sm5643_51_data_ag::device_info_get( rfdevice_pa_info_type *pa_info )
{
  boolean ret_val = FALSE;

  if ( NULL == pa_info )
  {
    return FALSE;
  }
  else
  {
    pa_info->mfg_id = 0x02e9;
    pa_info->prd_id = 0x89;
    pa_info->prd_rev = 0;
    pa_info->num_ports = RFDEVICE_PA_sm5643_51_NUM_PORTS;
    pa_info->num_pa_ranges = 4;
	pa_info->rffev20_usid_p3 = FALSE;	
    ret_val = TRUE;
  }
  return ret_val;
}

boolean rfdevice_pa_sm5643_51_data_ag::get_temperature_lut(int16 **lut_addr, uint8 *lut_size)
{
   boolean ret_val = FALSE;   

   if (lut_addr != NULL)  
   {
      *lut_addr = NULL; 
      *lut_size = -1;
      ret_val = TRUE;
   }

   else 
   {
      ret_val = FALSE;
   }

   return ret_val;
}

uint32 rfdevice_pa_sm5643_51_data_ag::get_max_bw_supported()
{
   return 20000;
}
