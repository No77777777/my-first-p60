/*==============================================================================

FILE:      icbcfg_query_data.c

DESCRIPTION: This file implements the ICB Configuration driver.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/icb/config/sm7150/icbcfg_query_data.c#1 $ 
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    -----------------------------------------------------------
2017/01/31  sds    Updated to integrated format
2016/09/19  sds    Created
 
                Copyright (c) 2016-2017 Qualcomm Technologies, Inc.
                             All Rights Reserved.
                          QUALCOMM Proprietary/GTDR
==============================================================================*/
#include "icbcfgi.h"
#include "icbcfg_query_target.h"
#include "HALhwio.h"

/*---------------------------------------------------------------------------*/
/*          Macro and constant definitions                                   */
/*---------------------------------------------------------------------------*/
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

/*============================================================================
                        DEVICE CONFIG PROPERTY DATA
============================================================================*/

/*---------------------------------------------------------------------------*/
/*          Properties data for device ID  = "icbcfg/boot"                   */
/*---------------------------------------------------------------------------*/

/* ICBcfg Boot Configuration Data*/


/*---------------------------------------------------------------------------*/
/* sm7150                                                                  */
/*---------------------------------------------------------------------------*/

icbcfg_mem_region_type map_ddr_regions_sm7150[] =
{
  { 0x080000000ULL, 0x400000000ULL },
};

/* Translation block base addresses. */
uint8 *trans_bases_sm7150[] = 
{
  (uint8 *)HWIO_ADDR(LLCC0_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
  (uint8 *)HWIO_ADDR(LLCC1_LLCC_BEAC_ADDR_TRANSLATOR_CFG),
};

icbcfg_device_config_type sm7150 =
{
  /* Chip version information for this device data. */
  CHIPINFO_FAMILY_SM7150,   /**< Chip family */
  false,                       /**< Exact match for version? */
  CHIPINFO_VERSION(0,0),       /**< Chip version */

  /* Device information. */
  ARRAY_SIZE(map_ddr_regions_sm7150), /**< Number of regions in the DDR map */
  map_ddr_regions_sm7150,             /**< Array of mappable DDR regions */
  6,                                     /**< Number of mapping segments. */
  2,                                     /**< Number of channels. */
  36,                                    /**< Address width in bits. */
  ICBCFG_ADDR_TRANS_LLCC,                /**< Address translator hardware type. */
  trans_bases_sm7150,                 /**< Translation block base addresses. */

  /* Prop lists. */
  NULL,           /**< Init time prop list. */
  NULL,           /**< Post-init time prop list. */
};

/* Definitions list */
icbcfg_device_config_type *configs_sm7150[] =
{
  &sm7150,
};

/* Exported target definitions */
icbcfg_info_type icbcfg_info =
{
  ARRAY_SIZE(configs_sm7150),
  configs_sm7150,
};
