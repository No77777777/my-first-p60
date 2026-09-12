/*=============================================================================

FILE:         tfunnel_config.c

DESCRIPTION:  
  This file has port configurations for trace funnels for a given chipset.
================================================================================
            Copyright (c) 2013, 2019 Qualcomm Technologies, Inc.
                         All Rights Reserved.
          Qualcomm Technologies Proprietary and Confidential
==============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/settings/debugtrace/tfunnel/config/agatti/tfunnel_config.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
2/7/2011   prasads  First draft created. 
===========================================================================*/
#include "com_dtypes.h"



#define IN_FUN0   0
#define IN_FUN1   1
#define MERG_FUN  2
#define QATB_FUN  3
#define WCSS_FUN  4



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

static TFunnelPortEnableConfig tfunnel_merge_port1[]={
   {MERG_FUN,1,NULL}
};

TFunnelPortEnableConfig tfunnel_port_stm[]={
   {IN_FUN0,7,tfunnel_merge_port0}
};

static TFunnelPortEnableConfig tfunnel_port_wcss[]={
   {IN_FUN1,3,tfunnel_merge_port1}
};

TFunnelPortEnableConfig tfunnel_port_wcss_tpda[]={
   {WCSS_FUN,0,tfunnel_port_wcss}
};








