#ifndef __TSENS_BSP_H__
#define __TSENS_BSP_H__
/*============================================================================
  @file TsensBsp.h

  Tsens BSP file.

                Copyright (c) 2012, 2014, 2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/* $Header: //components/rel/core.mpss/10.0/settings/hwengines/tsens/interface/TsensBsp.h#1 $ */

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "DALStdDef.h"

/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
typedef struct
{
   const char *pszSROTModule;  /* Name of SROT module */
   const char *pszTMModule;    /* Name of TM module */
#ifdef FEATURE_TSENS_THRESHOLDS
   uint32 uUpperLowerInt;      /* Upper / lower interrupt vector */
   uint32 uCriticalInt;        /* Critical interrupt vector */
#endif
} TsensControllerCfgType;

typedef struct
{
   uint8 ucController;  /* Controller index */
   uint8 ucChannel;     /* Channel index */
} TsensSensorCfgType;

typedef struct
{
   const TsensControllerCfgType *paControllerCfgs;  /* Array of controller configs */
   uint32 uNumControllers;                          /* Number of controllers */
   const TsensSensorCfgType *paSensorCfgs;          /* Array of sensor configs */
   uint32 uNumSensors;                              /* Number of sensors */
} TsensBspType;

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ----------------------------------------------------------------------*/

#endif /* #ifndef __TSENS_BSP_H__ */

