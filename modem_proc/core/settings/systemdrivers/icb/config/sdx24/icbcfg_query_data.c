/*==============================================================================

FILE:      icbcfg_query_data.c

DESCRIPTION: This file implements the ICB Configuration driver.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/icb/config/sdx24/icbcfg_query_data.c#1 $ 
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
/* SDM845                                                                    */
/*---------------------------------------------------------------------------*/
icbcfg_data_type icbcfg_boot_data_sdm845[] = 
{
  ICBCFG_DEFAULT(),
};

icbcfg_prop_type icbcfg_boot_prop_sdm845 = 
{
  /* Length of the config  data array */
  ARRAY_SIZE(icbcfg_boot_data_sdm845),
  /* Pointer to config data array */ 
  icbcfg_boot_data_sdm845
};

icbcfg_data_type icbcfg_post_data_sdm845[] = 
{
  ICBCFG_DEFAULT(),
};

icbcfg_prop_type icbcfg_post_prop_sdm845 = 
{
  /* Length of the config  data array */
  ARRAY_SIZE(icbcfg_post_data_sdm845),
  /* Pointer to config data array */ 
  icbcfg_post_data_sdm845                                    
};

icbcfg_mem_region_type map_ddr_regions_sdm845[] =
{
  { 0x080000000ULL, 0x400000000ULL },
};

/* Translation block base addresses. */
uint8 *trans_bases_sdm845[] = 
{
  (uint8 *)HWIO_ADDR(MEMNOC_LLCC0_ADDR_TRANSLATOR_SWID_LOW),
  (uint8 *)HWIO_ADDR(MEMNOC_LLCC1_ADDR_TRANSLATOR_SWID_LOW),
  (uint8 *)HWIO_ADDR(MEMNOC_LLCC2_ADDR_TRANSLATOR_SWID_LOW),
  (uint8 *)HWIO_ADDR(MEMNOC_LLCC3_ADDR_TRANSLATOR_SWID_LOW)
};

/* SDM845 - ALL */
icbcfg_device_config_type sdm845_all =
{
  /* Chip version information for this device data. */
  CHIPINFO_FAMILY_SDM845,   /**< Chip family */
  false,                    /**< Exact match for version? */
  0,                        /**< Chip version */

  /* Device information. */
  ARRAY_SIZE(map_ddr_regions_sdm845), /**< Number of regions in the DDR map */
  map_ddr_regions_sdm845,             /**< Array of mappable DDR regions */
  6,                                  /**< Number of mapping segments. */
  4,                                  /**< Number of channels. */
  36,                                 /**< Address width in bits. */
  ICBCFG_ADDR_TRANS_NOC,              /**< Address translator hardware type. */
  trans_bases_sdm845,                 /**< Translation block base addresses. */

  /* Prop lists. */
  NULL,                               /**< Init time prop list. */
  NULL,                               /**< Post-init time prop list. */
};

/* Definitions list */
icbcfg_device_config_type *configs_sdm845[] =
{
  &sdm845_all,
};

/* Exported target definitions */
icbcfg_info_type icbcfg_info =
{
  ARRAY_SIZE(configs_sdm845),
  configs_sdm845,
};
