/*============================================================================
  FILE:         TsensBsp.c

  OVERVIEW:     Auto-generated TSENS BSP
                * Chip: SDX24, version: all
                * Config file: TSENS_Chiron.xlsm
                * Config file format version: 3
                * Generator version: 1.2.0

                Copyright (c) 2017 Qualcomm Technologies, Inc.
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
      .pszSROTModule = "+TSENS0",
      .pszTMModule   = "+TSENS0_TM",
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

