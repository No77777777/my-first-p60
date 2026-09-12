
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

$Header: //Commercial/MPSS.DI.2.0.1.c1/Main/modem_proc/rfdevice_pa/src/rfdevice_pa_s5643_55_data_ag.cpp#2 $ 


=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/
#include "comdef.h"

#include "rfdevice_pa_s5643_55_data_ag.h"


#define RFDEVICE_PA_S5643_55_NUM_PORTS 107
//high-high-medium-low
#define RFDEVICE_PA_S5643_55_PA_SET_BIAS_NUM_REGS 2
static uint8 rfdevice_pa_s5643_55_pa_set_bias_regs[RFDEVICE_PA_S5643_55_PA_SET_BIAS_NUM_REGS] =  {0x01,0x03 };
static int16 rfdevice_pa_s5643_55_pa_set_bias_data[RFDEVICE_PA_S5643_55_NUM_PORTS][4][RFDEVICE_PA_S5643_55_PA_SET_BIAS_NUM_REGS] =
{
  { /* PORT NUM: 0 LTE_B1_MB1*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 1 LTE_B1_MB2*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 2 LTE_B1_MB3*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 3 LTE_B1_MB4*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 4 LTE_B1_MB5*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 5 LTE_B3_MB1 & LTE_B4_MB1 & LTE_B66_MB1*/
    { 0x06, 0xC9 },  /* PA Range: 0 */
    { 0x06, 0xC9 },  /* PA Range: 1 */
    { 0x06, 0x68 },  /* PA Range: 2 */
    { 0x07, 0x68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 6 LTE_B3_MB2 & LTE_B4_MB2 & LTE_B66_MB2*/
    { 0x06, 0xC9 },  /* PA Range: 0 */
    { 0x06, 0xC9 },  /* PA Range: 1 */
    { 0x06, 0x68 },  /* PA Range: 2 */
    { 0x07, 0x68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 7 LTE_B3_MB3 & LTE_B4_MB3 & LTE_B66_MB3*/
    { 0x06, 0xC9 },  /* PA Range: 0 */
    { 0x06, 0xC9 },  /* PA Range: 1 */
    { 0x06, 0x68 },  /* PA Range: 2 */
    { 0x07, 0x68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 8 LTE_B3_MB4 & LTE_B4_MB4 & LTE_B66_MB4*/
    { 0x06, 0xC9 },  /* PA Range: 0 */
    { 0x06, 0xC9 },  /* PA Range: 1 */
    { 0x06, 0x68 },  /* PA Range: 2 */
    { 0x07, 0x68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 9 LTE_B3_MB5 & LTE_B4_MB5 & LTE_B66_MB5*/
    { 0x06, 0xC9 },  /* PA Range: 0 */
    { 0x06, 0xC9 },  /* PA Range: 1 */
    { 0x06, 0x68 },  /* PA Range: 2 */
    { 0x07, 0x68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 10 LTE_B5_LB1 & LTE_B20_LB1 & LTE_B26_LB1*/
    { 0x82, 0xD4 },  /* PA Range: 0 */
    { 0x82, 0xD4 },  /* PA Range: 1 */
    { 0x82, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 11 LTE_B5_LB2 & LTE_B20_LB2 & LTE_B26_LB2*/
    { 0x82, 0xD4 },  /* PA Range: 0 */
    { 0x82, 0xD4 },  /* PA Range: 1 */
    { 0x82, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 12 LTE_B5_LB3 & LTE_B20_LB3 & LTE_B26_LB3*/
    { 0x82, 0xD4 },  /* PA Range: 0 */
    { 0x82, 0xD4 },  /* PA Range: 1 */
    { 0x82, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 13 LTE_B5_LB4 & LTE_B20_LB4 & LTE_B26_LB4*/
    { 0x82, 0xD4 },  /* PA Range: 0 */
    { 0x82, 0xD4 },  /* PA Range: 1 */
    { 0x82, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 14 LTE_B5_LB5 & LTE_B20_LB5 & LTE_B26_LB5*/
    { 0x82, 0xD4 },  /* PA Range: 0 */
    { 0x82, 0xD4 },  /* PA Range: 1 */
    { 0x82, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 15 LTE_B7_HB1 & LTE_B38_HB1 & LTE_B41_HB1*/
    { 0xFD, 0x83 },  /* PA Range: 0 */
    { 0xFD, 0x83 },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0x5B, 0xFB },  /* PA Range: 3 */
  },
  { /* PORT NUM: 16 LTE_B7_HB2 & LTE_B38_HB2 & LTE_B41_HB2*/
    { 0xFD, 0x83 },  /* PA Range: 0 */
    { 0xFD, 0x83 },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0x5B, 0xFB },  /* PA Range: 3 */
  },
  { /* PORT NUM: 17 LTE_B7_HB3 & LTE_B38_HB3 & LTE_B41_HB3*/
    { 0xFD, 0x83 },  /* PA Range: 0 */
    { 0xFD, 0x83 },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0x5B, 0xFB },  /* PA Range: 3 */
  },
  { /* PORT NUM: 18 LTE_B7_HB4 & LTE_B38_HB4 & LTE_B41_HB4*/
    { 0xFD, 0x83 },  /* PA Range: 0 */
    { 0xFD, 0x83 },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0x5B, 0xFB },  /* PA Range: 3 */
  },
  { /* PORT NUM: 19 LTE_B8_LB1*/
    { 0x31, 0xD4 },  /* PA Range: 0 */
    { 0x31, 0xD4 },  /* PA Range: 1 */
    { 0x34, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 20 LTE_B8_LB2*/
    { 0x31, 0xD4 },  /* PA Range: 0 */
    { 0x31, 0xD4 },  /* PA Range: 1 */
    { 0x34, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 21 LTE_B8_LB3*/
    { 0x31, 0xD4 },  /* PA Range: 0 */
    { 0x31, 0xD4 },  /* PA Range: 1 */
    { 0x34, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 22 LTE_B8_LB4*/
    { 0x31, 0xD4 },  /* PA Range: 0 */
    { 0x31, 0xD4 },  /* PA Range: 1 */
    { 0x34, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 23 LTE_B8_LB5*/
    { 0x31, 0xD4 },  /* PA Range: 0 */
    { 0x31, 0xD4 },  /* PA Range: 1 */
    { 0x34, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 24 LTE_B12_LB1 & LTE_B13_LB1 & LTE_B17_LB1 & LTE_B28_LB1*/
    { 0xD1, 0xF5 },  /* PA Range: 0 */
    { 0xD1, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 25 LTE_B12_LB2 & LTE_B13_LB2 & LTE_B17_LB2 & LTE_B28_LB2*/
    { 0xD1, 0xF5 },  /* PA Range: 0 */
    { 0xD1, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 26 LTE_B12_LB3 & LTE_B13_LB3 & LTE_B17_LB3 & LTE_B28_LB3*/
    { 0xD1, 0xF5 },  /* PA Range: 0 */
    { 0xD1, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 27 LTE_B12_LB4 & LTE_B13_LB4 & LTE_B17_LB4 & LTE_B28_LB4*/
    { 0xD1, 0xF5 },  /* PA Range: 0 */
    { 0xD1, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 28 LTE_B12_LB5 & LTE_B13_LB5 & LTE_B17_LB5 & LTE_B28_LB5*/
    { 0xD1, 0xF5 },  /* PA Range: 0 */
    { 0xD1, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 29 LTE_B34_MB1*/
    { 0x05, 0xF5 },  /* PA Range: 0 */
    { 0x05, 0xF5 },  /* PA Range: 1 */
    { 0x05, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 30 LTE_B34_MB2*/
    { 0x05, 0xF5 },  /* PA Range: 0 */
    { 0x05, 0xF5 },  /* PA Range: 1 */
    { 0x05, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 31 LTE_B34_MB3*/
    { 0x05, 0xF5 },  /* PA Range: 0 */
    { 0x05, 0xF5 },  /* PA Range: 1 */
    { 0x05, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 32 LTE_B34_MB4*/
    { 0x05, 0xF5 },  /* PA Range: 0 */
    { 0x05, 0xF5 },  /* PA Range: 1 */
    { 0x05, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 33 LTE_B34_MB5*/
    { 0x05, 0xF5 },  /* PA Range: 0 */
    { 0x05, 0xF5 },  /* PA Range: 1 */
    { 0x05, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 34 LTE_B39_MB1*/
    { 0x06, 0x26 },  /* PA Range: 0 */
    { 0x06, 0x26 },  /* PA Range: 1 */
    { 0x06, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 35 LTE_B39_MB2*/
    { 0x06, 0x26 },  /* PA Range: 0 */
    { 0x06, 0x26 },  /* PA Range: 1 */
    { 0x06, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 36 LTE_B39_MB3*/
    { 0x06, 0x26 },  /* PA Range: 0 */
    { 0x06, 0x26 },  /* PA Range: 1 */
    { 0x06, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 37 LTE_B39_MB4*/
    { 0x06, 0x26 },  /* PA Range: 0 */
    { 0x06, 0x26 },  /* PA Range: 1 */
    { 0x06, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 38 LTE_B39_MB5*/
    { 0x06, 0x26 },  /* PA Range: 0 */
    { 0x06, 0x26 },  /* PA Range: 1 */
    { 0x06, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 39 LTE_B30_HB1 & LTE_B40_HB1*/
    { 0xFA, 0x8C },  /* PA Range: 0 */
    { 0xFA, 0x8C },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0xCA, 0x50 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 40 LTE_B30_HB2 & LTE_B40_HB2*/
    { 0xFA, 0x8C },  /* PA Range: 0 */
    { 0xFA, 0x8C },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0xCA, 0x50 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 41 LTE_B30_HB3 & LTE_B40_HB3*/
    { 0xFA, 0x8C },  /* PA Range: 0 */
    { 0xFA, 0x8C },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0xCA, 0x50 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 42 LTE_B30_HB4 & LTE_B40_HB4*/
    { 0xFA, 0x8C },  /* PA Range: 0 */
    { 0xFA, 0x8C },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0xCA, 0x50 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 43 LTE_B41_HB1_PC2(power class 2)*/
    { 0xFD, 0x83 },  /* PA Range: 0 */
    { 0xFD, 0x83 },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0x5B, 0xFB },  /* PA Range: 3 */
  },
  { /* PORT NUM: 44 LTE_B41_HB2_PC2*/
    { 0xFD, 0x83 },  /* PA Range: 0 */
    { 0xFD, 0x83 },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0x5B, 0xFB },  /* PA Range: 3 */
  },
  { /* PORT NUM: 45 LTE_B41_HB3_PC2*/
    { 0xFD, 0x83 },  /* PA Range: 0 */
    { 0xFD, 0x83 },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0x5B, 0xFB },  /* PA Range: 3 */
  },
  { /* PORT NUM: 46 LTE_B41_HB4_PC2*/
    { 0xFD, 0x83 },  /* PA Range: 0 */
    { 0xFD, 0x83 },  /* PA Range: 1 */
    { 0xCA, 0x79 },  /* PA Range: 2 */
    { 0x5B, 0xFB },  /* PA Range: 3 */
  },
  { /* PORT NUM: 47 LTE_B71_LB1*/
    { 0xD2, 0xF5 },  /* PA Range: 0 */
    { 0xD2, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 48 LTE_B71_LB2*/
    { 0xD2, 0xF5 },  /* PA Range: 0 */
    { 0xD2, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 49 LTE_B71_LB3*/
    { 0xD2, 0xF5 },  /* PA Range: 0 */
    { 0xD2, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 50 LTE_B71_LB4*/
    { 0xD2, 0xF5 },  /* PA Range: 0 */
    { 0xD2, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 51 LTE_B71_LB5*/
    { 0xD2, 0xF5 },  /* PA Range: 0 */
    { 0xD2, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 52 WCDMA_B1_MB1*/
    { 0x39, 0xE7 },  /* PA Range: 0 */
    { 0x39, 0xE7 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 53 WCDMA_B1_MB2*/
    { 0x39, 0xE7 },  /* PA Range: 0 */
    { 0x39, 0xE7 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 54 WCDMA_B1_MB3*/
    { 0x39, 0xE7 },  /* PA Range: 0 */
    { 0x39, 0xE7 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 55 WCDMA_B1_MB4*/
    { 0x39, 0xE7 },  /* PA Range: 0 */
    { 0x39, 0xE7 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 56 WCDMA_B1_MB5*/
    { 0x39, 0xE7 },  /* PA Range: 0 */
    { 0x39, 0xE7 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 57 WCDMA_B2_MB1 & BC1_MB1*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x06, 0x66 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 58 WCDMA_B2_MB2 & BC1_MB2*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x06, 0x66 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 59 WCDMA_B2_MB3 & BC1_MB3*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x06, 0x66 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 60 WCDMA_B2_MB4 & BC1_MB4*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x06, 0x66 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 61 WCDMA_B2_MB5 & BC1_MB5*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x06, 0x66 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 62 WCDMA_B3_MB1 & WCDMA_B4_MB1 & BC4_MB1 &BC15_MB1*/
    { 0x06, 0xC9 },  /* PA Range: 0 */
    { 0x06, 0xC9 },  /* PA Range: 1 */
    { 0x06, 0x68 },  /* PA Range: 2 */
    { 0x07, 0x68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 63 WCDMA_B3_MB2 & WCDMA_B4_MB2 & BC4_MB2 &BC15_MB2*/
    { 0x06, 0xC9 },  /* PA Range: 0 */
    { 0x06, 0xC9 },  /* PA Range: 1 */
    { 0x06, 0x68 },  /* PA Range: 2 */
    { 0x07, 0x68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 64 WCDMA_B3_MB3 & WCDMA_B4_MB3 & BC4_MB3 &BC15_MB3*/
    { 0x06, 0xC9 },  /* PA Range: 0 */
    { 0x06, 0xC9 },  /* PA Range: 1 */
    { 0x06, 0x68 },  /* PA Range: 2 */
    { 0x07, 0x68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 65 WCDMA_B3_MB4 & WCDMA_B4_MB4 & BC4_MB4 &BC15_MB4*/
    { 0x06, 0xC9 },  /* PA Range: 0 */
    { 0x06, 0xC9 },  /* PA Range: 1 */
    { 0x06, 0x68 },  /* PA Range: 2 */
    { 0x07, 0x68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 66 WCDMA_B3_MB5 & WCDMA_B4_MB5 & BC4_MB5 &BC15_MB5*/
    { 0x06, 0xC9 },  /* PA Range: 0 */
    { 0x06, 0xC9 },  /* PA Range: 1 */
    { 0x06, 0x68 },  /* PA Range: 2 */
    { 0x07, 0x68 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 67 WCDMA_B5_LB1 & BC0_LB1 & BC10_LB1*/
    { 0x82, 0xD4 },  /* PA Range: 0 */
    { 0x82, 0xD4 },  /* PA Range: 1 */
    { 0x82, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 68 WCDMA_B5_LB2 & BC0_LB2 & BC10_LB2*/
    { 0x82, 0xD4 },  /* PA Range: 0 */
    { 0x82, 0xD4 },  /* PA Range: 1 */
    { 0x82, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 69 WCDMA_B5_LB3 & BC0_LB3 & BC10_LB3*/
    { 0x82, 0xD4 },  /* PA Range: 0 */
    { 0x82, 0xD4 },  /* PA Range: 1 */
    { 0x82, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 70 WCDMA_B5_LB4 & BC0_LB4 & BC10_LB4*/
    { 0x82, 0xD4 },  /* PA Range: 0 */
    { 0x82, 0xD4 },  /* PA Range: 1 */
    { 0x82, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 71 WCDMA_B5_LB5 & BC0_LB5 & BC10_LB5*/
    { 0x82, 0xD4 },  /* PA Range: 0 */
    { 0x82, 0xD4 },  /* PA Range: 1 */
    { 0x82, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 72 WCDMA_B8_LB1*/
    { 0x44, 0xC2 },  /* PA Range: 0 */
    { 0x44, 0xC2 },  /* PA Range: 1 */
    { 0x34, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 73 WCDMA_B8_LB2*/
    { 0x44, 0xC2 },  /* PA Range: 0 */
    { 0x44, 0xC2 },  /* PA Range: 1 */
    { 0x34, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 74 WCDMA_B8_LB3*/
    { 0x44, 0xC2 },  /* PA Range: 0 */
    { 0x44, 0xC2 },  /* PA Range: 1 */
    { 0x34, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 75 WCDMA_B8_LB4*/
    { 0x44, 0xC2 },  /* PA Range: 0 */
    { 0x44, 0xC2 },  /* PA Range: 1 */
    { 0x34, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 76 WCDMA_B8_LB5*/
    { 0x44, 0xC2 },  /* PA Range: 0 */
    { 0x44, 0xC2 },  /* PA Range: 1 */
    { 0x34, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 77 TD34_MB1*/
    { 0x05, 0xF5 },  /* PA Range: 0 */
    { 0x05, 0xF5 },  /* PA Range: 1 */
    { 0x05, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 78 TD34_MB2*/
    { 0x05, 0xF5 },  /* PA Range: 0 */
    { 0x05, 0xF5 },  /* PA Range: 1 */
    { 0x05, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 79 TD34_MB3*/
    { 0x05, 0xF5 },  /* PA Range: 0 */
    { 0x05, 0xF5 },  /* PA Range: 1 */
    { 0x05, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 80 TD34_MB4*/
    { 0x05, 0xF5 },  /* PA Range: 0 */
    { 0x05, 0xF5 },  /* PA Range: 1 */
    { 0x05, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 81 TD34_MB5*/
    { 0x05, 0xF5 },  /* PA Range: 0 */
    { 0x05, 0xF5 },  /* PA Range: 1 */
    { 0x05, 0x54 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 82 TD39_MB1*/
    { 0x06, 0x26 },  /* PA Range: 0 */
    { 0x06, 0x26 },  /* PA Range: 1 */
    { 0x06, 0x56 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 83 TD39_MB2*/
    { 0x06, 0x26 },  /* PA Range: 0 */
    { 0x06, 0x26 },  /* PA Range: 1 */
    { 0x06, 0x56 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 84 TD39_MB3*/
    { 0x06, 0x26 },  /* PA Range: 0 */
    { 0x06, 0x26 },  /* PA Range: 1 */
    { 0x06, 0x56 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 85 TD39_MB4*/
    { 0x06, 0x26 },  /* PA Range: 0 */
    { 0x06, 0x26 },  /* PA Range: 1 */
    { 0x06, 0x56 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 86 TD39_MB5*/
    { 0x06, 0x26 },  /* PA Range: 0 */
    { 0x06, 0x26 },  /* PA Range: 1 */
    { 0x06, 0x56 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 87 LTE_B2_MB1 & LTE_B25_MB1*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x06, 0x66 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 88 LTE_B2_MB2 & LTE_B25_MB2*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x06, 0x66 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 89 LTE_B2_MB3 & LTE_B25_MB34*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x06, 0x66 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 90 LTE_B2_MB4 & LTE_B25_MB4*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x06, 0x66 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 91 LTE_B2_MB5 & LTE_B25_MB5*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x06, 0x66 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 92 BC6_MB1*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 93 BC6_MB2*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 94 BC6_MB3*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 95 BC6_MB4*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 96 BC6_MB5*/
    { 0x05, 0xF8 },  /* PA Range: 0 */
    { 0x05, 0xF8 },  /* PA Range: 1 */
    { 0x05, 0x46 },  /* PA Range: 2 */
    { 0x07, 0x65 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 97 LTE_B12_LB1_LB2IN & LTE_B13_LB1_LB2IN & LTE_B17_LB1_LB2IN & LTE_B28_LB1_LB2IN*/
    { 0xD1, 0xF5 },  /* PA Range: 0 */
    { 0xD1, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 98 LTE_B12_LB2_LB2IN & LTE_B13_LB2_LB2IN & LTE_B17_LB2_LB2IN & LTE_B28_LB2_LB2IN*/
    { 0xD1, 0xF5 },  /* PA Range: 0 */
    { 0xD1, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 99 LTE_B12_LB3_LB2IN & LTE_B13_LB3_LB2IN & LTE_B17_LB3_LB2IN & LTE_B28_LB3_LB2IN*/
    { 0xD1, 0xF5 },  /* PA Range: 0 */
    { 0xD1, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 100 LTE_B12_LB4_LB2IN & LTE_B13_LB4_LB2IN & LTE_B17_LB4_LB2IN & LTE_B28_LB4_LB2IN*/
    { 0xD1, 0xF5 },  /* PA Range: 0 */
    { 0xD1, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 101 LTE_B12_LB5_LB2IN & LTE_B13_LB5_LB2IN & LTE_B17_LB5_LB2IN & LTE_B28_LB5_LB2IN*/
    { 0xD1, 0xF5 },  /* PA Range: 0 */
    { 0xD1, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 102 LTE_B71_LB1_LB2IN*/
    { 0xD2, 0xF5 },  /* PA Range: 0 */
    { 0xD2, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 103 LTE_B71_LB2_LB2IN*/
    { 0xD2, 0xF5 },  /* PA Range: 0 */
    { 0xD2, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 104 LTE_B71_LB3_LB2IN*/
    { 0xD2, 0xF5 },  /* PA Range: 0 */
    { 0xD2, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },            
  { /* PORT NUM: 105 LTE_B71_LB4_LB2IN*/
    { 0xD2, 0xF5 },  /* PA Range: 0 */
    { 0xD2, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 106 LTE_B71_LB5_LB2IN*/
    { 0xD2, 0xF5 },  /* PA Range: 0 */
    { 0xD2, 0xF5 },  /* PA Range: 1 */
    { 0xD1, 0x93 },  /* PA Range: 2 */
    { 0x83, 0x24 },  /* PA Range: 3 */
  },
};

//high-high-medium-low
#define RFDEVICE_PA_S5643_55_PA_SET_RANGE_NUM_REGS 1
static uint8 rfdevice_pa_s5643_55_pa_set_range_regs[RFDEVICE_PA_S5643_55_PA_SET_RANGE_NUM_REGS] =  {0x00,};
static int16 rfdevice_pa_s5643_55_pa_set_range_data[RFDEVICE_PA_S5643_55_NUM_PORTS][4][RFDEVICE_PA_S5643_55_PA_SET_RANGE_NUM_REGS] =
{	
  { /* PORT NUM: 0 LTE_B1_MB1*/
    { 0x34,  },  /* PA Range: 0 */
    { 0x34,  },  /* PA Range: 1 */
    { 0x34,  },  /* PA Range: 2 */
    { 0x34,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 1 LTE_B1_MB2*/
    { 0x3C,  },  /* PA Range: 0 */
    { 0x3C,  },  /* PA Range: 1 */
    { 0x3C,  },  /* PA Range: 2 */
    { 0x3C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 2 LTE_B1_MB3*/
    { 0x44,  },  /* PA Range: 0 */
    { 0x44,  },  /* PA Range: 1 */
    { 0x44,  },  /* PA Range: 2 */
    { 0x44,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 3 LTE_B1_MB4*/
    { 0x4C,  },  /* PA Range: 0 */
    { 0x4C,  },  /* PA Range: 1 */
    { 0x4C,  },  /* PA Range: 2 */
    { 0x4C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 4 LTE_B1_MB5*/
    { 0x54,  },  /* PA Range: 0 */
    { 0x54,  },  /* PA Range: 1 */
    { 0x54,  },  /* PA Range: 2 */
    { 0x54,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 5 LTE_B3_MB1 & LTE_B4_MB1 & LTE_B66_MB1*/
    { 0x34,  },  /* PA Range: 0 */
    { 0x34,  },  /* PA Range: 1 */
    { 0x34,  },  /* PA Range: 2 */
    { 0x34,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 6 LTE_B3_MB2 & LTE_B4_MB2 & LTE_B66_MB2*/
    { 0x3C,  },  /* PA Range: 0 */
    { 0x3C,  },  /* PA Range: 1 */
    { 0x3C,  },  /* PA Range: 2 */
    { 0x3C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 7 LTE_B3_MB3 & LTE_B4_MB3 & LTE_B66_MB3*/
    { 0x44,  },  /* PA Range: 0 */
    { 0x44,  },  /* PA Range: 1 */
    { 0x44,  },  /* PA Range: 2 */
    { 0x44,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 8 LTE_B3_MB4 & LTE_B4_MB4 & LTE_B66_MB4*/
    { 0x4C,  },  /* PA Range: 0 */
    { 0x4C,  },  /* PA Range: 1 */
    { 0x4C,  },  /* PA Range: 2 */
    { 0x4C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 9 LTE_B3_MB5 & LTE_B4_MB5 & LTE_B66_MB5*/
    { 0x54,  },  /* PA Range: 0 */
    { 0x54,  },  /* PA Range: 1 */
    { 0x54,  },  /* PA Range: 2 */
    { 0x54,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 10 LTE_B5_LB1 & LTE_B20_LB1 & LTE_B26_LB1*/
    { 0x0C, },  /* PA Range: 0 */
    { 0x0C, },  /* PA Range: 1 */
    { 0x0C, },  /* PA Range: 2 */
    { 0x0C, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 11 LTE_B5_LB2 & LTE_B20_LB2 & LTE_B26_LB2*/
    { 0x14, },	/* PA Range: 0 */
    { 0x14, },	/* PA Range: 1 */
    { 0x14, },	/* PA Range: 2 */
    { 0x14, },	/* PA Range: 3 */
  },
  { /* PORT NUM: 12 LTE_B5_LB3 & LTE_B20_LB3 & LTE_B26_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1C, },  /* PA Range: 1 */
    { 0x1C, },  /* PA Range: 2 */
    { 0x1C, },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 13 LTE_B5_LB4 & LTE_B20_LB4 & LTE_B26_LB4*/
    { 0x24,  },  /* PA Range: 0 */
    { 0x24,  },  /* PA Range: 1 */
    { 0x24,  },  /* PA Range: 2 */
    { 0x24,  },  /* PA Range: 3 */
  },   
  { /* PORT NUM: 14 LTE_B5_LB5 & LTE_B20_LB5 & LTE_B26_LB5*/
    { 0x2C, },  /* PA Range: 0 */
    { 0x2C, },  /* PA Range: 1 */
    { 0x2C, },  /* PA Range: 2 */
    { 0x2C, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 15 LTE_B7_HB1 & LTE_B38_HB1 & LTE_B41_HB1*/
    { 0x5C,  },  /* PA Range: 0 */
    { 0x5C,  },  /* PA Range: 1 */
    { 0x5C,  },  /* PA Range: 2 */
    { 0x5C,  },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 16 LTE_B7_HB2 & LTE_B38_HB2 & LTE_B41_HB2*/
    { 0x64,  },  /* PA Range: 0 */
    { 0x64,  },  /* PA Range: 1 */
    { 0x64,  },  /* PA Range: 2 */
    { 0x64,  },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 17 LTE_B7_HB3 & LTE_B38_HB3 & LTE_B41_HB3*/
    { 0x6C,  },  /* PA Range: 0 */
    { 0x6C,  },  /* PA Range: 1 */
    { 0x6C,  },  /* PA Range: 2 */
    { 0x6C,  },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 18 LTE_B7_HB4 & LTE_B38_HB4 & LTE_B41_HB4*/
    { 0x74,  },  /* PA Range: 0 */
    { 0x74,  },  /* PA Range: 1 */
    { 0x74,  },  /* PA Range: 2 */
    { 0x74,  },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 19 LTE_B8_LB1*/
    { 0x0C, },  /* PA Range: 0 */
    { 0x0C, },  /* PA Range: 1 */
    { 0x0C, },  /* PA Range: 2 */
    { 0x0C, },  /* PA Range: 3 */
  },  
  { /* PORT NUM: 20 LTE_B8_LB2*/
    { 0x14, },  /* PA Range: 0 */
    { 0x14, },  /* PA Range: 1 */
    { 0x14, },  /* PA Range: 2 */
    { 0x14, },  /* PA Range: 3 */
  }, 
  { /* PORT NUM: 21 LTE_B8_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1C, },  /* PA Range: 1 */
    { 0x1C, },  /* PA Range: 2 */
    { 0x1C, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 22 LTE_B8_LB4*/
    { 0x24, },  /* PA Range: 0 */
    { 0x24, },  /* PA Range: 1 */
    { 0x24, },  /* PA Range: 2 */
    { 0x24, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 23 LTE_B8_LB5*/
    { 0x2C,  },  /* PA Range: 0 */
    { 0x2C,  },  /* PA Range: 1 */
    { 0x2C,  },  /* PA Range: 2 */
    { 0x2C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 24 LTE_B12_LB1 & LTE_B13_LB1 & LTE_B17_LB1 & LTE_B28_LB1*/
    { 0x0E,  },  /* PA Range: 0 */
    { 0x0E,  },  /* PA Range: 1 */
    { 0x0E,  },  /* PA Range: 2 */
    { 0x0C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 25 LTE_B12_LB2 & LTE_B13_LB2 & LTE_B17_LB2 & LTE_B28_LB2*/
    { 0x16,  },  /* PA Range: 0 */
    { 0x16,  },  /* PA Range: 1 */
    { 0x16,  },  /* PA Range: 2 */
    { 0x14,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 26 LTE_B12_LB3 & LTE_B13_LB3 & LTE_B17_LB3 & LTE_B28_LB3*/
    { 0x1E,  },  /* PA Range: 0 */
    { 0x1E,  },  /* PA Range: 1 */
    { 0x1E,  },  /* PA Range: 2 */
    { 0x1C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 27 LTE_B12_LB4 & LTE_B13_LB4 & LTE_B17_LB4 & LTE_B28_LB4*/
    { 0x26,  },  /* PA Range: 0 */
    { 0x26,  },  /* PA Range: 1 */
    { 0x26,  },  /* PA Range: 2 */
    { 0x24,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 28 LTE_B12_LB5 & LTE_B13_LB5 & LTE_B17_LB5 & LTE_B28_LB5*/
    { 0x2E,  },  /* PA Range: 0 */
    { 0x2E,  },  /* PA Range: 1 */
    { 0x2E,  },  /* PA Range: 2 */
    { 0x2C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 29 LTE_B34_MB1*/
    { 0x34, },  /* PA Range: 0 */
    { 0x34, },  /* PA Range: 1 */
    { 0x34, },  /* PA Range: 2 */
    { 0x34, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 30 LTE_B34_MB2*/
    { 0x3C, },  /* PA Range: 0 */
    { 0x3C, },  /* PA Range: 1 */
    { 0x3C, },  /* PA Range: 2 */
    { 0x3C, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 31 LTE_B34_MB3*/
    { 0x44, },  /* PA Range: 0 */
    { 0x44, },  /* PA Range: 1 */
    { 0x44, },  /* PA Range: 2 */
    { 0x44, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 32 LTE_B34_MB4*/
    { 0x4C,  },  /* PA Range: 0 */
    { 0x4C,  },  /* PA Range: 1 */
    { 0x4C,  },  /* PA Range: 2 */
    { 0x4C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 33 LTE_B34_MB5*/
    { 0x54,  },  /* PA Range: 0 */
    { 0x54,  },  /* PA Range: 1 */
    { 0x54,  },  /* PA Range: 2 */
    { 0x54,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 34 LTE_B39_MB1*/
    { 0x36, },  /* PA Range: 0 */
    { 0x36, },  /* PA Range: 1 */
    { 0x34, },  /* PA Range: 2 */
    { 0x34, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 35 LTE_B39_MB2*/
    { 0x3E, },  /* PA Range: 0 */
    { 0x3E, },  /* PA Range: 1 */
    { 0x3C, },  /* PA Range: 2 */
    { 0x3C, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 36 LTE_B39_MB3*/
    { 0x46, },  /* PA Range: 0 */
    { 0x46, },  /* PA Range: 1 */
    { 0x44, },  /* PA Range: 2 */
    { 0x44, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 37 LTE_B39_MB4*/
    { 0x4E, },  /* PA Range: 0 */
    { 0x4E, },  /* PA Range: 1 */
    { 0x4C, },  /* PA Range: 2 */
    { 0x4C, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 38 LTE_B39_MB5*/
    { 0x56,  },  /* PA Range: 0 */
    { 0x56,  },  /* PA Range: 1 */
    { 0x54,  },  /* PA Range: 2 */
    { 0x54,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 39 LTE_B30_HB1 & LTE_B40_HB1*/
    { 0x5C,  },  /* PA Range: 0 */
    { 0x5C,  },  /* PA Range: 1 */
    { 0x5C,  },  /* PA Range: 2 */
    { 0x5C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 40 LTE_B30_HB2 & LTE_B40_HB2*/
    { 0x64,  },  /* PA Range: 0 */
    { 0x64,  },  /* PA Range: 1 */
    { 0x64,  },  /* PA Range: 2 */
    { 0x64,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 41 LTE_B30_HB3 & LTE_B40_HB3*/
    { 0x6C,  },  /* PA Range: 0 */
    { 0x6C,  },  /* PA Range: 1 */
    { 0x6C,  },  /* PA Range: 2 */
    { 0x6C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 42 LTE_B30_HB4 & LTE_B40_HB4*/
    { 0x74,  },  /* PA Range: 0 */
    { 0x74,  },  /* PA Range: 1 */
    { 0x74,  },  /* PA Range: 2 */
    { 0x74,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 43 LTE_B41_HB1_PC2(power class 2)*/
    { 0x5C, },  /* PA Range: 0 */
    { 0x5C, },  /* PA Range: 1 */
    { 0x5C, },  /* PA Range: 2 */
    { 0x5C, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 44 LTE_B41_HB2_PC2*/
    { 0x64, },  /* PA Range: 0 */
    { 0x64, },  /* PA Range: 1 */
    { 0x64, },  /* PA Range: 2 */
    { 0x64, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 45 LTE_B41_HB3_PC2*/
    { 0x6C,  },  /* PA Range: 0 */
    { 0x6C,  },  /* PA Range: 1 */
    { 0x6C,  },  /* PA Range: 2 */
    { 0x6C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 46 LTE_B41_HB4_PC2*/
    { 0x74,  },  /* PA Range: 0 */
    { 0x74,  },  /* PA Range: 1 */
    { 0x74,  },  /* PA Range: 2 */
    { 0x74,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 47 LTE_B71_LB1*/
    { 0x0C, },  /* PA Range: 0 */
    { 0x0C, },  /* PA Range: 1 */
    { 0x0E, },  /* PA Range: 2 */
    { 0x0C, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 48 LTE_B71_LB2*/
    { 0x14, },  /* PA Range: 0 */
    { 0x14, },  /* PA Range: 1 */
    { 0x16, },  /* PA Range: 2 */
    { 0x14, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 49 LTE_B71_LB3*/
    { 0x1C, },  /* PA Range: 0 */
    { 0x1C, },  /* PA Range: 1 */
    { 0x1E, },  /* PA Range: 2 */
    { 0x1C, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 50 LTE_B71_LB4*/
    { 0x24,  },  /* PA Range: 0 */
    { 0x24,  },  /* PA Range: 1 */
    { 0x26,  },  /* PA Range: 2 */
    { 0x24,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 51 LTE_B71_LB5*/
    { 0x2C,  },  /* PA Range: 0 */
    { 0x2C,  },  /* PA Range: 1 */
    { 0x2E,  },  /* PA Range: 2 */
    { 0x2C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 52 WCDMA_B1_MB1*/
    { 0x34,  },  /* PA Range: 0 */
    { 0x34,  },  /* PA Range: 1 */
    { 0x34,  },  /* PA Range: 2 */
    { 0x34,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 53 WCDMA_B1_MB2*/
    { 0x3C,  },  /* PA Range: 0 */
    { 0x3C,  },  /* PA Range: 1 */
    { 0x3C,  },  /* PA Range: 2 */
    { 0x3C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 54 WCDMA_B1_MB3*/
    { 0x44,  },  /* PA Range: 0 */
    { 0x44,  },  /* PA Range: 1 */
    { 0x44,  },  /* PA Range: 2 */
    { 0x44,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 55 WCDMA_B1_MB4*/
    { 0x4C,  },  /* PA Range: 0 */
    { 0x4C,  },  /* PA Range: 1 */
    { 0x4C,  },  /* PA Range: 2 */
    { 0x4C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 56 WCDMA_B1_MB5*/
    { 0x54,  },  /* PA Range: 0 */
    { 0x54,  },  /* PA Range: 1 */
    { 0x54,  },  /* PA Range: 2 */
    { 0x54,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 57 WCDMA_B2_MB1*/
    { 0x34,  },  /* PA Range: 0 */
    { 0x34,  },  /* PA Range: 1 */
    { 0x34,  },  /* PA Range: 2 */
    { 0x34,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 58 WCDMA_B2_MB2*/
    { 0x3C,  },  /* PA Range: 0 */
    { 0x3C,  },  /* PA Range: 1 */
    { 0x3C,  },  /* PA Range: 2 */
    { 0x3C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 59 WCDMA_B2_MB3*/
    { 0x44,  },  /* PA Range: 0 */
    { 0x44,  },  /* PA Range: 1 */
    { 0x44,  },  /* PA Range: 2 */
    { 0x44,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 60 WCDMA_B2_MB4*/
    { 0x4C,  },  /* PA Range: 0 */
    { 0x4C,  },  /* PA Range: 1 */
    { 0x4C,  },  /* PA Range: 2 */
    { 0x4C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 61 WCDMA_B2_MB5*/
    { 0x54,  },  /* PA Range: 0 */
    { 0x54,  },  /* PA Range: 1 */
    { 0x54,  },  /* PA Range: 2 */
    { 0x54,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 62 WCDMA_B3_MB1 & WCDMA_B4_MB1*/
    { 0x34,  },  /* PA Range: 0 */
    { 0x34,  },  /* PA Range: 1 */
    { 0x34,  },  /* PA Range: 2 */
    { 0x34,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 63 WCDMA_B3_MB2 & WCDMA_B4_MB2*/
    { 0x3C,  },  /* PA Range: 0 */
    { 0x3C,  },  /* PA Range: 1 */
    { 0x3C,  },  /* PA Range: 2 */
    { 0x3C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 64 WCDMA_B3_MB3 & WCDMA_B4_MB3*/
    { 0x44,  },  /* PA Range: 0 */
    { 0x44,  },  /* PA Range: 1 */
    { 0x44,  },  /* PA Range: 2 */
    { 0x44,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 65 WCDMA_B3_MB4 & WCDMA_B4_MB4*/
    { 0x4C,  },  /* PA Range: 0 */
    { 0x4C,  },  /* PA Range: 1 */
    { 0x4C,  },  /* PA Range: 2 */
    { 0x4C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 66 WCDMA_B3_MB5 & WCDMA_B4_MB5*/
    { 0x54,  },  /* PA Range: 0 */
    { 0x54,  },  /* PA Range: 1 */
    { 0x54,  },  /* PA Range: 2 */
    { 0x54,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 67 WCDMA_B5_LB1*/
    { 0x0C },  /* PA Range: 0 */
    { 0x0C },  /* PA Range: 1 */
    { 0x0C },  /* PA Range: 2 */
    { 0x0C },  /* PA Range: 3 */
  },
  { /* PORT NUM: 68 WCDMA_B5_LB2*/
    { 0x14 },  /* PA Range: 0 */
    { 0x14 },  /* PA Range: 1 */
    { 0x14 },  /* PA Range: 2 */
    { 0x14 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 69 WCDMA_B5_LB3*/
    { 0x1C },  /* PA Range: 0 */
    { 0x1C },  /* PA Range: 1 */
    { 0x1C },  /* PA Range: 2 */
    { 0x1C },  /* PA Range: 3 */
  },
  { /* PORT NUM: 70 WCDMA_B5_LB4*/
    { 0x24 },  /* PA Range: 0 */
    { 0x24 },  /* PA Range: 1 */
    { 0x24 },  /* PA Range: 2 */
    { 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 71 WCDMA_B5_LB5*/
    { 0x2C },  /* PA Range: 0 */
    { 0x2C },  /* PA Range: 1 */
    { 0x2C },  /* PA Range: 2 */
    { 0x2C },  /* PA Range: 3 */
  },
  { /* PORT NUM: 72 WCDMA_B8_LB1*/
    { 0x0C },  /* PA Range: 0 */
    { 0x0C },  /* PA Range: 1 */
    { 0x0C },  /* PA Range: 2 */
    { 0x0C },  /* PA Range: 3 */
  },
  { /* PORT NUM: 73 WCDMA_B8_LB2*/
    { 0x14 },  /* PA Range: 0 */
    { 0x14 },  /* PA Range: 1 */
    { 0x14 },  /* PA Range: 2 */
    { 0x14 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 74 WCDMA_B8_LB3*/
    { 0x1C },  /* PA Range: 0 */
    { 0x1C },  /* PA Range: 1 */
    { 0x1C },  /* PA Range: 2 */
    { 0x1C },  /* PA Range: 3 */
  },
  { /* PORT NUM: 75 WCDMA_B8_LB4*/
    { 0x24 },  /* PA Range: 0 */
    { 0x24 },  /* PA Range: 1 */
    { 0x24 },  /* PA Range: 2 */
    { 0x24 },  /* PA Range: 3 */
  },
  { /* PORT NUM: 76 WCDMA_B8_LB5*/
    { 0x2C },  /* PA Range: 0 */
    { 0x2C },  /* PA Range: 1 */
    { 0x2C },  /* PA Range: 2 */
    { 0x2C },  /* PA Range: 3 */
  },
  { /* PORT NUM: 77 TD34_MB1*/
    { 0x34,  },  /* PA Range: 0 */
    { 0x34,  },  /* PA Range: 1 */
    { 0x34,  },  /* PA Range: 2 */
    { 0x34,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 78 TD34_MB2*/
    { 0x3C,  },  /* PA Range: 0 */
    { 0x3C,  },  /* PA Range: 1 */
    { 0x3C,  },  /* PA Range: 2 */
    { 0x3C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 79 TD34_MB3*/
    { 0x44,  },  /* PA Range: 0 */
    { 0x44,  },  /* PA Range: 1 */
    { 0x44,  },  /* PA Range: 2 */
    { 0x44,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 80 TD34_MB4*/
    { 0x4C,  },  /* PA Range: 0 */
    { 0x4C,  },  /* PA Range: 1 */
    { 0x4C,  },  /* PA Range: 2 */
    { 0x4C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 81 TD34_MB5*/
    { 0x54,  },  /* PA Range: 0 */
    { 0x54,  },  /* PA Range: 1 */
    { 0x54,  },  /* PA Range: 2 */
    { 0x54,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 82 TD39_MB1*/
    { 0x36,  },  /* PA Range: 0 */
    { 0x36,  },  /* PA Range: 1 */
    { 0x34,  },  /* PA Range: 2 */
    { 0x34,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 83 TD39_MB2*/
    { 0x3E,  },  /* PA Range: 0 */
    { 0x3E,  },  /* PA Range: 1 */
    { 0x3C,  },  /* PA Range: 2 */
    { 0x3C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 84 TD39_MB3*/
    { 0x46,  },  /* PA Range: 0 */
    { 0x46,  },  /* PA Range: 1 */
    { 0x44,  },  /* PA Range: 2 */
    { 0x44,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 85 TD39_MB4*/
    { 0x4E,  },  /* PA Range: 0 */
    { 0x4E,  },  /* PA Range: 1 */
    { 0x4C,  },  /* PA Range: 2 */
    { 0x4C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 86 TD34_MB5*/
    { 0x56,  },  /* PA Range: 0 */
    { 0x56,  },  /* PA Range: 1 */
    { 0x54,  },  /* PA Range: 2 */
    { 0x54,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 87 LTE_B2_MB1 & LTE_B25_MB1*/
    { 0x34,  },  /* PA Range: 0 */
    { 0x34,  },  /* PA Range: 1 */
    { 0x34,  },  /* PA Range: 2 */
    { 0x34,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 88 LTE_B2_MB2 & LTE_B25_MB2*/
    { 0x3C,  },  /* PA Range: 0 */
    { 0x3C,  },  /* PA Range: 1 */
    { 0x3C,  },  /* PA Range: 2 */
    { 0x3C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 89 LTE_B2_MB3 & LTE_B25_MB3*/
    { 0x44,  },  /* PA Range: 0 */
    { 0x44,  },  /* PA Range: 1 */
    { 0x44,  },  /* PA Range: 2 */
    { 0x44,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 90 LTE_B2_MB4 & LTE_B25_MB4*/
    { 0x4C,  },  /* PA Range: 0 */
    { 0x4C,  },  /* PA Range: 1 */
    { 0x4C,  },  /* PA Range: 2 */
    { 0x4C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 91 LTE_B2_MB5 & LTE_B25_MB5*/
    { 0x54,  },  /* PA Range: 0 */
    { 0x54,  },  /* PA Range: 1 */
    { 0x54,  },  /* PA Range: 2 */
    { 0x54,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 92 BC6_MB1*/
    { 0x34,  },  /* PA Range: 0 */
    { 0x34,  },  /* PA Range: 1 */
    { 0x34,  },  /* PA Range: 2 */
    { 0x34,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 93 BC6_MB2*/
    { 0x3C,  },  /* PA Range: 0 */
    { 0x3C,  },  /* PA Range: 1 */
    { 0x3C,  },  /* PA Range: 2 */
    { 0x3C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 94 BC6_MB3*/
    { 0x44,  },  /* PA Range: 0 */
    { 0x44,  },  /* PA Range: 1 */
    { 0x44,  },  /* PA Range: 2 */
    { 0x44,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 95 BC6_MB4*/
    { 0x4C,  },  /* PA Range: 0 */
    { 0x4C,  },  /* PA Range: 1 */
    { 0x4C,  },  /* PA Range: 2 */
    { 0x4C,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 96 BC6_MB5*/
    { 0x54,  },  /* PA Range: 0 */
    { 0x54,  },  /* PA Range: 1 */
    { 0x54,  },  /* PA Range: 2 */
    { 0x54,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 97 LTE_B12_LB1_LB2IN & LTE_B13_LB1_LB2IN & LTE_B17_LB1_LB2IN & LTE_B28_LB1_LB2IN*/
    { 0x0F,  },  /* PA Range: 0 */
    { 0x0F,  },  /* PA Range: 1 */
    { 0x0F,  },  /* PA Range: 2 */
    { 0x0D,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 98 LTE_B12_LB2_LB2IN & LTE_B13_LB2_LB2IN & LTE_B17_LB2_LB2IN & LTE_B28_LB2_LB2IN*/
    { 0x17,  },  /* PA Range: 0 */
    { 0x17,  },  /* PA Range: 1 */
    { 0x17,  },  /* PA Range: 2 */
    { 0x15,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 99 LTE_B12_LB3_LB2IN & LTE_B13_LB3_LB2IN & LTE_B17_LB3_LB2IN & LTE_B28_LB3_LB2IN*/
    { 0x1F,  },  /* PA Range: 0 */
    { 0x1F,  },  /* PA Range: 1 */
    { 0x1F,  },  /* PA Range: 2 */
    { 0x1D,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 100 LTE_B12_LB4_LB2IN & LTE_B13_LB4_LB2IN & LTE_B17_LB4_LB2IN & LTE_B28_LB4_LB2IN*/
    { 0x27,  },  /* PA Range: 0 */
    { 0x27,  },  /* PA Range: 1 */
    { 0x27,  },  /* PA Range: 2 */
    { 0x25,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 101 LTE_B12_LB5_LB2IN & LTE_B13_LB5_LB2IN & LTE_B17_LB5_LB2IN & LTE_B28_LB5_LB2IN*/
    { 0x2F,  },  /* PA Range: 0 */
    { 0x2F,  },  /* PA Range: 1 */
    { 0x2F,  },  /* PA Range: 2 */
    { 0x2D,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 102 LTE_B71_LB1_LB2IN*/
    { 0x0D, },  /* PA Range: 0 */
    { 0x0D, },  /* PA Range: 1 */
    { 0x0F, },  /* PA Range: 2 */
    { 0x0D, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 103 LTE_B71_LB2_LB2IN*/
    { 0x15, },  /* PA Range: 0 */
    { 0x15, },  /* PA Range: 1 */
    { 0x17, },  /* PA Range: 2 */
    { 0x15, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 104 LTE_B71_LB3_LB2IN*/
    { 0x1D, },  /* PA Range: 0 */
    { 0x1D, },  /* PA Range: 1 */
    { 0x1F, },  /* PA Range: 2 */
    { 0x1D, },  /* PA Range: 3 */
  },
  { /* PORT NUM: 105 LTE_B71_LB4_LB2IN*/
    { 0x25,  },  /* PA Range: 0 */
    { 0x25,  },  /* PA Range: 1 */
    { 0x27,  },  /* PA Range: 2 */
    { 0x25,  },  /* PA Range: 3 */
  },
  { /* PORT NUM: 106 LTE_B71_LB5_LB2IN*/
    { 0x2D,  },  /* PA Range: 0 */
    { 0x2D,  },  /* PA Range: 1 */
    { 0x2F,  },  /* PA Range: 2 */
    { 0x2D,  },  /* PA Range: 3 */
  },
};


#define RFDEVICE_PA_S5643_55_PA_ON_NUM_REGS 1
static uint8 rfdevice_pa_s5643_55_pa_on_regs[RFDEVICE_PA_S5643_55_PA_ON_NUM_REGS] =  {RFFE_INVALID_REG_ADDR /*Warning: Not Specified*/, };
static int16 rfdevice_pa_s5643_55_pa_on_data[RFDEVICE_PA_S5643_55_NUM_PORTS][RFDEVICE_PA_S5643_55_PA_ON_NUM_REGS] =
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
};


#define RFDEVICE_PA_S5643_55_PA_OFF_NUM_REGS 1
static uint8 rfdevice_pa_s5643_55_pa_off_regs[RFDEVICE_PA_S5643_55_PA_OFF_NUM_REGS] =  {0x00, };
static int16 rfdevice_pa_s5643_55_pa_off_data[RFDEVICE_PA_S5643_55_NUM_PORTS][RFDEVICE_PA_S5643_55_PA_OFF_NUM_REGS] =
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
};


#define RFDEVICE_PA_S5643_55_PA_TRIGGER_NUM_REGS 1
static uint8 rfdevice_pa_s5643_55_pa_trigger_regs[RFDEVICE_PA_S5643_55_PA_TRIGGER_NUM_REGS] =  {0x1C, };
static int16 rfdevice_pa_s5643_55_pa_trigger_data[RFDEVICE_PA_S5643_55_NUM_PORTS][RFDEVICE_PA_S5643_55_PA_TRIGGER_NUM_REGS] =
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
};

/* singleton instance ptr */
rfdevice_pa_data *rfdevice_pa_s5643_55_data_ag::rfdevice_pa_s5643_55_data_ptr = NULL;

rfdevice_pa_data * rfdevice_pa_s5643_55_data_ag::get_instance()
{
  if (rfdevice_pa_s5643_55_data_ptr == NULL)
  {
    rfdevice_pa_s5643_55_data_ptr = (rfdevice_pa_data *)new rfdevice_pa_s5643_55_data_ag();
  }
  return( (rfdevice_pa_data *)rfdevice_pa_s5643_55_data_ptr);
}

//constructor
rfdevice_pa_s5643_55_data_ag::rfdevice_pa_s5643_55_data_ag()
  :rfdevice_pa_data()
{
}

//destructor
rfdevice_pa_s5643_55_data_ag::~rfdevice_pa_s5643_55_data_ag()
{
  rfdevice_pa_s5643_55_data_ptr = NULL;
}

boolean rfdevice_pa_s5643_55_data_ag::settings_data_get( rfdevice_pa_cfg_params_type *cfg, 
                                                          rfdevice_pa_reg_settings_type *settings)
{
  boolean ret_val = FALSE;

  if (NULL == settings || NULL == cfg)
  {
    return FALSE;
  }

  if (cfg->port >= RFDEVICE_PA_S5643_55_NUM_PORTS )
  {
    settings->addr = NULL;
    settings->data = NULL;
    settings->num_regs = 0;
    return FALSE;
  }

  if ( (cfg->req == RFDEVICE_PA_SET_BIAS_DATA) && (cfg->pa_range < 4) )
  {
    settings->addr = &(rfdevice_pa_s5643_55_pa_set_bias_regs[0]);
    settings->data = &(rfdevice_pa_s5643_55_pa_set_bias_data[cfg->port][cfg->pa_range][0]);
    settings->num_regs = RFDEVICE_PA_S5643_55_PA_SET_BIAS_NUM_REGS;
    settings->bias_reg_idx = 0;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_SET_RANGE_DATA) && (cfg->pa_range < 4) )
  {
    settings->addr = &(rfdevice_pa_s5643_55_pa_set_range_regs[0]);
    settings->data = &(rfdevice_pa_s5643_55_pa_set_range_data[cfg->port][cfg->pa_range][0]);
    settings->num_regs = RFDEVICE_PA_S5643_55_PA_SET_RANGE_NUM_REGS;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_ON_DATA) )
  {
    settings->addr = &(rfdevice_pa_s5643_55_pa_on_regs[0]);
    settings->data = &(rfdevice_pa_s5643_55_pa_on_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_PA_S5643_55_PA_ON_NUM_REGS;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_OFF_DATA) )
  {
    settings->addr = &(rfdevice_pa_s5643_55_pa_off_regs[0]);
    settings->data = &(rfdevice_pa_s5643_55_pa_off_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_PA_S5643_55_PA_OFF_NUM_REGS;
    ret_val = TRUE;
  }

  if ( (cfg->req == RFDEVICE_PA_TRIGGER_DATA) )
  {
    settings->addr = &(rfdevice_pa_s5643_55_pa_trigger_regs[0]);
    settings->data = &(rfdevice_pa_s5643_55_pa_trigger_data[cfg->port][0]);
    settings->num_regs = RFDEVICE_PA_S5643_55_PA_TRIGGER_NUM_REGS;
    ret_val = TRUE;
  }

  return ret_val;
}

boolean rfdevice_pa_s5643_55_data_ag::sequence_data_get( rfdevice_pa_cfg_params_type *cfg, 
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

boolean rfdevice_pa_s5643_55_data_ag::device_info_get( rfdevice_pa_info_type *pa_info )
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
    pa_info->prd_rev = 5;
    pa_info->num_ports = RFDEVICE_PA_S5643_55_NUM_PORTS;
    pa_info->num_pa_ranges = 4;
	pa_info->rffev20_usid_p3 = FALSE;	
    ret_val = TRUE;
  }
  return ret_val;
}

boolean rfdevice_pa_s5643_55_data_ag::get_temperature_lut(int16 **lut_addr, uint8 *lut_size)
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

uint32 rfdevice_pa_s5643_55_data_ag::get_max_bw_supported()
{
   return 20000;
}
