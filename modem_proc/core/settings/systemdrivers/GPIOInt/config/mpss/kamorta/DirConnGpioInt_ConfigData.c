/*==============================================================================

  D A L   G P I O   I N T E R R U P T   C O N T R O L L E R    

DESCRIPTION
  This file has the direct connect GPIO interrupt support for the 8960 target.

REFERENCES

        Copyright © 2011 Qualcomm Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/GPIOInt/config/mpss/kamorta/DirConnGpioInt_ConfigData.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------

===========================================================================*/

#include "DALReg.h"
#include "GPIOIntInterface.h"
#include "DirConnGpioIntInterface.h"
/*
 * Modem Interrupt Config Map
 */

DirConnGpioIntConfigMapType InterruptConfigMap[] =
{
  {GPIOINT_NONE, GPIOINT_DIRECTCONNECT_0, 444,0}, /* Direct connect 0 owned by MSS guest OS*/
  {GPIOINT_NONE, GPIOINT_DIRECTCONNECT_1, 445,0}, /* Direct connect 1 owned by MSS guest OS*/
};












