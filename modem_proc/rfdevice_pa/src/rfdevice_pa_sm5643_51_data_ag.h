
#ifndef RFDEVICE_PA_sm5643_51_DATA_AG
#define RFDEVICE_PA_sm5643_51_DATA_AG


#ifdef __cplusplus
extern "C" {
#endif
/*
WARNING: This file is auto-generated.

Generated using: pa_autogen.pl
Generated from:  2.1.82 of RFDevice_PA.xlsm
*/

/*=============================================================================

          R F C     A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

Copyright (c) 2009 - 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //Commercial/MPSS.DI.2.0.1.c1/Main/modem_proc/rfdevice_pa/src/rfdevice_pa_sm5643_51_data_ag.h#2 $ 


=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/
#include "comdef.h"

#include "rfdevice_pa_data.h"

class rfdevice_pa_sm5643_51_data_ag:public rfdevice_pa_data
{
public:
    static rfdevice_pa_data * get_instance();
    boolean settings_data_get( rfdevice_pa_cfg_params_type *cfg, 
                               rfdevice_pa_reg_settings_type *settings);
    boolean sequence_data_get( rfdevice_pa_cfg_params_type *cfg, 
                               rfdevice_pa_cmd_seq_type *cmd_seq);
    boolean device_info_get( rfdevice_pa_info_type *pa_info );
    boolean get_temperature_lut( int16 **lut_addr, uint8 *lut_size );
	uint32 get_max_bw_supported(void);
    ~rfdevice_pa_sm5643_51_data_ag(void);  /*  Destructor  */
protected:
  rfdevice_pa_sm5643_51_data_ag(void);  /*  Constructor  */

private:
  static rfdevice_pa_data *rfdevice_pa_sm5643_51_data_ptr;

};


#ifdef __cplusplus
}
#endif



#endif

