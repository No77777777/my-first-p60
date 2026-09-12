/*============================================================================
  FILE:         TsensBsp.c

  OVERVIEW:     Auto-generated TSENS BSP
                * Chip: SDM4xx, version: all
                * Config file: TSENS_Agatti.xlsm, version: 4.0
                * Config file format version: 4
                * Generator version: 1.6.0

                Copyright (c) 2020 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "TsensBsp.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define ARRAY_LENGTH(a) (sizeof(a) / sizeof((a)[0]))

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/
static const TsensControllerCfgType gaTsensControllerCfgs[] =
{
   /* Controller 0 */
   {
      .pszSROTModule         = "+TSENS0",
      .pszTMModule           = "+TSENS0_TM",
   },
};

static const TsensSensorCfgType gaTsensSensorCfgs[] =
{
   /* Sensor 0 */
   {
      .ucController                = 0,
      .ucChannel                   = 0,
   },

   /* Sensor 1 */
   {
      .ucController                = 0,
      .ucChannel                   = 1,
   },

   /* Sensor 2 */
   {
      .ucController                = 0,
      .ucChannel                   = 2,
   },

   /* Sensor 3 */
   {
      .ucController                = 0,
      .ucChannel                   = 3,
   },

   /* Sensor 4 */
   {
      .ucController                = 0,
      .ucChannel                   = 4,
   },

   /* Sensor 5 */
   {
      .ucController                = 0,
      .ucChannel                   = 5,
   },

   /* Sensor 6 */
   {
      .ucController                = 0,
      .ucChannel                   = 6,
   },

   /* Sensor 7 */
   {
      .ucController                = 0,
      .ucChannel                   = 7,
   },

   /* Sensor 8 */
   {
      .ucController                = 0,
      .ucChannel                   = 8,
   },

   /* Sensor 9 */
   {
      .ucController                = 0,
      .ucChannel                   = 9,
   },
};

const TsensBspType TsensBsp[] =
{
   {
      .paControllerCfgs    = gaTsensControllerCfgs,
      .uNumControllers     = ARRAY_LENGTH(gaTsensControllerCfgs),
      .paSensorCfgs        = gaTsensSensorCfgs,
      .uNumSensors         = ARRAY_LENGTH(gaTsensSensorCfgs),
   }
};

