/*==============================================================================

FILE:      NOC_error_data.c

DESCRIPTION: This file contains target/platform specific configuration data.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

//#CHANGE - Update when put in the depot
$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/icb/config/sdm845/wcss/NOC_error_data.c#1 $ 
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    -----------------------------------------------------------
2015/12/02  pm     Port to 8998, including Sideband Manager support
2014/12/09  tb     Split OEM modifiable data from internal data
2014/09/03  tb     Ported for 8996
2014/11/14  tb     Ported for 8994
2012/10/03  av     Created
 
        Copyright (c) 2012 - 2016 QUALCOMM Technologies, Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
==============================================================================*/
#include "NOC_error_cfg.h"
#include "ABTimeout_HWIO_WCSS.h"

/*============================================================================
                           TARGET SPECIFIC DATA
============================================================================*/

/*============================================================================
                        DEVICE CONFIG PROPERTY DATA
============================================================================*/

/* Internal NOC Configuration Data*/
const NOCERR_info_type NOCERR_cfgdata[] = 
{ 
  [0] = {
    .name = "WNOC",
    .base_addr = (void*) HWIO_WCSS_HM_A_NOC_CFG_WNOC_OBS_ID_COREID_ADDR,
    .errlog_filter = NOC_ERRLOG2 | NOC_ERRLOG6,
    .sb_base_addr = (void*) HWIO_WCSS_HM_A_NOC_CFG_WNOC_SBM_ID_COREID_ADDR,
    .intr_enable = TRUE,
    .error_fatal = TRUE,
    .sb_enable_mask = 0x0000002C,
    .sb_flagout_mask = 0x00000030,
  },
};

const NOCERR_propdata_type NOCERR_propdata =
{
    /* Length of the config data array */
    .len = sizeof(NOCERR_cfgdata)/sizeof(NOCERR_info_type), 
    /* Pointer to internal config data array */ 
    .NOCInfo = NOCERR_cfgdata,
};
