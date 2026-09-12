/*==============================================================================

  TPDM configuration.

DESCRIPTION
  This file has attributes for TPDMs in the SoC.

REFERENCES

        Copyright © 2015 QUALCOMM Technologies Inc.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/settings/debugtrace/tpdm/config/sm6150/tpdm_config.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
2/7/2011   prasads  First draft created. 
===========================================================================*/
#include "com_dtypes.h"
#include "qdss_tpdm.h"
#include "../../dal/src/DalTPDM.h"


TPDMDeviceProp tpdm_device_attributes[]={
	{0x18AD4000,"wcss","wcss",TPDM_DSB|TPDM_CMB,NULL},
};

const TableLength tpdm_device_attributes_len[] = 
{
{sizeof(tpdm_device_attributes)/sizeof(TPDMDeviceProp)}
};
