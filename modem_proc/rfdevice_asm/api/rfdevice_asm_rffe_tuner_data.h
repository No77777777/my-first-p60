#ifndef RFDEVICE_ASM_RFFE_TUNER_DATA
#define RFDEVICE_ASM_RFFE_TUNER_DATA

/*!
  @file
  rfdevice_asm_rffe_tuner_data.h

  @brief
  This file contains the class definition for the rfdevice_asm_rffe_tuner_data,
  which serves as base class for the vendor specific ASM settings data and configuration.

*/

/*===========================================================================

Copyright (c) 2011-13 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_asm.mpss/1.32/api/rfdevice_asm_rffe_tuner_data.h#1 $

when       who     what, where, why
-------------------------------------------------------------------------------   
06/18/14   aca     therm support
02/19/13   sr      added device_info_get() api.
01/28/13   sr      init version
============================================================================*/
#include "comdef.h"
#include "rfa.h" 
#include "rfdevice_asm_types.h"
#include "rfdevice_asm.h"
#include "rfdevice_asm_data.h"


class rfdevice_asm_rffe_tuner_data : public rfdevice_asm_data
{
public:

	virtual uint8 get_scenario(uint64 pcc_band, uint64 lte_active_band);
	virtual uint64 get_priority_band(void);


    // Destructor
    virtual ~rfdevice_asm_rffe_tuner_data();

protected:
  rfdevice_asm_rffe_tuner_data(void);  /*  Constructor  */

private:
};


#endif


