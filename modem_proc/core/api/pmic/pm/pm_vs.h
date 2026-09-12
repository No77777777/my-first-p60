#ifndef __PM_VS_H__
#define __PM_VS_H__

/** @file pm_vs.h 
 *
 *  This header file contains enums and API definitions for the
 *  voltage switch power rail driver.
 */
/*
 *  Copyright (c) 2010-2015 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/api/pmic/pm/pm_vs.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/10/13   kt      Changing the file as per Henry's Phase1 changes.  
02/08/11   hw      Merging changes from the PMIC Distributed Driver Arch branch
06/04/10   hw      Creation of Voltage Switch (VS) Module
========================================================================== */
/*===========================================================================

                        HEADER FILES

===========================================================================*/

/**
 *  Voltage switch peripheral index. This enum type contains all required
 *  VS regulators. 
 */
enum
{
  PM_VS_LVS_1,
  PM_VS_LVS_2,
  PM_VS_LVS_3,
  PM_VS_LVS_4,
  PM_VS_LVS_5,
  PM_VS_LVS_6,
  PM_VS_LVS_7,
  PM_VS_LVS_8,
  PM_VS_LVS_9,
  PM_VS_LVS_10,
  PM_VS_LVS_11,
  PM_VS_LVS_12,
  PM_VS_MVS_1,
  PM_VS_OTG_1,
  PM_VS_HDMI_1,
  PM_VS_INVALID
};

#endif /* __PM_VS_H__ */
