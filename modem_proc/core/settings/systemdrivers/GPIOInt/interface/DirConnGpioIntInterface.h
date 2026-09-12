#ifndef _DIRCONNGPIOINTINTERFACE_H_
#define _DIRCONNGPIOINTINTERFACE_H_
/*===========================================================================

  DirConnGpioInt Settings   

DESCRIPTION
  This file contains enums, constants and structure defintions required
  for the GPIOInt settings interface

REFERENCES

       Copyright © 2016 Qualcomm Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/GPIOInt/interface/DirConnGpioIntInterface.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/

#include "DALStdDef.h"
#include "GPIOIntInterface.h"


/*
 * DirConnGpioIntConfigMapType
 * Structure storing the interrupt data map accross all processors.
 * uint32          gpio;                     GPIO pin number
 * uint32          interrupt_id; l1 main interrupt line for direct connects.
 * uint32          polarity;                 polarity of the interrupt.
 */
typedef struct
{
  uint32                                 gpio;
  GPIOIntInterruptType                   interrupt;
  uint32                                 interrupt_id;
  uint32                                 flags;
} DirConnGpioIntConfigMapType;


#endif /* _DIRCONNGPIOINTINTERFACE_H_ */
