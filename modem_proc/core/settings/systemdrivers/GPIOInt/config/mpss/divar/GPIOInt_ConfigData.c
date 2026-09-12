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

$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/GPIOInt/config/mpss/divar/GPIOInt_ConfigData.c#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
5/5/2017   hvm     GPIOInt driver refactored. 
1/1/2010   aratin  First draft created. 
===========================================================================*/

#include "DALReg.h"
#include "GPIOIntInterface.h"

GPIOIntFnDescType* priority_list_gpioint_drivers[3] = {
  //&PdcGpio_Procs,       // func ptr table for PDC GPIO Int Driver
  &DirConnGpio_Procs, // func ptr table for DirConn GPIO Int driver //No GPIO direct connects
  &SummaryGpio_Procs    // func ptr table for Summary GPIO Int driver
};
