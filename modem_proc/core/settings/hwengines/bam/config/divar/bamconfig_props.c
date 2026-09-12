/**
  @file bamconfig_props.c
  @brief
  This file contains implementation of the interfaces that provide
  target specific base addresses & irq vector data to the BAM driver.

*/
/*
===============================================================================
                            Edit History

$Header:

when       who     what, where, why
--------   ---     ------------------------------------------------------------
11/07/13   SA      Added devcfg props support for tgtcfg and supported BAMs

===============================================================================
                   Copyright (c) 2013-2019 Qualcomm Technologies Incorporated.
                          All Rights Reserved.
                    Qualcomm Confidential and Proprietary.
===============================================================================
*/
#ifdef DAL_CONFIG_IMAGE_APPS
#ifndef BAM_APPS_CONFIG
#define BAM_APPS_CONFIG
#endif
#endif

#include "bamtgtsetting.h"
#include "bamtgtcfgdata.h"

