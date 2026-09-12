#ifndef QDSS_CONTROL_PRIV_H
#define QDSS_CONTROL_PRIV_H

/*=============================================================================

FILE:         qdss_control_priv.h

DESCRIPTION:  Private data structures used by the qdss control subsystem

================================================================================
Copyright (c) 2014-2015 Qualcomm Technologies, Inc.
        All Rights Reserved.
Qualcomm Technologies, Inc. Confidential and Proprietary.
==============================================================================*/
#include "DDITFunnel.h"
#include "DDITMC.h"
#include "DDICTI.h"
#include "DDITPDM.h"
#include "DDITPDA.h"
#include "DALSys.h"
#include "tracer.h"
#include "qdss_control_config.h"
#include "qdss_utils.h"
#include "qdss_diag.h"
#include "uSleep_mode_trans.h"
#include "ClockDefs.h"
#include "npa.h"
#include "DDIClock.h"


struct qdss_clock{
   DALDEVICEHANDLE hClock;
   int use_count;

   ClockIdType wcss_dbg_apb_clk_id;
   ClockIdType wcss_dbg_atb_clk_id;
   ClockIdType wcss_dbg_ts_clk_id;
   ClockIdType wcss_dbg_css_apb_clk_id;
   ClockIdType wcss_dbg_css_atb_clk_id;
   ClockIdType wcss_dbg_css_ts_clk_id;
};


struct qdss_control {
   DALSYSSyncHandle hSync;
   DALDEVICEHANDLE hFunnel;
   DALDEVICEHANDLE hTMC;
   DALDEVICEHANDLE hCTI;
   DALDEVICEHANDLE hTPDA;
   DALDEVICEHANDLE hTPDM;
   uint8 current_trace_sink;
   boolean bTraceSinkEnabled;
   tracer_client_handle_t hTracer;
   uSleep_notification_cb_handle huSleep;
   uint32 test_data_count;
   npa_client_handle npa_client;   
   struct qdss_clock clk;
};

extern struct qdss_control qdss;

int qdss_ensure_hw_ready(void);
int qdss_trace_sink_enable(void);
int qdss_trace_sink_disable(void);
void ETBSwitchToCBUF(void);
void ETBSwitchToHWFIFO(void);
extern void (*pfn_qdss_retention_enter)(void);
extern void (*pfn_qdss_retention_exit)(void);
void qdss_csr_timestamp_sync(void);
void qdss_csr_timestamp_sync_renable(void);




#endif //QDSS_CONTROL_PRIV_H
