/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  F3 Trace NV Configuration Initialization
                
GENERAL DESCRIPTION
  Contains handlers to configure F3 trace based on NV settings.
    
INITIALIZATION AND SEQUENCING REQUIREMENTS
  Should be called after NV is initialized.

Copyright (c) 2012-2016, 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*==========================================================================

                        EDIT HISTORY FOR MODULEW
$Header: //components/rel/core.mpss/10.0/services/diag/f3_trace/src/diag_f3_trace_nvcfg.c#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
05/25/18   gn      Read NV item value using diag_nv_read (non-blocking)
11/19/13   rh      Added tracking for current source of F3 Trace settings 
03/04/13   rh      NV detail really read into detail and NV control to control 
02/13/13   rh      Fixed compiler errors 
12/19/12   rh      Transitioned REX signal calls to Services OSAL 
08/24/12   mcg     Initial file creation

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "diag_f3_tracei.h"
#include "nv.h"
#include "osal.h"
#include "diagnv.h"
#include "ULog_Diag.h"

extern ULogHandle diag_err_log_handle;

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/
#define  DIAG_F3_TRACE_NV_CMD_SIG        0x0008

/*===========================================================================

FUNCTION diag_f3_trace_nvcfg_init

DESCRIPTION
  Initializes f3 trace configuration based on nv settings
  -note that diag_f3_trace can be configured through other mechanisms (eg devcfg)
   which may have higher precedence. 

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void diag_f3_trace_nvcfg_init(void)
{
  nv_item_type nv_control, nv_detail;
  nv_stat_enum_type ret_val;

  ULogFront_RealTimeInit(&diag_err_log_handle, "DiagErrors", DIAG_ERR_ULOG_SIZE, ULOG_MEMORY_LOCAL, ULOG_LOCK_OS);

  /* Read the diag_debug_detail NV Item */
  ret_val = diag_nv_read(NV_DIAG_DEBUG_DETAIL_I, &nv_detail);
  
  if (ret_val != NV_DONE_S)
  {
    /* NV Item has yet to be written, or was not read correctly. */
    ULogFront_RealTimePrintf(diag_err_log_handle, 1, "Reading diag_f3_trace_detail_mask from NV returned error code : %d",ret_val);
    return;
  }

  ULogFront_RealTimePrintf(diag_err_log_handle, 1, "Read diag_f3_trace_detail_mask from NV : 0x%x", nv_detail.diag_debug_detail);

  /* Read the diag_debug_control NV Item */ 
  ret_val = diag_nv_read(NV_DIAG_DEBUG_CONTROL_I, &nv_control); 
  if (ret_val != NV_DONE_S)
  {
    /* NV Item has yet to be written, or was not read correctly. */
    ULogFront_RealTimePrintf(diag_err_log_handle, 1, "Reading diag_f3_trace_control_mask from NV returned error code : %d",ret_val);
    return;
  }

  ULogFront_RealTimePrintf(diag_err_log_handle, 1, "Read diag_f3_trace_control_mask from NV : 0x%x", nv_control.diag_debug_control);
  
  //good to initialize main f3 trace module
  diag_f3_trace_config_source = DIAG_F3_TRACE_CONFIG_NV;
  diag_f3_trace_init_configs (nv_control.diag_debug_control, nv_detail.diag_debug_detail);
}


