/*==============================================================================
   FILE:        MPMIntDevCfgData.c

  OVERVIEW:     MPM hardware to local interrupt/gpio mapping info. This
                mapping is target specific and may change for different 
                targets. Even mapping could change between masters within a
                same target as some master may want a mapped irq as wakeup
                interrupt while other may not.
 
  DEPENDENCIES: None

                Copyright (c) 2015-2021 QUALCOMM Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
				flat file : kamorta_v1.0_p3q3r59_MTO

===============================================================================
$Header: //components/rel/core.mpss/10.0/power/mpm/config/asic/kamorta/MPMIntDevCfgData.c#4 $
$DateTime: 2021/07/15 23:27:29 $
=============================================================================*/ 

/* -----------------------------------------------------------------------
**                           INCLUDES
** ----------------------------------------------------------------------- */
#include "vmpm_internal.h"

/* -----------------------------------------------------------------------
**                           DATA
** ----------------------------------------------------------------------- */

/**
 * Table mapping the MPM Pin number to the GPIO and/or IRQ pin 
 * number per the IP Catalog. 
 */
mpm_int_pin_mapping_type MpmInterruptPinNum_Mapping_kamorta_xml[] =
{
  /* Trigger                 Local Pin#         Pin Type   */
  /* -------                 -------------      --------   */
  /*0*/
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  /*5*/
  { MPM_TRIGGER_LEVEL_HIGH, MPM_UNMAPPED_INT,  MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, 24              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, 83              , MPM_GPIO },
  /*10*/
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  /*15*/
  { MPM_TRIGGER_LEVEL_HIGH, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, 4               , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 11              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 13              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 86              , MPM_GPIO },
  /*20*/
  { MPM_TRIGGER_RISING_EDGE, 17              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 80              , MPM_GPIO },
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  /*25*/
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, 31              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 39              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 46              , MPM_GPIO },
  /*30*/
  { MPM_TRIGGER_RISING_EDGE, 65              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 66              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 67              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 69              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 70              , MPM_GPIO },
  /*35*/
  { MPM_TRIGGER_RISING_EDGE, 75              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 79              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, 81              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 84              , MPM_GPIO },
  /*40*/
  { MPM_TRIGGER_RISING_EDGE, 85              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 28              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 87              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 88              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  /*45*/
  { MPM_TRIGGER_RISING_EDGE, 89              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 93              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 94              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 95              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 96              , MPM_GPIO },
  /*50*/
  { MPM_TRIGGER_RISING_EDGE, 97              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 14              , MPM_GPIO },
  { MPM_TRIGGER_LEVEL_HIGH,  18              , MPM_GPIO },
  { MPM_TRIGGER_LEVEL_HIGH,  19              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 32              , MPM_GPIO },
  /*55*/
  { MPM_TRIGGER_RISING_EDGE, 33              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 34              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 35              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 36              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 6               , MPM_GPIO },
  /*60*/
  { MPM_TRIGGER_RISING_EDGE, 62              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 63              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 64              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 8               , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  /*65*/
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  /*70*/
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_LEVEL_HIGH,  25              , MPM_GPIO },
  { MPM_TRIGGER_LEVEL_HIGH,  72              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 27              , MPM_GPIO },
  { MPM_TRIGGER_RISING_EDGE, 91              , MPM_GPIO },
  /*75*/
  { MPM_TRIGGER_RISING_EDGE, 3               , MPM_GPIO },
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  /*80*/
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_LEVEL_HIGH,  0               , MPM_GPIO },
  /*85*/
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  /*90*/
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_RISING_EDGE, MPM_UNMAPPED_INT, MPM_OPEN },
  { MPM_TRIGGER_LEVEL_HIGH,  92              , MPM_IRQ },
  { MPM_TRIGGER_LEVEL_HIGH,  89              , MPM_IRQ },
  { MPM_TRIGGER_LEVEL_HIGH,  90              , MPM_IRQ },
  /*95*/
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_OPEN },
  
  /* End of the table entry */
  { MPM_TRIGGER_LEVEL_HIGH,  MPM_UNMAPPED_INT, MPM_EOT  },
};
