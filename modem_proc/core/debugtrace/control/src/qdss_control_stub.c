/*=============================================================================

FILE:         qdss_control_stub.c

DESCRIPTION:  

================================================================================
            Copyright (c) 2014 Qualcomm Technologies, Inc.
                         All Rights Reserved.
          Qualcomm Technologies Proprietary and Confidential
==============================================================================*/

#include "qdss.h"


//Stubs
int qdss_stm_trace_enable(void)
{
   //All config is done by the root PD.
   //TBD: should we turn clocks on here.
   return 0;
}

int qdss_stm_trace_disable(void)
{
   return 0;
}

int qdss_stm_async_gen(void) 
{
   return 0;
}

int qdss_tfunnel_enable_port(const char *port_name)
{
   return 0;
}


int qdss_tfunnel_disable_port(const char *port_name)
{
   return 0;
}

int qdss_control_set_sink(uint8 sinkid)
{
   return 0;
}


void ETBSwitchToHWFIFO(void)
{
}

void QDSS_OpenSSCFunnel(void)
{
}

void QDSS_CloseSSCFunnel(void)
{
}

void qdss_control_init(void) 
{
}
