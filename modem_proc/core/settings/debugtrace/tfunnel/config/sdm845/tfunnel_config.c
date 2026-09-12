/*=============================================================================

FILE:         tfunnel_config.c

DESCRIPTION:  
  This file has port configurations for trace funnels for a given chipset.
================================================================================
            Copyright (c) 2013 Qualcomm Technologies, Inc.
                         All Rights Reserved.
          Qualcomm Technologies Proprietary and Confidential
==============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/settings/debugtrace/tfunnel/config/sdm845/tfunnel_config.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
2/7/2011   prasads  First draft created. 
===========================================================================*/
#include "com_dtypes.h"



#define IN_FUN0   0
#define IN_FUN1   1
#define MERG_FUN  2
#define QATB_FUN  3
#define DLST_FUN  4
#define WCSS_FUN  5



typedef struct TypeTFunnelPortEnableConfig TFunnelPortEnableConfig;

/*
This structure defines the topology of a funnel configuration.
*/
struct TypeTFunnelPortEnableConfig{
   uint32 funnel_id;
   uint32 port_id;
   TFunnelPortEnableConfig *pFunnelConfig;
};

/*
Define supported funnel configurations.
*/
static TFunnelPortEnableConfig tfunnel_merge_port0[]={
   {MERG_FUN,0,NULL}
};

TFunnelPortEnableConfig tfunnel_port_mpss[]={
   {IN_FUN0,1,tfunnel_merge_port0}
};

TFunnelPortEnableConfig tfunnel_port_stm[]={
   {IN_FUN0,7,tfunnel_merge_port0}
};


TFunnelPortEnableConfig tfunnel_qatb[]={
   {IN_FUN0,6,tfunnel_merge_port0}
};

TFunnelPortEnableConfig tfunnel_dlst[]={
   {QATB_FUN,5,tfunnel_qatb}
};

TFunnelPortEnableConfig tfunnel_port_wcss[]={
   {DLST_FUN,2,tfunnel_dlst}
};

TFunnelPortEnableConfig tfunnel_port_wcss_noc[]={
   {WCSS_FUN,0,tfunnel_port_wcss}
};

TFunnelPortEnableConfig tfunnel_port_wcss_tpda[]={
   {WCSS_FUN,1,tfunnel_port_wcss}
};








