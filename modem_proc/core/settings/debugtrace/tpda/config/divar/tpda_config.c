/*=============================================================================

FILE:         tpda_config.c

DESCRIPTION:   This file has attributes for TPDAs managed by TPDA driver

================================================================================
            Copyright (c) 2015, 2018 Qualcomm Technologies, Inc.
                         All Rights Reserved.
          Qualcomm Technologies Proprietary and Confidential
==============================================================================*/
#include "com_dtypes.h"
#include "../../dal/src/DalTPDA.h"

/*TPDM name listed in TPDAPortSpec must match name in TPDM config */

static TPDAPortSpec tpda_wcss_list[]={
   {"wcss",PORT0}
};

TPDADeviceProp tpda_device_list[]={
	{0x0CAD5000,"wcss",24,BIT_MASK(PORT0),"port_wcss_tpda",tpda_wcss_list,sizeof(tpda_wcss_list)/sizeof(TPDAPortSpec)},
};

const TableLength tpda_device_list_len[] = {
   {sizeof(tpda_device_list)/sizeof(TPDADeviceProp)}
};
