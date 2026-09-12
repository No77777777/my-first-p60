/*==============================================================================

  D A L   G P I O   M A N A G E R

DESCRIPTION
  This file has the direct connect GPIO interrupt mapping required for
  this target.

REFERENCES

    Copyright © 2015 QUALCOMM Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/GPIOMgr/config/mpss/divar/GPIOMgrConfigData.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
4/1/2015   cpaulo  First draft created. 
===========================================================================*/
#include "com_dtypes.h"
#include "GPIOMgrInterface.h"

GPIOMgrConfigMapType GPIOMgrConfigMap[] =
{
  {0, 444, GPIOMGR_UNUSED_GPIO,   0, GPIOMGR_DIRECTCONNECT_MSS   }, /* MSS direct connect 0  */
  {1, 445, GPIOMGR_UNUSED_GPIO,   0, GPIOMGR_DIRECTCONNECT_MSS   }, /* MSS direct connect 1 */
  {0, 0, 0, 0, 0}, /* End */  
};

GPIOMgrConfigPDType GPIOMgrConfigPD[] =
{
  {0, GPIOMGR_DEVICE_MODEM   },
  {0, GPIOMGR_DEVICE_NONE    }
};

GPIOMgrDirConnBaseMapType GPIOMgrDirConnBaseMap[] =
{
  {GPIOMGR_DIRECTCONNECT_MSS,     0 },
  {GPIOMGR_DIRECTCONNECT_NONE,    0 },
};









