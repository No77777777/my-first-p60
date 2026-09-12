/*==============================================================================

FILE:      icbcfg_data.c

DESCRIPTION: This file implements the ICB Configuration driver.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/icb/config/nicobar/icbcfg_query_data.c#1 $ 
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    -----------------------------------------------------------
2015/09/23  sds    Created
 
        Copyright (c) 2017 Qualcomm Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
==============================================================================*/
#include "icbcfg.h"
#include "icbcfgi.h"
#include "icbcfg_hwio.h"
#include "HALbimc.h"
#include "HALbimcHwioGeneric.h"

/*---------------------------------------------------------------------------*/
/*          Macro and constant definitions                                   */
/*---------------------------------------------------------------------------*/
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

/* DDR slave indexes */
#define SLAVE_DDR_CH0 0
#define SLAVE_DDR_CH1 1

/*============================================================================
                        DEVICE CONFIG PROPERTY DATA
============================================================================*/

/*---------------------------------------------------------------------------*/
/*          Properties data for device ID  = "icbcfg/boot"                   */
/*---------------------------------------------------------------------------*/

/* ICBcfg Boot Configuration Data*/

icbcfg_data_type icbcfg_boot_data[] = 
{
  ICBCFG_DEFAULT()
};

icbcfg_prop_type icbcfg_boot_prop = 
{
  /* Length of the config  data array */
  ARRAY_SIZE(icbcfg_boot_data),
  /* Pointer to config data array */ 
  icbcfg_boot_data                                   
};

/* DDR map information. */
icbcfg_mem_region_type map_ddr_regions[] =
{
  { 0x020000000ULL, 0x400000000ULL },
};

uint32 channel_map[2] = { SLAVE_DDR_CH0, SLAVE_DDR_CH1 };

HAL_bimc_InfoType bimc_hal_info =
{
  (uint8_t *)BIMC_BASE, /* Base address */
  19200,                 /* QoS frequency */
  {
    0,
    0,
    0,
    0,
    0,
    0,
    4, /**< Number of segments for address decode. */
  }
};

/* Make sure the config region is always prohibited when "resetting" */
HAL_bimc_SlaveSegmentType safe_reset_seg =
{
  true,
  0x00000000ULL,                 /* start of config region */
  0x20000000ULL,                 /* 512 MB */
  BIMC_SEGMENT_TYPE_SUBTRACTIVE,
  BIMC_INTERLEAVE_NONE,
};

